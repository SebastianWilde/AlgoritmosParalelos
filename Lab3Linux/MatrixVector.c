#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
void Llenarmatriz(double * matriz, int col, int fila)
{
    for (int i=0;i<(fila*col);i++) 
    {
        *(matriz + i)=1+(double)(10*random()/(RAND_MAX+1.0));
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
    x=malloc(n*sizeof(double));
    MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE,MPI_COMM_WORLD);
    for (local_i = 0; local_i < local_m; local_i++) 
    {
      local_y[local_i] = 0.0;
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
int main(int argc, char * argv[]) 
{ 
    int comn_sz, rank;
    double *matriz, *vector, *result, *comprueba , *subFinal; 
    double startwtime, endwtime;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comn_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int tam=16384;
    vector=malloc(tam*sizeof(double));
    result=malloc(tam*sizeof(double));
    matriz=malloc(tam*tam*sizeof(double));
    startwtime = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) 
    {
        Llenarmatriz(matriz,tam,tam);
        //Mostrarmatriz(matriz,tam,tam);
        printf("vector\n");
        //Llenarmatriz(vector,1,tam);/*
        for (unsigned int i = 0; i < tam; i++) 
        {
            vector[i] = (double)(10*random()/(RAND_MAX+1.0));
        }//*///
        MPI_Bcast(matriz,tam*tam,MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(vector,tam,MPI_DOUBLE, 0, MPI_COMM_WORLD);
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
        MPI_Bcast(matriz,tam*tam,MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(vector,tam,MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    double *local_matriz, *local_vec;
    int division,inicio,fin;
    division = (int)(tam/comn_sz);
    inicio = division*rank;
    fin = inicio+division-1;
    if (rank==comn_sz-1)
    {
        fin=fin+(tam%comn_sz);
    }
    local_matriz=malloc((fin-inicio+1)*tam*sizeof(double));
    local_vec=malloc((fin-inicio+1)*sizeof(double));
    subFinal=malloc((fin-inicio+1)*sizeof(double));
    for (int i=inicio,k=0; i <=fin; i++,k++)
    {
        for (int j=0; j<tam;j++)
        {
            local_matriz[k*tam+j]=matriz[i*tam+j];
        }
    }
    for (int i=inicio,j=0; i<=fin; i++,j++)
    {
        local_vec[j]=vector[i];
    }
    free(matriz);
    free(vector);
    Mat_vect_mult(local_matriz,local_vec,subFinal,fin-inicio+1,tam,fin-inicio+1);
    MPI_Allgather(subFinal,fin-inicio+1,MPI_DOUBLE,result,fin-inicio+1,MPI_DOUBLE,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
/*
    if (rank == 0)
    {
        printf("El resultado obtenido result el esperado son:\n");
        printf("El resultado secuencial\n");
        Printvector(comprueba,tam);
        printf("El resultado paralelo\n");
        Printvector(result,tam);
    }*/
    free(result);
    endwtime = MPI_Wtime();
    MPI_Finalize();
    printf("wall clock time = %f\n",(endwtime - startwtime)*1000);
    return 0;
}