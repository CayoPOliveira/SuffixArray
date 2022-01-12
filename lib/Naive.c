#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Naive.h"

int comp_str(const void *a, const void *b)
{
  const t_suffix *stra = (const t_suffix *)a;
  const t_suffix *strb = (const t_suffix *)b;
  return strcmp(stra->suffix, strb->suffix) < 0 ? 0 : 1;
}

//  NAIVE ===============================================
int *Naive(char *Text, uint_t n, int_sa *SA)
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