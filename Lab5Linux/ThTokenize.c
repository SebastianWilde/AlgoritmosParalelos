#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>

#define MAX 500
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int thread_count;
sem_t* sems;
FILE* stdin;
char * saveptr;

void* Tokenize(void* rank)
{
	long my_rank=(long) rank;
	int count;
	int next=(my_rank+1)%thread_count;
	char *fg_rv;
	char my_line[MAX];
	char *my_string;
	sem_wait(&sems[my_rank]);
	fg_rv=fgets(my_line,MAX,stdin);
	sem_post(&sems[next]);
	while (fg_rv!= NULL)
	{
		printf("Thread %ld >my line = %s\n",my_rank,my_line );
		count=0;
		my_string=strtok(my_line,"\t\n");
		//my_string=strtok_r(my_line," \t\n",&saveptr);
		while (my_string != NULL)
		{
			count++;
			printf("Thread %ld > string %d = %s\n", my_rank,count,my_string);
			my_string=strtok(NULL, "\t\n");
			//my_string=strtok_r(NULL, "\t\n",&saveptr);
		}
		sem_wait(&sems[my_rank]);
		fg_rv=fgets(my_line,MAX,stdin);
		sem_post(&sems[next]);
	}
	return NULL;
}
int main (int argc, char * argv[])
{
	stdin=fopen("Token.txt","r");
	long thread;
	pthread_t* thread_handles;
	thread_count=strtol(argv[1],NULL,10);
	thread_handles=malloc (thread_count*sizeof(pthread_t));
    sems=malloc (thread_count*sizeof(sem_t));
    struct timeval t_ini, t_fin;
  	double secs;
  	gettimeofday(&t_ini, NULL);
  	sem_post(&sems[0]);
	for (thread=0;thread<thread_count;thread++)
		pthread_create(&thread_handles[thread],NULL,Tokenize,(void*)thread);
	for (thread=0;thread<thread_count;thread++)
		pthread_join(thread_handles[thread],NULL);
	gettimeofday(&t_fin, NULL);
  	secs = timeval_diff(&t_fin, &t_ini);
  	printf("%.16g milliseconds\n", secs * 1000.0);
	free(thread_handles);
	return 0;
}