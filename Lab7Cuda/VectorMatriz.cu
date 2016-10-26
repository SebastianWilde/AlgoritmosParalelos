
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <Windows.h>
#include <stdio.h>

__global__ 
void VecMatkernel(float *d_A, float *d_B, float *d_C, int fil,int col)
{
	int IdX = threadIdx.x + blockIdx.x*blockDim.x;
	float sum = 0;
	if (IdX < fil)
	{
		for (int i = 0; i<col; i++)
			sum += d_B[i] * d_A[(i*col) + IdX];
		d_C[IdX] = sum;
	}
}

void LLenarMatrix(float*mat, int fil, int col)
{
	int i;
	for (i = 0; i < fil*col; i++)
			mat[i] = i * 2;
}

void Print(float*mat, int fil, int col)
{
	int i, j;
	for (i = 0; i < fil; i++)
	{
		for (j = 0; j < col; j++)
			printf("%3.0f ", mat[i*col + j]);
		printf("\n");
	}

}

int main()
{
	float *A, *B, *C, *d_A, *d_B, *d_C;
	int n_fila, n_columna;
	n_fila = n_columna = 5;
	A = (float*)malloc(sizeof(float)*n_fila*n_columna);
	B = (float*)malloc(sizeof(float)*n_columna);
	C = (float*)malloc(sizeof(float)*n_fila);
	LLenarMatrix(A,n_fila,n_columna);
	LLenarMatrix(B, n_columna, 1);
	printf("La matriz es\n");
	Print(A, n_fila, n_columna);
	printf("El vector es\n");
	Print(B, 1, n_columna);
	cudaMalloc((void**)& d_A, sizeof(float)*n_fila*n_columna);
	cudaMalloc((void**)& d_B, sizeof(float)*n_columna);
	cudaMalloc((void**)& d_C, sizeof(float)*n_fila);
	cudaMemcpy(d_A, A, sizeof(float)*n_fila*n_columna, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, sizeof(float)*n_columna, cudaMemcpyHostToDevice);
	int N = n_fila*n_columna;
	VecMatkernel << < N/ 8 + 1, 8 >> >(d_A, d_B, d_C, n_fila,n_columna);
	cudaMemcpy(C, d_C, sizeof(float)*n_fila, cudaMemcpyDeviceToHost);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	printf("La rpta es\n");
	Print(C, n_fila, 1);
	free(A);
	free(B);
	free(C);
	system("PAUSE");
    return 0;
}