#ifndef DC3_H
#define DC3_H

#define int_t int
#define uint_t unsigned int
#define int_sa uint_t
#define int_lcp uint_t
#define int_str uint_t

#define DEBUG 0
#define example 0

//====================================================
#define min(a, b) ((a) < (b) ? (a) : (b))

// Min SegTree ========================================
int_lcp *segtreeBuild(int_lcp V[], int_t n);
int_lcp segtreeQuery(int_lcp tree[], int_t n, uint_t a, uint_t b);

//  DC3 ===============================================
int radixPass(int_str *Text, uint_t n, int_sa *SA, int sigma, int p);
int DC3(int_str *Text, uint_t n, int_sa *SA, int_t sigma);
int DC3_LCP(int_str *Text, uint_t n, int_sa *SA, int_lcp *LCP, int_t sigma);

#endif