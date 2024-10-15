#pragma once

#ifndef DEFINITION_MATRIX_HXX
#define DEFINITION_MATRIX_HXX

#include <vector>
#include <string>
#include <initializer_list>
#include "linalg.hxx"

namespace linalg {

    template<typename T>
    class Matrix {
    public:
        // Structs
        struct CRS;
        struct CCS;
        struct COO;

        // Constructors
        Matrix();
        Matrix(const Matrix<T>& _oth);

        // Guide for 'sparse' string
        // "CRS" / "CSR"
        // "CCS" / "CSC"
        // "COO"
        // "def"
        // "all"
        Matrix(const std::vector<std::vector<T>>& _oth, const std::string& sparse);
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

        // Matrix<T> operator/(const std::vector<std::vector<T>>& _oth) const;
        // Matrix<T> operator/(const Matrix<T>& _oth) const;
        // Matrix<T> operator/(const long double& scalar) const;

        // Methods
        static CRS init_crs(const std::vector<std::vector<T>> _matrix);
        static CCS init_ccs(const std::vector<std::vector<T>> _matrix);
        static COO init_coo(const std::vector<std::vector<T>> _matrix);

        T& get(int i, int j);

        void set(int i, int j, const T& _val);

        std::string print();

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
        // "CRS" / "CCS" / "COO" / "def" / "all"
        std::string                             matrix_state;

        mutable int                             rows;
        mutable int                             cols;
    };

    template<typename T>
    struct Matrix<T>::CRS {
        std::vector<T>                          values;
        std::vector<long long>                  col_indexes;
        std::vector<long long>                  row_pointers;

        std::string print();

        // Matrix-like value receiving/setting
        T& operator()(int i, int j);
        const T& operator()(int i, int j) const;
        void set(int i, int j, const T& value);
    };

    template<typename T>
    struct Matrix<T>::CCS {
        std::vector<T>                          values;
        std::vector<long long>                  col_pointers;
        std::vector<long long>                  row_indexes;

        // Matrix-like value receiving/setting
        T& operator()(int i, int j);
        const T& operator()(int i, int j) const;
        void set(int i, int j, const T& _val);
    };

    template<typename T>
    struct Matrix<T>::COO {
        std::vector<T>                          values;
        std::vector<long long>                  col_indexes;
        std::vector<long long>                  row_indexes;

        // Matrix-like value receiving/setting
        T& operator()(int i, int j);
        const T& operator()(int i, int j) const;
        void set(int i, int j, const T& _val);
    };

}

#include "../../src/matrix/matrix_constructors.hxx"
#include "../../src/matrix/matrix_operations.hxx"
#include "../../src/matrix/matrix_operators.hxx"
#include "../../src/sparsemtxes/crs.hxx"
#include "../../src/sparsemtxes/ccs.hxx"
#include "../../src/sparsemtxes/coo.hxx"

#endif // DEFINITION_MATRIX_HXX