/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 2
*/

#include "gauss_functions.h"
#include <Eigen/Dense>
#include "fast-cpp-csv-parser/csv.h"
#include <fstream>    
#include <random>     
#include <stdexcept>  


using namespace Eigen;

MatrixXd read_matrix_from_csv(const std::string& filename) {
    io::LineReader reader(filename);
    std::vector<std::vector<double>> data;
    
    // Чтение заголовка (если есть) для определения количества столбцов
    if (char* line = reader.next_line()) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> first_row;
        
        while (std::getline(ss, cell, ',')) {
            first_row.push_back(std::stod(cell));
        }
        data.push_back(first_row);
    }
    
    while (char* line = reader.next_line()) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;
        
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }
        data.push_back(row);
    }
    
    MatrixXd matrix(data.size(), data[0].size());
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            matrix(i, j) = data[i][j];
        }
    }
    return matrix;
}


void write_matrix_to_csv(const std::string& filename, const MatrixXd& matrix) {
    std::ofstream file(filename);
    IOFormat csv_format(FullPrecision, DontAlignCols, ", ", "\n");
    file << matrix.format(csv_format);
}


void generate_random_system(int size, const std::string& filename, unsigned int seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-10.0, 10.0);


    MatrixXd A = MatrixXd::Zero(size, size);
    VectorXd x = VectorXd::Zero(size);
    VectorXd b = VectorXd::Zero(size);

    for (int i = 0; i < size; ++i) {
        x(i) = dist(gen);
        for (int j = 0; j < size; ++j) {
            A(i, j) = dist(gen);
        }
        // Делаем матрицу диагонально доминирующей для устойчивости
        A(i, i) = A.row(i).sum() + 1.0;
    }

    b = A * x;

    // Объединение в одну матрицу для сохранения
    MatrixXd Ab(size, size + 1);
    Ab << A, b;

    write_matrix_to_csv(filename, Ab);
}

VectorXd gaussian_elimination(const MatrixXd& A, const VectorXd& b) {
    MatrixXd Ab(A.rows(), A.cols() + 1);
    Ab << A, b;

    // Прямой ход
    for (int i = 0; i < Ab.rows(); ++i) {
        // Частичный выбор главного элемента
        int max_row = i;
        for (int k = i + 1; k < Ab.rows(); ++k) {
            if (abs(Ab(k, i)) > abs(Ab(max_row, i))) {
                max_row = k;
            }
        }
        Ab.row(i).swap(Ab.row(max_row));

        // Обнуление элементов под главной диагональю
        for (int k = i + 1; k < Ab.rows(); ++k) {
            double factor = Ab(k, i) / Ab(i, i);
            Ab.row(k) -= factor * Ab.row(i);
        }
    }

    // Обратный ход
    VectorXd x(Ab.rows());
    for (int i = Ab.rows() - 1; i >= 0; --i) {
        x(i) = Ab(i, Ab.cols() - 1);
        for (int j = i + 1; j < Ab.rows(); ++j) {
            x(i) -= Ab(i, j) * x(j);
        }
        x(i) /= Ab(i, i);
    }

    return x;
}

