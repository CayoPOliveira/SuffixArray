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

// DC3(SK) SA SORT////////////////////////////////
struct SK_buffers
{
  int n12; // 2/3n
  int n3;  // 1/3n

  int *sa12; // 2/3n +
  int *sa3;  // 1/3n = n

  int current_u;
  char *u;  // 3*(2/3n+1)
  int *sau; // 3*(2/3n+1)

  int radix_buckets[256];
  int *helper_buffer0; // 2/3n +
  int *helper_buffer1; // 2/3n = 4/3 n
  int *lex_remapped;   // alias for helper 0
};

struct SK_buffers *make_buffers(char *String, int N)
{
  struct SK_buffers *buffers = (struct SK_buffers *)malloc(sizeof(struct SK_buffers));

  buffers->n3 = (N % 3 == 0) ? N / 3 : (N / 3) + 1;
  buffers->n12 = N - buffers->n3;

  buffers->sa12 = (int *)malloc(buffers->n12 * sizeof(int));
  buffers->sa3 = (int *)malloc(buffers->n3 * sizeof(int));

  return buffers;
}

void bye_buffers(struct SK_buffers *buffers)
{
  free(buffers->sa12);
  free(buffers->sa3);
}

void radix_sort_sa12(char *String, int N, struct SK_buffers *buffers)
{
  int i, j, n12 = buffers->n12, aux = 0;
  int *sa12 = buffers->sa12;
  int radix_buckets[256];
  for (j = 0; j < 256; j++)
  {
    radix_buckets[j] = 0;
  }
  if (DEBUG)
  {
    printf("\nRadix_sort_sa12:\nsa12: ");
    for (i = 0; i < n12; i++)
    {
      printf("%d ", buffers->sa12[i]);
    }
  }
  for (i = 2; i >= 0; i--)
  {
    for (j = 0; j < 256; j++)
    {
      radix_buckets[j] = 0;
    }
    for (j = 0; j < n12; j++)
    {
      radix_buckets[String[sa12[j] + i]]++;
    }
    for (j = 0; j < 256; j++)
    {
      if (radix_buckets[j] != 0)
      {
        aux += radix_buckets[j];
        radix_buckets[j] = aux - radix_buckets[j];
      }
    }
    int a[n12];
    for (j = 0; j < n12; j++)
    {
      a[radix_buckets[String[sa12[j] + i]]++] = sa12[j];
    }
    for (j = 0; j < n12; j++)
    {
      sa12[j] = a[j];
    }
    if (DEBUG)
    {
      printf("\nRadix_sort_sa12:\nsa12: ");
      for (i = 0; i < n12; i++)
      {
        printf("%d ", sa12[i]);
      }
    }
  }
  if (DEBUG)
  {
    printf("\n");
  }
}

static void construct_sa3(char *String, int N, struct SK_buffers *buffers)
{
  int i = 0, j, n3 = buffers->n3;
  int *sa3 = buffers->sa3;

  if ((N - 1) % 3 == 0)
  {
    sa3[i++] = N - 1;
  }
  int n12 = buffers->n12;
  for (j = 0; j < n12; j++)
  {
    int p = buffers->sa12[j];
    if ((p % 3) == 1)
    {
      sa3[i++] = p - 1;
    }
  }

  //raddix_sort_sa3
  int radix_buckets[256];
  for (j = 0; j < 256; j++)
  {
    radix_buckets[j] = 0;
  }
  for (j = 0; j < n3; j++)
  {
    radix_buckets[sa3[j]]++;
  }
  int aux = 0;
  for (j = 0; j < 256; j++)
  {
    if (radix_buckets[j] != 0)
    {
      aux += radix_buckets[j];
      radix_buckets[j] = aux - radix_buckets[j];
    }
  }
  int a[n3];
  for (j = 0; j < n3; j++)
  {
    a[radix_buckets[String[sa3[j]]]++] = sa3[j];
  }
  for (j = 0; j < n3; j++)
  {
    sa3[j] = a[j];
  }
  if (DEBUG)
  {
    printf("\nRadix_sort_sa3:\nsa3: ");
    for (i = 0; i < n12; i++)
    {
      printf("%d ", sa3[n12]);
    }
    printf("\n");
  }
}

void construct_sa12(char *String, int N, struct SK_buffers *buffers)
{

  int i, j;
  for (j = i = 0; i < N; i++)
  {
    if (i % 3 != 0)
      buffers->sa12[j++] = i;
    if (DEBUG && j > buffers->n12)
    {
      printf("n12: %d\n", buffers->n12);
      perror("n12 foi execido");
      break;
    }
  }
  if (DEBUG)
  {
    printf("\n%s\nsa12:", String);
    for (i = 0; i < buffers->n12; i++)
    {
      printf("%d ", buffers->sa12[i]);
    }
  }
  //radix_sort_sa12(String, N, buffers);
}

int *SK(char *String, int N)
{
  int *SA = (int *)malloc(N * sizeof(int));

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

  struct SK_buffers *buffers = make_buffers(String, N);

  construct_sa12(String, N, buffers);
  construct_sa3(String, N, buffers);

  //Merge SA

  //

  bye_buffers(buffers);
  return SA;
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

  int *SA_SK = SK(String, N);
  int *SA_Naive = Naive(String, N);

  //TESTES
  printf("\n%s\n", String);
  int flag = 1;
  for (int i = 0; i < N; i++)
  {
    printf("%d\t%d\n", SA_Naive[i], SA_SK[i]);
    if (SA_SK[i] != SA_Naive[i])
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
  fwrite(SA_SK, sizeof(int), N, arq);
  fclose(arq);

  //< FREE MEMORY >//
  free(SA_SK);
  free(SA_Naive);
  free(String);

  return 0;
}