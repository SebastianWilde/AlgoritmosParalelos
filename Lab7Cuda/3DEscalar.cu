
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <Windows.h>
#include <stdio.h>


__global__ 
void foo_kernel(float *R,float *S, const int nx, const int ny, const int nz)
{
	unsigned int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int yIndex = blockIdx.y* blockDim.y + threadIdx.y;
	unsigned int zIndex = blockIdx.z* blockDim.z + threadIdx.z;

	if ((xIndex < nx) && (yIndex < xIndex) && (zIndex < nz))
	{
		unsigned int index = xIndex + nx*yIndex + nx*ny*zIndex;
		S[index] = R[index]*2;
	}
}

int main()
{
	//Variables espacio host
	float *A, *B,*d_A,*d_B;
	int nx, ny, nz,i;
	nx = ny = nz = 16;
	int size = nx*ny*nz*sizeof(float);
	A = (float*)malloc(size);
	B = (float*)malloc(size);
	for (i = 0; i<nx*ny*nz; i++)
	{
		A[i] = i%10;
	}
	//Varables device
	cudaMalloc((void**)&d_A,size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMalloc((void**)&d_B, size);
	//Calculo de bloques
	int threadsInX = 8;
	int threadsInY = 8;
	int threadsInZ = 8;
	int blocksInX = (nx + threadsInX - 1) / threadsInX;
	int blocksInY = (ny + threadsInY - 1) / threadsInY;
	int blocksInZ = (nz + threadsInZ - 1) / threadsInZ;
	dim3 Dg = dim3(blocksInX, blocksInY*blocksInZ);
	dim3 Db = dim3(threadsInX, threadsInY, threadsInZ);
	foo_kernel <<<Dg,Db>>>(A,B, nx, ny, nz);
	system("PAUSE");
	return 0;
}
