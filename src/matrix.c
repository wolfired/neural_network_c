#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "matrix.h"

void matrix_new(Matrix **ins, size_t row, size_t col, float *raw)
{
    assert(NULL != ins);
    if (NULL == ins)
    {
        return;
    }

    assert(NULL == *ins);
    if (NULL != *ins)
    {
        return;
    }

    *ins = (Matrix *)malloc(sizeof(Matrix));
    matrix_ctor(*ins, row, col, raw);
}

void matrix_del(Matrix **ins)
{
    if (NULL == ins)
    {
        return;
    }

    if (NULL == *ins)
    {
        return;
    }

    matrix_dtor(*ins);
    *ins = NULL;
}

void matrix_ctor(Matrix *thiz, size_t row, size_t col, float *raw)
{
    assert(NULL != thiz);
    if (NULL == thiz)
    {
        return;
    }

    thiz->row = row;
    thiz->col = col;
    if (NULL == raw)
    {
        if (0 != thiz->row && 0 != thiz->col)
        {
            thiz->raw = (float *)malloc(sizeof(float) * thiz->row * thiz->col);
        }
    }
    else
    {
        thiz->raw = raw;
    }
}

void matrix_dtor(Matrix *thiz)
{
    if (NULL == thiz)
    {
        return;
    }

    if (NULL == thiz->raw)
    {
        free(thiz->raw);
        thiz->raw = NULL;
    }
}

void matrix_add(Matrix *lhs, Matrix *rhs, Matrix **ins)
{
    assert(NULL != lhs && NULL != rhs);
    if (NULL == lhs || NULL == rhs)
    {
        return;
    }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if (NULL == lhs->raw || NULL == rhs->raw)
    {
        return;
    }

    assert(lhs->row == rhs->row && lhs->col == rhs->col);
    if (lhs->row != rhs->row || lhs->col != rhs->col)
    {
        return;
    }

    assert(NULL != ins);
    if (NULL == ins)
    {
        return;
    }

    matrix_new(ins, lhs->row, lhs->col, NULL);

    size_t count = lhs->row * lhs->col;

    for (size_t i = 0; i < count; ++i)
    {
        (*ins)->raw[i] = lhs->raw[i] + rhs->raw[i];
    }
}

void matrix_add_assign(Matrix *lhs, Matrix *rhs)
{
    assert(NULL != lhs && NULL != rhs);
    if (NULL == lhs || NULL == rhs)
    {
        return;
    }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if (NULL == lhs->raw || NULL == rhs->raw)
    {
        return;
    }

    assert(lhs->row == rhs->row && lhs->col == rhs->col);
    if (lhs->row != rhs->row || lhs->col != rhs->col)
    {
        return;
    }

    size_t count = lhs->row * lhs->col;

    for (size_t i = 0; i < count; ++i)
    {
        lhs->raw[i] += rhs->raw[i];
    }
}

void matrix_sub(Matrix *lhs, Matrix *rhs, Matrix **ins)
{
    assert(NULL != lhs && NULL != rhs);
    if (NULL == lhs || NULL == rhs)
    {
        return;
    }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if (NULL == lhs->raw || NULL == rhs->raw)
    {
        return;
    }

    assert(lhs->row == rhs->row && lhs->col == rhs->col);
    if (lhs->row != rhs->row || lhs->col != rhs->col)
    {
        return;
    }

    assert(NULL != ins);
    if (NULL == ins)
    {
        return;
    }

    matrix_new(ins, lhs->row, lhs->col, NULL);

    size_t count = lhs->row * lhs->col;

    for (size_t i = 0; i < count; ++i)
    {
        (*ins)->raw[i] = lhs->raw[i] - rhs->raw[i];
    }
}

void matrix_sub_assign(Matrix *lhs, Matrix *rhs)
{
    assert(NULL != lhs && NULL != rhs);
    if (NULL == lhs || NULL == rhs)
    {
        return;
    }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if (NULL == lhs->raw || NULL == rhs->raw)
    {
        return;
    }

    assert(lhs->row == rhs->row && lhs->col == rhs->col);
    if (lhs->row != rhs->row || lhs->col != rhs->col)
    {
        return;
    }

    size_t count = lhs->row * lhs->col;

    for (size_t i = 0; i < count; ++i)
    {
        lhs->raw[i] -= rhs->raw[i];
    }
}

void matrix_mut(Matrix *lhs, Matrix *rhs, Matrix **ins)
{
    assert(NULL != lhs && NULL != rhs);
    if (NULL == lhs || NULL == rhs)
    {
        return;
    }

    assert(NULL != lhs->raw && NULL != rhs->raw);
    if (NULL == lhs->raw || NULL == rhs->raw)
    {
        return;
    }

    assert(lhs->col == rhs->row);
    if (lhs->col != rhs->row)
    {
        return;
    }

    assert(NULL != ins);
    if (NULL == ins)
    {
        return;
    }

    matrix_new(ins, lhs->row, rhs->col, NULL);
    matrix_set_all(*ins, 0.0);

    for (size_t r = 0; r < lhs->row; ++r)
    {
        for (size_t c = 0; c < rhs->col; ++c)
        {
            size_t index = r * lhs->col + c;
            size_t offset = r * lhs->col;
            for (size_t m = 0; m < lhs->col; ++m)
            {
                (*ins)->raw[index] += lhs->raw[offset + m] * rhs->raw[m * rhs->col + c];
            }
        }
    }
}

void matrix_mut_assign(Matrix *lhs, Matrix *rhs)
{
    Matrix *ins = NULL;
    matrix_mut(lhs, rhs, &ins);

    matrix_move(ins, lhs);

    matrix_del(&ins);
}

void matrix_move(Matrix *src, Matrix *dst)
{
    assert(NULL != src);
    if (NULL == src)
    {
        return;
    }

    assert(NULL != dst);
    if (NULL == dst)
    {
        return;
    }

    matrix_dtor(dst);
    matrix_ctor(dst, src->row, src->col, src->raw);

    src->raw = NULL;
}

void matrix_set_all(Matrix *thiz, float value)
{
    size_t count = thiz->row * thiz->col;
    for (size_t i = 0; i < count; ++i)
    {
        thiz->raw[i] = value;
    }
}

void matrix_set_one(Matrix *thiz, size_t r, size_t c, float value)
{
    assert(NULL != thiz);
    if (NULL == thiz)
    {
        return;
    }

    assert(NULL != thiz->raw);
    if (NULL == thiz->raw)
    {
        return;
    }

    size_t index = r * thiz->col + c;
    thiz->raw[index] = value;
}

void matrix_print(Matrix *thiz)
{
    assert(NULL != thiz);
    if (NULL == thiz)
    {
        return;
    }

    assert(NULL != thiz->raw);
    if (NULL == thiz->raw)
    {
        return;
    }

    for (size_t r = 0; r < thiz->row; r++)
    {
        size_t offset = r * thiz->col;
        for (size_t c = 0; c < thiz->col; c++)
        {
            printf("%8.3f", thiz->raw[offset + c]);
            if (c < thiz->col - 1)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
