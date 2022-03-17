#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#define MPIT 1<<26
using namespace std;
int main(int c,char**v){
    MPI_Init(&c,&v);

    MPI_Status stat;

    float x,y,dist;
    srand(0);
    int rank,size,count=0;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    for(int i=0;i<MPIT;i++){
        x = ((float)rand())/RAND_MAX;
        y = ((float)rand())/RAND_MAX;
        dist = x*x+y*y;
        if(dist<1) count++;
    }

    int sum=0;
    MPI_Reduce(&count,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    // if(rank!=0){
    //     MPI_Send(&count,1,MPI_INT,0,1,MPI_COMM_WORLD);
    // }else{
    //     long sum=count;
    //     for(int i=1;i<size;i++){
    //         int temp;
    //         MPI_Recv(&temp,1,MPI_INT,i,1,MPI_COMM_WORLD,&stat);
    //         sum+=temp;
    //     }
    //     cout<<"PI is approximately "<< ((double)(4*sum))/(size*MPIT);
    // }
    rank==0 && cout<<"PI is approximately "<< ((double)(4*sum))/(size*MPIT)<<endl;

    MPI_Finalize();
}