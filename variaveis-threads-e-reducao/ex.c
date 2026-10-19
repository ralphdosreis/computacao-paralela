#include <stdio.h>
#include <omp.h>

int main(void)
{
  int valores[6] = {30, 20, 45, 55, 60, 80};
  int alertas = 0;
  int soma_critica = 0;
  int limite = 50;

#pragma omp parallel for num_threads(2) \
    schedule(static, 3)                 \
    reduction(+ : alertas, soma_critica)

  for (int i = 0; i < 6; i++)
  {
    if (valores[i] > limite)
    {
      alertas++;
      soma_critica += valores[i];
    }
  }

  printf("Alertas: %d\n", alertas);
  printf("Soma critica: %d\n", soma_critica);

  return 0;
}