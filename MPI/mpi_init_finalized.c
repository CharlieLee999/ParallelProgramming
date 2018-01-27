// mpicc mpi_init_finalized.c -Wall -o test.x

/* 
Test MPI_Initialized( int *flag )
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main( int argc, char* argv[] )
{

    int me, size, flag_init, flag_final; 
    MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &me );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Initialized( &flag_init );
	MPI_Finalized( &flag_final );

	printf( "MPI_Initialized: %d\n", flag_init );
	printf( "MPI_finalized: %d\n", flag_final );
	
	MPI_Finalize();
}
