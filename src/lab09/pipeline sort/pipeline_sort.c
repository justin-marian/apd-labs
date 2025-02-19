#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int N;

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

// Use 'mpirun -np 20 --oversubscribe ./pipeline_sort' to run the application with more processes
int main(int argc, char **argv) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

	N = nProcesses - 1;
	printf("Hello from %i/%i\n", rank, N);

	if (rank == MASTER) { // This code is run by a single process
		int initialValue = -1;
		int sorted = 0;
		int i, val;

		int aux;
		int *v = (int*)malloc(sizeof(int) * N );
		int *vQSort = (int*)malloc(sizeof(int) * N);

		// generate the vector v with random values
		// DO NOT MODIFY
		srandom(42);
		for (i = 0; i < N ; i++)
			v[i] = random() % 200;
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < N ; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N , sizeof(int), cmp);

		// TODO sort the vector v
		for (int i = 0; i < N ; i++) {
			MPI_Send(&initialValue, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
			MPI_Send(&v[i], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}

		for (int i = 0; i < N ; i++) {
			MPI_Recv(&v[i], 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD, NULL);
		}

		displayVector(v);
		compareVectors(v, vQSort);
	} else {
		// TODO sort the vector v
		int actual_value;
		MPI_Recv(&actual_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);

		int recv_value;
		for (int i = 0; i < nProcesses - rank; i++) {
			MPI_Recv(&recv_value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);
			if (actual_value == -1) {
				actual_value = recv_value;
			} else if (actual_value <= recv_value) {
				MPI_Send(&recv_value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			} else {
				MPI_Send(&actual_value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				actual_value = recv_value;
			}
		}

		MPI_Send(&actual_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}
