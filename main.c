// DC3_felipe.c used to compute SA

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "dc3_lcp.h"

int validate = 1;
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

  char *Text;
  int n;

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
    // Text = "banaananabanaabnaabannbanbanbanabna";
    //   Text = "banana";
    Text = "mississippi";

    n = strlen(Text) + 1;
  }

  // printf("Text = %s$\n", Text);

  // Text as int
  int_str *uText = (int_str *)malloc((n + 2) * sizeof(int_str));

  for (int_t i = 0; i < n - 1; i++)
    uText[i] = Text[i];
  uText[n - 1] = uText[n] = uText[n + 1] = '$';

  // allocate
  int_sa *SA = (int_sa *)malloc(n * sizeof(int_sa));
  for (int i = 0; i < n; i++)
    SA[i] = 0;
  int_sa *dc3_LCP = (int_lcp *)malloc(n * sizeof(int_lcp));

  double clk;
  if (TIME)
  {
    time_start(&t_start, &c_start);
    DC3(uText, n, SA, 255);
    clk = time_stop(t_start, c_start);
  }

  // sort
  if (TIME)
  {
    time_start(&t_start, &c_start);
  }
  DC3LCP(uText, n, SA, dc3_LCP, 255);
  if (TIME)
  {
    time_stop(t_start, c_start);
    printf("==DC3+LCP==\n");
  }

#if DEBUG
  DBGP(uText, n, "\t")
  DBGP(SA, n, "\t");
#endif

  // LCP
  int_t *LCP = (int_t *)malloc(n * sizeof(int_t));
  int_t *kas_LCP = (int_t *)malloc(n * sizeof(int_t));
  int_t *phi_LCP = (int_t *)malloc(n * sizeof(int_t));
  for (int i = 0; i < n; i++)
    LCP[i] = kas_LCP[i] = phi_LCP[i] = 0;

  if (TIME)
  {
    time_start(&t_start, &c_start);
  }
  // Naive_lcp(uText, n, SA, LCP);
  if (TIME)
  {
    printf("CLOCK = %lf\t", clk + time_stop(t_start, c_start));
    printf("==LCP O(n^2)==\n");
    time_start(&t_start, &c_start);
  }
  KASAI(uText, n, SA, kas_LCP);
  if (TIME)
  {
    printf("CLOCK = %lf\t", clk + time_stop(t_start, c_start));
    printf("==KASAI O(n)==\n");
  }
  PHI(uText, n, SA, phi_LCP);
  if (TIME)
  {
    printf("CLOCK = %lf\t", clk + time_stop(t_start, c_start));
    printf("==PHI O(n)==\n");
  }

  if (validate)
  {
    for (int i = 0, count = 0; i < n && count < 10; i++)
      if (kas_LCP[i] != phi_LCP[i] || dc3_LCP[i] != kas_LCP[i])
      {
        printf("ERROR!!\ni=%d\tkas=%d\tphi=%d\tdc3=%d\n", i, kas_LCP[i], phi_LCP[i], dc3_LCP[i]);
        count++;
#if DEBUG
        print(SA, uText, n, n, 1);
#endif
      }
    // for (int i = 0; i < n; i++)
    // {
    //   printf("[%d]\t[%d]\n", dc3_LCP[i], LCP[i]);
    // }
  }

  /**/
  // deallocate
  free(SA);
  free(LCP);
  free(kas_LCP);
  free(phi_LCP);
  if (argc == 2)
    free(Text);
  free(dc3_LCP);
  free(uText);

  return 0;
}

/****************/
