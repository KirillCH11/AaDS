/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 3
*/

#include "encoder.h"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
    if (argc == 4) { 
        const std::string input_file = argv[1];
        const std::string encoded_file = argv[2];
        const std::string dict_file = argv[3];
        
        ShannonEncoder::encode(input_file, encoded_file, dict_file);
        std::cout << "Файл успешно закодирован. Результат в " << encoded_file 
                  << ", словарь в " << dict_file << std::endl;
    }
    else if (argc == 1) {
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
    }
    else {
        std::cerr << "Использование:\n"
                  << "  Для файлов: " << argv[0] << " <input> <encoded> <dict>\n"
                  << "  Для строки: " << argv[0] << std::endl;
        return 1;
    }
    return 0;
}
