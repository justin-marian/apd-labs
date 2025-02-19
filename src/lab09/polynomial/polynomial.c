#include<mpi.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define MASTER 0

int main(int argc, char **argv) {
	int rank, nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses - 1);

	if (rank == MASTER) { // This code is run by a single process
        int polynomialSize, n;
        int x = 5; // the value used to calculate the polynomial - f(5)

        /*
            In the input file, the format is as follows:
            number_of_coefficients
            coefficient x^0
            coefficient x^1
            etc.
        */

        FILE *polFunctionFile = fopen(argv[1], "rt");
        if (!fscanf(polFunctionFile, "%d", &polynomialSize)) {
            fprintf(stderr, "Error reading polynomial size\n");
        }
        /*
            The coefficients of the equation/polynomial will be saved in the array 'a.'
            For example, a = {1, 4, 4} => 1 * (x^2) + 4 * (x^1) + 4 * (x^0)
        */
        float *a = malloc(sizeof(float) * polynomialSize);

        for (int i = 0; i < polynomialSize; i++) {
            if (!fscanf(polFunctionFile, "%f", &a[i])) {
                fprintf(stderr, "Error reading polynomial coefficient\n");
            }
            printf("Read value %f\n", a[i]);
            /*
                Coefficients for x^1, x^2, etc. are sent to processes 1, 2, etc.
                Process 0 handles x^0 and has the value of the coefficient for x^0.
            */
            if (i != 0)
                MPI_Send(&a[i], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }

        fclose(polFunctionFile);

        // Send the values of x and the partial sum (in this case, the value of the coefficient for x^0)
        MPI_Send(&a[0], 1, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&x, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        
    } else {
        float val, sum;
        int x;

        /*
            Receive:
            - The coefficient corresponding to the process (e.g., Process 1 receives the coefficient for x^1)
            - The partial sum
            - The value of x from f(x)
            Calculate the value for c * x^r, where r is the rank of the current process
            and c is the coefficient for x^r, and add it to the sum.
        */
        MPI_Recv(&val, 1, MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, NULL);
        MPI_Recv(&sum, 1, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, NULL);
        MPI_Recv(&x, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);
        sum += val * pow(x, rank);

        if (rank == nProcesses - 1) {
            printf("Polynomial value is %f\n", sum);
        } else {
            // Send x and the partial sum to the next process
            MPI_Send(&sum, 1, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&x, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}