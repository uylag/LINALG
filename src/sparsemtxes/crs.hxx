#pragma once

#ifndef CRS_MATRIX_DECLARE
#define CRS_MATRIX_DECLARE

#include "../../include/linalg/matrix.hxx"
#include <iostream>
#include <sstream>
#include <string>

template<typename T>
typename linalg::Matrix<T>::CRS linalg::Matrix<T>::init_crs(const std::vector<std::vector<T>> _matrix) {
    auto logger = spdlog::get("global_logger");
    CRS mtx;

    logger->debug("Starting initialization of CRS sparse matrix");

    if (_matrix.empty()) {
        logger->warn("Error during `init_crs` operation on line {}: Empty matrix.\n", __LINE__);
        std::cerr << "Error during `init_crs` operation on line " << __LINE__ << ": Empty matrix.\n";
        return mtx;
    }

    mtx.row_pointers.push_back(0); // Initializing first element of pointers with zero

    long long rows = _matrix.size();
    long long cols = _matrix[0].size();

    for (long long i = 0; i < rows; ++i) {
        int cnt = 0; // Number of nonzero elements in row
        for (long long j = 0; j < cols; ++j) {
            if (_matrix[i][j] != 0) {
                mtx.values.push_back(_matrix[i][j]);
                mtx.col_indexes.push_back(j);
                cnt++;
            }
        }
        logger->debug("Successfully pushed values on iteration index[{}]. Number of elements in row-{}", i, cnt);
        mtx.row_pointers.push_back(
                mtx.row_pointers.back() + cnt
        ); // ROW_INDEX[i] = ROW_INDEX[i-1] + nzc; (nonzero count)
        logger->debug("Pushed back `row_pointers` value-{}", (mtx.row_pointers.back() + cnt));
    }

    logger->info("CRS sparse matrix created successfully. Returning structure");

    return mtx;
}

template<typename T>
T& linalg::Matrix<T>::CRS::operator()(int i, int j) {
    auto logger = spdlog::get("global_logger");

    if (i < 0 || i >= row_pointers.size() - 1) {
        logger->error("CRS row index-{} out of bounds", i);
        throw std::out_of_range("CRS row index out of bounds");
    }

    int row_start = row_pointers[i];
    int row_end = row_pointers[i + 1];

    for (int idx = row_start; idx < row_end; ++idx) {
        if (col_indexes[idx] == j) {
            logger->debug("CRS: Successfully found value at position ({}, {})", i, j);
            return values[idx];
        }
    }

    logger->debug("CRS: Value is zero at position ({}, {}). Returning", i, j);
    
    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
const T& linalg::Matrix<T>::CRS::operator()(int i, int j) const {
    auto logger = spdlog::get("global_logger");

    if (i < 0 || i >= row_pointers.size() - 1) {
        logger->error("CRS row index-{} out of bounds", i);
        throw std::out_of_range("CRS row index out of bounds");
    }

    int row_start = row_pointers[i];
    int row_end = row_pointers[i + 1];

    for (int idx = row_start; idx < row_end; ++idx) {
        if (col_indexes[idx] == j) {
            logger->debug("CRS: Successfully found value at position ({}, {})", i, j);
            return values[idx];
        }
    }

    logger->debug("CRS: Value is zero at position ({}, {}). Returning", i, j);
    
    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
void linalg::Matrix<T>::CRS::set(int i, int j, const T& _val) {
    auto logger = spdlog::get("global_logger");

    logger->debug("Start setting value {} at position ({}, {})", _val, i, j);

    if (i < 0 || i >= row_pointers.size() - 1) {
        logger->error("CRS row index out of bounds.");
        throw std::out_of_range("CRS row index out of bounds.");
    }

    int row_start = row_pointers[i];
    int row_end = row_pointers[i + 1];

    for (int idx = row_start; idx < row_end; ++idx) {
        if (col_indexes[idx] == j) {
            if (_val != 0) {
                values[idx] = _val;
            } else {
                logger->debug("Value at position idx: {}. Setting it with 0", values[idx]);

                // Value on position (i, j) or in our case (idx) now 0 so we need to erase it from the vector
                // idx pointing in position (i, j) of current matrix
                values.erase(values.begin() + idx);
                col_indexes.erase(col_indexes.begin() + idx);

                for (long long k = i + 1; k < row_pointers.size(); ++k) {
                    row_pointers[k]--;
                }
            }
            logger->info("CRS: Value set in position: ({}, {})", i, j);
            logger->flush();
            return;
        }
        if (col_indexes[idx] > j) break; // Element not found, should insert before this
    }

    // If we reach here, the element (i, j) doesn't exist yet (meaning 0 at that position)
    if (_val != 0) {
        int insert_pos = row_start;

        logger->debug("Element yet not exist (0 in position)");
        logger->debug("Starting `while` loop. Catching infinity-loop error in log");

        while (insert_pos < row_end && col_indexes[insert_pos] < j) {
            logger->debug(
                "\nIteration:\n" 
                "Insert position selector: {}\n"
                "Row end: {}\n"
                "col_indexes[insert_pos]: {}\n\n",
                insert_pos,
                row_end,
                col_indexes[insert_pos]
            );

            insert_pos++;
        }

        col_indexes.insert(col_indexes.begin() + insert_pos, j);
        values.insert(values.begin() + insert_pos, _val);

        // Adjust number of elements in each row starting from the row we added element
        for (long long k = i + 1; k < row_pointers.size(); ++k) {
            row_pointers[k]++;
        }
    }

    logger->info("CRS: Value set in position: ({}, {})", i, j);
    logger->flush();
}

template<typename T>
std::string linalg::Matrix<T>::CRS::print() {
    auto logger = spdlog::get("global_logger");

    logger->debug("Start printing CRS matrix");

    if (row_pointers.size() == 0) {
        logger->error("CRS matrix is empty, can't print");
        logger->flush();

        std::cerr << "CRS matrix is empty, can't print.\n";
        return "";
    }

    std::stringstream output;
    output << "[\n";

    long long max_cols = 0;

    for (long long idx = 0; idx < col_indexes.size(); ++idx) {
        logger->debug(
            "\nCurrent idx: {}\n"
            "Current max_cols: {}\n"
            "Current col_indexes[idx]: {}\n"
            "Current col_indexes.size(): {}\n",
            idx,
            max_cols,
            col_indexes[idx],
            col_indexes.size()
        );

        if (col_indexes[idx] > max_cols) max_cols = col_indexes[idx];
    }

    std::stringstream temp;
    temp << "Col_indexes: [";

    for (int i = 0; i < col_indexes.size(); ++i) {
        temp << "" << col_indexes[i] << " ";
    }

    temp << "]";

    logger->error(temp.str());

    max_cols++; // Column index is 0-based, incrementing

    logger->debug("Max_cols variable value {}", max_cols);
    logger->flush();

    for (long long row = 0; row < row_pointers.size() - 1; ++row) {
        output << "[";
        long long col = 0;
        logger->debug("Iterating for loop - row {} from {}", row, row_pointers.size());

        // Traverse through each non-zero element in the row
        for (long long idx = row_pointers[row]; idx < row_pointers[row + 1]; ++idx) {
            logger->debug("idx: {} ; max: {}", idx, row_pointers[row + 1]);
            logger->debug("Starting `while` loop: iterating through all zero elements");

            if (idx >= col_indexes.size()) {
                logger->error("Matrix incorrect, column doesn't exist");
                std::cerr << "Matrix incorrect, column doesn't exist\n";

                return "";
            }

            // Fill zeros for any missing column before the next nonzero symbol
            while (col < col_indexes[idx]) {
                logger->debug(
                    "\nIteration:\n"
                    "Current column: {}\n"
                    "Current idx: {}\n"
                    "Current col_indexes[idx]: {}\n"
                    "Current row: {}\n"
                    "Current row_pointers[row]: {}\n"
                    "Current row_pointers[row + 1]: {}\n",
                    col,
                    idx,
                    col_indexes[idx],
                    row,    
                    row_pointers[row],
                    row_pointers[row + 1]
                );

                output << "0, ";
                col++;
            }
            output << values[idx] << ", ";
            col++;

            if (col > 20) {
                logger->error("Column `col` too big - {}", col);
                return "";
            }
        }
        
        logger->debug("Starting `while` loop, trailling zeros for any remaining column");

        // Fill in trailling zeros for any remaining columns in this row
        while (col < max_cols) {
            logger->debug(
                "\nIteration:"
                "Current column: {}"
                "Current max_cols: {}",
                col,
                max_cols
            );

            output << "0, ";
            col++;

            if (col > 20) {
                logger->error("Column `col` too big - {}", col);
                return "";
            }
        }

        output << "],\n";
    }
    output << "]";

    logger->info("Matrix:\n" + output.str());
    return output.str();
}

#endif // CRS_MATRIX_DECLARE