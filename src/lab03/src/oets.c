#include <pthread.h>
#include <math.h>
/// #include <time.h>
#include <stdio.h>
#include <stdlib.h>

int N, P;
int *v, *vQSort;
pthread_barrier_t barrier;

#define MIN(a, b)   ((a) <= (b) ? (a) : (b))

#define SWAP_VAL(type, a, b) do { 	\
    type temp = (a);    			\
    (a) = (b);               		\
    (b) = temp;          			\
} while (0)

#define SWAP_PTR(type, a, b) do { 	\
    type *temp = (a);    			\
    (a) = (b);               		\
    (b) = temp;          			\
} while (0)

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

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

void get_args(int argc, char **argv) {
	if(argc < 3) {
		fprintf(stderr, "Insufficient no. of parameters ../out/oets N P\n");
		exit(EXIT_FAILURE);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
}

void init(void) {
	int i;

	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);

	if (!v || !vQSort) {
		fprintf(stderr, "ERROR: MALLOC ARRAYS...");
		exit(EXIT_FAILURE);
	}

	// SEED the random number generator.
	srand(42);

	for (i = 0; i < N; ++i) v[i] = rand() % N;
}

void print(void) {
	printf("v:\n");
	display_vector(v);
	printf("vQSort:\n");
	display_vector(vQSort);
	compare_vectors(v, vQSort);
}

void* thread_func(void *arg) {
	int thread_id = *(int*)arg;

 	int start = thread_id * ((double)N/P);
  	int end = MIN(N, (thread_id + 1) * ((double)N/P));
  	
	int start_even = start % 2 == 0 ? start : start + 1;
  	int start_odd = start % 2 == 1 ? start : start + 1;

	int i, k;

  	for (k = 0; k < N; k++) {
    	for (i = start_even; i < end && i < N - 1; i += 2) {
      		if (v[i] > v[i + 1]) {
				SWAP_VAL(int, v[i], v[i + 1]);
      		}
    	}
		pthread_barrier_wait(&barrier);
    	for (i = start_odd; i < end && i < N - 1; i += 2) {
      		if (v[i] > v[i + 1]) {
				SWAP_VAL(int, v[i], v[i + 1]);
      		}
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
	int i;
	for (i = 0; i < N; ++i) vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	pthread_barrier_init(&barrier, NULL, P);
  	// WORK() : PARALLEL OETS SORT!
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

	// CLASSIC BUBBLE SORT
	int sorted = 0;

	while (!sorted) {
		sorted = 1;

		for (i = 0; i < N - 1; ++i) {
			if(v[i] > v[i + 1]) {
				SWAP_VAL(int, v[i], v[i + 1]);
			}
		}
	}

	print();
	free(v);
	free(vQSort);

	return EXIT_SUCCESS;
}
