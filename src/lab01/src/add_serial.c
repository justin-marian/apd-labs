#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_DIV  (float)1000000000.0

int *arr, arr_size, row_size;

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

int main(int argc, char **argv) {
  // Check if CMD-line arguments are provided.
  if (argc < 3) {
    fprintf(stderr, "ERROR: SPECIFY ARR SIZE & ELEMENTS PER ROW!\n");
    exit(EXIT_FAILURE);
  }

  arr_size = atoi(argv[1]);
  row_size = atoi(argv[2]);
  printf("SIZE ARRAY: %d\nELEMENTS PER ROW: %d\n\n", arr_size, row_size);

  // Create and initialize an integer array.
  arr = malloc(arr_size * sizeof(int));
  for (int i = 0; i < arr_size; ++i) {
    arr[i] = i;
  }

  print(arr, arr_size, row_size);

  struct timespec start, finish; 
  double elapsed; 

  /* Measure the time it takes to perform serial sum. */

  // Start time.
  clock_gettime(CLOCK_MONOTONIC, &start); 
  // WORK() : SERIAL SUM WITH +100!
  for (int i = 0; i < arr_size; ++i) {
    arr[i] += 100;
  }
  // Finish time.
  clock_gettime(CLOCK_MONOTONIC, &finish);

  // Calculate elapsed time.
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / TIME_DIV; 
  printf("\nTIME TO EXECUTE SERIAL ADD: %f\n\n", elapsed);

  print(arr, arr_size, row_size);
  free(arr);

  return EXIT_SUCCESS;
}
