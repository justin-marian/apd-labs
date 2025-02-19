#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b)   ((a) <= (b) ? (a) : (b))

int N, P, *v;
pthread_barrier_t Barrier;

typedef struct {
	int id;
	int nr_primes;
	int sum_pos_primes;
} PairSol;

void get_args(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "Insufficient no. of parameters\n");
		exit(EXIT_FAILURE);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);

	int k = 0;
	v = malloc(sizeof(int) * N);

	for (int i = 3; i < argc; ++i) {
		v[k++] = atoi(argv[i]);
	}
}

int is_prime(int n) {
	if (n <= 1)
		return 0;
	for (int d = 2; d <= n / 2; d++)
		if (n % d == 0)
			return 0;
	return 1;
}

void *thread_func(void *arg) {
    PairSol* sol_id = (PairSol *)arg;

    int start = sol_id->id * (double)N / P;
    int end = MIN(N, (sol_id->id + 1) * (double)N / P);

	for (int i = start; i < end; ++i) {
		if (is_prime(v[i])) {
			sol_id->nr_primes++;
			sol_id->sum_pos_primes += i;
		}
	}

    pthread_barrier_wait(&Barrier);
    pthread_exit(NULL);
    return NULL;
}

void *result_func(void *arg) {
    PairSol *sol = (PairSol *)arg;
    int total_primes = 0, total_sum_pos = 0;

    for (int i = 0; i < P; ++i) {
        total_primes += sol[i].nr_primes;
        total_sum_pos += sol[i].sum_pos_primes;
    }

    printf("Total num of primes: %d\nTotal sum of positions: %d\n", total_primes, total_sum_pos);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char **argv) {
	get_args(argc, argv);

  	pthread_t threads[P];
	PairSol sol[P];

  	int r;
  	int id;
  	void *status = NULL;

    pthread_barrier_init(&Barrier, NULL, P);
  	for (id = 0; id < P; ++id) {
		sol[id].id = id;
		sol[id].nr_primes = 0;
		sol[id].sum_pos_primes = 0;
		r = pthread_create(&threads[id], NULL, thread_func, &sol[id]);
		    
	  	if (r) {
      		fprintf(stderr, "ERROR: CREATE THREAD ID=%d\n", id);
  			exit(EXIT_FAILURE);
	  	}
	}
	for (id = 0; id < P; ++id) {
		r = pthread_join(threads[id], &status);

		if (r) {
      		fprintf(stderr, "ERROR: WAITING THREAD ID=%d\n", id);
    		exit(EXIT_FAILURE);
	  	}
	}
    pthread_barrier_destroy(&Barrier);

    pthread_t result_thread;
    pthread_create(&result_thread, NULL, result_func, &sol);
    pthread_join(result_thread, NULL);

	free(v);
	return EXIT_SUCCESS;
}
