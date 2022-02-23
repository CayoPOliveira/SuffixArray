#ifndef MM90_H
#define MM90_H

#define int_t int
#define uint_t unsigned int
#define int_sa uint_t
#define int_lcp uint_t
#define int_str uint_t

//  MM90 ===============================================
int *MM(int_str *Text, uint_t N, int_sa *SA);
int *MM_LCP(int_str *Text, uint_t N, int_sa *SA, int_lcp *LCP);

#endif