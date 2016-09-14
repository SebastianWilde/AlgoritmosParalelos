#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
int thread_count,n,m;
double *matrix,*result,*vec;
void Llenarmatriz(double * matriz, int fila, int col)
{
    int i;
    for (i=0;i<(fila*col);i++) 
    {
        *(matriz + i)=1+(double)(10*random()/(RAND_MAX+1.0));
    }
    return;
}
void Mostrarmatriz(double *matriz, int fila, int col)
{
    int i,j;
    for ( i = 0; i < fila; i++) 
    {
        for (j = 0; j < col; j++) 
        {
            printf("%7g ",matriz[i*col+j]);
        }
        printf("\n");
    }
    return;
}
void* Pth_mat_vect(void* rank)
{
	long my_rank=(long) rank;
	int i,j;
	int local_m=m/thread_count;
	int my_first_row=my_rank*local_m;
	int my_last_row=(my_rank+1)*local_m-1;

	for(i=my_first_row;i<=my_last_row;i++)
	{
		result[i]=0.0;
		for(j=0;j<n;j++)
			result[i]=matrix[i*n+j]*vec[j];
	}
	return NULL;
}
int main (int argc, char * argv[])
{
	long thread;
	pthread_t* thread_handles;
	thread_count=strtol(argv[1],NULL,10);
	thread_handles=malloc (thread_count*sizeof(pthread_t));
	m=4;
	n=4000;
	vec=malloc(n*sizeof(double));
    result=malloc(n*sizeof(double));
    matrix=malloc(m*n*sizeof(double));
    Llenarmatriz(matrix,m,n);
    Llenarmatriz(vec,n,1);
    clock_t t;
    t = clock();
	for (thread=0;thread<thread_count;thread++)
		pthread_create(&thread_handles[thread],NULL,Pth_mat_vect,(void*)thread);
	for (thread=0;thread<thread_count;thread++)
		pthread_join(thread_handles[thread],NULL);
	free(thread_handles);
	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("La multiplicacion tomó %f segundos en ejecutarse \n", time_taken);
	/*printf("Here la matriz\n");
	Mostrarmatriz(matrix,m,n);
	printf("Here vector\n");
	Mostrarmatriz(vec,1,n);
	printf("Here el resultado\n");
	Mostrarmatriz(result,1,n);*/
	return 0;
}