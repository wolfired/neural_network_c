#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int main(int argc, char** argv) {
    Matrix* m0 = NULL;
    matrix_new(&m0, 3, 2, NULL);
    matrix_set_one(m0, 0, 0, 0.0);
    matrix_set_one(m0, 0, 1, 0.1);
    // matrix_set_one(m0, 0, 2, 0.2);
    matrix_set_one(m0, 1, 0, 1.0);
    matrix_set_one(m0, 1, 1, 1.1);
    // matrix_set_one(m0, 1, 2, 1.2);
    matrix_set_one(m0, 2, 0, 2.0);
    matrix_set_one(m0, 2, 1, 2.1);
    // matrix_set_one(m0, 2, 2, 2.2);

    Matrix* m1 = NULL;
    matrix_mul_scalar_assign(m0, 10.0);

    matrix_print(m0);

    matrix_del(&m0);
    matrix_del(&m1);

    return EXIT_SUCCESS;
}
