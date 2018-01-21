/* Approximation of pi with Gregory-Leibniz series
   Numerical calculation with mpi
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	

    int i, me=0, nProcs; 
	int max_n = atoi( argv[1] );
	MPI_Status status;

	//	printf("max N is %d\n", max_n);
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &me );
	MPI_Comm_size( MPI_COMM_WORLD, &nProcs );

	if( me==0 && argc < 2){
	    printf( "argument please! \n" );
	    return (-1);
	}
	
    double globalSum=0;
	
	double  *localBuffer, localSum = 0;
	int localChunk = (int)max_n / nProcs;
	localBuffer = (double *) malloc( localChunk  * sizeof(double) );	

	//	printf( "Local chunk size is %d \n", localChunk);
	for (i = 0; i < localChunk; i++){
	    localBuffer[i] = localChunk * me + i;
	    localSum += pow(-1, localBuffer[i] ) / ( 2 * localBuffer[i] + 1 );
		//printf( "local buffer is %lf \n", localBuffer[i]);
	}  

	printf("local sum [%d] is %.9lf \n", me, localSum);
	MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if( me == 0 ){
	  double pi = 4 * globalSum;
	  printf("Numerical Pi is %.9lf \n", pi);
	}

	MPI_Finalize();

	return 0;
}
