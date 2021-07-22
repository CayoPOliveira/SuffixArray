#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Lê na entrada o nome do arquivo com a String
//geradora do SA e do LCP e devolve essa
//string e seu tamanho
char *String_Read(size_t *size)
{
  char file[100];
  scanf("%s", file);
  FILE *arq = fopen(file, "r");

  if (arq == NULL)
  {
    printf("ERRO NA LEITURA DE ARQUIVO\n");
    *size = 1;
    return "$\0";
  }

  fseek(arq, 0, SEEK_END);
  *size = ftell(arq) + 1;
  rewind(arq);

  char *String = (char *)malloc((*size) * sizeof(char));
  fgets(String, *size, arq);
  String[*size - 1] = '$';
  String[*size] = '\0';

  fclose(arq);
  return String;
}

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

  int *SA = Naive(String, N);

  //TESTES
  printf("\n%s\n", String);
  for (int i = 0; i < N; i++)
  {
    printf("%d\n", SA[i]);
  }

  //SALVA AS INFORMAÇÕES EM UM ARQUIVO
  FILE *arq = fopen("SA.bin", "wb");
  fwrite(&N, sizeof(int), 1, arq);
  fwrite(String, sizeof(char), N, arq);
  fwrite(SA, sizeof(int), N, arq);
  fclose(arq);

  //< LIBERA MEMÓRIA ALOCADA >//
  free(SA);
  free(String);

  return 0;
}
