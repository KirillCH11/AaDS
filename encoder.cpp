#include "encoder.h"
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

namespace {
    void writeDict(const string& file, const vector<pair<char, vector<bool>>>& codes) {
        ofstream out(file, ios::binary);
        size_t count = codes.size();
        out.write((char*)&count, sizeof(count)); 
        
        for (auto& p : codes) {
            out.put(p.first);
            size_t len = p.second.size();
            out.write((char*)&len, sizeof(len));
            
            unsigned char byte = 0;
            for (size_t i = 0; i < len; i++) {
                if (p.second[i]) byte |= 1 << (7 - (i % 8));
                if (i % 8 == 7 || i == len-1) {
                    out.put(byte);
                    byte = 0;
                }
            }
        }
    }

    void writeEncoded(const string& file, 
                     const vector<char>& data, 
                     const vector<pair<char, vector<bool>>>& codes) {
        map<char, vector<bool>> codeMap;
        for (auto& p : codes) codeMap[p.first] = p.second;
        
        ofstream out(file, ios::binary);
        vector<bool> bits;
        
        for (char c : data) {
            auto& code = codeMap[c];
            bits.insert(bits.end(), code.begin(), code.end());
        }
        
        size_t bitCount = bits.size();
        out.write((char*)&bitCount, sizeof(bitCount));
        
        unsigned char byte = 0;
        for (size_t i = 0; i < bits.size(); i++) {
            if (bits[i]) byte |= 1 << (7 - (i % 8));
            if (i % 8 == 7 || i == bits.size()-1) {
                out.put(byte);
                byte = 0;
            }
        }
    }
}

vector<pair<char, vector<bool>>> ShannonEncoder::buildCodes(const vector<char>& data) {
    map<char, int> freq;
    for (char c : data) freq[c]++;
    
    vector<pair<double, char>> probs;
    int total = data.size();
    for (auto& p : freq) {
        probs.emplace_back((double)p.second/total, p.first);
    }
    sort(probs.rbegin(), probs.rend());

    vector<pair<char, vector<bool>>> codes;
    double cumProb = 0.0;
    
    for (auto& p : probs) {
        int len = ceil(-log2(p.first));
        double code = cumProb;
        vector<bool> bits;

        for (int i = 0; i < len; i++) {
            code *= 2;
            bits.push_back(code >= 1.0);
            if (code >= 1.0) code -= 1.0;
        }
        
        codes.emplace_back(p.second, bits);
        cumProb += p.first;
    }
    
    return codes;
}

void ShannonEncoder::encode(const string& input, 
                          const string& encoded, 
                          const string& dict) {
    ifstream in(input, ios::binary);
    vector<char> data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    
    auto codes = buildCodes(data);
    
    cout << "Коды символов:\n";
    for (const auto& [symbol, code] : codes) {
        cout << "'" << symbol << "': ";
        for (bool bit : code) cout << bit;
        cout << "\n";
    }
    
    map<char, vector<bool>> codeMap;
    for (const auto& p : codes) codeMap[p.first] = p.second;
    
    cout << "Закодированная строка: ";
    for (char c : data) {
        for (bool bit : codeMap.at(c)) {
            cout << bit;
        }
        cout << " ";
    }
    cout << "\n";
    
    writeDict(dict, codes);
    writeEncoded(encoded, data, codes);
}
