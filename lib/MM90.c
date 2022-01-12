#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MM90.h"

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