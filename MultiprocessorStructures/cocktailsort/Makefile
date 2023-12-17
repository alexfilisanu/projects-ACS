all: cocktailsort-serial cocktailsort-mpi cocktailsort-openmp cocktailsort-pthreads cocktailsort-mpi-openmp cocktailsort-mpi-pthreads

cocktailsort-serial: cocktailsort-serial.c utilities.c
	gcc -o cocktailsort-serial cocktailsort-serial.c utilities.c

cocktailsort-mpi: cocktailsort-mpi.c utilities.c
	mpicc -o cocktailsort-mpi cocktailsort-mpi.c utilities.c

cocktailsort-openmp: cocktailsort-openmp.c utilities.c
	gcc -fopenmp -o cocktailsort-openmp cocktailsort-openmp.c utilities.c

cocktailsort-pthreads: cocktailsort-pthreads.c utilities.c	
	gcc -pthread -o cocktailsort-pthreads cocktailsort-pthreads.c utilities.c

cocktailsort-mpi-openmp: cocktailsort-mpi-openmp.c utilities.c
	mpicc -fopenmp -o cocktailsort-mpi-openmp cocktailsort-mpi-openmp.c utilities.c

cocktailsort-mpi-pthreads: cocktailsort-mpi-pthreads.c utilities.c
	mpicc -pthread -o cocktailsort-mpi-pthreads cocktailsort-mpi-pthreads.c utilities.c

clean:
	rm -f cocktailsort-serial cocktailsort-mpi cocktailsort-openmp cocktailsort-pthreads cocktailsort-mpi-openmp cocktailsort-mpi-pthreads

.PHONY: all clean
