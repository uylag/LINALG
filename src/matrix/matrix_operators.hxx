#pragma once

#ifndef MATRIX_OPERATIONS_HXX
#define MATRIX_OPERATIONS_HXX

#include "../../include/linalg/matrix.hxx"

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator+() {
    return linalg::Matrix<T>();
}

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator-() {
    return linalg::Matrix<T>();
}

template<typename T>
linalg::Matrix<T> linalg::Matrix<T>::operator+(const std::vector<std::vector<T>> &_oth) const {
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

#endif // MATRIX_OPERATIONS_HXX