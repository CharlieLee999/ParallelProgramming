// mpicc my_scatter.c -Wall -o test.x

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int my_scatter( int *buffer, int size, int me, int root, int left, int right );

int main( int argc, char *argv[] )
{
  int i, me, nProcs, root, size = 11;
  int *buffer;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &me );
  MPI_Comm_size( MPI_COMM_WORLD, &nProcs );

  root = atoi( argv[1] );

  buffer = (int *) malloc( size * sizeof(int) );
  if( me == root )
	for( i=0; i<size; i++ )
	  buffer[i] = i;

  my_scatter( buffer, size, me, root, 0, nProcs-1 );

  printf( "[%d] reveived %d\n", me, buffer[me] );
  
  free(buffer);
  MPI_Finalize();
  return 0;
}

int my_scatter( int *buffer, int size, int me, int root, int left, int right )
{
  int mid_point, next_root, TAG, left_root, right_root;
  MPI_Status status;
  int *send_head_addr, send_size;
  TAG = 123;
  
  //printf("me[%d] {root: %d  left: %d  right: %d}\n", me, root, left, right );

  if( left == right )
	return 1;

  mid_point = ( left + right ) / 2;

  if( root <= mid_point )
  {
	right_root = next_root = right;
	left_root = root;
	send_head_addr = &buffer[ mid_point+1 ];
	send_size = right - mid_point;
  }
  else
  {
	left_root  = next_root = left;
	right_root = root;
	send_head_addr = &buffer[ left ];
	send_size = mid_point - left  + 1;
  }

  if( me == root )
	MPI_Send( send_head_addr, send_size, MPI_INT, next_root, TAG, MPI_COMM_WORLD );

  if ( me == next_root )
	MPI_Recv( send_head_addr, send_size, MPI_INT, root,      TAG, MPI_COMM_WORLD, &status );

  if( me <= mid_point )
	my_scatter( buffer, size, me, left_root, left, mid_point );
  //  	my_scatter( &buffer[left], mid_point-left+1, me, left_root, left, mid_point );
  else
	my_scatter( buffer, size, me, right_root, mid_point+1, right );
  //my_scatter( &buffer[mid_point+1], right-mid_point, me, right_root, mid_point+1, right );

  return 0;
}
