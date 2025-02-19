#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0

int main (int argc, char **argv) {
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Root process generates a random number.
    // Broadcasts to all processes.

    int rand_num;
    rand_num = rand() % 100;

    if (rank == ROOT) {
        printf("RANK[%d] BEFORE BROADCAST[%d].\n", rank, rand_num);
        rand_num++;
    	MPI_Bcast(&rand_num, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(&rand_num, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        printf("RANK[%d] AFTER BROADCAST[%d].\n", rank, rand_num);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}