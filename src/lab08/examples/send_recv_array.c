#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

int main (int argc, char **argv) {
    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int arr[SIZE];

    int i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // The current process ID / Rank.
    MPI_Get_processor_name(hostname, &len);

    srand(42);

    if (rank == 0) {
        for (i = 0; i < SIZE; ++i) {
            arr[i] = i;
        }

        printf("Process with rank [%d] has the following array:\n", rank);
        for (i = 0; i < SIZE; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");

        MPI_Send(arr, SIZE, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printf("Process with rank [%d] sent the array.\n", rank);
    } else {
        MPI_Status status;
        MPI_Recv(arr, SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("Process with rank [%d], received array with tag %d.\n",
                rank, status.MPI_TAG);

        printf("Process with rank [%d] has the following array:\n", rank);
        for (i = 0; i < SIZE; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
