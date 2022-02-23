#include <stdio.h>
#include <stdlib.h>
#include "DC3.h"

int_lcp *segtreeBuild(int_lcp V[], int_t n)
{
  int_lcp *tree = (int_lcp *)malloc((2 * n) * sizeof(int));
  // leaves
  for (int i = 0; i < n; i++)
    tree[i + n] = V[i];
  // Min-tree
  for (int i = n - 1; i > 0; i--)
    tree[i] = min(tree[2 * i], tree[2 * i + 1]);
  return tree;
}
// retorna um valor (min) do intervalo [a, b]
int_lcp segtreeQuery(int_lcp tree[], int_t n, uint_t a, uint_t b)
{
  int s = 0;
  a += n;
  b += n;
  // Min-tree
  s = tree[a];
  while (a <= b)
  {
    if (a % 2 == 1)
      s = min(s, tree[a++]);
    if (b % 2 == 0)
      s = min(s, tree[b--]);
    a /= 2;
    b /= 2;
  }
  return s;
}

//  DC3 ===============================================
int radixPass(int_str *Text, uint_t n, int_sa *SA, int sigma, int p)
{
  // count the number of symbols
  int_t *C = (int_t *)malloc(sigma * sizeof(int_t));
  for (int i = 0; i < sigma; i++)
    C[i] = 0;
  for (int i = 0; i < n; i++)
    C[Text[SA[i] + p]]++;

  // bucket sorting
  int_t *B = (int_t *)malloc(sigma * sizeof(int_t));
  B[0] = 0;
  int_t aux = C[0];
  for (int i = 1; i < sigma; i++)
  {
    B[i] = B[i - 1] + aux;
    aux = C[i];
  }

  // radix pass
  int_sa *tmp = (int_sa *)malloc((n + 2) * sizeof(int_sa)); // temporary array
  for (int i = 0; i < n; i++)
    tmp[B[Text[SA[i] + p]]++] = SA[i];
  for (int i = 0; i < n; i++)
    SA[i] = tmp[i];

  free(C);
  free(B);
  free(tmp);

  return 0;
}

int DC3(int_str *Text, uint_t n, int_sa *SA, int_t sigma)
{
#if DEBUG
  printf("==DC3==\n");
#endif
  int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3;
  int n12 = n1 + n2;
  int_sa *SA12 = (int_sa *)malloc((n12 + 1) * sizeof(int_sa));

  int i12 = 0;
  for (int i = 1; i < n; i++)
    if (i % 3 == 1 || i % 3 == 2)
      SA12[i12++] = i; // add suffixes i%3==1 and i%3==2

#if DEBUG
  print(SA12, Text, n12, n, 0);
  printf("n0 = %d\n", n0);
  printf("n1 = %d\n", n1);
  printf("n2 = %d\n", n2);
  printf("n12 = %d\n", n12);
#endif

  // LSD radix steps
  radixPass(Text, n12, SA12, sigma, 2);
  radixPass(Text, n12, SA12, sigma, 1);
  radixPass(Text, n12, SA12, sigma, 0);

#if DEBUG
  printf("==\nRADIX\n==\n");
  print(SA12, Text, n12, n, 0);
#endif

  // reduced text
  int_str *TextU = (int_str *)malloc((n12 + 3) * sizeof(int_str));
  for (int i = 0; i < n12 + 3; i++)
    TextU[i] = 0;

  // middle position
  TextU[n1] = 0;
  uint_t name = 0, c0 = -1, c1 = -1, c2 = -1;
  // naming
  for (int i = 0; i < n12; i++)
  {
    if (Text[SA12[i]] != c0 || Text[SA12[i] + 1] != c1 || Text[SA12[i] + 2] != c2)
    {
      c0 = Text[SA12[i]];
      c1 = Text[SA12[i] + 1];
      c2 = Text[SA12[i] + 2];
      name++;
    }
    if (SA12[i] % 3 == 1)
      TextU[SA12[i] / 3] = name; // first half
    else
      TextU[SA12[i] / 3 + (n1 + 1)] = name; // second half
  }

#if DEBUG
  for (int i = 0; i < n12 + 1; i++)
    printf("%u ", TextU[i]);
  printf("\n");
#endif

  // DC3 (recursive)
  if (name < n12)
  {

    printf("====RECURSIVE====\n");
#if DEBUG
    for (int i = 0; i < n12 + 1; i++)
      printf("%d ", TextU[i]);
    printf("\n");
#endif

    // recursive call
    DC3(TextU, n12 + 1, SA12, name + 1);

#if DEBUG
    printf("==\nBACK\n==\n");
    for (int i = 0; i < n12 + 1; i++)
      printf("%d ", TextU[i]);
    printf("\n");
    print(SA12, TextU, n12 + 1, n12 + 1, 0);
#endif

    // Map SA12 into SA (ignore first element of SA12, which is the middle 0)
    for (int i = 0; i < n12; i++)
    {
      if (SA12[i + 1] < n1 + 1)
        SA12[i] = SA12[i + 1] * 3 + 1; // i%3==1
      else
        SA12[i] = (SA12[i + 1] - (n1 + 1)) * 3 + 2; // i%3==2
    }

#if DEBUG
    printf("==\nSA\n==\n");
    print(SA12, Text, n12, n, 1);
#endif
  }

  // SA0
  int_sa *SA0 = (int_sa *)malloc(n0 * sizeof(int_sa));
  int i0 = 0;
  if (n % 3 == 1)
    SA0[i0++] = n - 1; // if last position is mod 0
  for (int i = 0; i < n12; i++)
    if (SA12[i] % 3 == 1)
      SA0[i0++] = SA12[i] - 1;

  radixPass(Text, n0, SA0, sigma, 0);
#if DEBUG
  print(SA0, Text, n0, n, 0);
#endif

  // ISA
  int_sa *ISA12 = (int_sa *)malloc((n12 + n0) * sizeof(int_sa));
  for (int i = 0; i < n12; i++)
    ISA12[SA12[i]] = i;

  // merge SA12 and SA0
  int i = i0 = i12 = 0;
  while (i12 < n12 && i0 < n0)
  {

    if (Text[SA12[i12]] < Text[SA0[i0]])
      SA[i] = SA12[i12++];
    else if (Text[SA0[i0]] < Text[SA12[i12]])
      SA[i] = SA0[i0++];
    else
    { // first symbol is equal
      if (SA12[i12] % 3 == 1)
      { // SA0 vs. SA1
        if (ISA12[SA12[i12] + 1] < ISA12[SA0[i0] + 1])
          SA[i] = SA12[i12++]; // use ISA12
        else
          SA[i] = SA0[i0++];
      }
      else
      { // SA0 vs. SA2
        if (Text[SA12[i12] + 1] < Text[SA0[i0] + 1])
          SA[i] = SA12[i12++];
        else if (Text[SA0[i0] + 1] < Text[SA12[i12] + 1])
          SA[i] = SA0[i0++];
        else
        { // second symbol is (also) equal
          if (ISA12[SA12[i12] + 2] < ISA12[SA0[i0] + 2])
            SA[i] = SA12[i12++]; // use ISA12
          else
            SA[i] = SA0[i0++];
        }
      }
    }
    i++;
  }
  // remaining elements in SA0 and SA12
  while (i12 < n12)
    SA[i++] = SA12[i12++];
  while (i0 < n0)
    SA[i++] = SA0[i0++];

#if DEBUG
  printf("==\nSA (merged)\n==\n");
  print(SA, Text, n, n, 0);
#endif

  free(SA12);
  free(TextU);
  free(SA0);
  free(ISA12);

  return 0;
}

int DC3_LCP(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP, int_t sigma)
{

#if DEBUG
  printf("==DC3LCP==\n");
#endif

  int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3;
  int n12 = n1 + n2;
  int_sa *SA12 = (int_sa *)malloc((n12 + 1) * sizeof(int_sa));
  int_lcp *LCP12 = (int_lcp *)malloc((n12 + 1) * sizeof(int_lcp));
  LCP12[0] = LCP12[n12 - 1] = 0;

  int i12 = 0;
  for (int i = 1; i < n; i++)
    if (i % 3 == 1 || i % 3 == 2)
      SA12[i12++] = i; // add suffixes mod 1 and mod 2

#if DEBUG
  print(SA12, Text, n12, n, 0);
  printf("n0 = %d\n", n0);
  printf("n1 = %d\n", n1);
  printf("n2 = %d\n", n2);
  printf("n12 = %d\n", n12);
#endif

  // LSD radix steps
  radixPass(Text, n12, SA12, sigma, 2);
  radixPass(Text, n12, SA12, sigma, 1);
  radixPass(Text, n12, SA12, sigma, 0);

#if DEBUG
  printf("==\nRADIX\n==\n");
  print(SA12, Text, n12, n, 0);
#endif

  // reduced text
  int_str *TextU = (int_str *)malloc((n12 + 3) * sizeof(int_str));
  for (int i = 0; i < n12 + 3; i++)
    TextU[i] = 0;

  // middle position
  TextU[n1 + 1] = 0;
  uint_t name = 0, c0 = -1, c1 = -1, c2 = -1;
  // naming
  for (int i = 0; i < n12; i++)
  {
    if (Text[SA12[i]] != c0 || Text[SA12[i] + 1] != c1 || Text[SA12[i] + 2] != c2)
    {
      c0 = Text[SA12[i]];
      c1 = Text[SA12[i] + 1];
      c2 = Text[SA12[i] + 2];
      name++;
    }
    if (SA12[i] % 3 == 1)
      TextU[SA12[i] / 3] = name; // first half
    else
      TextU[SA12[i] / 3 + (n1 + 1)] = name; // second half
  }

#if DEBUG
  for (int i = 0; i < n12 + 1; i++)
    printf("%u ", TextU[i]);
  printf("\n");
#endif

  if (name == n12)
  {
#if example
    printf("\n++All Suffixes are unique++\nText=");
    for (int i = 0; i < n; i++)
    {
      printf("[%d] ", Text[i]);
    }
    printf("\n->SA12 = ");
    for (int i = 0; i < n12; i++)
      printf("[%d] ", SA12[i]);
    printf("\n->LCP12 = ");
    for (int i = 0; i < n12; i++)
      printf("[0] ");
#endif
    for (int i = 0; i < n12; i++)
    {
      LCP12[i] = 0;
    }
  }
  // DC3 (recursive)
  else //(name < n12)
  {
    printf("\n====RECURSIVE====\n");
#if DEBUG
    for (int i = 0; i < n12 + 1; i++)
      printf("%d ", TextU[i]);
    printf("\n");
#endif

    // recursive call
    DC3_LCP(TextU, n12 + 1, SA12, LCP12, name + 1);

#if DEBUG
    printf("==\nBACK\n==\n");
    for (int i = 0; i < n12 + 1; i++)
      printf("%d ", TextU[i]);
    printf("\n");
    print(SA12, TextU, n12 + 1, n12 + 1, 0);
#endif

#if example
    printf("\n==BACK==\nText=");
    for (int i = 0; i < n; i++)
    {
      printf("[%d] ", Text[i]);
    }
    printf("\n->SA12 = ");
#endif
    // Map SA12 into SA (ignore first element of SA12, which is the middle 0)
    // +lcp12
    for (int i = 0; i < n12; i++)
    {
      if (SA12[i + 1] < n1 + 1)
      {
        SA12[i] = SA12[i + 1] * 3 + 1; // i%3==1
      }
      else
      {
        SA12[i] = (SA12[i + 1] - (n1 + 1)) * 3 + 2; // i%3==2
      }
      LCP12[i] = LCP12[i + 1];
    }
#if example
    for (int i = 0; i < n12; i++)
    {
      printf("[%d] ", SA12[i]);
    }
    printf("\n->LCP12 = ");
    for (int i = 0; i < n12; i++)
    {
      printf("[%d] ", LCP12[i]);
    }
#endif

#if DEBUG
    printf("==\nSA\n==\n");
    print(SA12, Text, n12, n, 1);
#endif
  }

  // SA0
  int_sa *SA0 = (int_sa *)malloc(n0 * sizeof(int_sa));
  int i0 = 0;
  if (n % 3 == 1)
    SA0[i0++] = n - 1; // if last position is mod 0
  for (int i = 0; i < n12; i++)
    if (SA12[i] % 3 == 1)
      SA0[i0++] = SA12[i] - 1;

  radixPass(Text, n0, SA0, sigma, 0);

#if example
  printf("\n->SA0 = ");
  for (int i = 0; i < n0; i++)
  {
    printf("[%d] ", SA0[i]);
  }
#endif
#if DEBUG
  print(SA0, Text, n0, n, 0);
#endif

  // ISA
  int_sa *ISA12 = (int_sa *)malloc((n12 + n0) * sizeof(int_sa));
  for (int i = 0; i < n12; i++)
    ISA12[SA12[i]] = i;

  // merge SA12 and SA0
  int i = 0;
  i12 = 0;
  i0 = 0;
  while (i12 < n12 && i0 < n0)
  {
    if (Text[SA12[i12]] < Text[SA0[i0]])
    {
      SA[i] = SA12[i12++];
    }
    else if (Text[SA0[i0]] < Text[SA12[i12]])
    {
      SA[i] = SA0[i0++];
    }
    else
    { // first symbol is equal
      if (SA12[i12] % 3 == 1)
      { // SA0 vs. SA1
        if (ISA12[SA12[i12] + 1] < ISA12[SA0[i0] + 1])
          SA[i] = SA12[i12++]; // use ISA12
        else
          SA[i] = SA0[i0++];
      }
      else
      { // SA0 vs. SA2
        if (Text[SA12[i12] + 1] < Text[SA0[i0] + 1])
          SA[i] = SA12[i12++];
        else if (Text[SA0[i0] + 1] < Text[SA12[i12] + 1])
          SA[i] = SA0[i0++];
        else
        { // second symbol is (also) equal
          if (ISA12[SA12[i12] + 2] < ISA12[SA0[i0] + 2])
            SA[i] = SA12[i12++]; // use ISA12
          else
            SA[i] = SA0[i0++];
        }
      }
    }
    i++;
  }
  // remaining elements in SA0 and SA12
  while (i12 < n12)
    SA[i++] = SA12[i12++];
  while (i0 < n0)
    SA[i++] = SA0[i0++];

#if example
  printf("\n->SA = ");
  for (int i = 0; i < n; i++)
  {
    printf("[%d] ", SA[i]);
  }
  printf("\n");
#endif

  // min query lcp12
  int_lcp *tree_lcp12 = segtreeBuild(LCP12, n12);

  // Compute LCP
#if example
  printf("\n->Computing LCP\n");
#endif
  int notc, c;
  for (int i = 0; i < n; i++)
  {
    LCP[i] = 0;
  }
  for (int i = 1; i < n; i++)
  {
    notc = 1;
    c = 0;
    int SAi = SA[i], SAi1 = SA[i - 1];
#if example
    printf("\nSA[i-1] = %d\tSA[i] = %d\n", SAi1, SAi);
#endif
    while (c < 3) // SA[i] and SA[i-1] not in SA12
    {
      if (Text[SAi1 + c] != Text[SAi + c])
      {
        LCP[i] = c;
        notc = 0;
        break;
      }
      if ((SAi1 + c) % 3 != 0 && (SAi + c) % 3 != 0)
      {
        break;
      }
      c++;
    }
#if example
    printf("c=%d\tnotc=%d\t", c, notc);
#endif
    if (!notc)
      continue;

    int l = ISA12[SAi1 + c], r = ISA12[SAi + c];
    if (l > r)
    {
      l = l + r;
      r = l - r;
      l = l - r;
    }
    l = segtreeQuery(tree_lcp12, n12, l + 1, r);
#if example
    printf("l = %d\t", l);
#endif
    LCP[i] = c + (3 * l);
    // verify other 2 lcp
    if (Text[SAi1 + LCP[i]] == Text[SAi + LCP[i]])
    {
      LCP[i]++;
#if example
      printf("lcp++\t");
#endif
      if (Text[SAi1 + LCP[i]] == Text[SAi + LCP[i]])
      {
        LCP[i]++;
#if example
        printf("lcp++\t");
#endif
      }
    }
  }
  if (example)
    printf("\n");
#if DEBUG
  printf("==\nSA (merged)\n==\n");
  print(SA, Text, n, n, 0);
#endif

  free(SA12);
  free(TextU);
  free(SA0);
  free(ISA12);
  free(LCP12);
  free(tree_lcp12);

  return 0;
}