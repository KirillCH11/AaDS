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

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
