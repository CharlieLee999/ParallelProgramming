// mpicc 9-COMM-split.c -Wall -o test.x

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main( int argc, char *argv[] )
{
   int me_WORLD, nP_WORLD;   

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &me_WORLD );
   MPI_Comm_size( MPI_COMM_WORLD, &nP_WORLD );

   if( nP_WORLD % 5 != 0 && me_WORLD == 0 ){
      printf( "[%d] nP is not a multiple of 5 -- try again\n", me_WORLD );
      return -1;
   }

   int color, key;
   MPI_Comm COMM_row;
   color = me_WORLD / 5;  // We logically map the processes onto a grid of
                          // size r * 5
   key   = me_WORLD;      // for ordering purposes
   
   // procs           ->  me/5
   // 0  1  2  3  4       0 0 0 0 0   -> same communicator
   // 5  6  7  8  9       1 1 1 1 1
   // 10 11 12 13 14      2 2 2 2 2
   // ...                 . . . 


   // row communicator 
   MPI_Comm_split( MPI_COMM_WORLD, color, key, &COMM_row );

   int me_ROW, nP_ROW;
   MPI_Comm_rank( COMM_row, &me_ROW );
   MPI_Comm_size( COMM_row, &nP_ROW );
   
   if( color == 1 )
   printf("[%d]\tCOMM_row rank: %d / %d\n", me_WORLD, me_ROW, nP_ROW );

   
  #if 0
   // 0  1  2  3  4       0 1 2 3 4   
   // 5  6  7  8  9       0 1 2 3 4
   // 10 11 12 13 14      0 1 2 3 4
   // ...                 . . .    

   // column communicator    
   MPI_Comm COMM_col;
   int me_COL, nP_COL;
   MPI_Comm_split( MPI_COMM_WORLD, me_WORLD%5, key, &COMM_col );

   MPI_Comm_rank( COMM_col, &me_COL );
   MPI_Comm_size( COMM_col, &nP_COL );
   printf("[%d]\t\t\tCOMM_col rank: %d / %d\n", me_WORLD, me_COL, nP_COL );






   // bcast by rows 
   MPI_Barrier( MPI_COMM_WORLD );
   printf(" [%d]\t before bcast: %d\n", me_ROW, me_WORLD );
   MPI_Bcast( &me_WORLD, 1, MPI_INT, nP_ROW-1, COMM_row );
   printf(" [%d]\t\t\t after bcast:  %d\n", me_ROW, me_WORLD );





   // reduction by columns
   MPI_Barrier( MPI_COMM_WORLD );
   int result = 0;
   printf("[%d] my-data: %d\n", me_WORLD, me_COL );
   MPI_Reduce( &me_COL, &result, 1, MPI_INT, MPI_MAX, nP_COL-1, COMM_col );
   if( me_COL == nP_COL-1 )
      printf("[%d]\t\t\t Reduction: %d\n", me_WORLD, result );
#endif 
   
   
   MPI_Comm_free( &COMM_row );
   //MPI_Comm_free( &COMM_col );
   MPI_Finalize();
   return 0;
}


