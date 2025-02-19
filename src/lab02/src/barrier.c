#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2
#define TIME_DIV    (float)1000000000.0

pthread_barrier_t barrier;

/**
 * @brief Demonstrates the usage of a barrier.
 * @details Designed to be executed by two threads. 
 * It prints "1" when the first thread reaches the barrier 
 * and "2" when the second thread reaches the barrier.
 * @param arg Pointer to the thread's ID (as an int).
 * @return NULL.
 */
void* thread_func(void *arg) {
	int id = *(int *)arg;

	if (id == 1) printf("1\n");
	pthread_barrier_wait(&barrier);
	if (id == 0) printf("2\n");

	pthread_exit(NULL);
	return NULL;
}

int main(void) {
	struct timespec start, finish; 
  	double elapsed; 
	
	pthread_t threads[NUM_THREADS];
	int ids[NUM_THREADS];

	int r;
	int id;
	void *status;

  	// Start time.
  	clock_gettime(CLOCK_MONOTONIC, &start);
	// WORK() : PARALLEL TESTING BARRIER FOR 2 THREADS!
  	// Split work between threads.
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	for (id = 0; id < NUM_THREADS; ++id) {
		ids[id] = id;
		r = pthread_create(&threads[id], NULL, thread_func, &ids[id]);

		if (r) {
			fprintf(stderr, "ERROR: CREATE THREAD ID=%d\n", id);
			exit(EXIT_FAILURE);
		}
	}
  	// Wait for all threads to finish.
	for (id = 0; id < NUM_THREADS; ++id) {
		r = pthread_join(threads[id], &status);

		if (r) {
			fprintf(stderr, "ERROR: WAITING THREAD ID=%d\n", id);
			exit(EXIT_FAILURE);
		}
	}
	pthread_barrier_destroy(&barrier);
  	// Finish time.
  	clock_gettime(CLOCK_MONOTONIC, &finish);

  	// Calculate elapsed time.
  	elapsed = (finish.tv_sec - start.tv_sec);
  	elapsed += (finish.tv_nsec - start.tv_nsec) / TIME_DIV; 
  	printf("\nTIME TO EXECUTE BARRIER THREADS: %f\n\n", elapsed);

	return EXIT_SUCCESS;
}
