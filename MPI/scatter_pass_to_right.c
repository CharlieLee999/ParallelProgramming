// mpicc scatter_pass_to_right.c -Wall -o test.x

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main( int argc, char *argv[] )
{
  int i, me, nProcs, chunk_size,total_size,  *src_buffer, *rec_buffer;
  int *send_head_addr, *rec_head_addr, left_procs, right_procs;
  int TAG = 100;
  MPI_Status status;
  chunk_size = atoi( argv[1] );
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &me );
  MPI_Comm_size( MPI_COMM_WORLD, &nProcs );

  total_size = nProcs * chunk_size; 
  rec_buffer = (int *) malloc( total_size * sizeof(int) );

  if( me == 0 )
	{
	  src_buffer = (int *) malloc( total_size * sizeof(int) ); 
	  for( i=0; i<total_size; i++ )
		src_buffer[i] = i;
	}
  MPI_Scatter( src_buffer, chunk_size, MPI_INT,
			   &rec_buffer[me*chunk_size], chunk_size, MPI_INT, 0, MPI_COMM_WORLD );

  for( i=0; i<chunk_size; i++ )
	//printf( "me[%d] received rec_buffer[%d]: %d\n", me, me*chunk_size+i, rec_buffer[me*chunk_size+i] );
  
  for( i=0; i<nProcs-1; i++)
  {
	send_head_addr = &rec_buffer[(me-i+nProcs)%nProcs * chunk_size];
	rec_head_addr  = &rec_buffer[(me-i-1+nProcs)%nProcs * chunk_size];
	left_procs     = (me - 1 + nProcs) % nProcs;
	right_procs    = (me + 1 + nProcs) % nProcs;
	
	MPI_Send( send_head_addr, chunk_size, MPI_INT, right_procs, TAG, MPI_COMM_WORLD );
	MPI_Recv( rec_head_addr,  chunk_size, MPI_INT, left_procs,  TAG, MPI_COMM_WORLD, &status );
  }

  for( int j=0; j<nProcs; j++)
  {
	//printf( "j is %d\n", j);
	if( me == j )
	for( i=0; i<total_size; i++)
	{
	  printf("\tme[%d] received rec_buffer[%d]: %d\n", me, i, rec_buffer[i] );
	}
  }

  free(rec_buffer);
  MPI_Finalize();
  return 0;
}
