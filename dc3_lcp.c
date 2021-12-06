#include "dc3_lcp.h"

//  Debug Print ===============================================
int DBGP(int_t *v, uint_t n, char c)
{
  // output
  for (int_t i = 0; i < n; i++)
  {
    printf("i:%d[%d]%c", i, v[i], c);
  }
  return 0;
}

//  NAIVE ===============================================
int comp_str(const void *a, const void *b)
{
  const t_suffix *stra = (const t_suffix *)a;
  const t_suffix *strb = (const t_suffix *)b;
  return strcmp(stra->suffix, strb->suffix) < 0 ? 0 : 1;
}

// Ordena os sufixos e cria os vetores SA e LCP
int *Naive(char *Text, uint_t n, int_sa *SA)
{
#if DEBUG
  printf("==NAIVE==\n");
#endif
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

//  MM90 ===============================================
int *MM(int_str *Text, uint_t N, int_sa *SA)
{
  int i, j, H, saH, pos, aux;

  int *ISA = (int *)malloc(N * sizeof(int));   // ISA[SA[i]]==i
  int *Count = (int *)malloc(N * sizeof(int)); // Counter
  int *SB = (int *)malloc(N * sizeof(int));    // Start of Bucket
  int *B = (int *)malloc(N * sizeof(int));     // Bucket

  int *SA2H = (int *)malloc(N * sizeof(int)); // stage H sort result
  int *SB2H = (int *)malloc(N * sizeof(int)); // Index

  for (i = 0; i < N; i++)
  {
    SA[i] = ISA[i] = Count[i] = SB[i] = 0;
    B[i] = 0;
  }

  // K=0//BUCKET SORT//////
  int bucket[256];
  for (i = 0; i < 256; i++)
    bucket[i] = 0;
  for (i = 0; i < N; i++)
  {
    bucket[(int)Text[i]]++; // Count any char
  }
  aux = 0;
  for (i = 0; i < 256; i++)
    if (bucket[i] != 0) // char of string
    {
      aux += bucket[i];
      bucket[i] = aux - bucket[i]; // accumulated sum
      B[bucket[i]] = 1;
    }
  for (i = 0; i < N; i++) // First char sort in SA
  {
    j = (int)Text[i];
    SA[bucket[j]] = i;
    ISA[i] = bucket[j];
    bucket[j]++;

    // initiated
    SA2H[i] = Count[i] = SB[i] = 0;
  }
  //////////////////////
#if DEBUG
  printf("  B:\t");
  DBGP(B, N, "\t");
  printf("\n");
  printf(" SB:\t");
  DBGP(SB, N, "\t");
  printf("\n");
  printf(" SA:\t");
  DBGP(SA, N, "\t");
  printf("\n");
  printf("ISA:\t");
  DBGP(ISA, N, "\t");
  printf("\n");
#endif
  ////////////////////
  for (H = 1; H < N; H *= 2)
  {
    /// Count number of buckets//
    int nbucket = 0;
    for (j = 0, i = 0; i < N; i++)
    {
      SB2H[i] = 0;
      Count[i] = 0;
      if (B[i]) // first suffix bucket
      {
        nbucket++;
        j = i;
      }
      // update start of bucket
      SB[i] = j;
    }
    if (nbucket == N)
      break; // Complete
    ////////////////////
    SA2H[0] = N - 1;
    for (i = 0; i < N; i++)
    {
      saH = SA[i] - H;
      if (saH < 0)
        continue;

      pos = SB[ISA[saH]] + Count[SB[ISA[saH]]];
      Count[SB[ISA[saH]]]++;
      // induce suffix
      SA2H[pos] = saH;
      ISA[saH] = pos;

      SB2H[pos] = SB[i]; // bucket id

      if (B[pos] == 0 && SB2H[pos] != SB2H[pos - 1]) // new bucket
      {
        B[pos] = 1;
      }
    }

    for (j = 0, i = 0; i < N; i++)
    {
      // update SA
      SA[i] = SA2H[i];
    }
    /////////////////////
#if DEBUG
    printf("  B:\t");
    DBGP(B, N, "\t");
    printf("\n");
    printf(" SB:\t");
    DBGP(SB, N, "\t");
    printf("\n");
    printf(" SA:\t");
    DBGP(SA, N, "\t");
    printf("\n");
    printf("ISA:\t");
    DBGP(ISA, N, "\t");
    printf("\n");
#endif
  }

  ////////////////////////////////////////////////////////////////
  free(ISA);
  free(Count);
  free(SB);
  free(B);
  free(SA2H);
  free(SB2H);

  return 0;
}

int *MMLCP(int_str *Text, uint_t N, int_sa *SA, int_lcp *LCP)
{
  int_t i, j, H, saH, pos, aux;

  int_sa *ISA = (int_sa *)malloc(N * sizeof(int_sa));   // ISA[SA[i]]==i
  uint_t *Count = (uint_t *)malloc(N * sizeof(uint_t)); // Counter
  uint_t *SB = (uint_t *)malloc(N * sizeof(uint_t));    // Start of Bucket
  uint_t *B = (uint_t *)malloc(N * sizeof(uint_t));     // Bucket

  int_sa *SA2H = (int_sa *)malloc(N * sizeof(int_sa)); // stage H sort result
  uint_t *SB2H = (uint_t *)malloc(N * sizeof(uint_t)); // Index

  for (i = 0; i < N; i++)
  {
    SA[i] = ISA[i] = Count[i] = SB[i] = 0;
    B[i] = 0;
  }

  // K=0//BUCKET SORT//////
  int bucket[256];
  for (i = 0; i < 256; i++)
    bucket[i] = 0;
  for (i = 0; i < N; i++)
  {
    bucket[(int)Text[i]]++; // Count any char
  }
  aux = 0;
  for (i = 0; i < 256; i++)
    if (bucket[i] != 0) // char of string
    {
      aux += bucket[i];
      bucket[i] = aux - bucket[i]; // accumulated sum
      B[bucket[i]] = 1;
    }
  for (i = 0; i < N; i++) // First char sort in SA
  {
    j = (int)Text[i];
    SA[bucket[j]] = i;
    ISA[i] = bucket[j];
    bucket[j]++;

    // initiated
    SA2H[i] = Count[i] = SB[i] = 0;
    if (B[i])
      LCP[i] = 0;
    else
      LCP[i] = -1;
  }
  //////////////////////
#if DEBUG
  printf("  B:\t");
  DBGP(B, N, "\t");
  printf("\n");
  printf(" SB:\t");
  DBGP(SB, N, "\t");
  printf("\n");
  printf(" SA:\t");
  DBGP(SA, N, "\t");
  printf("\n");
  printf("ISA:\t");
  DBGP(ISA, N, "\t");
  printf("\n");
  printf("LCP:\t");
  DBGP(LCP, N, "\t");
  printf("\n");
#endif
  ////////////////////
  for (H = 1; H < N; H *= 2)
  {
    /// Count number of buckets//
    int nbucket = 0;
    for (j = 0, i = 0; i < N; i++)
    {
      SB2H[i] = 0;
      Count[i] = 0;
      if (B[i]) // first suffix bucket
      {
        nbucket++;
        j = i;
      }
      // update start of bucket
      SB[i] = j;
    }
    if (nbucket == N)
      break; // Complete
    ////////////////////
    SA2H[0] = N - 1;
    for (i = 0; i < N; i++)
    {
      saH = SA[i] - H;
      if (saH < 0)
        continue;

      pos = SB[ISA[saH]] + Count[SB[ISA[saH]]];
      Count[SB[ISA[saH]]]++;
      // induce suffix
      SA2H[pos] = saH;
      ISA[saH] = pos;

      SB2H[pos] = SB[i]; // bucket id

      if (B[pos] == 0 && SB2H[pos] != SB2H[pos - 1]) // new bucket
      {
        B[pos] = 1;
        LCP[pos] = H;
      }
      else if (B[pos] == 0)
      {
        LCP[pos] = H;
      }
    }

    for (j = 0, i = 0; i < N; i++)
    {
      // update SA
      SA[i] = SA2H[i];
    }
    /////////////////////
#if DEBUG
    printf("  B:\t");
    DBGP(B, N, "\t");
    printf("\n");
    printf(" SB:\t");
    DBGP(SB, N, "\t");
    printf("\n");
    printf(" SA:\t");
    DBGP(SA, N, "\t");
    printf("\n");
    printf("ISA:\t");
    DBGP(ISA, N, "\t");
    printf("\n");
    printf("LCP:\t");
    DBGP(LCP, N, "\t");
    printf("\n");
#endif
  }

  // final LCP
  for (i = 1; i < N; i++)
  {
    j = LCP[i] * 2;
    while (LCP[i] + 1 < j &&
           Text[SA[i] + LCP[i]] == Text[SA[i - 1] + LCP[i]]) // lcp(sa(i), sa(i-1))++
      LCP[i]++;
  }

  ////////////////////////////////////////////////////////////////
  free(ISA);
  free(Count);
  free(SB);
  free(B);
  free(SA2H);
  free(SB2H);

  return 0;
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

int min(int a, int b)
{
  if (a < b)
    return a;
  return b;
}

int_lcp *segtreeBuild(int V[], int n)
{
  int *tree = (int *)malloc((2 * n) * sizeof(int));
  // leaves
  for (int i = 0; i < n; i++)
    tree[i + n] = V[i];
  // Min-tree
  for (int i = n - 1; i > 0; i--)
    tree[i] = min(tree[2 * i], tree[2 * i + 1]);
  return tree;
}
// retorna um valor (min) do intervalo [a, b]
int_lcp segtreeQuery(int tree[], int n, int a, int b)
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

int DC3LCP(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP, int_t sigma)
{

#if DEBUG
  printf("==DC3LCP==\n");
#endif

  int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3;
  int n12 = n1 + n2;
  int_sa *SA12 = (int_sa *)malloc((n12 + 1) * sizeof(int_sa));
  int_lcp *lcp12 = (int_lcp *)malloc(n12 * sizeof(int_lcp));
  lcp12[0] = lcp12[n12 - 1] = 0;

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
    for (int i = 0; i < n12; i++)
    {
      lcp12[i] = 0;
    }
  }
  // DC3 (recursive)
  else //(name < n12)
  {

    printf("====RECURSIVE====\n");
#if DEBUG
    for (int i = 0; i < n12 + 1; i++)
      printf("%d ", TextU[i]);
    printf("\n");
#endif

    // recursive call
    DC3LCP(TextU, n12 + 1, SA12, lcp12, name + 1);

#if DEBUG
    printf("==\nBACK\n==\n");
    for (int i = 0; i < n12 + 1; i++)
      printf("%d ", TextU[i]);
    printf("\n");
    print(SA12, TextU, n12 + 1, n12 + 1, 0);
#endif

    // Map SA12 into SA (ignore first element of SA12, which is the middle 0)
    //+lcp12
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
      lcp12[i] = lcp12[i + 1];
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
  int i = 0;
  i12 = 0;
  i0 = 0;
  while (i12 < n12 && i0 < n0)
  {
    LCP[i] = lcp12[i12];
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

  // min query lcp12
  int_lcp *tree_lcp12 = segtreeBuild(lcp12, n12);

  // Compute LCP
  int notc, c;
  for (int i = 0; i < n; i++)
  {
    LCP[i] = 0;
  }
  for (int i = 1; i < n; i++)
  {
    notc = true;
    c = 0;
    int SAi = SA[i], SAi1 = SA[i - 1];

    while (c < 3) //SA[i] and SA[i-1] not in SA12
    {
      if (Text[SAi1 + c] != Text[SAi + c])
      {
        LCP[i] = c;
        notc = false;
        break;
      }
      if ((SAi1 + c) % 3 != 0 && (SAi + c) % 3 != 0)
      {
        break;
      }
      c++;
    }
    if (!notc)
      continue;

    int l = ISA12[SAi1 + c], r = ISA12[SAi + c];
    if (l > r)
    {
      l = l + r;
      r = l - r;
      l = l - r;
    }

    LCP[i] = c + (3 * segtreeQuery(tree_lcp12, n12, l + 1, r));
    // verify other 2 lcp
    if (Text[SAi1 + LCP[i]] == Text[SAi + LCP[i]])
    {
      LCP[i]++;
      if (Text[SAi1 + LCP[i]] == Text[SAi + LCP[i]])
        LCP[i]++;
    }
  }

#if DEBUG
  printf("==\nSA (merged)\n==\n");
  print(SA, Text, n, n, 0);
#endif

  free(SA12);
  free(TextU);
  free(SA0);
  free(ISA12);
  free(lcp12);
  free(tree_lcp12);

  return 0;
}

//  LCP ===============================================
int *Naive_lcp(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP)
{
  LCP[0] = 0;
  for (int i = 1; i < n; i++)
  {
    int j = 0;
    while (Text[SA[i] + j] == Text[SA[i - 1] + j])
      j++;
    LCP[i] = j;
  }
  return 0;
}

int *KASAI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP)
{
  LCP[0] = 0;

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

  free(ISA);
  return 0;
}

int *PHI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP)
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
  return 0;
}

int lcp_compare(int_lcp *a, int_lcp *b, uint_t n)
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