#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b)   ((a) <= (b) ? (a) : (b))

// L - WIDTH MATRIX
// P - NUMBER OF THREADS
// N = L * L - NUMBER ELEMENTS TO BE SORTED
int L, P, N;
int *v, *vQSort;
int **M;
pthread_barrier_t barrier;

void compare_vectors(int *a, int *b) {
	int i;

	for (i = 0; i < N; ++i) {
		if (a[i] != b[i]) {
			printf("INCORRECT SORT\n");
			return;
		}
	}

	printf("CORRECT SORT\n");
}

void display_vector(int *v) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; ++i) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

void display_matrix(int **M) {
	int i, j;
	int display_width = 2 + log10(N);

	for (i = 0; i < L; ++i) {
		for (j = 0; j < L; ++j) {
			printf("%*i", display_width, M[i][j]);
		}
		printf("\n");
	}
}

void copy_matrix_in_vector(int *v, int **M) {
	int i, j;

	for (i = 0; i < L; ++i) {
		if (i % 2 == 0) {
			for (j = 0; j < L; ++j) {
				v[i * L + j] = M[i][j];
			}
		} else {
			for (j = L; j > 0; j--) {
				v[i * L + (L - j)] = M[i][j-1];
			}
		}
	}
}

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

int cmpdesc(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return B - A;
}

void get_args(int argc, char **argv) {
	if(argc < 3) {
		fprintf(stderr, "Insufficient no. of parameters ../out/shear L P\n");
		exit(EXIT_FAILURE);
	}

	L = atoi(argv[1]);
	P = atoi(argv[2]);
	N = L * L;
}

void init(void) {
	int i, j;

	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	M = malloc(sizeof(int*) * L);

	if (!v || !vQSort || !M) {
		fprintf(stderr, "ERROR: MALLOC ARRAYS...");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < L; ++i) {
		M[i] = malloc(sizeof(int) * L);
	}

	// SEED the random number generator.
	srand(42);

	for (i = 0; i < L; ++i) {
		for (j = 0; j < L; ++j) {
			M[i][j] = rand() % N;
		}
	}
}

void print(void) {
	printf("M:\n");
	display_matrix(M);
	copy_matrix_in_vector(v, M);
	printf("v:\n");
	display_vector(v);
	printf("vQSort:\n");
	display_vector(vQSort);
	compare_vectors(v, vQSort);	
}

// IMPLEMENTATION PARALLEL SHEAR SORT!
void* thread_func(void *arg) {
	int thread_id = *(int *)arg;

	int i, j, k;
	int temp[L];

	int start = thread_id * (double) L / P;
  	int end = MIN(L, (thread_id + 1) * (double) L / P);

  	for (k = 0; k < log(N); ++k) {
  		for (i = start; i < end; ++i) {
			(i % 2) ? \
				qsort(M[i], L, sizeof(int), cmpdesc) : \
				qsort(M[i], L, sizeof(int), cmp);
		}
  		pthread_barrier_wait(&barrier);

  		for (i = start; i < end; ++i) {
			for (j = start; j < end; ++j) temp[j] = M[j][i];
			qsort(temp, L, sizeof(int), cmp);
			for (j = start; j < end; ++j) M[j][i] = temp[j];
		}
		pthread_barrier_wait(&barrier);
  	}

	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char **argv) {
	get_args(argc, argv);
	init();

	pthread_t threads[P];
	int ids[P];

	int r;
	int id;
	void *status = NULL;

	// SORT ETALON VECTOR.
	copy_matrix_in_vector(vQSort, M);
	qsort(vQSort, N, sizeof(int), cmp);

	pthread_barrier_init(&barrier, NULL, P);
  	// WORK() : PARALLEL SHEAR SORT!
  	// Split work between threads.
  	for (id = 0; id < P; ++id) {
		ids[id] = id;
		r = pthread_create(&threads[id], NULL, thread_func, &ids[id]);
		    
	  	if (r) {
      		fprintf(stderr, "ERROR: CREATE THREAD ID=%d\n", id);
  			exit(EXIT_FAILURE);
	  	}
	}
	// Wait for all threads to finish.
	for (id = 0; id < P; ++id) {
		r = pthread_join(threads[id], &status);

		if (r) {
      		fprintf(stderr, "ERROR: WAITING THREAD ID=%d\n", id);
    		exit(EXIT_FAILURE);
	  	}
	}
	pthread_barrier_destroy(&barrier);

	int i, j, k;
	int temp[L];

	// shear sort clasic - trebuie paralelizat
	for (k = 0; k < log(N) + 1; ++k) {
		// se sorteaza liniile pare crescator
		// se sorteaza liniile impare descrescator
		for (i = 0; i < L; ++i) {
			if (i % 2) {
				qsort(M[i], L, sizeof(int), cmpdesc);
			} else {
				qsort(M[i], L, sizeof(int), cmp);
			}
		}

		// se sorteaza coloanele descrescator
		for (i = 0; i < L; ++i) {
			for (j = 0; j < L; ++j) {
				temp[j] = M[j][i];
			}

			qsort(temp, L, sizeof(int), cmp);

			for (j = 0; j < L; ++j) {
				M[j][i] = temp[j];
			}
		}
	}

	print();
	free(v); free(vQSort);
	for (i = 0; i < L; ++i) free(M[i]);
	free(M);

	return EXIT_SUCCESS;
}
