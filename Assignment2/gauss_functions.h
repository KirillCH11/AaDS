/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 2
*/

#ifndef GAUSS_FUNCTIONS_H
#define GAUSS_FUNCTIONS_H

#include <Eigen/Dense>
#include <string>

Eigen::MatrixXd read_matrix_from_csv(const std::string& filename);
void write_matrix_to_csv(const std::string& filename, const Eigen::MatrixXd& matrix);
Eigen::VectorXd gaussian_elimination(const Eigen::MatrixXd& A, const Eigen::VectorXd& b);
void generate_random_system(int size, const std::string& filename, unsigned int seed = 42);

#endif
