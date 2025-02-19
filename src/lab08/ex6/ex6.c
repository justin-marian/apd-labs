#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define GROUP_SIZE 4

int main(int argc, char **argv) {
    int old_size, new_size;
    int old_rank, new_rank;

    int recv_rank;
    MPI_Comm custom_group;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &old_size); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &old_rank); // The current process ID / Rank.

    // Determine the new rank and size within the group.
    int group_number = old_rank / GROUP_SIZE;

    // Split the MPI_COMM_WORLD in small groups.
    // Create sub-communicators for groups of 4 processes.
    MPI_Comm_split(MPI_COMM_WORLD, group_number, old_rank, &custom_group);
    MPI_Comm_size(custom_group, &new_size);
    MPI_Comm_rank(custom_group, &new_rank);

    // Send the rank to the next process within the group.
    int next_rank = (new_rank + 1) % GROUP_SIZE;
    int prev_rank = (new_rank - 1 + GROUP_SIZE) % GROUP_SIZE;

    // Send the rank to the next process.
    MPI_Send(&new_rank, 1, MPI_INT, next_rank, 0, custom_group);
    // Receive the rank.
    MPI_Recv(&recv_rank, 1, MPI_INT, prev_rank, 0, custom_group, MPI_STATUS_IGNORE);

    // Display information.
    printf("Rank [%d] / size [%d] in MPI_COMM_WORLD and rank [%d] / size [%d] in custom group.\n",
           old_rank, old_size, new_rank, new_size);
    printf("Process [%d] from group [%d] received [%d].\n",
           new_rank, group_number, recv_rank);

    MPI_Comm_free(&custom_group);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
