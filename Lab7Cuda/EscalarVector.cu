#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <Windows.h>

__global__
void vecMulKernel(float* a, int b, float* c, int nx, int ny)
{
	int IdX = threadIdx.x + blockDim.x * blockIdx.x; //Columna
	int IdY = blockIdx.y*blockDim.y + threadIdx.y; //Filas
	if (IdX<nx && IdY<ny) c[IdY*nx + IdX] = a[IdY*nx + IdX] * b;
}

int main(void)
{
	int n_columnas = 24;
	int n_filas = 24;
	int i,j;
	int B = 10;
	int N = n_columnas*n_filas;
	float *A,*C, *d_A,*d_C;
	int size = N * sizeof(float);
	A = (float*)malloc(size);
	C = (float*)malloc(size);
	//inicializacion de los vectores
	for (i = 0; i<N; i++)
	{
		A[i] = i;
	}
	//impresion vectores
	printf("Primer vector\n");
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4;j++)
			printf("%4.1f ", A[i*n_columnas+j]);
		printf("\n");
	}

	//Memoria en GPU
	cudaMalloc((void **)&d_A, size);
	cudaMalloc((void **)&d_C, size);
	//Copiar datos
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	/*
	//Calculo de bloques
	int block_size = 8;
	int n_blocks = N / block_size + (N%block_size == 0 ? 0 : 1);
	*/
	dim3 DimGrid((n_columnas - 1) / 16 + 1, (n_filas - 1) / 16 + 1, 1);
	dim3 DimBlock(16, 16, 1);
	// Kernel invocation 
	vecMulKernel <<< DimGrid, DimBlock >>> (d_A, B, d_C, n_columnas,n_filas);
	//vecAddKernel<<<ceil(N/8.0), 8>>>(d_A, d_B, d_C, N);	
	cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	// Free device memory for A, B, C
	printf("Respuesta\n");
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			printf("%4.1f ", C[i*n_columnas + j]);
		printf("\n");
	}
	free(A);
	free(C);
	cudaFree(d_A);
	cudaFree(d_C);
	system("PAUSE");
	return 0;
}