/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 3
*/

#include "encoder.h"
#include <iostream>
#include <vector>

int main() {
    std::string input;
    std::cout << "Введите строку для кодирования: ";
    std::getline(std::cin, input);
    
    std::vector<char> data(input.begin(), input.end());
    auto codes = ShannonEncoder::buildCodes(data);
    
    std::cout << "Коды символов:\n";
    for (const auto& [symbol, code] : codes) {
        std::cout << "'" << symbol << "': ";
        for (bool bit : code) std::cout << bit;
        std::cout << "\n";
    }
    
    std::cout << "Закодированная строка:\n";
    for (char c : input) {
        for (const auto& [symbol, code] : codes) {
            if (symbol == c) {
                for (bool bit : code) std::cout << bit;
                std::cout << " ";
                break;
            }
        }
    }
    std::cout << "\n";
    return 0;
}
