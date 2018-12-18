#include "crs/matrix.h"

int main(int , char **)
{
    Matrix<int, 0> matrix;
    const size_t upperBound = 9;
    for (size_t i = 0; i <= upperBound; ++i) {
        matrix[i][i] = i;
        matrix[upperBound - i][i] = i;
    }
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Non-default values count equals " << matrix.size() << std::endl;
    std::cout << "Row\tColumn\tValue" << std::endl;
    for (auto cell: matrix) {
        size_t row;
        size_t col;
        int val;
        std::tie(row, col, val) = cell;
        std::cout << row << "\t" << col << "\t" << val << std::endl;
    }
    return 0;
}
