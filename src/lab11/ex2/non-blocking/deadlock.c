#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 500000

int main(int argc, char *argv[]) {
    int numtasks, rank, len;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // The current process ID / Rank.

    srand(42);
    int num1[SIZE], num2[SIZE];
    MPI_Request request;
    MPI_Status status;

    if (rank == 0) {
        for (int i = 0; i < SIZE; i++) {
            num1[i] = 100;
        }

        // Non-blocking send
        MPI_Isend(num1, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        // Non-blocking receive
        MPI_Irecv(num2, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        // Wait for both sends and receives to complete
        MPI_Barrier(MPI_COMM_WORLD);

        int flag;
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P0] The receive operation is over\n");
        } else {
            printf("[P0] The receive operation is not over yet\n");
            MPI_Wait(&request, &status);
        }
    } else {
        for (int i = 0; i < SIZE; i++) {
            num2[i] = 200;
        }

        // Non-blocking send
        MPI_Isend(num2, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        // Non-blocking receive
        MPI_Irecv(num1, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        // Wait for both sends and receives to complete
        MPI_Barrier(MPI_COMM_WORLD);

        int flag;
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P1] The receive operation is over\n");
        } else {
            printf("[P1] The receive operation is not over yet\n");
            MPI_Wait(&request, &status);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
