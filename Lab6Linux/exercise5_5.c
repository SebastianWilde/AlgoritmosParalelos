#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) 
{
	float a [] = {4.0, 3.0, 3.0, 1000.0};
	int i ;
	float sum = 0.0;
	for ( i = 0; i < 4; i ++)
	sum += a [ i ];
	printf ("sum = %4.1f\n", sum );
	float sum2 = 0.0;
	# pragma omp parallel for num_threads (2) \
	reduction (+: sum2 )
	for ( i = 0; i < 4; i ++)
	sum2 += a [ i ];
	printf ("sum2 = %4.1f\n", sum2);
	return 0;
}