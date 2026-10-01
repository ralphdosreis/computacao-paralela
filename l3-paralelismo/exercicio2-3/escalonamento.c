#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[])
{
  int max;
  sscanf(argv[1], "%d", &max);

  long int sum = 0;

  double inicio = omp_get_wtime();
#pragma omp parallel for reduction(+ : sum) schedule(runtime)
  for (int i = 1; i <= max; i++)
  {
    printf("%2d @ %d\n", i, omp_get_thread_num());
    sleep(i < 4 ? i + 1 : 1);
    sum = sum + i;
  }

  double fim = omp_get_wtime();

  printf("Soma = %ld\n", sum);
  printf("Tempo total = %.2f segundos\n", fim - inicio);

  return 0;
}

// use: gcc -fopenmp escalonamento.c -o escalonamento
// export OMP_NUM_THREADS=4 (Número de threads desejado)
// export OMP_SCHEDULE="static,1" ou export OMP_SCHEDULE="dynamic,1" (Método de escalonamento e quantidades distribuida à cada execução)
// escalonamento 20 (Sequencias de números, no caso de 1 à 20)