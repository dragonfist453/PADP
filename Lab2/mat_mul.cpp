#include <iostream>
#include <omp.h>
#include<stdlib.h>
using namespace std;

int main()
{
    
    int r1, c1, r2, c2;
    cout << "Enter r1 c1 r2 c2 ";
    cin >> r1 >> c1 >> r2 >> c2;
    srand(0);
    if(r2!=c1) {cerr<<"Not match r2 c1";return 1;}

    int *m1 = (int*)malloc(r1*c1*sizeof(int)),*m2 = (int*)malloc(r2*c2*sizeof(int)),*r = (int*)malloc(r1*c2*sizeof(int));
    if(m1==nullptr or m2==nullptr or r==nullptr) return 1;
    for(int i=0;i<r1;i++) for(int j=0;j<c1;j++) m1[c1*i+j]=i+j;
    for(int i=0;i<r2;i++) for(int j=0;j<c2;j++) m2[c2*i+j]=i-j;
    for(int i=0;i<r1;i++) for(int j=0;j<c2;j++) r[c2*i+j]=0;

    double t =omp_get_wtime();
    #pragma omp parallel for num_threads(6)
    for(int i=0;i<r1;i++) for(int j=0;j<c2;j++) for(int k=0;k<r2;k++) r[c2*i+j]+=m1[c1*i+k]*m2[c2*k+j];
    t = omp_get_wtime()-t;

    cout<<"Done, t="<<t<<'\n';


    return 0;
}