#include <iostream>
#include "include/linalg.hxx"

int main() {
    linalg::Matrix<long double> matrix({
        {1, 0, 3},
        {4, 5, 7},
        {0, 0, 1},
        {0, 0, 4}
    }, "CRS");

    matrix.set(0, 1, 5);
    matrix.print();
}