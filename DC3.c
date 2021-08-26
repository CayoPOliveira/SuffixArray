#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int DEBUG = 1;

// NAIVE SA SORT////////////////////////////////

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
//  DC3

void raddix_sort(int *String, int *sa, int m, int pos) //pos = lenght(substring)-1
{
  int i, aux = 0;
  int bucket_id[256];
  int sa_copy[m];
  for (int P = pos; P > -1; P--) //P char order
  {
    for (i = 0; i < 256; i++)
    { //map
      bucket_id[i] = 0;
    }
    for (i = 0; i < m; i++)
    { //count and copy sa
      bucket_id[String[sa[i] + P]]++;
      sa_copy[i] = sa[i];
    }
    for (i = 0; i < 256; i++)
    { //first position
      aux += bucket_id[i];
      bucket_id[i] = aux - bucket_id[i];
    }
    for (i = 0; i < m; i++)
    { //sort
      int c = String[sa_copy[i] + P];
      sa[bucket_id[c]++] = sa_copy[i];
    }
  }
}

void sort_sa12(int *String, int N, int *sa12, int m12) //não funciona ainda
{
  int i, j = 0, k = 0, flag = 1;

  int isa12[N];
  for (i = 0; i < m12; i++) //order sa12
  {
    isa12[sa12[i]] = i;
  }

  int str_aux[N + 2];
  for (i = 0; i < N; i++)
  {
    str_aux[i] = String[i];
  }
  str_aux[N] = str_aux[N + 1] = (int)'$';

  // strcpy(str_aux, String);
  // strcat(str_aux, "$$");
  raddix_sort(str_aux, sa12, m12, 2);
  int n = m12 + 1, mid = (N + 2) / 3;
  int *u = (int *)malloc(n * sizeof(int));
  for (i = 1; i < m12; i++)
  { //unique triplets
    if (str_aux[sa12[i]] != str_aux[sa12[i - 1]] ||
        str_aux[sa12[i] + 1] != str_aux[sa12[i - 1] + 1] ||
        str_aux[sa12[i] + 2] != str_aux[sa12[i - 1] + 2])
      k++;
    if (sa12[i] % 3 == 1)
    {
      u[sa12[i] / 3] = k;
    }
    else
    {
      u[(sa12[i] / 3) + mid] = k;
    }
  }

  DC3_recursivo(u, n, sa12);
  free(u);
}

void build_sa3(int *String, int N, int *sa12, int m12, int *sa3, int m3)
{
  int i, j = 0;
  //first step, sa12 order
  for (i = 0; i < m12; i++)
  {
    if (sa12[i] % 3 == 1)
      sa3[j++] = sa12[i] - 1;
  }

  //second step, raddix_sort(fist char)
  raddix_sort(String, sa3, m3, 0);
  int bucket_id[256];
  int sa3_copy[m3];
  for (i = 0; i < 256; i++)
  { //map
    bucket_id[i] = 0;
  }
  for (i = 0; i < m3; i++)
  { //count and copy sa3
    bucket_id[String[sa3[i]]]++;
    sa3_copy[i] = sa3[i];
  }
  int aux = 0;
  for (i = 0; i < 256; i++)
  { //first position
    aux += bucket_id[i];
    bucket_id[i] = aux - bucket_id[i];
  }
  for (i = 0; i < m3; i++)
  { //sort
    int c = String[sa3_copy[i]];
    sa3[bucket_id[c]] = sa3_copy[i];
  }
}

void merge(int *String, int N, int *sa12, int m12, int *sa3, int m3, int *SA)
{
  int i, j = 0, k = 0;
  int isa12[N];

  for (i = 0; i < m12; i++) //order sa12
  {
    isa12[sa12[i]] = i;
  }

  for (i = 0; i < N; i++)
  {
    if (String[sa3[j]] < String[sa12[k]]) //sa3
    {
      SA[i] = sa3[j++];
    }
    else if (String[sa3[j]] > String[sa12[k]]) //sa12
    {
      SA[i] = sa12[k++];
    }
    else //try again
    {
      if (sa12[k] % 3 == 1)
      {
        if (isa12[sa3[j] + 1] < isa12[sa12[k] + 1]) //sa3
        {
          SA[i] = sa3[j++];
        }
        else //sa12
        {
          SA[i] = sa12[k++];
        }
      }
      else //sa12[k] % 3 == 2
      {
        if (String[sa3[j] + 1] < String[sa12[k] + 1]) //sa3
        {
          SA[i] = sa3[j++];
        }
        else if (String[sa3[j] + 1] > String[sa12[k] + 1]) //sa12
        {
          SA[i] = sa12[k++];
        }
        else //sa12 order
        {
          if (isa12[sa3[j] + 2] < isa12[sa12[k] + 2]) //sa3
          {
            SA[i] = sa3[j++];
          }
          else //sa12
          {
            SA[i] = sa12[k++];
          }
        }
      }
    }
  }
}

void DC3_recursivo(int *u, int N, int *SA)
{

  int m3 = (N + 2) / 3;
  int m12 = N - m3;
  int *sa3 = (int *)malloc(m3 * sizeof(int));
  int *sa12 = (int *)malloc(m12 * sizeof(int));
  int i, j = 0;
  for (i = 0; i < N && j < m12; i++)
  {
    if (i % 3 != 0)
      sa12[j++] = i;
  }

  sort_sa12(u, N, sa12, m12);
  build_sa3(u, N, sa12, m12, sa3, m3);
  merge(u, N, sa12, m12, sa3, m3, SA);

  free(sa3);
  free(sa12);
}

void DC3(int *String, int N, int *SA)
{

  // Trivial special cases
  if (N == 0)
  {
    SA[0] = 0;
    return;
  }
  else if (N == 1)
  {
    SA[0] = 1;
    SA[1] = 0;
    return;
  }

  int m3 = (N + 2) / 3;
  int m12 = N - m3;
  int *sa3 = (int *)malloc(m3 * sizeof(int));
  int *sa12 = (int *)malloc(m12 * sizeof(int));
  int i, j = 0;
  for (i = 0; i < N && j < m12; i++)
  {
    if (i % 3 != 0)
      sa12[j++] = i;
  }

  sort_sa12(String, N, sa12, m12);
  build_sa3(String, N, sa12, m12, sa3, m3);
  merge(String, N, sa12, m12, sa3, m3, SA);

  free(sa3);
  free(sa12);
}

int main(int argc, char *argv[])
{
  char *String = (char *)malloc(13 * sizeof(char));
  strcpy(String, "mississippi$");
  if (argc == 2)
  {
    String = (char *)malloc((1 + strlen(argv[1])) * sizeof(char));
    strcpy(String, argv[1]);
    strcat(String, "$");
  }
  int N = strlen(String);

  int *SA_DC3 = (int *)malloc(N * sizeof(int));
  DC3(String, N, SA_DC3);
  int *SA_Naive = Naive(String, N);

  //TESTES
  printf("\n%s\n", String);
  int flag = 1;
  for (int i = 0; i < N; i++)
  {
    printf("%d\t%d\n", SA_Naive[i], SA_DC3[i]);
    if (SA_DC3[i] != SA_Naive[i])
      flag = 0;
  }
  if (flag)
    printf("GREAT!\n");
  else
    printf("THIS IS BAD!\n");

  //SALVA AS INFORMAÇÕES EM UM ARQUIVO
  FILE *arq = fopen("SA.bin", "wb");
  fwrite(&N, sizeof(int), 1, arq);
  fwrite(String, sizeof(char), N, arq);
  fwrite(SA_DC3, sizeof(int), N, arq);
  fclose(arq);

  //< FREE MEMORY >//
  free(SA_DC3);
  free(SA_Naive);
  free(String);

  return 0;
}