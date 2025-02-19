#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

/** PSEUDOCODE SCAN -- DON'T USE MPI_Scan!
 for (i = 1; i < procs; i *= 2)
 |   if (rank + i < procs)
 |   |    Send to P with RANK = [rank + i]
 |   |    if (rank - i >= 0)
 |   |    |    Recv to P with RANK = [rank - i]
 |   |    |    ADD VALUE RECV n
 |   |    -
 |   -
 -
*/

/** Example SCAN :
    l = [1, 2, 3, 4, 5, 6]
    op = +
    result = 1 + 2 + 3 + 4 + 5 + 6 = 21

    Step:
    [1, 2, 3, 4, 5, 6]
    [1, 3, 3, 4, 5, 6]
    [1, 3, 6, 4, 5, 6]
    [1, 3, 6, 10, 5, 6]
    [1, 3, 6, 10, 15, 6]
    [1, 3, 6, 10, 15, 21] -> result is 21

    LAST PROCESS GIVES US THE result !!!
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

    for (i = 1; i < procs; i *= 2) {
        if (rank + i < procs) {
            MPI_Send(&value, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD);
            printf("Process [%d] SEND result = %d\n", rank, value);
        }
        if (rank - i >= 0) {
            MPI_Recv(&n, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD, &status);
            value += n;
            printf("Process [%d] RECV result = %d\n", rank, value);
        }
    }

    if (rank == procs - 1) {
        printf("Final result = %d\n", value);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
