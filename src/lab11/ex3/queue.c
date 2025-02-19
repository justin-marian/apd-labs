#include <mpi.h>
#include <time.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CAP 1000

typedef struct {
    int size;
    int arr[CAP];
} queue;

int main(int argc, char **argv) {
    int numtasks, rank;
    queue q;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Create an MPI data type for the queue structure
    int blockLengths[4] = {1, 10, 100, 1000};

    MPI_Aint offsets[2];
    offsets[0] = offsetof(queue, size);
    offsets[1] = offsetof(queue, arr);
    MPI_Datatype queueType;
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Type_create_struct(2, blockLengths, offsets, types, &queueType);
    MPI_Type_commit(&queueType);

    srand(time(NULL));

    int numElementsToAdd = 10;

    // First process starts the circle.
    if (rank == 0) {
        q.size = 0;
        for (int i = 0; i < numElementsToAdd; i++) {
            q.arr[q.size] = rand() % CAP;
            q.size++;
        }

        // Send the queue to the next process.
        MPI_Send(&q, 1, queueType, 1, 0, MPI_COMM_WORLD);
    } else if (rank == numtasks - 1) {
        // Last process closes the circle.
        // Receives the queue from the previous process.
        MPI_Recv(&q, 1, queueType, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Generate random numbers and add them to the queue.
        for (int i = 0; i < numElementsToAdd; i++) {
            if (q.size < CAP) {
                q.arr[q.size] = rand() % CAP;
                q.size++;
            }
        }

        // Send the queue to the first process.
        MPI_Send(&q, 1, queueType, 0, 0, MPI_COMM_WORLD);
    } else {
        // Middle processes.
        // Receives the queue from the previous process.
        MPI_Recv(&q, 1, queueType, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Generate random numbers and add them to the queue.
        for (int i = 0; i < numElementsToAdd; i++) {
            if (q.size < CAP) {
                q.arr[q.size] = rand() % CAP;
                q.size++;
            }
        }

        // Send the queue to the next process.
        MPI_Send(&q, 1, queueType, rank + 1, 0, MPI_COMM_WORLD);
    }

    // Process 0 prints the elements from the queue
    if (rank == 0) {
        printf("Elements in the queue: %d\n", q.size);
        for (int i = 0; i < q.size; i++) {
            printf("%d ", q.arr[i]);
        }
        printf("\n");
    }

    // Free the MPI data type
    MPI_Type_free(&queueType);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
