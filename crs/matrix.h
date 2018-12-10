#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <optional>

template<typename T, T D = T()>
class CRS {
    using CrsItem = std::pair<size_t, T>;

    class VectorWrapper {
        class ValueWrapper {
        public:
            ValueWrapper(T &val) : value(val) {}
            ValueWrapper& operator=(T& val) {
                std::cerr << "Non-const &operator= val=" << val << std::endl;
                this->value = val;
                return *this;
            }
            ValueWrapper& operator=(const T& val) {
                std::cerr << "Const &operator= val=" << val << std::endl;
                this->value = val;
                return *this;
            }
            friend std::ostream& operator<<(std::ostream& os, const ValueWrapper &v) {
                os << v.value;
                return os;
            }
        private:
            T& value;
        };
        using WrappedType = std::vector<CrsItem>;
        using iterator = typename WrappedType::iterator;
        using reference = typename WrappedType::reference;

    public:
        VectorWrapper() : data() {}

        template<typename InputIt>
        VectorWrapper(InputIt f, InputIt l) : data(), first(f), last(l) {}

        iterator erase(iterator pos) {
            return data.erase(pos);
        }

        iterator begin() {
            return data.begin();
        }

        iterator end() {
            return data.end();
        }

        iterator insert(iterator pos, const CrsItem& value) {
            return data.insert(pos, value);
        }

        bool empty() {
            return data.empty();
        }

        reference at(size_t pos) {
            return data.at(pos);
        }

        ValueWrapper& operator[](size_t pos) {
            if (tmp != nullptr) {
                delete tmp;
                tmp = nullptr;
            }
            auto found = std::find_if(first, last, [&pos](CrsItem &item){return pos == item.first;});
            if (found != last) {
                tmp = new ValueWrapper(found->second);
            } else {
                tmp = new ValueWrapper(defaultValue);
            }
            return *tmp;
        }
    private:
        ValueWrapper* tmp = nullptr;
        WrappedType data;
        iterator first;
        iterator last;
        T defaultValue = D;
    };

public:
    CRS() {
        rowPeter.resize(1, 0);
    }

    void add(const T &value, size_t column, size_t row) {
        if (auto existPos = exists(column, row)) {
            if (value == D) {
                std::cout << "\tOperation remove " << column << " " << row << std::endl;
                sparsedColumn.erase(sparsedColumn.begin() + *existPos);
                for (size_t r = row + 1; r < rowPeter.size(); ++r) {
                    --rowPeter[r];
                }
                if (row + 2 == rowPeter.size()) {
                    if(auto l = std::lower_bound(rowPeter.begin(), rowPeter.end(), rowPeter.back()); l != rowPeter.end()) {
                        rowPeter.erase(l+1, rowPeter.end());
                    }
                }
            } else {
                std::cout << "\tOperation change value " << column << " " << row << std::endl;
                sparsedColumn.at(*existPos).second = value;
            }
        } else {
            std::cout << "\tOperation create " << column << " " << row << std::endl;
            if (row >= rowPeter.size() - 1) {
                rowPeter.resize(row + 2, rowPeter.back());
            }
            auto begin = sparsedColumn.begin() + rowPeter[row];
            auto end = sparsedColumn.begin() + rowPeter[row + 1];
            auto comparator = [](CrsItem item1, CrsItem item2){
                return item1.first < item2.first;
            };
            auto found = std::lower_bound(begin, end, CrsItem(column, D), comparator);
            sparsedColumn.insert(found, {column, value});
            for (size_t r = row + 1; r < rowPeter.size(); ++r) {
                ++rowPeter[r];
            }
        }
    }

    std::optional<size_t> exists(size_t column, size_t row) {
        if (rowPeter.size() >= row + 2 && rowPeter[row] != rowPeter[row + 1] && !sparsedColumn.empty()) {
            auto begin = sparsedColumn.begin() + rowPeter[row];
            auto end = sparsedColumn.begin() + rowPeter[row + 1];
            auto comparator = [&column](CrsItem item){return column == item.first;};
            auto found = std::find_if(begin, end, comparator);
            if (found != end) {
                return found - sparsedColumn.begin();
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
        for (auto [c,v]: sparsedColumn) {
            std::cout << c+1 << " ";
        }
        std::cout << std::endl;

        std::cout << "Values: ";
        for (auto [c,v]: sparsedColumn) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    VectorWrapper& operator[](size_t rowIdx) {
        if (buf != nullptr) {
            delete buf;
        }
        if (rowIdx + 2 > rowPeter.size()) {
            buf = new VectorWrapper(sparsedColumn.begin() + rowPeter.back(), sparsedColumn.begin() + rowPeter.back());
        } else {
            buf = new VectorWrapper(sparsedColumn.begin() + rowPeter[rowIdx], sparsedColumn.begin() + rowPeter[rowIdx + 1]);
        }
        return *buf;
    }

private:
    VectorWrapper *buf = nullptr;
    VectorWrapper sparsedColumn;
    std::vector<size_t> rowPeter;
};

