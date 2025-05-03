/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment
*/

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace ascii85 {

  std::string encode(const std::vector<unsigned char>& data) {
      std::string result;
      size_t size = data.size();
      size_t i = 0;
      while(i < size) {
          uint32_t value = 0;
          int count = 0;
          unsigned char block[4] = {0,0,0,0};
          for(; count < 4 && i < size; ++count, ++i)
              block[count] = data[i];

          if(count == 4) {
              value = (static_cast<uint32_t>(block[0]) << 24) |
                      (static_cast<uint32_t>(block[1]) << 16) |
                      (static_cast<uint32_t>(block[2]) << 8)  |
                      (static_cast<uint32_t>(block[3]));
              if(value == 0) {
                  result.push_back('z');
                  continue;
              }
          } else {
              value = 0;
              for (int j = 0; j < count; j++) {
                  value |= static_cast<uint32_t>(block[j]) << (24 - 8*j);
              }
          }
          
          char encoded[5];
          for (int j = 4; j >= 0; j--) {
              encoded[j] = static_cast<char>(value % 85 + 33);
              value /= 85;
          }
          int toWrite = (count == 4) ? 5 : count + 1;
          result.append(encoded, toWrite);
      }
      
      return result;
  };
  
  std::vector<unsigned char> decode(const std::string& input) {
      std::vector<unsigned char> output;
      std::vector<char> group;
      auto processGroup = [&](bool finalize = false) {
          if(group.empty()) return;
          size_t groupSize = group.size();
          if (!finalize && groupSize < 5) return;
          if(finalize && groupSize == 1)
              throw std::runtime_error("Неверная длина последней группы данных!");
          int pad = 0;
          if(groupSize < 5) {
              pad = 5 - groupSize;
              for (int i = 0; i < pad; i++)
                  group.push_back('u');  //'u' = 117, т.к. 117-33 = 84
          }
          uint32_t value = 0;
          for (int i = 0; i < 5; i++) {
              char c = group[i];
              if(c < '!' || c > 'u') {
                  throw std::runtime_error("Неверный символ в потоке данных!");
              }
              value = value * 85 + (static_cast<uint32_t>(c) - 33);
          }
          unsigned char bytes[4];
          bytes[0] = static_cast<unsigned char>((value >> 24) & 0xFF);
          bytes[1] = static_cast<unsigned char>((value >> 16) & 0xFF);
          bytes[2] = static_cast<unsigned char>((value >> 8) & 0xFF);
          bytes[3] = static_cast<unsigned char>(value & 0xFF);
          int bytesToOutput = 4;
          if(finalize && pad > 0)
              bytesToOutput = 4 - pad;
          for (int i = 0; i < bytesToOutput; i++) {
              output.push_back(bytes[i]);
          }
          group.clear();
      };

      for (char c : input) {
          if (std::isspace(static_cast<unsigned char>(c))) continue;
          if(c == 'z') {
              if (!group.empty())
                  throw std::runtime_error("Символ 'z' не может встречаться внутри группы!");
              output.push_back(0);
              output.push_back(0);
              output.push_back(0);
              output.push_back(0);
          } else {
              group.push_back(c);
              if(group.size() == 5)
                  processGroup();
          }
      }
      if(!group.empty())
          processGroup(true);
      
      return output;
  };

};

#ifndef UNIT_TEST

std::vector<unsigned char> readStdinBinary() {
    std::vector<unsigned char> data;
    const std::size_t bufSize = 4096;
    char buffer[bufSize];
    while (true) {
        std::cin.read(buffer, bufSize);
        std::streamsize n = std::cin.gcount();
        if(n > 0)
            data.insert(data.end(), buffer, buffer + n);
        if(n < static_cast<std::streamsize>(bufSize))
            break;
    }
    return data;
};

std::string readStdinText() {
    std::ostringstream oss;
    char buffer[4096];
    while (std::cin.read(buffer, sizeof(buffer)))
        oss.write(buffer, std::cin.gcount());
    if(std::cin.gcount() > 0)
        oss.write(buffer, std::cin.gcount());
    return oss.str();
};

int main(int argc, char* argv[]) {
    bool decodeMode = false;
    if (argc >= 2) {
        std::string arg1(argv[1]);
        if(arg1 == "-d")
            decodeMode = true;
        else if(arg1 == "-e")
            decodeMode = false;
        else {
            std::cerr << "Использование: " << argv[0] << " [-e|-d]" << std::endl;
            return 1;
        }
    }
  
    try {
        if(decodeMode) {
            std::string input = readStdinText();
            std::vector<unsigned char> decoded = ascii85::decode(input);
            std::cout.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
        } else {
            std::vector<unsigned char> input = readStdinBinary();
            std::string encoded = ascii85::encode(input);
            std::cout.write(encoded.data(), encoded.size());
        }
    } catch (const std::exception& ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
};

#else

#include "gtest/gtest.h"

TEST(Ascii85EncodeDecode, EmptyInput) {
    std::vector<unsigned char> input;
    std::string encoded = ascii85::encode(input);
    EXPECT_EQ(encoded, "");
    std::vector<unsigned char> decoded = ascii85::decode(encoded);
    EXPECT_EQ(decoded, input);
}

TEST(Ascii85EncodeDecode, SimpleInput) {
    std::string text = "Hello, World!";
    std::vector<unsigned char> input(text.begin(), text.end());
    std::string encoded = ascii85::encode(input);
    std::vector<unsigned char> decoded = ascii85::decode(encoded);
    EXPECT_EQ(decoded, input);
}

TEST(Ascii85EncodeDecode, AllZeroBlock) {
    std::vector<unsigned char> input = {0,0,0,0};
    std::string encoded = ascii85::encode(input);
    EXPECT_EQ(encoded, "z");
    std::vector<unsigned char> decoded = ascii85::decode(encoded);
    EXPECT_EQ(decoded, input);
}

TEST(Ascii85Decode, IncompleteFinalGroup) {
    std::vector<unsigned char> input = {'M', 'a', 'n'};
    std::string encoded = ascii85::encode(input);
    std::vector<unsigned char> decoded = ascii85::decode(encoded);
    EXPECT_EQ(decoded, input);
}

TEST(Ascii85Decode, InvalidCharacter) {
    std::string invalid = "!!!!~";
    EXPECT_THROW({
        ascii85::decode(invalid);
    }, std::runtime_error);
}

TEST(Ascii85Decode, InvalidZUsage) {
    std::string invalid = "abzcd";
    EXPECT_THROW({
        ascii85::decode(invalid);
    }, std::runtime_error);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};

#endif
