all:
	gcc pi_montecarlo.c -fopenmp -o pi_montecarlo

mpi:
	mpic++ pi_montecarlo_mpi.cpp -o pi_montecarlo_mpi
	mpirun -np 4 pi_montecarlo_mpi

pi_montecarlo:
	gcc pi_montecarlo.c -fopenmp -o pi_montecarlo

clean:
	rm -f pi_montecarlo