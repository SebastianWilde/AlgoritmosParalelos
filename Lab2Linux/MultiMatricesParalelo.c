#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
void LlenarMatriz(int * matriz, int col, int fila)
{
	for (int i=0;i<(fila*col);i++) 
    {
		*(matriz + i)=1;
    }
    return;
}
void MostrarMatriz(int *matriz, int fila, int col)
{
    for (int i = 0; i < fila; i++) 
    {
		for (int j = 0; j < col; j++) 
		{
	    	printf("%7d ",matriz[i*col+j]);
		}
		printf("\n");
    }
    return;
}
int main(int argc, char **argv)
{
    int rank, comm_sz, rv, fila, col;
    int *matriz1=NULL;
    int *matriz2=NULL;
    int *matriz3=NULL;
    double startwtime, endwtime;
    int partitions, firstfila, lastfila, filastodo;
    int *resultfila;
    int suma;
    int completefilas;
    int dimension;
    MPI_Status status;
	dimension=10;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    fila = dimension;
    col = dimension;
    if (rank == 0) {
		matriz1 = malloc(fila * col * sizeof(int));
		matriz2 = malloc(fila * col * sizeof(int));
		matriz3 = malloc(fila * col * sizeof(int));
		if (matriz1 == NULL || matriz2 == NULL || matriz3 == NULL )
		{
		    MPI_Finalize();
		    if (matriz1!=NULL) free(matriz1);
		    if (matriz2!=NULL) free(matriz2);
		    if (matriz3!=NULL) free(matriz3);
		    exit(1);
		}
		LlenarMatriz(matriz1,fila,col);
		LlenarMatriz(matriz2,fila,col);
		startwtime = MPI_Wtime();
		printf("Primera matriz 1\n");
		MostrarMatriz(matriz1,fila,col);
		printf("\nPrimera matriz 2\n");
		MostrarMatriz(matriz2,fila,col);
		completefilas = 0;
		rv = MPI_Bcast(matriz1, fila * col, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Root, Broadcast said %d\n", rv);
		rv = MPI_Bcast(matriz2, fila * col, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Root, Broadcast said %d\n", rv);
		resultfila = malloc((col + 1) * sizeof(int));
		while (completefilas < fila) 
		{
		    MPI_Recv(resultfila, col + 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		    printf("recibido renglon %d de %d\n",resultfila[0], status.MPI_SOURCE);
		    completefilas++;
		    for (int i = 0; i<col; i++)
			    {
					matriz3[resultfila[0]*col+i]=resultfila[i+1];
			    }
		}
		endwtime = MPI_Wtime();
		printf("\nPrimera matriz 3\n");
		MostrarMatriz(matriz3,fila,col);
		printf("wall clock time = %f\n",endwtime - startwtime);
		if (matriz1!=NULL) free(matriz1);
		if (matriz2!=NULL) free(matriz2);
		if (matriz3!=NULL) free(matriz3);
    } 
    else 
    {
		matriz1 = malloc(fila * col * sizeof(int));
		matriz2 = malloc(fila * col * sizeof(int));
		if (matriz1 == NULL || matriz2 == NULL)
		{
		    MPI_Finalize();
		    if (matriz1!=NULL) free(matriz1);
		    if (matriz2!=NULL) free(matriz2);
		    if (matriz3!=NULL) free(matriz3);
		    exit(1);
		}
		rv = MPI_Bcast(matriz1, fila * col,MPI_INT, 0, MPI_COMM_WORLD);
		rv = MPI_Bcast(matriz2, fila * col,MPI_INT, 0, MPI_COMM_WORLD);
		partitions = comm_sz - 1;
		filastodo = (int) (fila / partitions);
		firstfila = filastodo * (rank - 1);
		lastfila = firstfila + filastodo - 1;
		if (rank == comm_sz - 1) {
		    lastfila = lastfila + (fila % partitions);
		}
		resultfila = malloc((col + 1) * sizeof(int));
		for (int i = firstfila; i <= lastfila; i++) 
		{
		    for (int j = 0; j < col; j++) 
		    {
				suma = 0;
				for (int k = 0; k < col; k++) 
				{
					suma += matriz1[i*col+k]*matriz2[k*col+j];
				}
				resultfila[j + 1] = suma;
		    }
		    resultfila[0] = i;
		    MPI_Send(resultfila, col + 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
	    if (matriz1!=NULL) free(matriz1);
	    if (matriz2!=NULL) free(matriz2);
	    if (matriz3!=NULL) free(matriz3);	
    }
    MPI_Finalize();
    return 0;
}