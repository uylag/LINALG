#pragma once

#ifndef COO_MATRIX_DECLARE
#define COO_MATRIX_DECLARE

#include <iostream>
#include "../../include/linalg/matrix.hxx"

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

#endif // COO_MATRIX_DECLARE