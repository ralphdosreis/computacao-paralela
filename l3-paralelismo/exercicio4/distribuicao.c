#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
  if (argc < 2)
    return 1;

  int max;
  sscanf(argv[1], "%d", &max);

  int ts = omp_get_max_threads();
  int sums[ts];

#pragma omp parallel
  {
    int t = omp_get_thread_num();

    int lo = (max * t) / ts + 1;
    int hi = (max * (t + 1)) / ts;

    sums[t] = 0;
    for (int i = lo; i <= hi; i++)
    {
      sums[t] = sums[t] + i;
    }
  }

  int sum = 0;
  for (int t = 0; t < ts; t++)
  {
    sum = sum + sums[t];
  }

  printf("%d\n", sum);
  return 0;
}