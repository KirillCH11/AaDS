/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 3
*/

#include "decoder.h"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

static vector<pair<char, vector<bool>>> readDict(const string& file) {
    ifstream in(file, ios::binary);
    size_t count;
    in.read((char*)&count, sizeof(count));

    vector<pair<char, vector<bool>>> codes;
    while (count--) {
        char c = in.get();
        size_t len;
        in.read((char*)&len, sizeof(len));
        
        vector<bool> bits;
        size_t bytes_to_read = (len + 7) / 8;
        for (size_t i = 0; i < bytes_to_read; ++i) {
            unsigned char byte = in.get();
            int bits_remaining = (i == bytes_to_read - 1) ? (len % 8 ? len % 8 : 8) : 8;
            for (int j = 0; j < bits_remaining; ++j) {
                bits.push_back(byte & (1 << (7 - j)));
            }
        }
        codes.emplace_back(c, bits);
    }
    return codes;
}

static vector<bool> readEncodedData(const string& file) {
    ifstream in(file, ios::binary);
    size_t bitCount;
    in.read((char*)&bitCount, sizeof(bitCount));

    vector<bool> bits;
    size_t bytes_to_read = (bitCount + 7) / 8;
    for (size_t i = 0; i < bytes_to_read; ++i) {
        unsigned char byte = in.get();
        int bits_remaining = (i == bytes_to_read - 1) ? (bitCount % 8 ? bitCount % 8 : 8) : 8;
        for (int j = 0; j < bits_remaining; ++j) {
            bits.push_back(byte & (1 << (7 - j)));
        }
    }
    return bits;
}

static vector<char> decodeData(const vector<bool>& bits,
                             const vector<pair<char, vector<bool>>>& codes) {
    vector<char> result;
    vector<bool> current_bits;
    
    // Создаем хеш-таблицу для быстрого поиска кодов
    unordered_map<string, char> code_map;
    for (const auto& [symbol, code] : codes) {
        string code_str;
        for (bool bit : code) {
            code_str += bit ? '1' : '0';
        }
        code_map[code_str] = symbol;
    }
    
    string current_code;
    for (bool bit : bits) {
        current_code += bit ? '1' : '0';
        
        if (code_map.count(current_code)) {
            result.push_back(code_map[current_code]);
            current_code.clear();
        }
    }
    return result;
}

void ShannonDecoder::decode(const string& encoded,
                          const string& dict,
                          const string& output) {
    auto codes = readDict(dict);
    auto bits = readEncodedData(encoded);
    auto data = decodeData(bits, codes);
    
    ofstream out(output, ios::binary);
    out.write(data.data(), data.size());
}
