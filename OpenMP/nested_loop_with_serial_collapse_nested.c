#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main( void )
{
    int i, j, n = 5;
    int counter = 0;
    
    // *** serial solution ***
    for ( i = 0; i < n; i++ ) {
        for ( j = 0; j < n; j++ ) {
        	counter++;
        	//printf( "Num threads: %d\n", omp_get_num_threads());
        }
    }
    printf( "counter is %d\n", counter );
    counter = 0;    
    
    
    
    // *** one solution ***
    // *** collapse(2)  ***
    #pragma omp parallel for num_threads(4) collapse(2) reduction(+:counter)
    for ( i = 0; i < n; i++ ) {
        for ( j = 0; j < n; j++ ) {
        	counter++;
        	//printf( "Num threads: %d\n", omp_get_num_threads());
        }
    }
    printf( "counter is %d\n", counter );
    counter = 0;
    
    // *** another solution ***
    // *** nested ***
    // Try also uncommenting the line below
    omp_set_nested(1);
    printf("Nested? %s\n", omp_get_nested() ? "TRUE" : "FALSE" );
    #pragma omp parallel for num_threads(4) reduction(+:counter)
    for ( i = 0; i < n; i++ ) {
        //printf( "Num threads: %d\n", omp_get_num_threads());
        #pragma omp parallel for num_threads(4) reduction(+:counter)
        for ( j = 0; j < n; j++ ) {
        	counter++;
        	//printf( "Num threads: %d\n", omp_get_num_threads());
            //printf( "\tNum threads: %d\n", omp_get_num_threads());
            //printf( "\t\t I am thread %d \n", omp_get_thread_num() );
        }
    }
    
    printf( "counter is %d\n", counter );
}
