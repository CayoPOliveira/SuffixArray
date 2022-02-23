#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "lib/DC3.h"
#include "lib/LCP.h"

#define int_t int
#define uint_t unsigned int
#define int_sa uint_t
#define int_lcp uint_t
#define int_str uint_t


int validate = 0;
int TIME = 0;

/****************/
// Time Test

void time_start(time_t *t_time, clock_t *c_clock)
{
  *t_time = time(NULL);
  *c_clock = clock();
}

double time_stop(time_t t_time, clock_t c_clock)
{
  double aux1 = (clock() - c_clock) / (double)(CLOCKS_PER_SEC);
  double aux2 = difftime(time(NULL), t_time);
  printf("CLOCK = %lf TIME = %lf\n", aux1, aux2);
  return aux1;
}

/****************/
int main(int argc, char *argv[])
{
  time_t t_start = 0;
  clock_t c_start = 0;

  char *Text = (char *)malloc(200 * sizeof(char));
  uint_t n;

  // intput data
  if (argc == 2)
  {
    FILE *arq = NULL;
    arq = fopen(argv[1], "r");
    if (arq == NULL)
    {
      perror("FALHA NA ABERTURA DO ARQUIVO");
      return 0;
    }
    fseek(arq, 0, SEEK_END);
    n = ftell(arq) + 1;
    rewind(arq);
    free(Text);
    Text = (char *)malloc(n * sizeof(char *));
    char *T_temp = (char *)malloc(n * sizeof(char));
    while (fscanf(arq, "%s", T_temp) != EOF)
    {
      strcat(Text, T_temp);
    }
    free(T_temp);
    fclose(arq);
  }
  else
  {
    // strcpy(Text, "banaananabanaabnaabannbanbanbanabna");
    strcpy(Text, "anabananabananabananabanana");
    // strcpy(Text, "mississippi");
    // strcpy(Text, "larissalimamoraesaguiar");
    // strcpy(Text, "cayophelliperamalhodeoliveira");

    n = strlen(Text) + 1;
  }

  // Text as int
  int_str *uText = (int_str *)malloc((n + 2) * sizeof(int_str));

  for (int_t i = 0; i < n - 1; i++)
    uText[i] = Text[i];
  uText[n - 1] = uText[n] = uText[n + 1] = '$';

  // allocate
  int_sa *SA = (int_sa *)malloc(n * sizeof(int_sa));
  for (int i = 0; i < n; i++)
    SA[i] = 0;
  int_lcp *dc3_LCP = (int_lcp *)malloc(n * sizeof(int_lcp));

  // sort
  if (TIME)
  {
    printf("==DC3+LCP==\n");
    time_start(&t_start, &c_start);
  }
  DC3_LCP(uText, n, SA, dc3_LCP, 255);
  if (TIME)
  {
    time_stop(t_start, c_start);
  }

  // LCP
  int_lcp *LCP = (int_lcp *)malloc(n * sizeof(int_lcp));
  for (int i = 0; i < n; i++)
    LCP[i] = 0;

  PHI(uText, n, SA, LCP);

  if (validate)
  {
    LCP_Compare(LCP, dc3_LCP, n);
  }

  // deallocate
  free(SA);
  free(LCP);
  free(dc3_LCP);
  free(uText);
  free(Text);

  return 0;
}

/****************/
