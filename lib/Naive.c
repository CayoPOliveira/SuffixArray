#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Naive.h"

//  DEBUG ===============================================
int SA_Compare(int_sa *a, int_sa *b, uint_t n)
{
  int flag = 1;
  printf("== SA Compare ==");
  for (int i = 0; i < n; i++)
  {
    if (a[i] != b[i])
    {
      printf("a[%d]=%d\tb[%d]=%d\n", i, a[i], i, b[i]);
      flag = 0;
    }
  }
  return flag;
}

void print(int_sa *SA, int_lcp *LCP, int_str *uT, int n)
{
  printf("i\tSA\tLCP\tSufixies\n");
  for (int i = 0; i < n; ++i)
  {
    printf("%d\t%d\t%d\t", i, SA[i], LCP[i]);
    for (int j = SA[i]; j < n; ++j)
      printf("%d", uT[j]);
    printf("$\n");
  }
}

//  ====================================================
int comp_str(const void *a, const void *b)
{
  const t_suffix *stra = (const t_suffix *)a;
  const t_suffix *strb = (const t_suffix *)b;
  return strcmp(stra->suffix, strb->suffix) < 0 ? 0 : 1;
}

//  NAIVE ===============================================
int *Naive_sa(char *Text, uint_t n, int_sa *SA)
{
  t_suffix *tmp = (t_suffix *)malloc(n * sizeof(t_suffix));

  for (int i = 0; i < n; i++)
  {
    tmp[i].suffix = Text + i;
    tmp[i].pos = i;
  }

  qsort(tmp, n, sizeof(t_suffix), comp_str);

  for (int i = 0; i < n; i++)
    SA[i] = tmp[i].pos;

  free(tmp);

  return 0;
}