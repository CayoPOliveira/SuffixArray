#ifndef NAIVE_H
#define NAIVE_H

#define int_t int
#define uint_t unsigned int
#define int_sa int
#define int_lcp int
#define int_str unsigned int

//  NAIVE ===============================================
typedef struct
{
  char *suffix;
  int_sa pos;
} t_suffix;

int comp_str(const void *a, const void *b);
int *Naive(char *Text, uint_t n, int_sa *SA);

#endif