#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <optional>

template<typename T>
using CrsItem = std::pair<size_t, T>;
template<typename T>
using SparsedColumn = std::vector<CrsItem<T>>;

template<typename T>
class Observer {
    public:
        virtual void remove(size_t r, size_t c) = 0;
        virtual T& insert(size_t r, size_t c, const T& v) = 0;
};

template<typename T, T D>
class ValueWrapper {
public:
    ValueWrapper(T *val, size_t r, size_t c, Observer<T> *obs)
      : value(val), row(r), column(c), observer(obs) {}

    ValueWrapper& operator=(const T& val) {
        if (*value == D) {
            value = &(observer->insert(row, column, val));
        } else if (val == D) {
            observer->remove(row, column);
            *value = D;
        } else {
            *value = val;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const ValueWrapper &v) {
        os << v.value;
        return os;
    }
private:
    T* value;
    size_t row;
    size_t column;
    Observer<T> *observer;
};

template<typename T, T D = T()>
class VectorWrapper {
    using iterator = typename SparsedColumn<T>::iterator;
    using reference = typename SparsedColumn<T>::reference;

public:
    template<typename InputIt>
    VectorWrapper(InputIt f, InputIt l, size_t r, Observer<T> *obs)
      : first(f), last(l), row(r), observer(obs) {}

    ValueWrapper<T,D>& operator[](size_t column) {
        if (tmp != nullptr) {
            delete tmp;
            tmp = nullptr;
        }
        auto found = std::find_if(first, last, [&column](CrsItem<T> &item){ return column == item.first; });
        if (found != last) {
            tmp = new ValueWrapper<T,D>(&(found->second), row, column, observer);
        } else {
            tmp = new ValueWrapper<T,D>(&defaultValue, row, column, observer);
        }
        return *tmp;
    }
private:
    iterator first;
    iterator last;
    size_t row;
    ValueWrapper<T,D>* tmp = nullptr;
    Observer<T> *observer;
    T defaultValue = D;
};

template<typename T, T D = T()>
class CRS : public Observer<T> {
public:
    explicit CRS() {
        rowPeter.resize(1, 0);
    }
    void remove(size_t row, size_t column) override {
        auto begin = sparsedColumn.begin() + rowPeter[row];
        auto end = sparsedColumn.begin() + rowPeter[row + 1];
        auto comparator = [&column](CrsItem<T> item){return column == item.first;};
        auto found = std::find_if(begin, end, comparator);
        if (found == end) {
            return;
        }
        sparsedColumn.erase(found);
        trimTail(row);
    }

    T& insert(size_t row, size_t column, const T& value) override {
        if (row >= rowPeter.size() - 1) {
            rowPeter.resize(row + 2, rowPeter.back());
        }
        auto begin = sparsedColumn.begin() + rowPeter[row];
        auto end = sparsedColumn.begin() + rowPeter[row + 1];
        auto comparator = [](CrsItem<T> item1, CrsItem<T> item2){
            return item1.first < item2.first;
        };
        auto found = std::lower_bound(begin, end, CrsItem<T>(column, D), comparator);
        auto inserted = sparsedColumn.insert(found, {column, value});
        for (size_t r = row + 1; r < rowPeter.size(); ++r) {
            ++rowPeter[r];
        }
        return inserted->second;
    }

    void dump() {
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
        std::cout << std::endl << std::endl;
    }

    VectorWrapper<T, D>& operator[](size_t row) {
        if (buf != nullptr) {
            delete buf;
        }
        if (row + 2 > rowPeter.size()) {
            buf = new VectorWrapper<T, D>(sparsedColumn.begin() + rowPeter.back(), sparsedColumn.begin() + rowPeter.back(), row, this);
        } else {
            buf = new VectorWrapper<T, D>(sparsedColumn.begin() + rowPeter[row], sparsedColumn.begin() + rowPeter[row + 1], row, this);
        }
        return *buf;
    }

private:
    void trimTail(size_t row) {
        for (size_t r = row + 1; r < rowPeter.size(); ++r) {
            --rowPeter[r];
        }
        if (row + 2 == rowPeter.size()) {
            if(auto l = std::lower_bound(rowPeter.begin(), rowPeter.end(), rowPeter.back()); l != rowPeter.end()) {
                rowPeter.erase(l+1, rowPeter.end());
            }
        }
    }

private:
    VectorWrapper<T, D> *buf = nullptr;
    SparsedColumn<T> sparsedColumn;
    std::vector<size_t> rowPeter;
};

