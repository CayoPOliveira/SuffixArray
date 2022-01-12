#include <stdio.h>
#include <stdlib.h>
#include "LCP.h"

//  DEBUG ===============================================
int LCP_Compare(int_lcp *a, int_lcp *b, uint_t n)
{
  int flag = 1;
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

//  NAIVE ===============================================
int LCP_Naive(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP)
{
  LCP[0] = 0;
  for (int i = 1; i < n; i++)
  {
    int j = 0;
    while (Text[SA[i] + j] == Text[SA[i - 1] + j])
      j++;
    LCP[i] = j;
  }
  return 1;
}

//  KASAI ===============================================
int KASAI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP)
{
  int_t *ISA = (int_t *)malloc(n * sizeof(int_t));
  for (int i = 0; i < n; i++)
  {
    ISA[SA[i]] = i;
  }

  int l = 0;
  for (int i = 0; i < n - 1; i++)
  {
    int j = ISA[i] - 1;
    while (Text[i + l] == Text[SA[j] + l])
      l++;
    LCP[j + 1] = l;
    (l > 0) ? l-- : l;
  }
  LCP[0] = 0;

  free(ISA);
  return 1;
}

//  PHI ===============================================
int PHI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP)
{
  int_t *phi = (int_t *)malloc(n * sizeof(int_t));
  int_t *plcp = (int_t *)malloc(n * sizeof(int_t));

  phi[SA[0]] = -1;
  for (int i = 1; i < n; i++)
  {
    phi[SA[i]] = SA[i - 1];
  }

  int l = 0;
  for (int i = 0; i < n - 1; i++)
  {
    int j = phi[i];
    while (Text[i + l] == Text[j + l])
      l++;
    plcp[i] = l;
    (l > 0) ? l-- : l;
  }
  plcp[n - 1] = 0;

  LCP[0] = 0;
  for (int i = 1; i < n; i++)
    LCP[i] = plcp[SA[i]];

  free(phi);
  free(plcp);
  return 1;
}