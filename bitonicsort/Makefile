CC=gcc
CFLAGS=-I.
LIBS=-lm
LIBS_OPENMP=-lm -fopenmp

all: bitonic bitonic_openmp bitonic_mpi

bitonic: bitonic.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS_OPENMP)

bitonic_openmp: bitonic_openmp.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS_OPENMP)


bitonic_mpi: bitonic_mpi.c
	mpicc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean all

clean:
	rm -f bitonic bitonic_openmp bitonic_mpi