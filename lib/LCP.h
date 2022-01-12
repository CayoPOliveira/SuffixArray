#ifndef LCP_H
#define LCP_H

#define int_t int
#define uint_t unsigned int
#define int_sa uint_t
#define int_lcp uint_t
#define int_str uint_t

// DEBUG
int_t LCP_Compare(int_lcp *a, int_lcp *b, uint_t n);

//  LCP ===============================================
int LCP_Naive(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP);
int KASAI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP);
int PHI(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP);

#endif