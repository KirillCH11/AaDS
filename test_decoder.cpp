/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 3
*/

#include "decoder.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

TEST(DecoderTest, BasicDecoding) {
    const string encoded_file = "test_encoded.bin";
    const string dict_file = "test_dict.bin";
    const string output_file = "test_output.txt";
    
    {
        ofstream dict(dict_file, ios::binary);
        size_t count = 2;
        dict.write((char*)&count, sizeof(count));
        
        dict.put('a');
        size_t len = 1;
        dict.write((char*)&len, sizeof(len));
        dict.put(0b00000000);

        dict.put('b');
        len = 2;
        dict.write((char*)&len, sizeof(len));
        dict.put(0b10000000);
    }
    
    {
        ofstream encoded(encoded_file, ios::binary);
        size_t bitCount = 4; 
        encoded.write((char*)&bitCount, sizeof(bitCount));
        encoded.put(0b01000000);
    }
    
    ShannonDecoder::decode(encoded_file, dict_file, output_file);
    
    ifstream in(output_file);
    string result((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    
    ASSERT_EQ(result, "aba");
    
    remove(encoded_file.c_str());
    remove(dict_file.c_str());
    remove(output_file.c_str());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
