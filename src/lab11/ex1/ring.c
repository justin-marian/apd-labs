#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv){
    int numtasks, rank;
    int recv_num, send_num;
    int next, prev, tag = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Request send_request, recv_request;

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number.
        send_num = rand() % 100;
        // Send the number to the next process.
        next = rank + 1;
        printf("Process MASTER %d sends value: %d.\n", rank, send_num);
        MPI_Isend(&send_num, 1, MPI_INT, next, tag, MPI_COMM_WORLD, &send_request);
    } else if (rank == numtasks - 1) {
        // Last process close the circle.
        prev = rank - 1;
        // Receives the number from the previous process.
        MPI_Irecv(&recv_num, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        printf("Process %d received value: %d\n", rank, recv_num);
        // Increments the number.
        recv_num += 2;
        // Sends the number to the first process.
        MPI_Isend(&recv_num, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &send_request);
    } else {
        // Middle process.
        prev = rank - 1;
        next = rank + 1;
        // Receives the number from the previous process.
        MPI_Irecv(&recv_num, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        printf("Process %d received value: %d\n", rank, recv_num);
        // Increments the number.
        recv_num += 2;
        // Sends the number to the next process.
        MPI_Isend(&recv_num, 1, MPI_INT, next, tag, MPI_COMM_WORLD, &send_request);
    }

    if (rank == 0) {
        MPI_Irecv(&recv_num, 1, MPI_INT, numtasks - 1, tag, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        printf("Process %d received value: %d.\n", rank, recv_num);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
