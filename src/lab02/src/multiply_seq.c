#include <pthread.h>
/// #include <time.h>
#include <stdio.h>
#include <stdlib.h>

/// #define TIME_DIV    (float)1000000000.0

int N;
int **a, **b, **c;

/**
 * @brief Get command-line arguments.
 * Parse CMD-line argument to set the value of N.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 */
void get_args(int argc, char **argv) {
	// Check if CMD-line arguments are provided.
	if(argc < 2) {
		fprintf(stderr, "Insufficient no. of parameters ./multiply_seq N \n");
		exit(EXIT_FAILURE);
	}

	N = atoi(argv[1]);
}

/**
 * @brief Initialize matrices and allocate memory.
 * Initializes matrices a, b, and c, and allocates memory for them.
 */
void init(void) {
	a = malloc(sizeof(int *) * N);
	b = malloc(sizeof(int *) * N);
	c = malloc(sizeof(int *) * N);

	if (!a || !b || !c) {
		fprintf(stderr, "ERROR: MALLOC MATRIXES...");
		exit(EXIT_FAILURE);
	}

	int i, j;

	for (i = 0; i < N; ++i) {
		a[i] = malloc(sizeof(int) * N);
		b[i] = malloc(sizeof(int) * N);
		c[i] = malloc(sizeof(int) * N);

		if(!a[i] || !b[i] || !c[i]) {
			fprintf(stderr, "ERROR: MALLOC ROWS...");
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < N; ++i) {
		for(j = 0; j < N; ++j) {
			c[i][j] = 0;

			if(i <= j) {
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

int main(int argc, char **argv) {
	/// struct timespec start, finish; 
  	/// double elapsed;

	int i, j, k;

	get_args(argc, argv);
	init();

  	/// Start time.
  	/// clock_gettime(CLOCK_MONOTONIC, &start);
	// WORK() : SEQUENTIALLY TESTING OUTER MULTIPLY!
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
  	/// Finish time.
  	/// clock_gettime(CLOCK_MONOTONIC, &finish);

  	/// Calculate elapsed time.
  	/// elapsed = (finish.tv_sec - start.tv_sec);
  	/// elapsed += (finish.tv_nsec - start.tv_nsec) / TIME_DIV; 
  	/// printf("\nTIME TO EXECUTE SERIAL NORMAL MATRIX MULTIPLY: %f\n\n", elapsed);

	print(c);
	free(a), free(b), free(c);

	return EXIT_SUCCESS;
}
