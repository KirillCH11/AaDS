/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 3
*/

#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <vector>
#include <utility>

class ShannonEncoder {
public:
    static std::vector<std::pair<char, std::vector<bool>>> buildCodes(const std::vector<char>& data);
    static void encode(const std::string& input, 
                      const std::string& encoded, 
                      const std::string& dict);
};

#endif
