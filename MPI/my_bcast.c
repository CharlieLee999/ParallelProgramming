// mpicc my_bcast.c -Wall -o test.x

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int my_bcast( double *, int, int, int, int, int );

int main( int argc, char *argv[] )
{
   int i, me, nProcs, root, size=7;   
   double *buffer;
   
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &me );
   MPI_Comm_size( MPI_COMM_WORLD, &nProcs );

   if( me == 0 && argc < 2 ){
      printf( "specify the root process, please\n" ); return -1;
   }
   root = atoi(argv[1]); 

   buffer = (double *) malloc( size * sizeof(double) );
   if( me == root )
      for( i=0; i<size; i++ ) buffer[i] = 0.2345*i+1.0;
      
   //   printf( "[%d]\t{%lf, %lf, ..., %lf}\n",
   //           me, buffer[0], buffer[1], buffer[size-1] );
   my_bcast( buffer, size, me, root, 0, nProcs-1 );
   
   printf( "[%d]\t\t\t\t\t{%lf, %lf, ..., %lf}\n",
           me, buffer[0], buffer[1], buffer[size-1] );
   
   MPI_Finalize();
   return 0;
}


int my_bcast( double *buffer, int size, int me, int root, int left, int right ){
   int mid_point, next_root, TAG;
   MPI_Status status;
   printf("[%d]  {root:%d left:%d right:%d}\n", me, root, left, right );
   
   if( left == right ) return 1;

   mid_point = (int)(left + right)/2;
   if( root <= mid_point ) next_root = right;
   else                    next_root = left;

   TAG = 123;
   if( me == root ){
      //      printf("[%d] sending to %d\n", me, next_root );
      MPI_Send( buffer, size, MPI_DOUBLE, next_root, TAG, MPI_COMM_WORLD );
   }
   
   if( me == next_root ){
      //      printf("[%d]\t\t\treceiving from %d\n", me, root );
      MPI_Recv(  buffer, size, MPI_DOUBLE, root, TAG, MPI_COMM_WORLD, &status );
   }

   int left_root, right_root;

   if( root <= mid_point ){ left_root = root;      right_root = next_root; }
   else                   { left_root = next_root; right_root = root; };
   
   if( me <= mid_point )
      my_bcast( buffer, size, me, left_root,  left,        mid_point );
   else
      my_bcast( buffer, size, me, right_root, mid_point+1, right );

   
   return 0;   
}

