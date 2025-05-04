#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>
#include <utility>

class ShannonDecoder {
public:
    static std::vector<std::pair<char, std::vector<bool>>> loadDictionary(const std::string& filename);
    static void decode(const std::string& inputFile, 
                     const std::string& dictionaryFile,
                     const std::string& outputFile);
};

#endif
