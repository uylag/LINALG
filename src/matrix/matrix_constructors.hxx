#pragma once

#ifndef MATRIX_CONSTRUCTORS_HXX
#define MATRIX_CONSTRUCTORS_HXX

#include "../../include/linalg/matrix.hxx"
#include <sstream>
#include <iostream>
#include <vector>

#pragma region Matrix Constructors

template <typename T>
linalg::Matrix<T>::Matrix() {
    auto logger = spdlog::get("global_logger");

    if (logger) {
        logger->debug("Default constructor called, initializing empty matrix.");
        logger->flush();  // Ensure logs are written to the file for testing
    }

    matrix = std::vector<std::vector<T>>(1, std::vector<T>(1));

    crs_matrix.values = std::vector<T>();
    crs_matrix.col_indexes = std::vector<long long>();
    crs_matrix.row_pointers = std::vector<long long>();

    ccs_matrix.values = std::vector<T>();
    ccs_matrix.col_pointers = std::vector<long long>();
    ccs_matrix.row_indexes = std::vector<long long>();

    coo_matrix.row_indexes = std::vector<long long>();
    coo_matrix.values = std::vector<T>();
    coo_matrix.col_indexes = std::vector<long long>();

    matrix_state = "all";
    rows = 1;
    cols = 1;
}

template<typename T>
linalg::Matrix<T>::Matrix(const std::vector<std::vector<T>> &_oth, const std::string &sparse) {
    auto logger = spdlog::get("global_logger");

    if (logger) {
        logger->debug("Matrix constructor called with sparse format: {}", sparse);
        logger->flush();
    }
    else {
        std::cerr << "Can't access logger with name global_logger\n";
    }
   
    int row_weight = _oth[0].size();
    logger->debug("Initialized `row_weight` with value [{}]", row_weight);

    for (auto& row: _oth) {
        if (row.size() != row_weight) {
            logger->error("Matrix is not formed: expected dim (" +
            std::to_string(_oth.size()) + ", " + std::to_string(row_weight) + "). Found: (" +
            std::to_string(_oth.size()) + ", " + std::to_string(row.size()) + ").\n");

            throw std::logic_error("Matrix is not formed: expected dim (" +
            std::to_string(_oth.size()) + ", " + std::to_string(row_weight) + "). Found: (" +
            std::to_string(_oth.size()) + ", " + std::to_string(row.size()) + ").\n");
        }
    }

    matrix = _oth;
    matrix_state = sparse;

    if (sparse == "CRS") {
        crs_matrix = linalg::Matrix<T>::init_crs(matrix);
    }
    else if (sparse == "CCS") {
        ccs_matrix = linalg::Matrix<T>::init_ccs(matrix);
    }
    else if (sparse == "COO") {
        coo_matrix = linalg::Matrix<T>::init_coo(matrix);
    }
    else if (sparse == "def");
    else if (sparse == "all") {
        crs_matrix = linalg::Matrix<T>::init_crs(matrix);
        ccs_matrix = linalg::Matrix<T>::init_ccs(matrix);
        coo_matrix = linalg::Matrix<T>::init_coo(matrix);
    }

    update_rows();
    update_cols();

    logger->debug("Matrix dimensionality updated to ({}, {})", rows, cols);
    logger->info("Matrix created. Dimensionality ({}, {})", rows, cols);

    logger->flush();
}

template<typename T>
linalg::Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init_matrix, const std::string &sparse) {
    auto logger = spdlog::get("global_logger");

    if (logger) {
        logger->debug("Matrix constructor called with sparse format: {}", sparse);
    }
    else {
        std::cerr << "Can't access logger with name global_logger\n";
    }
    
    auto first_row = init_matrix.begin();
    int row_weight = first_row->size();

    for (const auto& row : init_matrix) {
        if (row.size() != static_cast<size_t>(row_weight)) {
            std::ostringstream oss;
            oss << "Matrix is not formed: expected dim ("
                << init_matrix.size() << ", " << row_weight << "). Found: ("
                << init_matrix.size() << ", " << row.size() << ").";

            logger->error(oss.str());
            throw std::logic_error(oss.str());
        }
    }

    matrix = std::vector<std::vector<T>>(init_matrix.size());
    logger->debug("Variable matrix defined with constant count of rows: {}", init_matrix.size());
    matrix_state = sparse;

    size_t row = 0;
    for (const auto& row_list : init_matrix) {
        matrix[row] = std::vector<T>(row_list);
        ++row;
    }

    logger->debug("Matrix defined successfully");

    if (sparse == "CRS") {
        crs_matrix = linalg::Matrix<T>::init_crs(matrix);
    }
    else if (sparse == "CCS") {
        ccs_matrix = linalg::Matrix<T>::init_ccs(matrix);
    }
    else if (sparse == "COO") {
        coo_matrix = linalg::Matrix<T>::init_coo(matrix);
    }
    else if (sparse == "def");
    else if (sparse == "all") {
        crs_matrix = linalg::Matrix<T>::init_crs(matrix);
        ccs_matrix = linalg::Matrix<T>::init_ccs(matrix);
        coo_matrix = linalg::Matrix<T>::init_coo(matrix);
    }

    update_rows();
    update_cols();

    logger->debug("Matrix dimensionality updated to ({}, {})", rows, cols);
    logger->info("Matrix created. Dimensionality ({}, {})", rows, cols);

    logger->flush();
}

template<typename T>
linalg::Matrix<T>::Matrix(const linalg::Matrix<T> &_oth) {
    auto logger = spdlog::get("global_logger");

}

template<typename T>
linalg::Matrix<T>::~Matrix() {
    auto logger = spdlog::get("global_logger");
}

#pragma endregion

#endif // MATRIX_CONSTRUCTORS_HXX
