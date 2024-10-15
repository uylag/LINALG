#pragma once

#ifndef CCS_MATRIX_DECLARE
#define CCS_MATRIX_DECLARE

#include <iostream>
#include "../../include/linalg/matrix.hxx"

template<typename T>
typename linalg::Matrix<T>::CCS linalg::Matrix<T>::init_ccs(const std::vector<std::vector<T>> _matrix) {
    CCS mtx;

    if (_matrix.empty()) {
        std::cerr << "Error during `init_ccs` operation on line " << __LINE__ << ": Empty matrix.\n";
        return mtx;
    }
    // Initialize column pointers vector with the first entry being 0
    mtx.col_pointers.push_back(0);

    int rows = _matrix.size();
    int cols = _matrix[0].size();

    for ( long long j = 0; j < cols; ++j) {
        int cnt = 0; // Count of all nonzero elements in the current column

        for ( long long i = 0; i < rows; ++i) {
            if (_matrix[i][j] != 0) {
                mtx.row_indexes.push_back(i);
                mtx.values.push_back(_matrix[i][j]);
                cnt++;
            }
        }

        mtx.col_pointers.push_back(mtx.col_pointers.back() + cnt);  // COL_INDEX[i] = COL_INDEX[i-1] + nnz (nonzero count)
    }

    return mtx;
}

template<typename T>
T& linalg::Matrix<T>::CCS::operator()(int i, int j) {
    if (j < 0 || j >= col_pointers.size() - 1) {
        throw std::out_of_range("CCS column index out of bounds.\n");
    }

    int col_start = col_pointers[j];
    int col_end = col_pointers[j + 1];

    for ( long long idx = col_start; idx < col_end; ++idx) {
        if (row_indexes[idx] == i) {
            return values[idx];
        }
    }

    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
const T& linalg::Matrix<T>::CCS::operator()(int i, int j) const {
    if (j < 0 || j >= col_pointers.size() - 1) {
        throw std::out_of_range("CCS column index out of bounds.\n");
    }

    int col_start = col_pointers[j];
    int col_end = col_pointers[j + 1];

    for (long long idx = col_start; idx < col_end; ++idx) {
        if (row_indexes[idx] == i) {
            return values[idx];
        }
    }

    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
void linalg::Matrix<T>::CCS::set(int i, int j, const T& _val) {
    (void)sizeof(i);
    (void)sizeof(j);
    (void)sizeof(_val);
}

#endif // CCS_MATRIX_DECLARE