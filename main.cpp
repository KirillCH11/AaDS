#include "decoder.h"
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

vector<bool> parseBits(const string& bitStr) {
    vector<bool> bits;
    for (char c : bitStr) {
        if (c == '1') bits.push_back(true);
        else if (c == '0') bits.push_back(false);
    }
    return bits;
}

void manualDecoderTest() {
    cout << "Введите закодированную строку (биты через пробел):\n> ";
    string encodedStr;
    getline(cin, encodedStr);
    
    vector<bool> encodedBits;
    istringstream iss(encodedStr);
    string bitGroup;
    while (iss >> bitGroup) {
        for (char c : bitGroup) {
            if (c == '1') encodedBits.push_back(true);
            else if (c == '0') encodedBits.push_back(false);
        }
    }

    map<vector<bool>, char> dict;
    cout << "Введите словарь в формате 'символ:биты' (по одному на строке, пустая строка - конец):\n> ";
    
    while (true) {
        string line;
        getline(cin, line);
        if (line.empty()) break;
        
        size_t colonPos = line.find(':');
        if (colonPos == string::npos || colonPos == 0) {
            cout << "Ошибка формата. Используйте 'символ:биты'\n> ";
            continue;
        }
        
        char symbol = line[0];
        string bitsStr = line.substr(colonPos + 1);
        dict[parseBits(bitsStr)] = symbol;
    }

    string result;
    vector<bool> currentCode;
    
    for (bool bit : encodedBits) {
        currentCode.push_back(bit);
        auto it = dict.find(currentCode);
        if (it != dict.end()) {
            result += it->second;
            currentCode.clear();
        }
    }

    cout << "Декодированная строка: " << result << endl;
}

int main() {
    manualDecoderTest();
    return 0;
}
