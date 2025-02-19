#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

/** PSEUDOCODE REDUCE
 for (pas = 2; pas <= nr_procese; pas *= 2)
 |   if (rank % pas == 0)
 |   |    Send to P[id] with RANK[id]=[rank + (pas / 2)]
 |   |    ADD VALUE RECV n
 |   -
 |   else if (rank % (pas / 2) == 0)
 |   |    Send to P[id] with RANK[id]=[rank - (pas / 2)]
 |   -
 -
*/

int main (int argc, char **argv) {
    MPI_Status status;
    int procs, rank;

    int i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    int value = rank;

    for (i = 2; i <= procs; i *= 2) {
        if (rank % i == 0) {
            MPI_Recv(&n, 1, MPI_INT, rank + i / 2, 0, MPI_COMM_WORLD, &status);
            value += n;
        } else if (rank % (i / 2) == 0) {
            MPI_Send(&value, 1, MPI_INT, rank - i / 2, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == MASTER) {
        printf("Result = %d\n", value);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}

