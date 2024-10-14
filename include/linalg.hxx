//
// Created 2024/10/13 by Akayo Kiyoshima @uylag
//

#ifndef LINEAR_ALGEBRA_HXX
#define LINEAR_ALGEBRA_HXX

#include <vector>
#include <string>
#include <initializer_list>

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
            void set(int i, const T& _val) { values[i] = _val; };
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

        Matrix<T> operator+(const std::vector<std::vector<T>>& _oth) const;
        Matrix<T> operator+(const Matrix<T>& _oth) const;
        Matrix<T> operator+(const long double& scalar) const;

        Matrix<T> operator-(const std::vector<std::vector<T>>& _oth) const;
        Matrix<T> operator-(const Matrix<T>& _oth) const;
        Matrix<T> operator-(const long double& scalar) const;

        Matrix<T> operator*(const std::vector<std::vector<T>>& _oth) const;
        Matrix<T> operator*(const Matrix<T>& _oth) const;
        Matrix<T> operator*(const long double& scalar) const;

        Matrix<T> operator/(const std::vector<std::vector<T>>& _oth) const;
        Matrix<T> operator/(const Matrix<T>& _oth) const;
        Matrix<T> operator/(const long double& scalar) const;

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

#include "../src/linalg.cxx"

#endif //LINEAR_ALGEBRA_HXX

