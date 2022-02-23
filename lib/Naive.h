#ifndef NAIVE_H
#define NAIVE_H

#define int_t int
#define uint_t unsigned int
#define int_sa uint_t
#define int_lcp uint_t
#define int_str uint_t

// DEBUG
int SA_Compare(int_lcp *a, int_lcp *b, uint_t n);
void print(int_sa *SA, int_lcp *LCP, int_str *uT, int n);

//  NAIVE ===============================================
typedef struct
{
  char *suffix;
  int_sa pos;
} t_suffix;

int comp_str(const void *a, const void *b);
int *Naive_sa(char *Text, uint_t n, int_sa *SA);

#endif