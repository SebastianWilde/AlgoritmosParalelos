#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__
void vecAddKernel(float* a, float* b, float* c, int n)
{
	int i = threadIdx.x + blockDim.x * blockIdx.x;
	if(i<n) c[i] = a[i] + b[i];
}

int main(void) 
{
	int N = 24;
	int i;
	float *A, *B, *C, *d_A, *d_B, *d_C;
	int size = N * sizeof(float);
	A=(float*)malloc(size);
	B=(float*)malloc(size);
	C=(float*)malloc(size);
	//inicializacion de los vectores
	for (i=0;i<N;i++)
	{
		A[i]=i;
		B[i]=i*i-1;
	}
	//impresion vectores
	printf("Primer vector\n");
	for (i=0;i<4;i++)
		printf ("%4.1f ", A[i] );
	printf("\n");
	printf("Segundo vector\n");
	for (i=0;i<4;i++)
		printf ("%4.1f ", B[i] );
	printf("\n");

	//Memoria en GPU
	cudaMalloc((void **) &d_A, size);
	cudaMalloc((void **) &d_B, size);
	cudaMalloc((void **) &d_C, size);
	//Copiar datos
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	//Calculo de bloques
	int block_size=8;
	int n_blocks=N/block_size + (N%block_size == 0 ? 0:1);
	// Kernel invocation 
	vecAddKernel <<< n_blocks, block_size >>> (d_A, d_B, d_C, N);
	//vecAddKernel<<<ceil(N/8.0), 8>>>(d_A, d_B, d_C, N);	
	cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	// Free device memory for A, B, C
	printf("Respuesta\n");
	for (i=0;i<4;i++)
		printf ("%4.1f ", C[i] );
	printf("\n");
	free(A);
	free(B);
	free(C);
	cudaFree(d_A); 
	cudaFree(d_B); 
	cudaFree (d_C);	
	return 0;
}