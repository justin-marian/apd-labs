#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MULTI 5 // chunk dimension
#define ROOT 0

int main (int argc, char **argv) {
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int i;
    int num_elements = MULTI * numtasks;                            // total elements
    int *v_send = (int *)malloc(num_elements * sizeof(int));        // full vector
    int *v_recv = (int *)malloc(MULTI * sizeof(int));               // partial vector

    // ROOT process generates the values for the full vector.
    if (rank == 0) {
        for (i = 0; i < num_elements; ++i) {
            v_send[i] = i;
        }
    }

    // Scatter the vector to all processes.
    MPI_Scatter(v_send, MULTI, MPI_INT, v_recv, MULTI, MPI_INT, 0, MPI_COMM_WORLD);

    /*
     * Prints the values received after scatter.
     * NOTE: If MULTI changed, also change this line.
     */
    printf("Process [%d]: have elements %d %d %d %d %d.\n", rank, 
            v_recv[0], v_recv[1], v_recv[2], v_recv[3], v_recv[4]);

    // Each process increments the values of the partial vector received.
    // Gathers the values from all the processes.
    // The ROOT process prints the elements received.
    for (i = 0; i < MULTI; ++i)  v_recv[i] += rank;

    MPI_Gather(v_recv, MULTI, MPI_INT, v_send, MULTI, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        for (i = 0; i < num_elements; ++i) {
            printf("%d ", v_send[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
