#include <stdio.h>
#include <omp.h>

#define N 16

void prefix_sum(int v[])
{
  int aux[N];

  for (int offset = 1; offset < N; offset *= 2)
  {
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
      if (i >= offset)
        aux[i] = v[i] + v[i - offset];
      else
        aux[i] = v[i];
    }

#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
      v[i] = aux[i];
    }
  }
}

void imprimir(int v[])
{
  for (int i = 0; i < N; i++)
    printf("%d ", v[i]);

  printf("\n");
}

int main()
{
  int v1[N];
  int v2[N];

  for (int i = 0; i < N; i++)
  {
    v1[i] = 1;
    v2[i] = i + 1;
  }

  prefix_sum(v1);
  prefix_sum(v2);

  printf("Vetor 1:\n");
  imprimir(v1);

  printf("Vetor 2:\n");
  imprimir(v2);

  return 0;
}