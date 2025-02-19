#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#if false
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif

#define CONVERGENCE_COEF 100

/**
 * Run: mpirun -np 12 ./a.out
 */

static int num_neigh;
static int *neigh;

// Reads the neighbors of each process from a file
void read_neighbours(int rank) {
    FILE *fp;
    char file_name[20];
    sprintf(file_name, "./files/%d.in", rank);

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s for process %d\n", file_name, rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (fscanf(fp, "%d", &num_neigh) != 1) {
        printf("Error reading number of neighbours for process %d\n", rank);
        fclose(fp);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    neigh = malloc(sizeof(int) * num_neigh);
    if (neigh == NULL) {
        printf("Memory allocation failed for process %d\n", rank);
        fclose(fp);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (size_t i = 0; i < num_neigh; i++) {
        if (fscanf(fp, "%d", &neigh[i]) != 1) {
            printf("Error reading neighbour %zu for process %d\n", i, rank);
            fclose(fp);
            free(neigh);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    fclose(fp);
}

// Determines the parent-child relationships in the distributed topology
int* get_dst(int rank, int numProcs, int leader) {
    MPI_Status status;

    int *v = malloc(sizeof(int) * numProcs);
    int *vRecv = malloc(sizeof(int) * numProcs);

    if (v == NULL || vRecv == NULL) {
        printf("Memory allocation failed at process %d\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

	for (int i = 0; i < numProcs; i++) {
		v[i] = -1;
		vRecv[i] = -1;
	}

	if (rank == leader) {
		v[rank] = -1;
	} else {
        // Non-leader processes wait for a message from a parent
		MPI_Recv(vRecv, numProcs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		v[rank] = status.MPI_SOURCE;
	}


    // Each process sends its parent vector to neighbors except its own parent
	for (int i = 0; i < num_neigh; i++) { 
		if (v[rank] != neigh[i]) {
			MPI_Send(v, numProcs, MPI_INT, neigh[i], 0, MPI_COMM_WORLD);
		}
	}

    // Each process updates its parent vector with received vector of parents
	for (int i = 0; i < num_neigh; i++) {
		if (v[rank] != neigh[i]) {
			MPI_Recv(vRecv, numProcs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			for (int j = 0; j < numProcs; j++) {
				if (v[j] != vRecv[j] && vRecv[j] != -1) {
					v[j] = vRecv[j];
				}
			}
		}
	}

    // Non-leader processes propagate the updated parent vector to their parent and wait for the full topology
	if (rank != leader) {
		MPI_Send(v, numProcs, MPI_INT, v[rank], 0, MPI_COMM_WORLD);
		MPI_Recv(v, numProcs, MPI_INT, v[rank], 0, MPI_COMM_WORLD, &status);
	}

    // Each process sends the final topology only to its children
	for (int i = 0; i < num_neigh; i++) {
		if (v[neigh[i]] == rank) {
			MPI_Send(v, numProcs, MPI_INT, neigh[i], 0, MPI_COMM_WORLD);
		}
	}

	for (int i = 0; i < numProcs && rank == leader; i++) {
		printf("The node %d has the parent %d\n", i, v[i]);
	}

	free(vRecv);
	return v;
}

// Determines the leader process using a distributed election algorithm
int leader_chosing(int rank, int nProcesses) {
	MPI_Status status;
	int leader = rank;

    // Each process sends and receives leader information iteratively until convergence
	for (int k = 0; k < CONVERGENCE_COEF; k++) {
		// Each process sends its leader to neighbors that it knows and waits for a message from any neighbor
		// If the received leader is greater than the current leader, the current leader is updated
		for (int i = 0; i < num_neigh; i++) { 
			MPI_Send(&leader, 1, MPI_INT, neigh[i], 0, MPI_COMM_WORLD);
		}

		for (int i = 0; i < num_neigh; i++) { 
			int newLeader;
			MPI_Recv(&newLeader, 1, MPI_INT, neigh[i], 0, MPI_COMM_WORLD, &status);
            if (newLeader > leader) {
                leader = newLeader;
            }
        }
    }

	MPI_Barrier(MPI_COMM_WORLD);
	printf("%i/%i: leader is %i\n", rank + 1, nProcesses, leader);

	return leader;
}

// Estimates the total number of processes using a distributed averaging method
int get_number_of_nodes(int rank, int leader) {
    MPI_Status status;

	double val = (leader == rank) ? 1.0 : 0.0;
    double recvd = 0;

	// Each process sends its value to neighbors and updates 
	// its value with the average of the received values
	for (int k = 0; k < CONVERGENCE_COEF; k++) {
		for (int i = 0; i < num_neigh; i++) {
			MPI_Send(&val, 1, MPI_DOUBLE, neigh[i], 0, MPI_COMM_WORLD);
			MPI_Recv(&recvd, 1, MPI_DOUBLE, neigh[i], 0, MPI_COMM_WORLD, &status);
			val = (val + recvd) / 2;
		}
	}
	
    MPI_Barrier(MPI_COMM_WORLD);
    return (int)((round)  (1 / val));
}

int **get_topology(int rank, int nProcesses, int * parents, int leader) {
    MPI_Status status;

    int **topology = malloc(nProcesses * sizeof(int *));
    int **vTopology = malloc(nProcesses * sizeof(int *));
    
    if (topology == NULL || vTopology == NULL) {
        printf("Memory allocation failed at process %d\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (size_t i = 0; i < nProcesses; i++) {
        topology[i] = calloc(nProcesses, sizeof(int));
        vTopology[i] = calloc(nProcesses, sizeof(int));

        if (topology[i] == NULL || vTopology[i] == NULL) {
            printf("Memory allocation failed at process %d\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    for (size_t i = 0; i < num_neigh; i++) {
        topology[rank][neigh[i]] = 1;
    }

    // Each process receives topology information from its children
    for (size_t i = 0; i < num_neigh; i++) {
        if (parents[neigh[i]] == rank) {
			DEBUG_PRINT("Process %d waiting to receive topology from child %d\n", rank, neigh[i]);
            for (size_t j = 0; j < nProcesses; j++) {
                MPI_Recv(vTopology[j], nProcesses, MPI_INT, neigh[i], 0, MPI_COMM_WORLD, &status);
            }
            DEBUG_PRINT("Process %d received topology from child %d\n", rank, neigh[i]);
			// Merge topology matrices
            for (size_t j = 0; j < nProcesses; j++) {
                for (size_t k = 0; k < nProcesses; k++) {
                    topology[j][k] |= vTopology[j][k];
                }
            }
        }
    }

    if (rank != leader) {
		// Send topology to parent
		DEBUG_PRINT("Process %d sending topology to parent %d\n", rank, parents[rank]);
        for (size_t j = 0; j < nProcesses; j++) {
            MPI_Send(topology[j], nProcesses, MPI_INT, parents[rank], 0, MPI_COMM_WORLD);
        }
		// Receive complete topology from parent
		DEBUG_PRINT("Process %d waiting for complete topology from parent %d\n", rank, parents[rank]);
		for (size_t j = 0; j < nProcesses; j++) {
			MPI_Recv(topology[j], nProcesses, MPI_INT, parents[rank], 0, MPI_COMM_WORLD, &status);
		}
		DEBUG_PRINT("Process %d received complete topology from parent %d\n", rank, parents[rank]);
	}

	// Send topology to children
    for (size_t i = 0; i < num_neigh; i++) {
        if (parents[neigh[i]] == rank) {
            DEBUG_PRINT("Process %d sending full topology to child %d\n", rank, neigh[i]);
            for (size_t j = 0; j < nProcesses; j++) {
                MPI_Send(topology[j], nProcesses, MPI_INT, neigh[i], 0, MPI_COMM_WORLD);
            }
        }
    }

    for (size_t i = 0; i < nProcesses; i++) {
        free(vTopology[i]);
    }
    free(vTopology);

    return topology;
}

int main(int argc, char * argv[]) {
	int rank, nProcesses, num_procs, leader;
	int *parents, **topology;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

	if (nProcesses != 12) {
		printf("please run with: mpirun --oversubscribe -np 12 %s\n", argv[0]);
		MPI_Finalize();	
		exit(0);
	}
	 
	read_neighbours(rank);
	leader = leader_chosing(rank, nProcesses);
	
	MPI_Barrier(MPI_COMM_WORLD);

	parents = get_dst(rank, nProcesses, leader);

	MPI_Barrier(MPI_COMM_WORLD);

	num_procs = get_number_of_nodes(rank, leader);
	
	printf("%d/%d There are %d processes\n", rank + 1, nProcesses, num_procs);

	topology = get_topology(rank, nProcesses, parents, leader);

	for (size_t i = 0; i < nProcesses && rank == 0; i++)
	{
		for (size_t j = 0; j < nProcesses; j++)
		{
			printf("%2d ", topology[i][j]);	
		}
		printf("\n");
	}
	
	MPI_Finalize();
	return 0;
}