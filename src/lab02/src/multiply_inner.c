#include <pthread.h>
/// #include <time.h>
#include <stdio.h>
#include <stdlib.h>

/// #define TIME_DIV    (float)1000000000.0
#define MIN(a, b)   ((a) <= (b) ? (a) : (b))

int N, P;
int **a, **b, **c;
pthread_mutex_t mutex;

/* Simple secvential matrix multiplication.
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
*/

/**
 * @brief Mltiply the matrices A and B, result is in C.
 * Each thread is responsible for a specific column range.
 * @param arg A pointer to an integer indicating the thread's ID.
 */
void* thread_func(void *arg) {
	int id = *(int *)arg;

	int start = id * (double)N / P;
	int end = MIN((id + 1) * (double)N / P, N);

	int i, j, k, sum;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
			sum = 0;
            for (k = start; k < end; ++k) {
                sum += a[i][k] * b[k][j];
            }
			pthread_mutex_lock(&mutex);
			c[i][j] += sum;
			pthread_mutex_unlock(&mutex);
        }
    }

	pthread_exit(NULL);
	return NULL;
}

/**
 * @brief Get command-line arguments.
 * Parse CMD-line arguments to set the values of N and P.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 */
void get_args(int argc, char **argv) {
	// Check if CMD-line arguments are provided.
	if(argc < 3) {
		fprintf(stderr, "Insufficient no. of parameters ./multiply_inner N P\n");
		exit(EXIT_FAILURE);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
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
	
	get_args(argc, argv);
	init();

	pthread_t threads[P];
	int ids[P];

  	int r;
  	long id;
  	void *status = NULL;

  	/// Start time.
  	/// clock_gettime(CLOCK_MONOTONIC, &start);
	// WORK() : PARALLEL TESTING INNER MULTIPLY!
  	// Split work between threads.
	pthread_mutex_init (&mutex, NULL);
	for (id = 0; id < P; ++id) {
		ids[id] = id;
		r = pthread_create(&threads[id], NULL, thread_func, &ids[id]);
	
		if (r) {
			fprintf(stderr, "ERROR: CREATE THREAD ID=%ld\n", id);
  			exit(EXIT_FAILURE);
		}
	}
  	// Wait for all threads to finish.
	for (id = 0; id < P; ++id) {
		r = pthread_join(threads[id], status);

		if (r) {
      		fprintf(stderr, "ERROR: WAITING THREAD ID=%ld\n", id);
    		exit(EXIT_FAILURE);
	  	}
	}
	pthread_mutex_destroy(&mutex);
  	/// Finish time.
  	/// clock_gettime(CLOCK_MONOTONIC, &finish);

  	/// Calculate elapsed time.
  	/// elapsed = (finish.tv_sec - start.tv_sec);
  	/// elapsed += (finish.tv_nsec - start.tv_nsec) / TIME_DIV; 
  	/// printf("\nTIME TO EXECUTE PARALLEL MATRIX MIDDLE MULTIPLY: %f\n\n", elapsed);

	print(c);
	free(a), free(b), free(c);

	return EXIT_SUCCESS;
}
