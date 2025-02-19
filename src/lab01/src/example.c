#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// #define NUM_THREADS 2

// Ex 1
// void* f(void *arg) {
//   long id = *(long*) arg;
//   printf("Hello World din thread-ul %ld!\n", id);
//   pthread_exit(NULL);
//   return NULL;
// }

/**
 * @brief Odd-numbered threads.
 * Printing messages with odd step values.
 * @param arg Pointer to the thread's ID.
 * @return NULL.
 */
void* f1(void *arg) {
  long id = *(long *)arg;
  
  // Ex 3
  int i = 1;
  while (i < 100) {
    printf("Hello World from thread id=%2ld [step = %3d]!\n", id, i);
    i += 2;
  }

  pthread_exit(NULL);
  return NULL;
}

/**
 * @brief Even-numbered threads.
 * Printing messages with even step values.
 * @param arg Pointer to the thread's ID.
 * @return NULL.
 */
void* f2(void *arg) {
  long id = *(long *)arg;
  
  // Ex 3
  int i = 0;
  while (i < 100) {
    printf("Hello World from thread id=%2ld [step = %3d]!\n", id, i);
    i += 2;
  }

  pthread_exit(NULL);
  return NULL;
}

int main(void) {
  // Ex 2
  long num_cores = sysconf(_SC_NPROCESSORS_CONF);
  long num_threads = num_cores;
 	printf("NUM THREADS: %ld\n\n", num_threads);

  pthread_t threads[num_threads]; // NUM_THREADS
  long ids[num_threads];          // NUM_THREADS

  int r;
  long id;
  void *status = NULL;

  // WORK() : PARALLEL SUM WITH FOR ODD/EVEN!
  // Split work between threads.
  for (id = 0; id < num_threads; ++id) { // NUM_THREADS
    ids[id] = id;
    if (id == 0) {  // ODD
      r = pthread_create(&threads[id], NULL, f1, &ids[id]);
    } else {        // EVEN
      r = pthread_create(&threads[id], NULL, f2, &ids[id]);
    }

    if (r) {
      fprintf(stderr, "ERROR: CREATE THREAD ID=%ld\n", id);
      exit(EXIT_FAILURE);
    }
  }
  // Wait for all threads to finish.
  for (id = 0; id < num_threads; ++id) { // NUM_THREADS
    r = pthread_join(threads[id], &status);

    if (r) {
      fprintf(stderr, "ERROR: WAITING THREAD ID=%ld\n", id);
      exit(EXIT_FAILURE);
    }
  }

  pthread_exit(NULL);
  return EXIT_SUCCESS;
}
