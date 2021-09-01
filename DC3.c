#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//RADIX SORT PRECISA RECEBER O VALOR MAXIMO DE SEUS CARACTERES <INCLUIR ISSO AO FINAL>

int DEBUG = 0;
int SAVE = 0;

//  SAVE SA IN A FILE /////////////////////////
void save_file(char *file, int N, char *String, int *SA_DC3)
{
  FILE *arq = fopen(file, "wb");
  fwrite(&N, sizeof(int), 1, arq);
  fwrite(String, sizeof(char), N, arq);
  fwrite(SA_DC3, sizeof(int), N, arq);
  fclose(arq);
}

// NAIVE SA SORT  /////////////////////////////

//Função de comparação para o qsort
int comp_str(const void *a, const void *b)
{
  const char **stra = (const char **)a;
  const char **strb = (const char **)b;
  return strcmp(*stra, *strb);
}

//Ordena os sufixos e cria os vetores SA e LCP
int *Naive(char *String, int N)
{
  int *SA = (int *)malloc(N * sizeof(int));
  int i = 0;

  char **suffixes = (char **)malloc(N * sizeof(char *));
  for (i = 0; i < N; i++)
  {
    suffixes[i] = (char *)malloc(N * sizeof(char));
    strcpy(suffixes[i], String + i);
  }

  qsort(suffixes, N, sizeof(char *), comp_str);

  for (i = 0; i < N; i++)
  {
    SA[i] = N - strlen(suffixes[i]);
    free(suffixes[i]);
  }
  free(suffixes);

  return SA;
}

//  DC3 ////////////////////////////////

void raddix_sort(int *String, int *sa, int n, int pos, int K) //pos = lenght(substring)-1
{
  int i = 0, aux = 0;
  int *bucket_id = (int *)malloc(K * sizeof(int));
  int sa_copy[n];
  for (int P = pos; P > -1; P--) //P char order
  {
    for (i = 0; i < K; i++)
    { //map
      bucket_id[i] = 0;
    }
    for (i = 0; i < n; i++)
    { //count and copy sa
      bucket_id[String[sa[i] + P]]++;
      sa_copy[i] = sa[i];
    }
    if (DEBUG)
    {
      printf("\nBUCKET_ID\nid\n");
      for (i = 0; i < K; i++)
      { //first position
        printf("%d\n", bucket_id[i]);
      }
    }
    aux = 0;
    for (i = 0; i < K; i++)
    { //first position
      aux += bucket_id[i];
      bucket_id[i] = aux - bucket_id[i];
    }
    if (DEBUG)
    {
      printf("\nBUCKET_ID\nid\tsa\n");
      for (i = 0; i < K; i++)
      { //first position
        printf("%d\t%d\n", bucket_id[i], sa[i]);
      }
    }
    for (i = 0; i < n; i++)
    { //sort
      int c = String[sa_copy[i] + P];
      sa[bucket_id[c]++] = sa_copy[i];
    }
    if (DEBUG)
    {
      printf("\nBUCKET_ID\nid\tsa\n");
      for (i = 0; i < K; i++)
      { //first position
        printf("%d\t%d\n", bucket_id[i], sa[i]);
      }
    }
  }
  free(bucket_id);
}

void sort_sa12(int *u, int N, int *sa12, int n12, int K) //não funciona ainda
{
  int i = 0, j = 0, k = 1, flag = 1;

  int isa12[N];
  for (i = 0; i < n12; i++) //order sa12
  {
    isa12[sa12[i]] = i;
  }

  // int *str_aux = (int *)malloc(N * sizeof(int));
  // for (i = 0; i < N; i++)
  // {
  //   str_aux[i] = u[i];
  // }
  // str_aux[N] = str_aux[N + 1] = (int)'$';
  raddix_sort(u, sa12, n12, 2, K);

  int mid = (N + 1) / 3;
  int *u2 = (int *)malloc((n12) * sizeof(int)); //new string
  //u[n12] = u[n12 + 1] = u[n12 + 2] = 0;
  for (i = 1; i < n12; i++)
  { //unique triplets
    if (u[sa12[i] + 0] != u[sa12[i - 1] + 0] ||
        u[sa12[i] + 1] != u[sa12[i - 1] + 1] ||
        u[sa12[i] + 2] != u[sa12[i - 1] + 2])
      k++;
    if (sa12[i] % 3 == 1) //mod 1
      u2[sa12[i] / 3] = k;
    else //mod 2
      u2[(sa12[i] / 3) + mid] = k;
  }
  if (k < n12)
  {
    DC3(u2, n12, sa12, k + 1);
  }

  free(u2);
  //free(str_aux);
}

void build_sa3(int *String, int N, int *sa12, int m12, int *sa3, int m3, int K)
{
  int i = 0, j = 0;
  //first step, sa12 order
  for (i = 0; i < m12; i++)
  {
    if (sa12[i] % 3 == 1)
      sa3[j++] = sa12[i] - 1;
  }

  //second step, raddix_sort(fist char)
  raddix_sort(String, sa3, m3, 0, K + 1);

  return;
}

void merge(int *String, int N, int *sa12, int n12, int *sa3, int n3, int *SA)
{
  int i = 0, c3 = 0, c12 = 0;

  int isa12[N];
  for (i = 0; i < n12; i++) //order sa12
  {
    isa12[sa12[i]] = i;
  }

  if (N % 3 == 0)
  {
    SA[0] = N - 1;
    i = 1;
  }
  else
    i = 0;

  for (i; i < N; i++)
  {
    if (c3 >= n3) //  all sa3 suffixes in SA
    {
      while (i++ < N)
        SA[i] = sa12[c12++];

      break;
    }
    if (c12 >= n12) //  all sa12 suffixes in SA
    {
      while (i++ < N)
        SA[i] = sa3[c3++];

      break;
    }

    if (String[sa3[c3]] < String[sa12[c12]]) // sa3
      SA[i] = sa3[c3++];
    else if (String[sa3[c3]] > String[sa12[c12]]) //  sa12
      SA[i] = sa12[c12++];
    else //try again
    {
      if (sa12[c12] % 3 == 1)
      {
        SA[i] = (isa12[sa3[c3] + 1] < isa12[sa12[c12] + 1]) ? sa3[c3++] : sa12[c12++];
      }
      else //sa12[k] % 3 == 2
      {
        if (String[sa3[c3] + 1] < String[sa12[c12] + 1]) //sa3
        {
          SA[i] = sa3[c3++];
        }
        else if (String[sa3[c3] + 1] > String[sa12[c12] + 1]) //sa12
        {
          SA[i] = sa12[c12++];
        }
        else //sa12 order
        {
          SA[i] = (isa12[sa3[c3] + 2] < isa12[sa12[c12] + 2]) ? sa3[c3++] : sa12[c12++];
        }
      }
    }
  }
  return;
}

void DC3(int *u, int N, int *SA, int K)
{
  //  segment SA suffixes
  int n3 = (N + 2) / 3;
  int n12 = N - n3;
  int *sa3 = (int *)malloc(n3 * sizeof(int));
  int *sa12 = (int *)malloc(n12 * sizeof(int));
  int i = 0, j = 0, k = 0;
  for (i = 0; i < N && j < n12; i++)
  {
    if (i % 3 != 0)
      sa12[j++] = i;
  }

  //  DC3 algorithm ///////////////////////////
  //sort_sa12(u, N, sa12, n12, K);
  //build_sa3(u, N, sa12, n12, sa3, n3, K);
  //merge(u, N, sa12, n12, sa3, n3, SA);

  //  SA12  ///////////
  raddix_sort(u, sa12, n12, 2, K);

  int mid = (N + 1) / 3;
  int *u2 = (int *)malloc((n12) * sizeof(int)); //new string
  i = j = 0;
  k = 0;

  if (sa12[0] % 3 == 1) //mod 1
    u2[sa12[0] / 3] = k;
  else //mod 2
    u2[(sa12[0] / 3) + mid] = k;
  for (i = 1; i < n12; i++)
  { //unique triplets
    if (u[sa12[i] + 0] != u[sa12[i - 1] + 0] ||
        u[sa12[i] + 1] != u[sa12[i - 1] + 1] ||
        u[sa12[i] + 2] != u[sa12[i - 1] + 2])
      k++;
    if (sa12[i] % 3 == 1) //mod 1
      u2[sa12[i] / 3] = k;
    else //mod 2
      u2[(sa12[i] / 3) + mid] = k;
  }
  if (k < n12)
  {
    DC3(u2, n12, sa12, k + 1);
  }

  //  SA3   ///////////
  i = j = 0;
  //first step, sa12 order
  for (i = 0; i < n12; i++)
  {
    if (sa12[i] % 3 == 1)
      sa3[j++] = sa12[i] - 1;
  }
  //second step, raddix_sort(fist char)
  raddix_sort(u, sa3, n3, 0, K);

  // MERGE ////////////
  int c3 = 0, c12 = 0;

  int isa12[N];
  for (i = 0; i < n12; i++) //order sa12
    isa12[sa12[i]] = i;

  if (N % 3 == 0)
  {
    SA[0] = N - 1;
    i = 1;
  }
  else
    i = 0;

  for (i; i < N; i++)
  {
    if (c3 >= n3) //  all sa3 suffixes in SA
    {
      while (i++ < N)
        SA[i] = sa12[c12++];

      break;
    }
    if (c12 >= n12) //  all sa12 suffixes in SA
    {
      while (i++ < N)
        SA[i] = sa3[c3++];

      break;
    }

    if (u[sa3[c3]] < u[sa12[c12]]) // sa3
      SA[i] = sa3[c3++];
    else if (u[sa3[c3]] > u[sa12[c12]]) //  sa12
      SA[i] = sa12[c12++];
    else //try again
    {
      if (sa12[c12] % 3 == 1)
      {
        SA[i] = (isa12[sa3[c3] + 1] < isa12[sa12[c12] + 1]) ? sa3[c3++] : sa12[c12++];
      }
      else //sa12[k] % 3 == 2
      {
        if (u[sa3[c3] + 1] < u[sa12[c12] + 1]) //sa3
        {
          SA[i] = sa3[c3++];
        }
        else if (u[sa3[c3] + 1] > u[sa12[c12] + 1]) //sa12
        {
          SA[i] = sa12[c12++];
        }
        else //sa12 order
        {
          SA[i] = (isa12[sa3[c3] + 2] < isa12[sa12[c12] + 2]) ? sa3[c3++] : sa12[c12++];
        }
      }
    }
  }

  //  free  ////////////
  free(sa3);
  free(sa12);
  free(u2);

  return;
}

int main(int argc, char *argv[])
{
  char String[] = "mississippi$$$";
  //  convert to 4 byte array
  int *str = NULL;
  int N = 14, K = 256;
  if (argc == 2)
  {
    N = strlen(argv[1]) + 2;
    str = (int *)malloc(N * sizeof(int));
    for (int i = 0; i < N - 2; i++)
    {
      str[i] = (int)argv[1][i];
    }
    str[N - 1] = str[N - 2] = str[N - 3] = (int)'$';
  }
  else
  {
    str = (int *)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++)
    {
      str[i] = (int)String[i];
    }
  }

  int *SA_DC3 = (int *)malloc(N * sizeof(int));
  DC3(str, N - 2, SA_DC3, K);

  int *SA_Naive = Naive(String, N);

  //TESTES
  printf("\n%s\ni\tnaive\tdc3\n", String);
  int flag = 1;
  for (int i = 0; i < N; i++)
  {
    printf("%d\t%d\t%d\n", i, SA_Naive[i], SA_DC3[i]);
    if (SA_DC3[i] != SA_Naive[i])
      flag = 0;
  }
  if (flag)
    printf("GREAT!\n");
  else
    printf("THIS IS BAD!\n");

  //SALVA AS INFORMAÇÕES EM UM ARQUIVO
  if (SAVE)
    save_file("SA.bin", N, String, SA_DC3);

  //< FREE MEMORY >//
  free(SA_DC3);
  free(SA_Naive);
  free(str);

  return 0;
}