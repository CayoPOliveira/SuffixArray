#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void raddix_sort(int *String, int *SA, int n, int pos, int K) //pos = lenght(substring)-1
{
  int i = 0, aux = 0;
  int *bucket_id = (int *)malloc(K * sizeof(int));
  int *sa_copy = (int *)malloc((n + 2) * sizeof(int));
  for (int P = pos; P >= 0; P--) //P char order
  {
    for (i = 0; i < K; i++)
    { //map
      bucket_id[i] = 0;
    }
    if (DEBUG)
    {
      for (i = 0; i < n; i++)
      {
        printf("suffix%d: %d%d%d\n", SA[i], String[SA[i]], String[SA[i] + 1], String[SA[i] + 2]);
      }
    }
    for (i = 0; i < n; i++)
    { //count and copy sa
      bucket_id[String[SA[i] + P]]++;
      sa_copy[i] = SA[i];
    }
    aux = 0;
    for (i = 0; i < K; i++)
    { //first position
      aux += bucket_id[i];
      bucket_id[i] = aux - bucket_id[i];
    }
    if (DEBUG)
    {
      printf("\nBUCKET_ID\ni\tid\n");
      for (i = 0; i < K; i++)
      { //first position
        printf("%d\t%d\n", i, bucket_id[i]);
      }
      printf("\nsa\n");
      for (i = 0; i < n; i++)
      { //first position
        printf("%d\t", SA[i]);
      }
      printf("\n");
    }
    for (i = 0; i < n; i++)
    { //sort
      SA[bucket_id[String[sa_copy[i] + P]]++] = sa_copy[i];
    }
    if (DEBUG)
    {
      printf("\nsa\n");
      for (i = 0; i < n; i++)
      { //first position
        printf("%d\t", SA[i]);
      }
      printf("\n");
    }
  }
  free(bucket_id);
  free(sa_copy);
}

void DC3(int *str, int N, int *SA, int K)
{
  //  segment SA suffixes
  int n3 = (N + 2) / 3;
  int n12 = N - n3;

  int *sa12 = (int *)malloc(n12 * sizeof(int));
  int i = 0, j = 0;
  for (i = 0; i < N; i++)
  {
    if (i % 3 != 0)
      sa12[j++] = i;
  }

  //  SA12  ///////////
  raddix_sort(str, sa12, n12, 2, K);
  //COLOCAR O [0] NO MEIO
  int mid = (N + 1) / 3;
  int *u = (int *)malloc((n12 + 3) * sizeof(int)); //new string
  int id = 1;

  if (sa12[0] % 3 == 1) //mod 1
    u[sa12[0] / 3] = id;
  else //mod 2
    u[(sa12[0] / 3) + mid + 1] = id;
  for (i = 1; i < n12; i++)
  { //unique triplets
    if (str[sa12[i] + 0] != str[sa12[i - 1] + 0] ||
        str[sa12[i] + 1] != str[sa12[i - 1] + 1] ||
        str[sa12[i] + 2] != str[sa12[i - 1] + 2])
      id++;
    if (sa12[i] % 3 == 1) //mod 1
      u[sa12[i] / 3] = id;
    else //mod 2
      u[(sa12[i] / 3) + mid + 1] = id;
  }
  u[mid] = u[n12 + 1] = u[n12 + 2] = 0;
  if (id < n12)
  {
    printf("====RECURSIVE====\n");
    //recursion call
    DC3(u, n12, sa12, id + 1);

    //map sa12
    for (i = 0; i < n12; i++)
    {
      if (sa12[i + 1] < mid + 1)
        sa12[i] = sa12[i + 1] * 3 + 1; // i%3==1
      else
        sa12[i] = (sa12[i + 1] - (mid + 1)) * 3 + 2; // i%3==2
    }
  }

  //  SA3   ///////////
  int *sa3 = (int *)malloc(n3 * sizeof(int));
  //first step, sa12 order
  for (i = 0, j = 0; i < n12; i++)
  {
    if (sa12[i] % 3 == 1)
      sa3[j++] = sa12[i] - 1;
  }
  //second step, raddix_sort(fist char)
  raddix_sort(str, sa3, n3, 0, K);

  // MERGE ////////////
  int c3 = 0, c12 = 0;

  int *isa12 = (int *)malloc(N * sizeof(int));
  for (i = 0; i < n12; i++) //inverse sa12
    isa12[sa12[i]] = i;

  if ((N - 1) % 3 == 0)
  {
    SA[0] = N - 1;
    i = 1;
  }
  else
    i = 0;

  while (i < N)
  {
    if (c3 >= n3) //  all sa3 suffixes in SA
    {
      while (i < N)
        SA[i++] = sa12[c12++];
      break;
    }
    if (c12 >= n12) //  all sa12 suffixes in SA
    {
      while (i < N)
        SA[i++] = sa3[c3++];
      break;
    }

    if (str[sa3[c3]] < str[sa12[c12]]) // sa3
      SA[i] = sa3[c3++];
    else if (str[sa3[c3]] > str[sa12[c12]]) //  sa12
      SA[i] = sa12[c12++];
    else //try again
    {
      if (sa12[c12] % 3 == 1)
      {
        SA[i] = (isa12[sa3[c3] + 1] < isa12[sa12[c12] + 1]) ? sa3[c3++] : sa12[c12++];
      }
      else //sa12[k] % 3 == 2
      {
        if (str[sa3[c3] + 1] < str[sa12[c12] + 1]) //sa3
        {
          SA[i] = sa3[c3++];
        }
        else if (str[sa3[c3] + 1] > str[sa12[c12] + 1]) //sa12
        {
          SA[i] = sa12[c12++];
        }
        else //sa12 order
        {
          SA[i] = (isa12[sa3[c3] + 2] < isa12[sa12[c12] + 2]) ? sa3[c3++] : sa12[c12++];
        }
      }
    }
    i++;
  }

  //  free  ////////////
  free(sa3);
  free(sa12);
  free(isa12);
  free(u);
}

int main(int argc, char *argv[])
{
  char *String = "mississippi";
  int N;
  //  convert to 4 byte array
  if (argc == 2)
  {
    N = strlen(argv[1]) + 1;
    String = (char *)malloc(N * sizeof(char));
    strcpy(String, argv[1]);
  }
  else
  {
    //String = "abca";
    //String = "anabanana"
    //String = "abracadabra";
    //String = "otorrinolaringologista";
    N = strlen(String);
  }
  int *u = (int *)malloc((N + 2) * sizeof(int));
  for (int i = 0; i < N; i++)
  {
    u[i] = String[i];
  }
  u[N - 1] = u[N] = u[N + 1] = '$';

  int *SA_DC3 = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++)
    SA_DC3[i] = 0;

  //  sort
  DC3(u, N, SA_DC3, 256);

  int *SA_Naive = Naive(String, N);

  //TESTES
  printf("\n%s\ni\tnaive\tdc3\tN=%d\n", String, N);
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
  free(u);
  if (argc == 2)
    free(String);

  return 0;
}