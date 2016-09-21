#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int thread_count;
struct timeval t_ini1, t_fin1, t_ini2, t_fin2;
double creacion;
double destruccion;

void* example(void* rango)
{
	gettimeofday(&t_fin1, NULL);
	creacion += timeval_diff(&t_fin1, &t_ini1);
	int x=5;
	x+=1;
	gettimeofday(&t_ini2, NULL);
	return NULL;
}
int main(int argc,char* argv[])
{

	long thread;
	pthread_t* thread_handles;
	creacion=destruccion=0;
	thread_count=strtol(argv[1],NULL,10);
	thread_handles=malloc (thread_count*sizeof(pthread_t));
	for(thread=0;thread<thread_count;thread++)
	{
		gettimeofday(&t_ini1, NULL);
		pthread_create(&thread_handles[thread],NULL,example,(void *)thread);
		pthread_join(thread_handles[thread],NULL);
		pthread_detach(thread_handles[thread]);
		gettimeofday(&t_fin2, NULL);
		destruccion += timeval_diff(&t_fin2, &t_ini2);
	}
	double secs1=creacion/thread_count;
	double secs2=destruccion/thread_count;
  	printf("Tiempo de creacion %.16g milliseconds\n", secs1 * 1000.0);
  	printf("Tiempo de destruccion %.16g milliseconds\n", secs2 * 1000.0);
	free(thread_handles);
	return 0;
}