// mpicc my_scatter_matrix_procs.c -Wall -o test.x

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main( int argc, char *argv[] )
{
   int me_WORLD, nP_WORLD;   
   int i;

   int root = atoi( argv[1] );         //global rank of root
   int chunk_size = atoi( argv[2] );   //the size of data each procs has
   int root_COL, root_ROW, *buffer, *recBuffer, *finalBuffer;
   int col_num, row_num;
   
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &me_WORLD );
   MPI_Comm_size( MPI_COMM_WORLD, &nP_WORLD );
   
   if( nP_WORLD % 5 != 0 && me_WORLD == 0 ){
      printf( "[%d] nP is not a multiple of 5 -- try again\n", me_WORLD );
      return -1;
   }

   buffer = (int *) malloc( nP_WORLD * chunk_size * sizeof(int) );
   col_num = 5;
   row_num = nP_WORLD/ col_num;
   
   if( me_WORLD == root )
   {
        for( i=0; i<nP_WORLD * chunk_size; i++ )
		{
		  buffer[i] = i;
		  printf("\t\t Initial buffer[%d] is %d\n", i, buffer[i] );
		}
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
   // printf("[%d]\tCOMM_row rank: %d / %d\n", me_WORLD, me_ROW, nP_ROW );

   

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
   
   if(me_WORLD%5 == root%5 )
   {
   	 recBuffer = (int *) malloc( col_num * chunk_size * sizeof(int) );

	 /* ### WRONG!!! Because only the procs changes the root_col while others don't do that */
	 //if(me_WORLD == root)
	 // root_COL = me_COL;
	 
	 root_COL = (int)root/5;
     printf( "Global rank[%d]' new col root is %d\n", me_WORLD, root_COL );

	 MPI_Scatter(buffer, col_num*chunk_size, MPI_INT, recBuffer, col_num*chunk_size, MPI_INT, root_COL, COMM_col );
	 
	 for( i=0; i<col_num*chunk_size; i++ )
	   printf( "Procs[%d(%d)] receiced recBuffer[%d] is %d\n", me_WORLD, me_COL,  i, recBuffer[i] );
	 //my_scatter(buffer, col_num, me_COL, root_COL, 0, nProcs-1 );
   }
   

   
   for( i=0; i<row_num; i++ )
   {
   	 finalBuffer = (int *) malloc( chunk_size * sizeof(int) );
   	 
   	 root_ROW = (int)root%5;

     if(me_WORLD/5 == i)
     {
     	printf( "Global rank[%d]' new row root is %d\n", me_WORLD, root_ROW );
     	MPI_Scatter( recBuffer, chunk_size, MPI_INT, finalBuffer, chunk_size, MPI_INT, root_ROW, COMM_row );
     	
     	for( i=0; i<chunk_size; i++ )
	      printf("\tProcs[%d] receiced finalBuffer[%d] is %d\n", me_WORLD, i, finalBuffer[i] );
     }
     
     
   }
         
   MPI_Comm_free( &COMM_row );
   MPI_Comm_free( &COMM_col );
   free(buffer);
   //free(recBuffer);
   //free(finalBuffer);
   
   MPI_Finalize();
   return 0;
}


