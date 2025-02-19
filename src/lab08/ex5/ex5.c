#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0
#define MAGIK_NUM 69

int main (int argc, char **argv) {
    int numtasks, rank;
    int value, tag;

    int i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Checks the number of processes allowed.
    if (numtasks != 2) {
        printf("Wrong number of processes. Only 2 allowed!\n");
        MPI_Finalize();
        return EXIT_SUCCESS;
    }

    // How many numbers will be sent.
    int send_numbers = 10;

    if (rank == 0) {
        srand(time(NULL));
        // Generate the random numbers.
        // Generate the random tags.
        // Sends the numbers with the tags to the second process.
        for (i = 0; i < send_numbers; ++i) {
            value = rand() % MAGIK_NUM;
            tag = rand() % MAGIK_NUM;

            printf("Process [%d] recv %d with tag %d.\n", rank, value, tag);
            MPI_Send(&value, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        }
    } else {
        // Receives the information from the first process.
        // Prints the numbers with their corresponding tags.
        for (i = 0 ; i < send_numbers; ++i) {
            MPI_Status status;
    
            MPI_Recv(&value, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Process [%d] sent value %d with tag %d.\n", status.MPI_SOURCE, value, status.MPI_TAG);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
