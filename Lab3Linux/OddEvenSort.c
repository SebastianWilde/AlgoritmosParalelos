/*
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
//Codigo de ordenacion de las diapositivas
void PrintVector(int a[], int n)
{
	for (int i=0;i<n;i++)
		printf("%7d ",a[i]);
}
void Odd_even_sort(int a[], int n)
{
	int phase, i, temp;
	for(phase=0;phase<n;phase++)
	{
		if (phase%2==0)
		{
			for (i=1;i<n;i+=2)
			{
				if (a[i-1]>a[i])
				{
					temp= a[i];
					a[i]=a[i-1];
					a[i-1]=temp;					
				}
			}
		}
		else
		{
			for (i=1;i<n-1;i+=2)
			{
				if (a[i]>a[i+1])
				{
					temp= a[i];
					a[i]=a[i+1];
					a[i+1]=temp;					
				}
			}
		}
	}
}
int main(int argc, char * argv[]) 
{
	int * vec;
	int tam, comn_sz, rank;
	tam=10;
	vec=malloc(tam*sizeof(int));
    for (int i=0;i<tam;i++)
    	*(vec + i)=1+(int)(10*random()/(RAtamD_MAX+1.0));
 /*   PrintVector(vec,tam);
    printf("Ordenado\n");
    Odd_even_sort(vec,tam);
    PrintVector(vec,tam);*//*
    double startwtime, endwtime;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comn_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vec=malloc(tam*sizeof(int));
    for (int i=0;i<tam;i++)
    	*(vec + i)=1+(int)(10*random()/(RAtamD_MAX+1.0));
    if (rank==0)
    {
    	MPI_Bcast(vector,tam,MPI_ItamT, 0, MPI_COMM_WORLD);
    }
    else
    {
     	MPI_Bcast(vector,tam,MPI_ItamT, 0, MPI_COMM_WORLD);
    }
    int * local_vec;
    int division,inicio,fin;
    division=(int)tam/comn_sz;
    inicio=rank*division;
    fin=inicio+division-1;
    if (rank==comn_sz-1)
    {
    	fin=fin+(tam%comn_sz);
    }
    local_vec=malloc((fin-inicio+1)*sizeof(int))
    for (int i=0,int k=inicio;i<fin-inicio+1;i++,k++;)
    	local_vec[i]=vec[k];
    Odd_even_sort(local_vec,fin-inicio+1);
    MPI_Finalize();
	return 0; 
}*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define tam 10

void PrintVector(int a[], int n)
{
	printf("El vector es:\n");
	for (int i=0;i<n;i++)
		printf("%7d ",a[i]);
}
/* comparison function for qsort */
int cmp(const void* ap, const void* bp) {
  int a = * ((const int*) ap);
  int b = * ((const int*) bp);

  if (a < b) 
  {
    return -1;
  } 
  else if (a > b) 
  {
    return 1;
  } 
  else 
  {
    return 0;
  }
}

int max_index(int* data) 
{
  int i, max = data[0], maxi = 0;
  for (i = 1; i < tam; i++) 
  {
    if (data[i] > max) 
    {
      max = data[i];
      maxi = i;
    }
  }
  return maxi;
}

int min_index(int* data) 
{
  int i, min = data[0], mini = 0;
  for (i = 1; i < tam; i++) 
  {
    if (data[i] < min) 
    {
      min = data[i];
      mini = i;
    }
  }
  return mini;
}

void parallel_sort(int* data, int rank, int size) {
  int i;
  int local_vec[tam];
  for (i = 0; i < size; i++) {
	qsort(data, tam, sizeof(int), &cmp);
    int partener;
    if (i % 2 == 0) {
      if (rank % 2 == 0) {
        partener = rank + 1;
      } else {
        partener = rank - 1;
      }
    } else {
      if (rank % 2 == 0) {
        partener = rank - 1;
      } else {
        partener = rank + 1;
      }
    }

    if (partener < 0 || partener >= size) {
      continue;
    }
    if (rank % 2 == 0) {
      MPI_Send(data, tam, MPI_INT, partener, 0, MPI_COMM_WORLD);
      MPI_Recv(local_vec, tam, MPI_INT, partener, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    } else {
      MPI_Recv(local_vec, tam, MPI_INT, partener, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      MPI_Send(data, tam, MPI_INT, partener, 0, MPI_COMM_WORLD);
    }
    if (rank < partener) {
      while (1) {
        int mini = min_index(local_vec);
        int maxi = max_index(data);
        if (local_vec[mini] < data[maxi]) {
          int temp = local_vec[mini];
          local_vec[mini] = data[maxi];
          data[maxi] = temp;
        } else {
          break;
        }
      }
    } else {
      while (1) {
        int maxi = max_index(local_vec);
        int mini = min_index(data);
        if (local_vec[maxi] > data[mini]) {
          int temp = local_vec[maxi];
          local_vec[maxi] = data[mini];
          data[mini] = temp;
        } else {
          break;
        }
      }
    }
  }
}

int main(int argc, char** argv) 
{
	int * vec;
	int comn_sz, rank;
  	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &comn_sz);
  	vec=malloc(tam*sizeof(int));
    for (int i=0;i<tam;i++)
    	*(vec + i)=1+(int)(10*random()/(RAND_MAX+1.0));
    PrintVector(vec,tam);
  	parallel_sort(vec, rank, comn_sz);
  	PrintVector(vec,tam);
  	MPI_Finalize( );
  	return 0;
}