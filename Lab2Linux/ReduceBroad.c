#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
bool encontrar(int * vec,int tam,int num)
{
    bool encontrado=0;
    for(int i=0;i<tam;i++)
    {
        if (vec[i] == num)
        {
            encontrado = 1;
            return encontrado;
        }
    }
    return encontrado;
}

int * Remover(int *vec ,int tam, int num)
{
    int *vec2=NULL;
    int aux=0;
    vec2 = (int*)malloc((tam-1)*sizeof(int));
    for (int i=0;i<tam;i++)
    {
        if (vec[i] != num)
        {
            *(vect2 + aux)=vec[i];
            aux++;
        }
    }
    return vec2;
}

void Reduce(int*msj, int tam)
{
    int rank, comm_sz;
    int *vector=NULL;
    int limit=comm_sz/2;
    int resta=1;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    vector = (int*)malloc((comm_sz/2)*sizeof(int));
    for (int i=0;i<comm_sz;i+=2)
        *(vector + i)=i;
    while (limit>1)
    {
        if (!encontrado(vector,limit,rank))
        {
            MPI_Send(&msj[rank],tam,MPI_INT,rank-resta,0,MPI_COMM_WORLD);
            resta+=1;
        }
        else
        {
            MPI_Recv(msj, tam, MPI_INT, rank+resta-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int sum=msj[rank]+*msj;
            if (!rank==0)
            {
                resta+=1;
                vector=Remover(vector,limit,rank);
                limit-=1;
                MPI_Send(&sum,tam,MPI_INT,rank-resta,0,MPI_COMM_WORLD);
            }
        }
    }

}

int main(int argc, char** argv)
{
    double startwtime, endwtime;
    int comm_sz, rank;
    float msj;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Barrier(MPI_COMM_WORLD);
    startwtime = MPI_Wtime();
    endwtime = MPI_Wtime();
    printf("Codigo se ejecuto en %f segundos\n",endwtime - startwtime);
    MPI_Finalize();
    return 0;
}