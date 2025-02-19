#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int recv_num;

int main (int argc, char **argv) {
    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Generate a unique seed based on the process rank.
    srand(rank);
    recv_num = rand();

    if (rank == 0) {

        // First process starts the circle.
        // Send the number to the next process.
        MPI_Status status;

        fprintf(stdout, "Before SEND : rank[%d], number[%d].\n", rank, recv_num);
        MPI_Send(&recv_num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD, &status);
        fprintf(stdout, "After  RECV : rank[%d], number[%d], tag[%d].\n", rank, recv_num, status.MPI_TAG);

    } else if (rank == numtasks - 1) {

        // Last process closes the circle.
        // Receives the number from the previous process.
        // Increments the number.
        // Sends the number to the first process.

        MPI_Status status;

        // Check if there's more than one process before receiving and sending.
        MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        recv_num += 2;
        printf("After  RECV : rank[%d], number[%d], tag[%d].\n", rank, recv_num, status.MPI_TAG);
        MPI_Send(&recv_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    } else {

        // Middle process.
        // Receives the number from the previous process.
        // Increments the number.
        // Sends the number to the next process.

        MPI_Status status;

        // Check if there's more than one process before receiving and sending.
        MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        recv_num += 2;
        printf("After  RECV : rank[%d], number[%d], tag[%d].\n", rank, recv_num, status.MPI_TAG);
        MPI_Send(&recv_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
