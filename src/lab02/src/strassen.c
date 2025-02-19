#include <stdio.h>
#include <stdlib.h>

int N;
int **a, **b, **c;

int **M1, **AUXM11, **AUXM12;
int **M6, **AUXM61, **AUXM62;
int **M7, **AUXM71, **AUXM72;
int **M2, **AUXM21;
int **M3, **AUXM31;
int **M4, **AUXM41;
int **M5, **AUXM51;

/**
 * @brief Get command-line arguments.
 * Parse CMD-line arguments to set the value of N.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 */
void get_args(int argc, char **argv) {
	// Check if CMD-line arguments are provided.
	if(argc < 2) {
		fprintf(stderr, "Insufficient no. of parameters ./strassen N\n");
		exit(EXIT_FAILURE);
	}

	N = atoi(argv[1]);
}

/**
 * @brief Allocate memory for matrices.
 * @param n no. of rows
 * @param m no. of columns
 */
int** alloc_matrix(int n, int m) {
	int **matrix = malloc(sizeof(int *) * n);
	
	if (!matrix) {
		fprintf(stderr, "ERROR: MALLOC MATRIX...");
		exit(EXIT_FAILURE);
	}

	int i;

	for (i = 0; i < n; ++i) {
		matrix[i] = malloc(sizeof(int) * m);
		if (!matrix[i]) {
			printf("ERROR: ALLOC ROWS MATRIX...");
			exit(EXIT_FAILURE);
		}
	}

	return(matrix);
}

/**
 * @brief Initialize matrices and allocate memory.
 * Initializes matrices a, b, c, and for the submatrices
 * M1,M2,M3,M4,M5,M6,M7 allocates memory for them.
 */
void init(void) {
	int i, j;

	a = alloc_matrix(N, N);
	b = alloc_matrix(N, N);
	c = alloc_matrix(N, N);

	M1 = alloc_matrix(N, N);
	AUXM11 = alloc_matrix(N, N);
	AUXM12 = alloc_matrix(N, N);

	M2 = alloc_matrix(N, N);
	AUXM21 = alloc_matrix(N, N);

	M3 = alloc_matrix(N, N);
	AUXM31 = alloc_matrix(N, N);

	M4 = alloc_matrix(N, N);
	AUXM41 = alloc_matrix(N, N);

	M5 = alloc_matrix(N, N);
	AUXM51 = alloc_matrix(N, N);

	M6 = alloc_matrix(N, N);
	AUXM61 = alloc_matrix(N, N);
	AUXM62 = alloc_matrix(N, N);

	M7 = alloc_matrix(N, N);
	AUXM71 = alloc_matrix(N, N);
	AUXM72 = alloc_matrix(N, N);

	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			c[i][j] = 0;
			
			M1[i][j] = 0;
			AUXM11[i][j] = 0;
			AUXM12[i][j] = 0;
			
			M2[i][j] = 0;
			AUXM21[i][j] = 0;
			
			M3[i][j] = 0;
			AUXM31[i][j] = 0;
			
			M4[i][j] = 0;
			AUXM41[i][j] = 0;
			
			M5[i][j] = 0;
			AUXM51[i][j] = 0;
			
			M6[i][j] = 0;
			AUXM61[i][j] = 0;
			AUXM62[i][j] = 0;
			
			M7[i][j] = 0;
			AUXM71[i][j] = 0;
			AUXM72[i][j] = 0;
			
			if (i <= j) {
				a[i][j] = 1;
				b[i][j] = 1;
			} else {
				a[i][j] = 0;
				b[i][j] = 0;
			}
		}
	}
}

/**
 * @brief Print a matrix.
 * @param mat The matrix to be printed.
 */
void print(int **mat) {
	int i, j;

	for (i = 0; i < N; ++i) {
		for(j = 0; j < N; ++j) {
			printf("%i\t", mat[i][j]);
		}
		printf("\n");
	}
}

/** @brief Multiply two submatrices and accumulate the result in a destination submatrix. */
void mul_matrix(int **C, int startCi, int startCj, 
				int **A, int startAi, int startAj,
				int **B, int startBi, int startBj) {
	int i, j, k;

	for (i = 0; i < N/2; ++i) {
		for (j = 0; j < N/2; ++j) {
			for (k = 0; k < N/2; k++) {
				C[startCi + i][startCj + j] += A[startAi + i][startAj + k] * B[startBi + k][startBj + j];
			}
		}
	}
}

/** @brief Add two submatrices and accumulate the result in a destination submatrix. */
void add_matrix(int **C, int startCi, int startCj, 
				int **A, int startAi, int startAj,
				int **B, int startBi, int startBj) {
	int i, j;

	for( i = 0; i < N/2; ++i) {
		for (j = 0; j < N/2; ++j) {
			C[startCi + i][startCj + j] = A[startAi + i][startAj + j] + B[startBi + i][startBj + j];
		}
	}
}

/** @brief Substract two submatrices and accumulate the result in a destination submatrix. */
void sub_matrix(int **C, int startCi, int startCj, 
				int **A, int startAi, int startAj,
				int **B, int startBi, int startBj) {
	int i, j;

	for (i = 0; i < N/2; ++i) {
		for (j = 0; j < N/2; ++j) {
			C[startCi + i][startCj + j] = A[startAi + i][startAj + j] - B[startBi + i][startBj + j];
		}
	}
}

int main(int argc, char **argv) {
	get_args(argc, argv);
	init();

	// calculul matricii M1
	add_matrix(AUXM11, 0, 0, a, 0, 0, a, N/2, N/2);
	add_matrix(AUXM12, 0, 0, b, 0, 0, b, N/2, N/2);
	mul_matrix(M1, 0, 0, AUXM11, 0, 0, AUXM12, 0, 0);

	// calculul matricii M2
	add_matrix(AUXM21, 0,0, a, N/2, 0, a, N/2, N/2);
	mul_matrix(M2, 0, 0, AUXM21, 0, 0, b, 0, 0);

	// calculul matricii M3
	sub_matrix(AUXM31, 0, 0, b, 0, N/2, b, N/2, N/2);
	mul_matrix(M3, 0, 0, a, 0, 0, AUXM31, 0, 0);

	// calculul matricii M4
	sub_matrix(AUXM41, 0, 0, b, N/2, 0, b, 0, 0);
	mul_matrix(M4, 0, 0, a, N/2, N/2, AUXM41, 0, 0);

	// calculul matricii M5
	add_matrix(AUXM51, 0,0, a, 0, 0, a, 0, N/2);
	mul_matrix(M5, 0, 0, AUXM51, 0, 0, b, N/2, N/2);

	// calculul matricii M6
	sub_matrix(AUXM61, 0, 0, a, N/2, 0, a, 0, 0);
	add_matrix(AUXM62, 0, 0, b, 0, 0, b, 0, N/2);
	mul_matrix(M6, 0, 0, AUXM61, 0, 0, AUXM62, 0, 0);

	// calculul matricii M7
	sub_matrix(AUXM71, 0, 0, a, 0, N/2, a, N/2, N/2);
	add_matrix(AUXM72, 0, 0, b, N/2, 0, b, N/2, N/2);
	mul_matrix(M7, 0, 0, AUXM71, 0, 0, AUXM72, 0, 0);

	// calculul submatricii C1,1
	add_matrix(c, 0, 0, M1, 0, 0, M4, 0, 0);
	sub_matrix(c, 0, 0, c, 0, 0, M5, 0, 0);
	add_matrix(c, 0, 0, c, 0, 0, M7, 0, 0);

	// calculul submatricii C1,2
	add_matrix(c, 0, N/2, M3, 0, 0, M5, 0, 0);

	// calculul submatricii C2,1
	add_matrix(c, N/2, 0, M2, 0, 0, M4, 0, 0);

	// calculul submatricii C2,2
	sub_matrix(c, N/2, N/2, M1, 0, 0, M2, 0, 0);
	add_matrix(c, N/2, N/2, c, N/2, N/2, M3, 0, 0);
	add_matrix(c, N/2, N/2, c, N/2, N/2, M6, 0, 0);

	print(c);

	return EXIT_SUCCESS;
}
