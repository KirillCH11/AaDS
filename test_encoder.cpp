/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 3
*/

#include "encoder.h"
#include <gtest/gtest.h>
#include <fstream>

TEST(EncoderTest, BasicEncoding) {
    const std::string input = "test_input.txt";
    const std::string encoded = "test_encoded.bin";
    const std::string dict = "test_dict.bin";
    
    {
        std::ofstream out(input);
        out << "abracadabra";
    }
    
    ShannonEncoder::encode(input, encoded, dict);
    
    std::ifstream enc(encoded, std::ios::binary);
    std::ifstream dct(dict, std::ios::binary);
    EXPECT_TRUE(enc.good());
    EXPECT_TRUE(dct.good());
    enc.close();
    dct.close();
    
    remove(input.c_str());
    remove(encoded.c_str());
    remove(dict.c_str());
}

TEST(EncoderTest, BinaryFileEncoding) {
    const std::string input = "test_binary.bin";
    const std::string encoded = "test_binary_encoded.bin";
    const std::string dict = "test_binary_dict.bin";
    {
        std::ofstream out(input, std::ios::binary);
        for (int i = 0; i < 256; i++) {
            out.put(static_cast<char>(i));
        }
    }
    
    ShannonEncoder::encode(input, encoded, dict);
    
    std::ifstream enc(encoded, std::ios::binary);
    std::ifstream dct(dict, std::ios::binary);
    EXPECT_TRUE(enc.good());
    EXPECT_TRUE(dct.good());
    
    enc.seekg(0, std::ios::end);
    dct.seekg(0, std::ios::end);
    EXPECT_GT(enc.tellg(), 0);
    EXPECT_GT(dct.tellg(), 0);
    enc.close();
    dct.close();
    
    remove(input.c_str());
    remove(encoded.c_str());
    remove(dict.c_str());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
