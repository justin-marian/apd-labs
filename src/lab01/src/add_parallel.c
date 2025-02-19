#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_DIV    (float)1000000000.0
#define MIN(a, b)   ((a) <= (b) ? (a) : (b))

int *arr, arr_size, row_size;
int num_threads;

/**
 * @brief   Print an array with specified elements per row.
 * @details Formatting the output (specified no. of elements per row).
 * Required width based on the maximum number in the array.
 * @param arr      Pointer to the integer array.
 * @param arr_size The size of the array.
 * @param row_size The number of elements to print per row.
 */
void print(int *arr, int arr_size, int row_size) {
  int max_num = 0;

  for (int i = 0; i < arr_size; ++i) {
    if (arr[i] > max_num) {
      max_num = arr[i];
    }
  }

  int width = 1;

  while (max_num > 9) {
    max_num /= 10;
    width++;
  }

  int count = 0;

  for (int i = 0; i < arr_size; ++i) {
    printf("%*d ", width, arr[i]);
    count++;
    if (count % row_size == 0) {
      printf("\n");
    }
  }

  if (count % row_size != 0) {
    printf("\n");
  }
}

/**
 * @brief Thread function to perform parallel addition to an array.
 * Each thread is responsible for adding 100 to a range of elements in the array.
 * @param arg Pointer to the thread's ID (as a long).
 * @return NULL.
 */
void* f(void *arg) {
  long id = *(long *)arg;
  
  // Ex 3
  // Range of elements to process based on the thread's ID.
  int start = id * (double)arr_size / num_threads;
  int end = MIN((id + 1) * (double)arr_size / num_threads, arr_size);

  int i = start;
  while (i < end) {
    arr[i++] += 100;
  }

  // for (i = 0; i < arr_size; ++i) {
  //   printf("Element array [%d] = \n", i);
  //   printf("Value[%d] from thread: %ld!\n", id);
  //   printf("Fom thread: %ld!\n", id);
  // }

  pthread_exit(NULL);
  return NULL;
}

int main(int argc, char **argv) {
  // Check if CMD-line arguments are provided.
  if (argc < 4) {
    fprintf(stderr, "ERROR: SPECIFY ARR SIZE, ELEMENTS PER ROW & NUM THREADS!\n");
    exit(EXIT_FAILURE);
  }

  arr_size = atoi(argv[1]);
  row_size = atoi(argv[2]);
  num_threads = atoi(argv[3]);
  printf("SIZE ARRAY: %d\nELEMENTS PER ROW: %d\nNUM THRADS: %d\n\n", arr_size, row_size, num_threads);

  // Create and initialize an integer array.
  arr = malloc(arr_size * sizeof(int));
  for (int i = 0; i < arr_size; ++i) {
    arr[i] = i;
  }

  print(arr, arr_size, row_size);

  struct timespec start, finish; 
  double elapsed; 

  pthread_t threads[num_threads];
  long ids[num_threads];

  /* Measure the time it takes to perform parallel sum. */

  int r;
  long id;
  void *status = NULL;

  // Start time.
  clock_gettime(CLOCK_MONOTONIC, &start);
  // WORK() : PARALLEL SUM WITH +100!
  // Split work between threads.
	for (id = 0; id < num_threads; ++id) {
		ids[id] = id;
	  r = pthread_create(&threads[id], NULL, f, &ids[id]);
    
	  if (r) {
      fprintf(stderr, "ERROR: CREATE THREAD ID=%ld\n", id);
  		exit(EXIT_FAILURE);
	  }
	}
  // Wait for all threads to finish.
  for (id = 0; id < num_threads; ++id) {
	  r = pthread_join(threads[id], &status);

	  if (r) {
      fprintf(stderr, "ERROR: WAITING THREAD ID=%ld\n", id);
    	exit(EXIT_FAILURE);
	  }
	}
  // Finish time.
  clock_gettime(CLOCK_MONOTONIC, &finish);

  // Calculate elapsed time.
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / TIME_DIV; 
  printf("\nTIME TO EXECUTE PARALLEL ADD: %f\n\n", elapsed);

  print(arr, arr_size, row_size);
  pthread_exit(NULL);
  free(arr);

  return EXIT_SUCCESS;
}
