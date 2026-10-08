#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void transposta(const float *A, float *B,
                           int linhas, int colunas)
{

  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x < colunas && y < linhas)
  {
    B[x * linhas + y] = A[y * colunas + x];
  }
}

int main()
{
  const int linhas = 2;
  const int colunas = 3;

  float A[6] = {1, 2, 3, 4, 5, 6};
  float B[6];

  float *d_A, *d_B;

  size_t tamanho = linhas * colunas * sizeof(float);

  cudaMalloc((void **)&d_A, tamanho);
  cudaMalloc((void **)&d_B, tamanho);

  cudaMemcpy(d_A, A, tamanho, cudaMemcpyHostToDevice);

  dim3 threadsPorBloco(16, 16);

  dim3 numeroBlocos(
      (colunas + 15) / 16,
      (linhas + 15) / 16);

  transposta<<<numeroBlocos, threadsPorBloco>>>(
      d_A, d_B, linhas, colunas);

  cudaMemcpy(B, d_B, tamanho, cudaMemcpyDeviceToHost);

  printf("Matriz transposta:\n");

  for (int i = 0; i < colunas; i++)
  {
    for (int j = 0; j < linhas; j++)
    {
      printf("%.0f ", B[i * linhas + j]);
    }
    printf("\n");
  }

  cudaFree(d_A);
  cudaFree(d_B);

  return EXIT_SUCCESS;
}
