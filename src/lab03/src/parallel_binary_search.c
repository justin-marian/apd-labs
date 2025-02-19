#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// #define INSIDE -1 	// NUMBER INSIDE RANGE
// #define OUTSIDE -2 	// NUMBER OUTSIDE RANGE

pthread_barrier_t barrier;

typedef struct my_arg {
	int N, P, number, id;
	int *left, *right, *keep_running;
	int *v, *found;
} my_arg;

// IMPLEMENTATION SEQUENTTUALLY BINARY SEARCH!
void binary_search(my_arg *data) {
	while (data->keep_running) {
		if (*data->left > *data->right) {
			printf("\n\nNUMBER NOT FOUND\n\n");
			return;
		}

        int mid = *data->left + *data->right / 2;

		if (data->v[mid] == data->number) {
			*data->keep_running = 0;
			printf("\n\nFOUND THE NUMBER %d ON IDX %d\n\n", data->number, mid + 1);
		} else if (data->v[mid] < data->number) {
            *data->left = mid + 1;
        } else {
            *data->right = mid - 1;
        }
	}
}

void display_vector(int *v, int N) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; ++i) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

// IMPLEMENTATION PARALLEL BINARY SEARCH!
void* thread_func(void *arg) {
    my_arg *data = (my_arg *)arg;

    while (*data->keep_running) {
		if (*data->left > *data->right) {
			if (data->id == 0) {
				printf("\n\nNUMBER NOT FOUND\n\n");
			}
			return NULL;
		}

        int size = *data->right - *data->left;
        int mid = *data->left + size / 2;

        pthread_barrier_wait(&barrier);
        if (data->v[mid] == data->number) {
            *data->found = mid;
			if (data->id == 0) {
				printf("\n\nFOUND THE NUMBER %d ON IDX %d\n\n", data->number, mid + 1);
			}
            *data->keep_running = 0;
        } else if (data->v[mid] < data->number) {
            *data->left = mid + 1;
        } else {
            *data->right = mid - 1;
        }
        pthread_barrier_wait(&barrier);
    }

	pthread_exit(NULL);
    return NULL;
}

int main(int argc, char **argv) {
	if (argc < 4) {
		printf("Usage:\n\t./ex N P number\n");
		return EXIT_FAILURE;
	}

	int N, P, number;
	N = atoi(argv[1]);
	P = atoi(argv[2]);
	number = atoi(argv[3]);

	int keep_running = 1, left = 0, right = N;

	int *v = (int*) malloc(N * sizeof(int));
	int *found = (int*) malloc(P * sizeof(int));

	pthread_t *threads = (pthread_t*) malloc(P * sizeof(pthread_t));
	my_arg *arguments = (my_arg*) malloc(P * sizeof(my_arg));

	int r;
	int id;
	void *status = NULL;

	pthread_barrier_init(&barrier, NULL, P);
	int i;
	for (i = 0; i < N; ++i) v[i] = i * 2;

	display_vector(v, N);

	for (id = 0; id < P; ++id) {
		arguments[id].id = id;
		arguments[id].N = N;
		arguments[id].P = P;
		arguments[id].number = number;
		arguments[id].left = &left;
		arguments[id].right = &right;
		arguments[id].keep_running = &keep_running;
		arguments[id].v = v;
		arguments[id].found = found;

		r = pthread_create(&threads[id], NULL, thread_func, &arguments[id]);

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

	free(v);
	free(threads);
	free(arguments);

	return EXIT_SUCCESS;
}
