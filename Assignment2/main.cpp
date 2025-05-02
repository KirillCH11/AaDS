/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 2
*/

#include <iostream>
#include "gauss_functions.h"
#include <Eigen/Dense>

using namespace Eigen;

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <input.csv> [output.csv]\n";
            std::cerr << "Or to generate random system: " << argv[0] << " --generate <size> <output.csv>\n";
            return 1;
        }

        // Режим генерации случайной системы
        if (std::string(argv[1]) == "--generate") {
            if (argc < 4) {
                std::cerr << "Specify size and output filename\n";
                return 1;
            }
            int size = std::stoi(argv[2]);
            std::string out_file = argv[3];
            generate_random_system(size, out_file);
            std::cout << "Generated " << size << "x" << size << " system to " << out_file << "\n";
            return 0;
        }

        // Режим решения системы
        std::string input_file = argv[1];
        std::string output_file = (argc > 2) ? argv[2] : "";

        MatrixXd Ab = read_matrix_from_csv(input_file);
        MatrixXd A = Ab.leftCols(Ab.cols() - 1);
        VectorXd b = Ab.rightCols(1);

        VectorXd x = gaussian_elimination(A, b);

        std::cout << "Solution:\n" << x << "\n";

        if (!output_file.empty()) {
            MatrixXd result(x.size(), 1);
            result.col(0) = x;
            write_matrix_to_csv(output_file, result);
            std::cout << "Solution saved to " << output_file << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
