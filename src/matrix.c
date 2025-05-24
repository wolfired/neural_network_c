#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void matrix_new(Matrix** ins, size_t row, size_t col, float* raw) {
    assert(NULL != ins);
    if(NULL == ins) { return; }

    assert(NULL == *ins);
    if(NULL != *ins) { return; }

    *ins = (Matrix*)malloc(sizeof(Matrix));
    matrix_ctor(*ins, row, col, raw);
}

void matrix_del(Matrix** ins) {
    if(NULL == ins) { return; }

    if(NULL == *ins) { return; }

    matrix_dtor(*ins);
    free(*ins);
    *ins = NULL;
}

void matrix_ctor(Matrix* thiz, size_t row, size_t col, float* raw) {
    assert(NULL != thiz);
    if(NULL == thiz) { return; }

    thiz->row = row;
    thiz->col = col;
    if(NULL == raw && 0 < thiz->row && 0 < thiz->col) { raw = (float*)calloc(thiz->row * thiz->col, sizeof(float)); }
    thiz->raw = raw;
}

void matrix_dtor(Matrix* thiz) {
    if(NULL == thiz) { return; }

    if(NULL != thiz->raw) {
        free(thiz->raw);
        thiz->raw = NULL;
    }
}

void matrix_add(Matrix* lhs, Matrix* rhs, Matrix** ins) {
    assert(NULL != lhs && NULL != rhs);
    if(NULL == lhs || NULL == rhs) { return; }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if(NULL == lhs->raw || NULL == rhs->raw) { return; }

    assert(lhs->row == rhs->row && lhs->col == rhs->col);
    if(lhs->row != rhs->row || lhs->col != rhs->col) { return; }

    assert(NULL != ins);
    if(NULL == ins) { return; }

    if(NULL == *ins) { matrix_new(ins, lhs->row, lhs->col, NULL); }

    size_t count = lhs->row * lhs->col;

    for(size_t i = 0; i < count; ++i) { (*ins)->raw[i] = lhs->raw[i] + rhs->raw[i]; }
}

inline void matrix_add_assign(Matrix* lhs, Matrix* rhs) {
    matrix_add(lhs, rhs, &lhs);
}

void matrix_sub(Matrix* lhs, Matrix* rhs, Matrix** ins) {
    assert(NULL != lhs && NULL != rhs);
    if(NULL == lhs || NULL == rhs) { return; }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if(NULL == lhs->raw || NULL == rhs->raw) { return; }

    assert(lhs->row == rhs->row && lhs->col == rhs->col);
    if(lhs->row != rhs->row || lhs->col != rhs->col) { return; }

    assert(NULL != ins);
    if(NULL == ins) { return; }

    if(NULL == *ins) { matrix_new(ins, lhs->row, lhs->col, NULL); }

    size_t count = lhs->row * lhs->col;

    for(size_t i = 0; i < count; ++i) { (*ins)->raw[i] = lhs->raw[i] - rhs->raw[i]; }
}

inline void matrix_sub_assign(Matrix* lhs, Matrix* rhs) {
    matrix_sub(lhs, rhs, &lhs);
}

void matrix_mul(Matrix* lhs, Matrix* rhs, Matrix** ins) {
    assert(NULL != lhs && NULL != rhs);
    if(NULL == lhs || NULL == rhs) { return; }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if(NULL == lhs->raw || NULL == rhs->raw) { return; }

    assert(lhs->col == rhs->row);
    if(lhs->col != rhs->row) { return; }

    assert(NULL != ins);
    if(NULL == ins) { return; }

    if(NULL == *ins) { matrix_new(ins, lhs->row, rhs->col, NULL); }

    for(size_t r = 0; r < lhs->row; ++r) {
        for(size_t c = 0; c < rhs->col; ++c) {
            size_t index = r * lhs->col + c;
            size_t rs    = r * lhs->col;
            for(size_t m = 0, ms = 0; m < lhs->col; ++m, ms += rhs->col) { (*ins)->raw[index] += lhs->raw[rs + m] * rhs->raw[ms + c]; }
        }
    }
}

void matrix_mul_assign(Matrix* lhs, Matrix* rhs) {
    Matrix* ins = NULL;
    matrix_mul(lhs, rhs, &ins);

    matrix_move_raw(ins, lhs);

    matrix_del(&ins);
}

void matrix_mul_scalar(Matrix* lhs, float scalar, Matrix** ins) {
    assert(NULL != lhs);
    if(NULL == lhs) { return; }

    assert(NULL != lhs->raw);
    if(NULL == lhs->raw) { return; }

    assert(NULL != ins);
    if(NULL == ins) { return; }

    if(NULL == *ins) { matrix_new(ins, lhs->row, lhs->col, NULL); }

    size_t count = lhs->row * lhs->col;

    for(size_t i = 0; i < count; ++i) { (*ins)->raw[i] = lhs->raw[i] * scalar; }
}

void matrix_mul_scalar_assign(Matrix* lhs, float scalar) {
    matrix_mul_scalar(lhs, scalar, &lhs);
}

void matrix_transpose(Matrix* thiz, Matrix** ins) {
    assert(NULL != thiz);
    if(NULL == thiz) { return; }

    assert(NULL != thiz->raw);
    if(NULL == thiz->raw) { return; }

    assert(NULL != ins);
    if(NULL == ins) { return; }

    if(NULL == *ins) { matrix_new(ins, thiz->col, thiz->row, NULL); }

    for(size_t r = 0; r < thiz->row; ++r) {
        size_t rs = r * thiz->col;
        for(size_t c = 0, cs = 0; c < thiz->col; c++, cs += (*ins)->col) { (*ins)->raw[cs + r] = thiz->raw[rs + c]; }
    }
}

void matrix_transpose_assign(Matrix* thiz) {
    Matrix* ins = NULL;
    matrix_transpose(thiz, &ins);

    matrix_move_raw(ins, thiz);

    matrix_del(&ins);
}

void matrix_move_raw(Matrix* src, Matrix* dst) {
    assert(NULL != src);
    if(NULL == src) { return; }

    assert(NULL != dst);
    if(NULL == dst) { return; }

    matrix_dtor(dst);
    matrix_ctor(dst, src->row, src->col, src->raw);

    src->raw = NULL;
}

void matrix_set_all(Matrix* thiz, float value) {
    size_t count = thiz->row * thiz->col;
    for(size_t i = 0; i < count; ++i) { thiz->raw[i] = value; }
}

void matrix_set_one(Matrix* thiz, size_t r, size_t c, float value) {
    assert(NULL != thiz);
    if(NULL == thiz) { return; }

    assert(NULL != thiz->raw);
    if(NULL == thiz->raw) { return; }

    assert(0 <= r && r < thiz->row && 0 <= c && c < thiz->col);
    if(0 > r || r >= thiz->row || 0 > c || c >= thiz->col) { return; }

    size_t index = r * thiz->col + c;

    thiz->raw[index] = value;
}

void matrix_print(Matrix* thiz) {
    assert(NULL != thiz);
    if(NULL == thiz) { return; }

    assert(NULL != thiz->raw);
    if(NULL == thiz->raw) { return; }

    for(size_t r = 0; r < thiz->row; ++r) {
        size_t rs = r * thiz->col;
        for(size_t c = 0; c < thiz->col; ++c) {
            printf("%8.3f", thiz->raw[rs + c]);
            if(c < thiz->col - 1) { printf(" "); }
        }
        printf("\n");
    }
}
