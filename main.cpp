#include "crs/matrix.h"

int main(int , char **)
{
    Matrix<int, 0> matrix;
    for (int i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
    }
    for (int i = 0, j = 9; i <= 9 && j >= 0; ++i, --j) {
        matrix[i][j] = j;
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
}
