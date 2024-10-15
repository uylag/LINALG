#pragma once

#ifndef MATRIX_OPERATIONS_HXX
#define MATRIX_OPERATIONS_HXX

#include "../../include/linalg/matrix.hxx"

#pragma region Matrix Operations and Methods

template<typename T>
T& linalg::Matrix<T>::get(int i, int j) {
    auto logger = spdlog::get("global_logger");

    if (matrix_state == "def" || matrix_state == "all") return matrix[i][j];
    else if (matrix_state == "CRS")                     return crs_matrix(i, j);
    else if (matrix_state == "CCS")                     return ccs_matrix(i, j);
    else if (matrix_state == "COO")                     return coo_matrix(i, j);
    
    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
void linalg::Matrix<T>::set(int i, int j, const T& _val) {
    auto logger = spdlog::get("global_logger");
    logger->debug("Main matrix function: Starting setting value {}, sparse mode: {}", _val, matrix_state);

    if (matrix_state == "def") matrix[i][j] = _val;
    else if (matrix_state == "CRS") crs_matrix.set(i, j, _val);
    else if (matrix_state == "CCS") ccs_matrix.set(i, j, _val);
    else if (matrix_state == "COO") coo_matrix.set(i, j, _val);
    else if (matrix_state == "all") 
    {
                                    crs_matrix.set(i, j, _val);
                                    ccs_matrix.set(i, j, _val);
                                    coo_matrix.set(i, j, _val);
                                    matrix[i][j] = _val;
    }
}

template<typename T>
std::string linalg::Matrix<T>::print() {
    auto logger = spdlog::get("global_logger");

    return crs_matrix.print();
}

#pragma endregion

#endif // MATRIX_OPERATIONS_HXX