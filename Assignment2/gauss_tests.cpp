/* Churkin Kirill st129364@student.spbu.ru
	AaDS; Assignment 2
*/

#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "gauss_functions.h"

using namespace Eigen;

TEST(GaussianElimination, SimpleSystem) {
    MatrixXd A(2, 2);
    A << 2, -1,
         -1, 3;
    VectorXd b(2);
    b << 1, 2;

    VectorXd x = gaussian_elimination(A, b);
    VectorXd expected(2);
    expected << 1, 1;

    ASSERT_TRUE(x.isApprox(expected, 1e-6));
}

TEST(GaussianElimination, RandomSystem) {
    const int size = 10;
    MatrixXd A = MatrixXd::Random(size, size);
    VectorXd x_expected = VectorXd::Random(size);
    
    // Делаем матрицу диагонально доминирующей
    for (int i = 0; i < size; ++i) {
        A(i, i) = A.row(i).sum() + 1.0;
    }
    
    VectorXd b = A * x_expected;
    VectorXd x = gaussian_elimination(A, b);
    
    ASSERT_TRUE(x.isApprox(x_expected, 1e-6));
}

TEST(CSVIO, ReadWrite) {
    MatrixXd original(3, 3);
    original << 1, 2, 3,
                4, 5, 6,
                7, 8, 9;
    
    const std::string filename = "test_csv.csv";
    write_matrix_to_csv(filename, original);
    MatrixXd read = read_matrix_from_csv(filename);
    
    ASSERT_TRUE(original.isApprox(read, 1e-6));
    std::remove(filename.c_str());
}
