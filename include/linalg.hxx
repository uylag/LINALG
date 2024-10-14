//
// Created 2024/10/13 by Akayo Kiyoshima @uylag
//

#ifndef LINEAR_ALGEBRA_HXX
#define LINEAR_ALGEBRA_HXX

#include <vector>
#include <string>
#include <initializer_list>
#include <iostream>
#include <sstream>

class linalg {
public:
    template <typename T>
    class Matrix {
    public:
        // Structs
        struct CRS {
            std::vector<T>                          values;
            std::vector<long long>                  col_indexes;
            std::vector<long long>                  row_pointers;

            std::string print();

            // Matrix-like value receiving/setting
            T &operator()(int i, int j);
            const T& operator()(int i, int j) const;
            void set(int i, int j, const T& value);

            // Values-vector value receiving/setting
            T& operator()(int i);
            const T& operator()(int i) const;
            void set(int i, const T& _val) { values[i] = _val; };
        };

        struct CCS {
            std::vector<T>                          values;
            std::vector<long long>                  col_pointers;
            std::vector<long long>                  row_indexes;

            // Matrix-like value receiving/setting
            T& operator()(int i, int j);
            const T& operator()(int i, int j) const;
            void set(int i, int j, const T& _val);

            // Values-vector value receiving/setting
            T& operator()(int i) { return values[i]; };
            const T& operator()(int i) const { return values[i]; };
            void set(int i, const T& _val) { values[i] = _val; };
        };

        struct COO {
            std::vector<T>                          values;
            std::vector<long long>                  col_indexes;
            std::vector<long long>                  row_indexes;

            // Matrix-like value receiving/setting
            T& operator()(int i, int j);
            const T& operator()(int i, int j) const;
            void set(int i, int j, const T& _val);

            // Values-vector value receiving/setting
            T& operator()(int i) { return values[i]; };
            const T& operator()(int i) const { return values[i]; };
            void set(int i, const T& _val) { values[i] = _val };
        };

        // Constructors
        Matrix();
        Matrix(const Matrix<T>& _oth);

        // Guide for 'sparse' string
        // "CRS" / "CSR"
        // "CCS" / "CSC"
        // "COO"
        // "def"
        // "all"
        explicit Matrix(const std::vector<std::vector<T>>& _oth, const std::string& sparse);
        explicit Matrix(std::initializer_list<std::initializer_list<T>> init_matrix, const std::string& sparse);

        // Destructor
        ~Matrix();

        // Operations
        Matrix<T> operator+();
        Matrix<T> operator-();

        Matrix<T> operator+(const std::vector<T>& _oth) const;
        Matrix<T> operator+(const Matrix<T>& _oth) const;
        Matrix<T> operator+(const long double& scalar) const;

        // Methods
        static CRS init_crs(const std::vector<std::vector<T>> _matrix);
        static CCS init_ccs(const std::vector<std::vector<T>> _matrix);
        static COO init_coo(const std::vector<std::vector<T>> _matrix);

        T& get(int i, int j) {
            if (matrix_state == "def" || matrix_state == "all") return matrix[i][j];
            else if (matrix_state == "CRS")                     return crs_matrix(i, j);
            else if (matrix_state == "CCS")                     return ccs_matrix(i, j);
            else if (matrix_state == "COO")                     return coo_matrix(i, j);
        }

        void set(int i, int j, const T& _val) {
            if (matrix_state == "def") matrix[i][j] = _val;
            else if (matrix_state == "CRS") crs_matrix.set(i, j, _val);
            else if (matrix_state == "CCS") ccs_matrix.set(i, j, _val);
            else if (matrix_state == "COO") coo_matrix.set(i, j, _val);
            else if (matrix_state == "all") {
                crs_matrix.set(i, j, _val);
                ccs_matrix.set(i, j, _val);
                coo_matrix.set(i, j, _val);
                matrix[i][j] = _val;
            }
        }

        std::string print() {
            return crs_matrix.print();
        }

        void update_rows() { rows = matrix.size(); };
        void update_cols() { cols = matrix[0].size(); };

    protected:

    private:
        mutable std::vector<std::vector<T>>     matrix;

        // CRS (Compressed Row Storage) / CSR (Compressed Sparse Row)
        mutable CRS                             crs_matrix;

        // CCS (Compressed Column Storage) / CSC (Compressed Sparse Column)
        mutable CCS                             ccs_matrix;

        // COO (Coordinate List)
        mutable COO                             coo_matrix;

        // `matrix_state`:
        // "CRS"
        // "CCS"
        // "COO"
        // "def"
        // "all"
        std::string                             matrix_state;

        mutable int                             rows;
        mutable int                             cols;
    };
};

#include <cstdint>
#include <stdexcept>

template <typename T>
linalg::Matrix<T>::Matrix() {
    matrix = std::vector<T>(1, std::vector<T>(1));

    crs_matrix.values = std::vector<T>();
    crs_matrix.col_indexes = std::vector<T>();
    crs_matrix.row_pointers = std::vector<T>();

    ccs_matrix.values = std::vector<T>();
    ccs_matrix.col_pointers = std::vector<T>();
    ccs_matrix.row_indexes = std::vector<T>();

    coo_matrix.row_indexes = std::vector<T>();
    coo_matrix.values = std::vector<T>();
    coo_matrix.col_indexes = std::vector<T>();

    matrix_state = "all";
    rows = 1;
    cols = 1;
}

template<typename T>
linalg::Matrix<T>::Matrix(const std::vector<std::vector<T>> &_oth, const std::string &sparse) {
    int row_weight = _oth[0].size();

    for (auto& row: _oth) {
        if (row.size() != row_weight) {
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
}

template<typename T>
linalg::Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init_matrix, const std::string &sparse) {
    auto first_row = init_matrix.begin();
    int row_weight = first_row->size();

    for (const auto& row : init_matrix) {
        if (row.size() != row_weight) {
            std::ostringstream oss;
            oss << "Matrix is not formed: expected dim ("
                << init_matrix.size() << ", " << row_weight << "). Found: ("
                << init_matrix.size() << ", " << row.size() << ").";
            throw std::logic_error(oss.str());
        }
    }

    matrix = std::vector<std::vector<T>>(init_matrix.size());
    matrix_state = sparse;

    size_t row = 0;
    for (const auto& row_list : init_matrix) {
        matrix[row] = std::vector<T>(row_list);
        ++row;
    }

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
}

template<typename T>
linalg::Matrix<T>::Matrix(const linalg::Matrix<T> &_oth) {


}

template<typename T>
linalg::Matrix<T>::~Matrix() {

}

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator+() {
    return linalg::Matrix<T>();
}

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator-() {
    return linalg::Matrix<T>();
}

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator+(const std::vector<T> &_oth) const {
    return linalg::Matrix<T>();
}

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator+(const linalg::Matrix<T> &_oth) const {
    return linalg::Matrix<T>();
}

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator+(const long double &scalar) const {
    return linalg::Matrix<T>();
}

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
    std::stringstream output;
    output << "[\n";
    int max = 0;

    for (size_t idx = 0; idx < col_indexes.size(); ++idx) {
        if (col_indexes[idx] > max) max = col_indexes[idx];
    }

    for (size_t pos = 1; pos < row_pointers.size(); ++pos) {
        output << "[";
        for (int idx = row_pointers[pos - 1]; idx < row_pointers[pos]; ++idx) {
            int zi = (idx == row_pointers[pos - 1] ? 0 : col_indexes[idx - 1]);
            while (zi < col_indexes[idx]) output << "0, ";
            output << values[idx] << ", ";
        }
        output << "],\n";
    }
    output << "]";

    std::cout << output.str();
    return output.str();
}

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

    for (size_t j = 0; j < cols; ++j) {
        int cnt = 0; // Count of all nonzero elements in the current column

        for (size_t i = 0; i < rows; ++i) {
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
    if (j < 0 || static_cast<size_t>(j) >= col_pointers.size() - 1) {
        throw std::out_of_range("CCS column index out of bounds.\n");
    }

    int col_start = col_pointers[j];
    int col_end = col_pointers[j + 1];

    for (size_t idx = col_start; idx < col_end; ++idx) {
        if (row_indexes[idx] = i) {
            return values[idx];
        }
    }

    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
const T& linalg::Matrix<T>::CCS::operator()(int i, int j) const {
    if (j < 0 || static_cast<size_t>(j) >= col_pointers.size() - 1) {
        throw std::out_of_range("CCS column index out of bounds.\n");
    }

    const static T def = static_cast<T>(0);
    return def;
}

template<typename T>
void linalg::Matrix<T>::CCS::set(int i, int j, const T& _val) {

}

template<typename T>
typename linalg::Matrix<T>::COO linalg::Matrix<T>::init_coo(const std::vector<std::vector<T>> _matrix) {
    COO mtx;
    return mtx;
}

template<typename T>
T& linalg::Matrix<T>::COO::operator()(int i, int j) {
    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
const T& linalg::Matrix<T>::COO::operator()(int i, int j) const{
    static T def = static_cast<T>(0);
    return def;
}

template<typename T>
void linalg::Matrix<T>::COO::set(int i, int j, const T& _val) {

}

#endif //LINEAR_ALGEBRA_HXX

