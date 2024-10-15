#pragma once

#ifndef CRS_MATRIX_DECLARE
#define CRS_MATRIX_DECLARE

#include "../../include/linalg/matrix.hxx"
#include <iostream>
#include <sstream>

template<typename T>
typename linalg::Matrix<T>::CRS linalg::Matrix<T>::init_crs(const std::vector<std::vector<T>> _matrix) {
    CRS mtx;

    if (_matrix.empty()) {
        std::cerr << "Error during `init_crs` operation on line " << __LINE__ << ": Empty matrix.\n";
        return mtx;
    }

    mtx.row_pointers.push_back(0); // Initializing first element of pointers with zero

    size_t rows = _matrix.size();
    size_t cols = _matrix[0].size();

    for (size_t i = 0; i < rows; ++i) {
        int cnt = 0; // Number of nonzero elements in row
        for (size_t j = 0; j < cols; ++j) {
            if (_matrix[i][j] != 0) {
                mtx.values.push_back(_matrix[i][j]);
                mtx.col_indexes.push_back(j);
                cnt++;
            }
        }
        mtx.row_pointers.push_back(
                mtx.row_pointers.back() + cnt
        ); // ROW_INDEX[i] = ROW_INDEX[i-1] + nzc; (nonzero count)
    }

    return mtx;
}

template<typename T>
T& linalg::Matrix<T>::CRS::operator()(int i, int j) {
    if (i < 0 || static_cast<size_t>(i) >= row_pointers.size() - 1) {
        throw std::out_of_range("CRS row index out of bounds");
    }

    int row_start = static_cast<int>(row_pointers[i]);
    int row_end = static_cast<int>(row_pointers[i + 1]);

    for (int idx = row_start; idx < row_end; ++idx) {
        if (col_indexes[idx] == j) {
            return values[idx];
        }
    }

    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
const T& linalg::Matrix<T>::CRS::operator()(int i, int j) const {
    if (i < 0 || static_cast<size_t>(i) >= row_pointers.size() - 1) {
        throw std::out_of_range("CRS row index out of bounds");
    }

    int row_start = static_cast<int>(row_pointers[i]);
    int row_end = static_cast<int>(row_pointers[i + 1]);

    for (int idx = row_start; idx < row_end; ++idx) {
        if (col_indexes[idx] == j) {
            return values[idx];
        }
    }

    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
T& linalg::Matrix<T>::CRS::operator()(int i) {
    return values[i];
}

template<typename T>
const T& linalg::Matrix<T>::CRS::operator()(int i) const {
    return values[i];
}

template<typename T>
void linalg::Matrix<T>::CRS::set(int i, int j, const T& _val) {
    if (i < 0 || static_cast<size_t>(i) >= row_pointers.size() - 1) {
        throw std::out_of_range("CRS row index out of bounds.");
    }

    int row_start = static_cast<int>(row_pointers[i]);
    int row_end = static_cast<int>(row_pointers[i + 1]);

    for (int idx = row_start; idx < row_end; ++idx) {
        if (col_indexes[idx] == j) {
            if (_val != 0) {
                values[idx] = _val;
            } else {
                // Value on position (i, j) or in our case (idx) now 0 so we need to erase it from the vector
                // idx pointing in position (i, j) of current matrix
                values.erase(values.begin() + idx);
                col_indexes.erase(col_indexes.begin() + idx);

                for (int k = i + 1; k < row_pointers.size(); ++k) {
                    row_pointers[k]--;
                }
            }
            return;
        }
        if (col_indexes[idx] > j) break; // Element not found, should insert before this
    }

    // If we reach here, the element (i, j) doesn't exist yet (meaning 0 at that position)
    if (_val != 0) {
        int insert_pos = row_start;

        while (insert_pos < row_end && col_indexes[insert_pos] < j) insert_pos++;

        col_indexes.insert(col_indexes.begin() + insert_pos, j);
        values.insert(values.begin() + insert_pos, _val);

        // Adjust number of elements in each row starting from the row we added element
        for (int k = i + 1; k < row_pointers.size(); ++k) {
            row_pointers[k]++;
        }
    }
}

template<typename T>
std::string linalg::Matrix<T>::CRS::print() {

    if (row_pointers.size() == 0) {
        std::cerr << "CRS matrix is empty, can't print.\n";
    }

    std::stringstream output;
    output << "[\n";

    int max_cols = 0;

    for (size_t idx = 0; idx < col_indexes.size(); ++idx) {
        if (col_indexes[idx] > max_cols) max_cols = col_indexes[idx];
    }

    max_cols++; // Column index is 0-based, incrementing

    for (size_t row = 0; row < row_pointers.size(); ++row) {
        output << "[";
        size_t col = 0;

        // Traverse through each non-zero element in the row
        for (int idx = row_pointers[row]; idx < row_pointers[row + 1]; ++idx) {
            // Fill zeros for any missing column before the next nonzero symbol
            while (col < col_indexes[idx]) {
                output << "0, ";
                col++;
            }
            output << values[idx] << ", ";
            col++;
        }
        // Fill in trailling zeros for any remaining columns in this row
        while (col < max_cols) {
            output << "0, ";
            col++;
        }

        output << "],\n";
    }
    output << "]";

    std::cout << output.str();
    return output.str();
}

#endif // CRS_MATRIX_DECLARE