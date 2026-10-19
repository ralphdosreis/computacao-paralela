#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

// Kernel executado na GPU
__global__ void transforma(int *v, int n)
{

  // Calcula o indice global da thread
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  // Verifica se o indice pertence ao vetor
  if (i < n)
  {

    if (i % 2 == 1)
    {
      // Indices impares: multiplica por 2
      v[i] = v[i] * 2;
    }
    else
    {
      // Indices pares: adiciona 5
      v[i] = v[i] + 5;
    }
  }
}

int main(void)
{

  // Vetor inicial na CPU
  int h_v[] = {-20, 20, 20, 40};

  int n = sizeof(h_v) / sizeof(h_v[0]);
  size_t tamanho = n * sizeof(int);

  // Ponteiro para a memoria da GPU
  int *d_v;

  // Exibe o vetor original
  printf("Vetor original: ");

  for (int i = 0; i < n; i++)
  {
    printf("%d ", h_v[i]);
  }

  printf("\n");

  // Aloca memoria na GPU
  cudaError_t erro = cudaMalloc(
      (void **)&d_v, tamanho);

  if (erro != cudaSuccess)
  {
    fprintf(stderr, "Erro cudaMalloc: %s\n",
            cudaGetErrorString(erro));
    return 1;
  }

  // Copia o vetor da CPU para a GPU
  cudaMemcpy(
      d_v,
      h_v,
      tamanho,
      cudaMemcpyHostToDevice);

  // Configuracao de execucao
  int threadsPorBloco = 4;
  int blocos = (n + threadsPorBloco - 1) / threadsPorBloco;

  // Executa o kernel na GPU
  transforma<<<blocos, threadsPorBloco>>>(d_v, n);

  // Verifica erro de lancamento
  erro = cudaGetLastError();

  if (erro != cudaSuccess)
  {
    fprintf(stderr, "Erro no kernel: %s\n",
            cudaGetErrorString(erro));
    cudaFree(d_v);
    return 1;
  }

  // Aguarda a conclusao do kernel
  erro = cudaDeviceSynchronize();

  if (erro != cudaSuccess)
  {
    fprintf(stderr, "Erro na execucao: %s\n",
            cudaGetErrorString(erro));
    cudaFree(d_v);
    return 1;
  }

  // Copia o resultado da GPU para a CPU
  cudaMemcpy(
      h_v,
      d_v,
      tamanho,
      cudaMemcpyDeviceToHost);

  // Exibe o resultado final
  printf("Vetor transformado: ");

  for (int i = 0; i < n; i++)
  {
    printf("%d ", h_v[i]);
  }

  printf("\n");

  // Libera a memoria da GPU
  cudaFree(d_v);

  return 0;
}
