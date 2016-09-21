#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING=100;

int main (void)
{
	char greeling[MAX_STRING];
	int comm_sz;
	int my_rank;

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

	if(my_rank!=0)
	{
		sprintf(greeling, "Greeling from process %d from %d!", my_rank,comm_sz);
		MPI_Send(greeling,strlen(greeling)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
		/*int wevadas=strlen(greeling);
		printf("%d\n",wevadas );*/
//		MPI_Send(greeling,strlen(greeling),MPI_CHAR,0,0,MPI_COMM_WORLD);
//		MPI_Send(greeling,MAX_STRING,MPI_CHAR,0,0,MPI_COMM_WORLD);

	}
	else
	{
		printf("Greeling from process %d from %d!\n", my_rank,comm_sz);
		int q;
		for(q=1;q<comm_sz;q+=2)
		{
			MPI_Recv(greeling,MAX_STRING,MPI_CHAR,q,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			printf("Hola%s\n",greeling );
		}
	}
	MPI_Finalize();
	return 0;
}