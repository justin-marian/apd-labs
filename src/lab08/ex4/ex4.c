#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT 3

int main (int argc, char **argv) {
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    int i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int value;

    if (rank == ROOT) {

        // The ROOT process receives an element from any source.
        // Prints the element and the source. HINT: MPI_Status.
        MPI_Status status;

        for (i = 0 ; i < 3; ++i) {
            MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            printf("Process [%d] sent %d.\n", status.MPI_SOURCE, value);
        }

    } else {

        // Generate a random number.
        srand(time(NULL));
        value = rand() % (rank * 5 + 1);

        printf("Process [%d] send %d.\n", rank, value);

        // Sends the value to the ROOT process.
        MPI_Send(&value, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
