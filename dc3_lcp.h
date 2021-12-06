#ifndef DC3_LCP_H
#define DC3_LCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define int_t int
#define uint_t unsigned int
#define int_sa int
#define int_lcp int
#define int_str unsigned int

#define DEBUG 0
#define true 1
#define false 0

int min(int a, int b);
//  Debug Print ===============================================
int DBGP(int_t *v, uint_t n, char c);
// Min SegTree ========================================
int_lcp *segtreeBuild(int V[], int n);
int_lcp segtreeQuery(int tree[], int n, int a, int b);

//  NAIVE ===============================================
typedef struct
{
  char *suffix;
  int_sa pos;
} t_suffix;

int comp_str(const void *a, const void *b);
// Ordena os sufixos e cria os vetores SA e LCP
int *Naive(char *Text, uint_t n, int_sa *SA);

//  MM90 ===============================================
int *MM(int_str *Text, uint_t N, int_sa *SA);
int *MMLCP(int_str *Text, uint_t N, int_sa *SA, int_lcp *LCP);

//  DC3 ===============================================
int radixPass(int_str *Text, uint_t n, int_sa *SA, int sigma, int p);
int DC3(int_str *Text, uint_t n, int_sa *SA, int_t sigma);
int DC3LCP(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP, int_t sigma);

//  LCP ===============================================
int *Naive_lcp(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP);
int *KASAI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP);
int *PHI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP);
int lcp_compare(int_lcp *a, int_lcp *b, uint_t n);

#endif