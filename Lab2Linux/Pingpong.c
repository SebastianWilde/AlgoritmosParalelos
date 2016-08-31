#include <mpi.h>
#include <stdio.h>
#include <string.h>
int limite=30;
int main(int argc, char *argv[])
{
	int rank,comm_sz;
	char palabra [4];
	char palabra_local[4];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank==1)
	{
		while(limite>15)
		{
			MPI_Recv(palabra,4,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			limite-=1;
			for (int i=0;i<4;i++)
				palabra_local[i]=palabra[i];
			palabra[0]='P';
			palabra[1]='i';
			palabra[2]='n';
			palabra[3]='g';
			MPI_Send(palabra,4,MPI_CHAR,0,0,MPI_COMM_WORLD);
			printf("Proceso %d = %s \n",rank,palabra_local);
		}
	}
	if(rank==0)
	{
		while(limite>15)
		{
			palabra[0]='P';
			palabra[1]='o';
			palabra[2]='n';
			palabra[3]='g';
			MPI_Send(palabra,4,MPI_CHAR,1,0,MPI_COMM_WORLD);
			MPI_Recv(palabra,4,MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			for (int i=0;i<4;i++)
				palabra_local[i]=palabra[i];			
			limite-=1;
			printf("Proceso %d = %s \n",rank,palabra_local);
			printf("%s\n",palabra );
		}
	}
	MPI_Finalize();
	return 0;
}