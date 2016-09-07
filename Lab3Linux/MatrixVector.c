#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
void Llenarmatriz(double * matriz, int col, int fila)
{
    for (int i=0;i<(fila*col);i++) 
    {
        *(matriz + i)=(double)(10*random()/(RAND_MAX+1.0));
    }
    return;
}
void Mostrarmatriz(double *matriz, int fila, int col)
{
    for (int i = 0; i < fila; i++) 
    {
        for (int j = 0; j < col; j++) 
        {
            printf("%7g ",matriz[i*col+j]);
        }
        printf("\n");
    }
    return;
}
void Mat_vect_mult(double local_A[], double local_x[], double local_y[],int  local_m, int  n ,int local_n )
{
    int local_i, j;
    double * x;
    x=malloc(n*sizeof(int));
    MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE,MPI_COMM_WORLD);
    for (local_i = 0; local_i < local_m; local_i++) 
    {
      local_y[local_i] = 0;
      for (j = 0; j < n; j++)
         local_y[local_i] += local_A[local_i*n+j]*x[j];
    }
    free(x);
}

void Printvector(double * vec, int tam)
{
    for (int i=0; i<tam; i++) 
    { 
        printf("%7g ", vec[i]); 
    } 
    return;
}
#define tam 1024
int main(int argc, char * argv[]) 
{ 
    int comn_sz, rank;
    double *matriz, *vector, *result, *local_matriz, *local_vec, *comprueba , *subFinal; 
    double startwtime, endwtime;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comn_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector = malloc(tam * sizeof(double));
    result = malloc(tam * sizeof(double));
    matriz = malloc(tam * tam * sizeof(double));
    if (rank == 0) 
    {
        Llenarmatriz(matriz,tam,tam);
        //Mostrarmatriz(matriz,tam,tam);
        printf("vector\n");
        for (unsigned int i = 0; i < tam; i++) 
        {
            vector[i] = (double)(10*random()/(RAND_MAX+1.0));
        }
        MPI_Bcast(matriz,tam*tam,MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(vector,tam,MPI_INT, 0, MPI_COMM_WORLD);
        comprueba = malloc(tam * sizeof(double));
        for (unsigned int i = 0; i < tam; i++) 
        {
            comprueba[i] = 0;
            for (unsigned int j = 0; j < tam; j++) 
            {
                comprueba[i] += matriz[i*tam+j] * vector[j];
            }
        }
    }
    else
    {
        MPI_Bcast(matriz,tam*tam,MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(vector,tam,MPI_INT, 0, MPI_COMM_WORLD);
    }
    int division = (int)(tam/comn_sz);
    int inicio = division * rank;
    int fin = inicio + division + 1;
    local_matriz = malloc((inicio + fin + 1)*tam * sizeof(double));
    local_vec = malloc((inicio + fin + 1) * sizeof(double));
    if (rank == comn_sz - 1)
    {
        fin += tam % comn_sz;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i=inicio,k=0; i <=fin; i++,k++)
    {
        for (int j=0; j<tam;j++)
        {
            local_matriz[k*tam+j]=matriz[i*tam+j];
        }
    }
    for (int i = inicio,j=0; i <=fin; i++,j++)
    {
        local_vec[j]=vector[i];
    }
    free(matriz);
    free(vector);
    startwtime = MPI_Wtime();
    // Otra barrera para asegurar que todas ejecuten el siguiente trozo de codigo lo
    MPI_Barrier(MPI_COMM_WORLD);
    subFinal = malloc((fin-inicio+1)*sizeof(double));
    Mat_vect_mult(local_matriz,local_vec,subFinal,fin-inicio+1,tam,fin-inicio+1);
    MPI_Allgather(subFinal,fin-inicio+1,MPI_DOUBLE,result,fin-inicio+1,MPI_DOUBLE,MPI_COMM_WORLD);
    endwtime = MPI_Wtime();
    MPI_Finalize();/*
    if (rank == 0)
     {
        printf("El resultado obtenido result el esperado son:\n");
        printf("El resultado secuencial\n");
        Printvector(comprueba,tam);
        printf("El resultado paralelo\n");
        Printvector(result,tam);
    }*/
    printf("wall clock time = %f\n",endwtime - startwtime);
    free(result);
}