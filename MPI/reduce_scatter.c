// mpicc reduce_scatter.c -Wall -o test.x 
// mpirun -np 4 ./test.x 2 localSize
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main( int argc, char *argv[] )
{
	int me = 0, nProcs, i, localSize;
	double *localBuffer = NULL, *localResult = NULL, *globalResult = NULL, *localResult_mpi = NULL;
	
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &me );
	MPI_Comm_size( MPI_COMM_WORLD, &nProcs );
	
	localSize = atoi( argv[1] );
	
	localBuffer = (double *) malloc( localSize * nProcs * sizeof(double) );
	localResult = (double *) malloc( localSize * sizeof(double) );

	if( me ==0 ){
		globalResult = (double *) malloc( localSize * nProcs * sizeof(double) );
	}

	for( i=0; i< localSize*nProcs; i++ ){
		
		localBuffer[i] = rand()%10;	
		printf( "Procs[%d]' localBuffer[%d] is %.16g \n", me, i, localBuffer[i] );		
	}
	
	MPI_Reduce( localBuffer, globalResult, localSize*nProcs, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		
	MPI_Scatter( globalResult, localSize, MPI_DOUBLE,
				 localResult,  localSize, MPI_DOUBLE,
				 0, MPI_COMM_WORLD );
	
	if( me==0){
		for( i=0; i< localSize*nProcs; i++){
			printf( "\tGlobalResult[%d] is %.16g\n", i, globalResult[i] );
		}
	}
	
	for( i=0; i< localSize; i++){
		printf( "\t\tProcs (%d)'s localResult[%d]: \t %.16g \n", me, i, localResult[i] );
	}
	
	localResult_mpi = (double *) malloc( localSize * sizeof(double) );
	int *localSizeVec = (int *) malloc( nProcs * sizeof(int) );
	
	for( i=0; i< nProcs; i++){
		localSizeVec[i] = localSize;
	}
	MPI_Reduce_scatter(localBuffer, localResult_mpi, localSizeVec, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
	
	for( i=0; i< localSize; i++){
		printf( "\t\t\tProcs (%d)'s localResult_mpi[%d]: \t %.16g \n", me, i, localResult_mpi[i] );
	}
	
	MPI_Finalize();
	
	free(localBuffer);
	free(localResult);
	free(globalResult);
	
	return 0;
}
