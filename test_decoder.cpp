#include "decoder.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

TEST(DecoderTest, BasicDecoding) {
    // 1. Создаем тестовые файлы
    const string encoded_file = "test_encoded.bin";
    const string dict_file = "test_dict.bin";
    const string output_file = "test_output.txt";
    
    // 2. Создаем словарь (a=0, b=10)
    {
        ofstream dict(dict_file, ios::binary);
        size_t count = 2;
        dict.write((char*)&count, sizeof(count));
        
        // Символ 'a' (код 0)
        dict.put('a');
        size_t len = 1;
        dict.write((char*)&len, sizeof(len));
        dict.put(0b00000000); // бит: 0
        
        // Символ 'b' (код 10)
        dict.put('b');
        len = 2;
        dict.write((char*)&len, sizeof(len));
        dict.put(0b10000000); // биты: 1 и 0
    }
    
    // 3. Создаем закодированные данные (строка "aba" = 0 10 0)
    {
        ofstream encoded(encoded_file, ios::binary);
        size_t bitCount = 4; // Теперь точно 4 бита (0 10 0)
        encoded.write((char*)&bitCount, sizeof(bitCount));
        encoded.put(0b01000000); // 0 10 0
    }
    
    // 4. Декодируем
    ShannonDecoder::decode(encoded_file, dict_file, output_file);
    
    // 5. Проверяем результат
    ifstream in(output_file);
    string result((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    
    ASSERT_EQ(result, "aba");
    
    // 6. Удаляем временные файлы
    remove(encoded_file.c_str());
    remove(dict_file.c_str());
    remove(output_file.c_str());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
