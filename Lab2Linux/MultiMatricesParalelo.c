/* Programa de multiplicación de matrices paralelizado,
   utilizando MPI.  2001, Daniel Manrique. */

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

/*Funciones de matrices*/

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
    // Inicializar mpi
    MPI_Init(&argc, &argv);

    /* determinar numero total de procesos y cual
       somos, asi como en que procesador (nodo) estamos
       corriendo. */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    fila = dimension;
    col = dimension;

    /* En MPI el proceso inicial (padre) tiene rank
       de 0, esta es la sección donde el proceso padre
       realiza su trabajo */
    if (rank == 0) {
		// Asignar memoria para tres matrices de de fila
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
		/*Determinar el momento de inicio de ejecución*/
		startwtime = MPI_Wtime();
		printf("Primera matriz 1\n");
		MostrarMatriz(matriz1,fila,col);
		printf("\nPrimera matriz 2\n");
		MostrarMatriz(matriz2,fila,col);
		completefilas = 0;
		/* el proceso con rank 0 transmite las dos
		   matrices a los demás, esto se hace por medio
		   de un broadcast de MPI a todos los miembros de
		   mi comunicador (MPI_COMM_WORLD) */
		rv = MPI_Bcast(matriz1, fila * col, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Root, Broadcast said %d\n", rv);
		rv = MPI_Bcast(matriz2, fila * col, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Root, Broadcast said %d\n", rv);
		resultfila = malloc((col + 1) * sizeof(int));
		while (completefilas < fila) 
		{
		    /* MPI_ANY_SOURCE se admiten valores de cualquier proceso. */
		    MPI_Recv(resultfila, col + 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		    printf("recibido renglon %d de %d\n",resultfila[0], status.MPI_SOURCE);
		    completefilas++;
		    for (int i = 0; i<col; i++)
			    {
					matriz3[resultfila[0]*col+i]=resultfila[i+1];
			    }
		}
		// anotar tiempo al
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
		/* En MPI, si mi rank no es 0, una llamada al
		   broadcast (notar el parametro 4 que es de 0)
		   indica recibir el broadcast del proceso con ese
		   rank. */
		rv = MPI_Bcast(matriz1, fila * col,MPI_INT, 0, MPI_COMM_WORLD);
		rv = MPI_Bcast(matriz2, fila * col,MPI_INT, 0, MPI_COMM_WORLD);

		/*de acuerdo a las dimensiones de las matrices
		   y al numero de procesos, calcular numero de
		   particiones, renglones por particion, el
		   primer renglon que tiene que resolver este
		   proceso, y el ultimo renglon.  */
		partitions = comm_sz - 1;
		filastodo = (int) (fila / partitions);
		firstfila = filastodo * (rank - 1);
		lastfila = firstfila + filastodo - 1;
		// el ultimo proceso amplia su limite para
		// tomar los huerfanitos
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