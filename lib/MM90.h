#ifndef MM90_H
#define MM90_H

#define int_t int
#define uint_t unsigned int
#define int_sa int
#define int_lcp int
#define int_str unsigned int

//  MM90 ===============================================
int *MM(int_str *Text, uint_t N, int_sa *SA);
int *MMLCP(int_str *Text, uint_t N, int_sa *SA, int_lcp *LCP);

#endif