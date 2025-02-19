#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10
#define MASTER 0

int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
    int rank, i;
    int nProcesses;
    MPI_Init(&argc, &argv);

    int pos[N];
    int *v = (int *)malloc(sizeof(int) * N);
    int *sortedV = (int *)malloc(sizeof(int) * N);

    for (i = 0; i < N; i++)
        pos[i] = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    if (rank == MASTER) {
        // Generate a random vector
        srand(time(NULL));
        for (i = 0; i < N; i++) {
            v[i] = rand() % 1000;
        }
    }

    // Broadcast the vector to all processes
    MPI_Bcast(v, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Calculate ranks
    for (i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (v[i] > v[j] || (v[i] == v[j] && i > j)) {
                pos[i]++;
            }
        }
    }

    // Gather positions from all processes to the master process
    MPI_Gather(&pos[rank * (N / nProcesses)], N / nProcesses, MPI_INT, pos, N / nProcesses, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
        // Rearrange the vector based on global ranks
        for (i = 0; i < N; i++) {
            sortedV[pos[i]] = v[i];
        }

        // Display and compare results
        printf("Sorted vector: ");
        for (i = 0; i < N; i++) {
            printf("%d ", sortedV[i]);
        }
        printf("\n");

        free(sortedV);
    }

    free(v);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
