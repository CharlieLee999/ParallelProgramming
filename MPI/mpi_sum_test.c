// mpicc mpi_sum_test.c -Wall -o test.x

/* 
MPI_Reduce(sendBuffer, recvBuffer, count, MPI_Type, Operation, root, comm)

Operation:
MPI_MAX	MPI_MIN	MPI_SUM	MPI_PROD	MPI_MAXLOC	MPI_MINLOC	MPI_BAND	MPI_BOR	
MPI_BXOR	MPI_LAND	MPI_LAND	MPI_LOR	    MPI_LXOR

*/


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main( int argc, char *argv[] )
{
	int i, me, nProcs, *localBuffer, *globalResult, localResult = 0 ;
	int size = atoi( argv[1] );
	
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &me );
	MPI_Comm_size( MPI_COMM_WORLD, &nProcs );
	
	localBuffer = (int *) malloc( size * sizeof(int) );
	
	if( me==0 )
		globalResult = (int *) malloc( 1 * sizeof(int) );
 	
	for( i=0; i<size; i++ )
	{
		localBuffer[i] = i + me;
		printf( "localBuffer[%d] of Procs [%d] is %d\n", i, me, localBuffer[i] );
	}
	
	for( i=0; i<size; i++ )
		localResult += localBuffer[i];
			
	MPI_Reduce(&localResult, globalResult, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );
	
	if( me==0 )
		printf( "\tSum of all localBuffer is %d\n", *globalResult );
		
	MPI_Finalize();
	
	return 0;
}


