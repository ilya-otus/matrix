#include "lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>

template<typename T, T D = T()>
class CRS {
public:
    CRS() {
        rowPeter.resize(1, 0);
    }

    void add(const T &value, size_t column, size_t row) {
        if (auto existPos = exists(column, row)) {
            if (value == D) {
                std::cout << "Operation remove" << std::endl;
                values.erase(values.begin() + *existPos);
                colIndex.erase(colIndex.begin() + *existPos);
                for (size_t r = row + 1; r < rowPeter.size(); ++r) {
                    --rowPeter[r];
                }
                if (row + 2 == rowPeter.size()) {
                    if(auto l = std::lower_bound(rowPeter.begin(), rowPeter.end(), rowPeter.back()); l != rowPeter.end()) {
                        rowPeter.erase(l+1, rowPeter.end());
                    }
                }
            }
            else {
                std::cout << "Operation change value" << std::endl;
                values[*existPos] = value;
            }
        } else {
            std::cout << "Operation create" << std::endl;
            if (row >= rowPeter.size() - 1) {
                rowPeter.resize(row + 2, rowPeter.back());
            }
            auto begin = colIndex.begin() + rowPeter[row];
            auto end = colIndex.begin() + rowPeter[row + 1];
            auto found = std::lower_bound(begin, end, column);
            auto insertedColumn = colIndex.insert(found, column);
            auto pos = insertedColumn - colIndex.begin();
            values.insert(values.begin() + pos, value);
            for (size_t r = row + 1; r < rowPeter.size(); ++r) {
                ++rowPeter[r];
            }
        }
    }

    std::optional<size_t> exists(size_t column, size_t row) {
        if (rowPeter[row] != rowPeter[row + 1] && !colIndex.empty()) {
            auto begin = colIndex.begin() + rowPeter[row];
            auto end = colIndex.begin() + rowPeter[row + 1];
            auto found = std::find(begin, end, column);
            if (found != end) {
                return found - colIndex.begin();
            }
        }
        return {};
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
    c1.add(1, 7, 2);
    c1.print();
    c1.add(1, 2, 0);
    c1.print();
    c1.add(3, 3, 0);
    c1.print();
    c1.add(7, 5, 2);
    c1.print();
    c1.add(5, 7, 0);
    c1.print();
    c1.add(5, 7, 10);
    c1.print();
    c1.add(0, 7, 10);
    c1.print();
    c1.add(2, 6, 10);
    c1.print();
    c1.add(0, 6, 10);
    c1.print();
    return 0;
}
