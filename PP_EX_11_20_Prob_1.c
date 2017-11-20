#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);
    int nthreads = atoi(argv[2]);
    
    
    double *b;
    b = (double *) malloc(N * sizeof(double));
    
    int i, j;
    omp_set_nested(1);
    #pragma omp parallel for private(i, j) num_threads(2)
        for (i = 0; i<N; i++)
        {
            b[i] = 1.0;
            #pragma omp parallel for private(j) num_threads(2)
            for (j = 1; j<i; j++)
                b[i] = b[i] * j;
        }
    
    for (i=0; i< N; i++)
        printf("%.3f\n", b[i]);
    
}
