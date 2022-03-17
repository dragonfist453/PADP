#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define BUFFER_SIZE 32

int main(int argc, char* argv[]) {
    int rank, numprocs, dest, iproc;
    int tag = 0;
    int root = 0,temp = 1;
    char* messages[] = {"HELLO", "CSE", "RVCE"};
    char message[BUFFER_SIZE];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != root) {
        strcpy(message, messages[rank-1]);
        MPI_Send(message, BUFFER_SIZE, MPI_CHAR, root, tag, MPI_COMM_WORLD);
    } else {
        for(temp=1;temp<numprocs;temp++){
            MPI_Recv(message, BUFFER_SIZE, MPI_CHAR, temp, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s in process with rank %d from process with rank %d\n", message, root, temp);
        }
    }
    MPI_Finalize();
    return 0;
}