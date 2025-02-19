#include <math.h>
/// #include <time.h>
#include <stdio.h>
#include <stdlib.h>

void display_vector(int *v, int N) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; ++i) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

// IMPLEMENTATION SEQUENTTUALLY BINARY SEARCH!
void binary_search(int keep_running, int number, int left, int right, int *v) {
	while (keep_running) {
		if (left > right) {
			printf("\n\nNUMBER NOT FOUND\n\n");
			return;
		}

        int mid = left + (right - left) / 2;

		if (v[mid] == number) {
			keep_running = 0;
			printf("\n\nFOUND THE NUMBER %d ON IDX %d\n\n", number, mid + 1);
		} else if (v[mid] < number) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage:\n\t./ex N number\n");
		return EXIT_FAILURE;
	}

	int N, number;
	N = atoi(argv[1]);
	number = atoi(argv[2]);

    int keep_running = 1, left = 0, right = N;

	int *v = (int*) malloc(N * sizeof(int));

	for (int i = 0; i < N; ++i) v[i] = i * 2;

	display_vector(v, N);

    binary_search(keep_running, number, left, right, v);

	free(v);

	return EXIT_SUCCESS;
}
