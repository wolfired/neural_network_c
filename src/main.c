#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int main(int argc, char **argv)
{
	Matrix m0;
	matrix_ctor(&m0, 2, 2, NULL);
	matrix_set_one(&m0, 0, 0, 1.0);

	Matrix *m1 = NULL;
	matrix_new(&m1, 2, 2, NULL);
	matrix_set_one(m1, 0, 0, 2.0);

	// Matrix *m2 = NULL;
	// matrix_mut(&m0, m1, &m2);
	matrix_mut_assign(&m0, m1);

	matrix_print(&m0);

	matrix_dtor(&m0);
	matrix_del(&m1);
	// matrix_del(&m2);

	return EXIT_SUCCESS;
}
