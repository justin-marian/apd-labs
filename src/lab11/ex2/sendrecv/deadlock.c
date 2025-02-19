#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 500000

int main(int argc, char *argv[]) {
    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(42);
    int num1[SIZE], num2[SIZE];

    if (rank == 0) {
        for (int i = 0; i < SIZE; i++) {
            num1[i] = 100;
        }
        // MPI_Send(&num1, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
        // MPI_Recv(&num2, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Use MPI_Sendrecv to combine send and receive in one call
        printf("Sending data to process 1\n");
        MPI_Sendrecv(num1, SIZE, MPI_INT, 1, 0, num2, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Receiving data from process 0\n");
    } else {
        for (int i = 0; i < SIZE; i++) {
            num2[i] = 200;
        }
        // MPI_Send(&num1, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // MPI_Recv(&num2, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Use MPI_Sendrecv to combine send and receive in one call
        printf("Sending data to process 0\n");
        MPI_Sendrecv(num2, SIZE, MPI_INT, 0, 0, num1, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Receiving data from process 1\n");
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}