#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ADDED_VALUE 2000
#define NUM_THREADS 2

int a;
pthread_mutex_t mutex;

/**
 * @brief Modify a shared variable with mutex protection.
 * Modifying a shared variable 'a' by adding 'ADDED_VALUE'.
 * It uses a mutex to protect access to the shared variable 
 * to avoid data race conditions.
 * @param arg Pointer to the thread's ID (as a long).
 * @return NULL.
 */
void* thread_func(void *arg) {
    long id = *(long *)arg;

    // pthread_mutex_lock(&mutex);
    printf("Thread %ld used!\n", id);
    a += ADDED_VALUE;
    // pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;
}

/* Always obtain 4000 for 2 threads and ADDED_VALUE 2000 */
int main(void) {
	pthread_t threads[NUM_THREADS];
	long ids[NUM_THREADS];

	int r;
	long id;
	void *status = NULL;

	// pthread_mutex_init(&mutex, NULL);
	for (id = 0; id < NUM_THREADS; ++id) {
		ids[id] = id;
		r = pthread_create(&threads[id], NULL, thread_func, &ids[id]);

		if (r) {
			fprintf(stderr, "ERROR: CREATE THREAD ID=%ld\n", id);
  			exit(EXIT_FAILURE);
		}
	}

	for (id = 0; id < NUM_THREADS; ++id) {
		r = pthread_join(threads[id], &status);

		if (r) {
			fprintf(stderr, "ERROR: WAITING THREAD ID=%ld\n", id);
    		exit(EXIT_FAILURE);
		}
	}
	// pthread_mutex_destroy(&mutex);

	printf("a = %d\n", a);

	return EXIT_SUCCESS;
}
