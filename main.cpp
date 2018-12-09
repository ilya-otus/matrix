#include "lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

template<typename T, T D = T()>
class CRS {
public:
    CRS() {
        rowPeter.resize(1, 0);
    }

    void add(const T &value, size_t column, size_t row) {
        if (exists(column, row)) {
            if (value == D) {
                auto eraseColumn = std::find(colIndex.begin() + rowPeter[row], colIndex.begin() + rowPeter[row + 1], column);
                values.erase(values.begin() + (eraseColumn - colIndex.begin()));
                colIndex.erase(eraseColumn);
                for (size_t r = row + 1; r < rowPeter.size(); ++r) {
                    --rowPeter[r];
                }
                if (row + 1 == rowPeter.size() && rowPeter[row] == rowPeter[row + 1]) {
                    rowPeter.erase(rowPeter.end() - 1);
                }
            }
            else {
                auto found = std::lower_bound(colIndex.begin() + rowPeter[row], colIndex.begin() + rowPeter[row + 1], column); // TODO: exclude duplicated operation
                values[found - colIndex.begin()] = value;
            }
        } else {
            if (row >= rowPeter.size() - 1) {
                rowPeter.resize(row + 2, rowPeter.back());
            }
            auto found = std::lower_bound(colIndex.begin() + rowPeter[row], colIndex.begin() + rowPeter[row + 1], column); // TODO: exclude duplicated operation
            auto insertedColumn = colIndex.insert(found, column);
            auto pos = insertedColumn - colIndex.begin();
            values.insert(values.begin() + pos, value);
            for (size_t r = row + 1; r < rowPeter.size(); ++r) {
                ++rowPeter[r];
            }
        }
    }

    bool exists(size_t column, size_t row) {
        if (rowPeter[row] != rowPeter[row + 1] && !colIndex.empty()) {
            auto found = std::lower_bound(colIndex.begin() + rowPeter[row], colIndex.begin() + rowPeter[row + 1], column); // TODO: exclude duplicated operation
            if (*found == column) {
                return true;
            }
        }
        return false;
    }

    void print() {
        std::cout << "Row indices: ";
        for (auto r: rowPeter) {
            std::cout << r+1 << " ";
        }
        std::cout << std::endl;

        std::cout << "Column indices: ";
        for (auto c: colIndex) {
            std::cout << c+1 << " ";
        }
        std::cout << std::endl;

        std::cout << "Values: ";
        for (auto v: values) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<size_t> rowPeter;
    std::vector<size_t> colIndex;
    std::vector<T> values;
};

int main(int argc, char *argv[])
{
    CRS<int, 0> c1;
    c1.add(1, 2, 0);
    c1.print();
    c1.add(3, 3, 0);
    c1.print();
    c1.add(5, 7, 0);
    c1.print();
    c1.add(7, 5, 2);
    c1.print();
    c1.add(1, 7, 2);
    c1.print();
    c1.add(5, 7, 0);
    c1.print();
    c1.add(2, 6, 1);
    c1.print();
    c1.add(0, 6, 1);
    c1.print();
    return 0;
}
