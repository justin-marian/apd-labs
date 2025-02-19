#include <pthread.h>
#include <math.h>
/// #include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b)   ((a) <= (b) ? (a) : (b))

#define SWAP(type, a, b) do { 	\
    type *temp = (a);    		\
    (a) = (b);               	\
    (b) = temp;          		\
} while (0)

int N, P;
int *v, *vQSort, *vNew;
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

void merge(int *source, int start, int mid, int end, int *destination) {
	int iA = start;
	int iB = mid;
	int i;

	for (i = start; i < end; ++i) {
		if (end == iB || (iA < mid && source[iA] <= source[iB])) {
			destination[i] = source[iA];
			iA++;
		} else {
			destination[i] = source[iB];
			iB++;
		}
	}
}

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

int is_power_of_two(int n) {
	if (n == 0) return 0;
	return (ceil(log2(n)) == floor(log2(n)));
}

void get_args(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "Insufficient no. of parameters ../out/merge N P (N = pow of 2)\n");
		exit(EXIT_FAILURE);
	}

	N = atoi(argv[1]);

	if (!is_power_of_two(N)) {
		fprintf(stderr, "N MUST BE POWER OF 2\n");
		exit(EXIT_FAILURE);
	}

	P = atoi(argv[2]);
}

void init(void) {
	int i;

	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);

	if (!v || !vQSort || !vNew) {
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

void *thread_func(void *arg) {
    int thread_id = *(int *)arg;

    int start = thread_id * (double)N / P;
    int end = MIN(N, (thread_id + 1) * (double)N / P);

	int width, i;
	int temp_start, temp_end;

    for (width = 1; width < N; width = 2 * width) {
        temp_start = start - (start % (2 * width));
        temp_end = end - (end % (2 * width));

        for (i = temp_start; i < temp_end; i = i + 2 * width) {
            merge(v, i, i + width, i + 2 * width, vNew);
        }

        pthread_barrier_wait(&barrier);
        if (thread_id == 0) SWAP(int, v, vNew);
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
  	// WORK() : PARALLEL MERGE SORT!
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

	// CLASSIC SEQUENTTUALLY MERGE SORT.
	int width;

	for (width = 1; width < N; width = 2 * width) {
		for (i = 0; i < N; i = i + 2 * width) {
			merge(v, i, i + width, i + 2 * width, vNew);
		}
		SWAP(int, v, vNew);
	}
	// CLASSIC SEQUENTTUALLY MERGE SORT.

	print();
	free(v);
	free(vQSort);
	free(vNew);

	return EXIT_SUCCESS;
}
