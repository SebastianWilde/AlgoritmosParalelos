#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
int     thread_count;
double  a, b, h;
int     n, local_n;
//sem_t   sem;
double  total;
//int flag;
pthread_mutex_t mutex;
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

double Trap(double local_a, double local_b, int local_n, double  h) 
{
    double integral;
    double x;
    int i;
    integral = ((local_a*local_a) + (local_b*local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++)
    {
        x = local_a + i*h;
        integral += (x*x);
    }
    integral = integral*h;
    return integral;
}

void *Thread_work(void* rank) 
{
    double  local_a;
    double  local_b;
    double  my_int;
    long    my_rank = (long) rank;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    my_int = Trap(local_a, local_b, local_n, h);
    //sem_wait(&sem);
    //while (flag!=my_rank);
    pthread_mutex_lock(&mutex);
    total += my_int;
    pthread_mutex_unlock(&mutex);
    //flag=(flag+1)%thread_count;
    //sem_post(&sem);
    return NULL;
}

int main(int argc, char** argv) {
    long i;
    pthread_t*  thread_handles;  
    total = 0.0;
    if (argc != 2) 
    {
       fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
       exit(0);
    }
    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, n\n");
    scanf("%lf %lf %d", &a, &b, &n);
    h = (b-a)/n;
    local_n = n/thread_count; 
    thread_handles = malloc (thread_count*sizeof(pthread_t));
    /* Initialize the semaphore */
    /* Second arg:  sem is shared among threads, not processes */
    /* Third arg:  initial value for sem = 1 = unlocked */
    //flag=0;
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    struct timeval t_ini, t_fin;
    double secs;
    gettimeofday(&t_ini, NULL);
    //sem_init(&sem, 0, 1);
    for (i = 0; i < thread_count; i++) 
    {
         pthread_create(&thread_handles[i], NULL, Thread_work, (void*) i);
    }
    for (i = 0; i < thread_count; i++)
    {
        pthread_join(thread_handles[i], NULL);
    }
    gettimeofday(&t_fin, NULL);
    secs = timeval_diff(&t_fin, &t_ini);
    printf("%.16g milliseconds\n", secs * 1000.0);
    printf("With n = %d trapezoids, our estimate\n",n);
    printf("of the integral from %f to %f = %19.15e\n",a, b, total);
    //sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    return 0;
} 
