#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct Matrix_ {
    size_t row;
    size_t col;
    float* raw;
} Matrix;

typedef struct MatrixView_ {
    Matrix const* const* ins_mat;
} MatrixView;

void matrix_new(Matrix** ins, size_t row, size_t col, float* raw);
void matrix_del(Matrix** ins);

void matrix_ctor(Matrix* const thiz, size_t row, size_t col, float* raw);
void matrix_dtor(Matrix* const thiz);

void matrix_add(Matrix const* const lhs, Matrix const* const rhs, Matrix** ins);
void matrix_add_assign(Matrix* lhs, Matrix* rhs);
void matrix_sub(Matrix* lhs, Matrix* rhs, Matrix** ins);
void matrix_sub_assign(Matrix* lhs, Matrix* rhs);
void matrix_mul(Matrix* lhs, Matrix* rhs, Matrix** ins);
void matrix_mul_assign(Matrix* lhs, Matrix* rhs);
void matrix_mul_scalar(Matrix* lhs, float scalar, Matrix** ins);
void matrix_mul_scalar_assign(Matrix* lhs, float scalar);
void matrix_transpose(Matrix* thiz, Matrix** ins);
void matrix_transpose_assign(Matrix* thiz);

void matrix_move_raw(Matrix* src, Matrix* dst);

void matrix_set_all(Matrix* thiz, float value);
void matrix_set_one(Matrix* thiz, size_t r, size_t c, float value);
void matrix_print(Matrix const* const thiz);

// void matrix_view_new(MatrixView** ins, Matrix const* const*);
// void matrix_view_del(MatrixView** ins);

#endif
