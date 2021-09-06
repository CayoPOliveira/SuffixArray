#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int int_t;
typedef unsigned int uint_t;

int validate = 1;

#define DEBUG 0

/****************/
int naive(int_t *SA, char *Text, int n);
int print(int *SA, uint_t *Text, int n, int m, int c);
/****************/

int radixPass(int_t *SA, uint_t *Text, int n, int sigma, int p){

  //count the number of symbols
  int_t *C = (int_t*) malloc(sigma*sizeof(int_t));
  for(int i=0; i<sigma; i++) C[i]=0;
  for(int i=0; i<n; i++) C[Text[SA[i]+p]]++;

  //bucket sorting
  int_t *B = (int_t*) malloc(sigma*sizeof(int_t));
  B[0] = 0;
  int_t tmp = C[0]; 
  for(int i=1; i<sigma; i++){ 
    B[i] = B[i-1]+tmp;
    tmp = C[i];
  }

  //radix pass
  int_t *tmp = (int_t*) malloc((n+2)*sizeof(int_t));//temporary array
  for(int i=0; i<n; i++) tmp[B[Text[SA[i]+p]]++] = SA[i];
  for(int i=0; i<n; i++) SA[i]=tmp[i];

  free(C);
  free(B);
  free(tmp);

return 0;
}

int DC3(int_t *SA, uint_t *Text, int n, int sigma){

  int n0=(n+2)/3, n1=(n+1)/3, n2=n/3;
  int n12 = n1+n2;
  int_t *SA12 = (int_t*) malloc((n12+1)*sizeof(int_t));

  int i12=0;
  for(int i=1; i<n; i++) if(i%3==1 || i%3==2) SA12[i12++] = i;//add suffixes i%3==1 and i%3==2

  #if DEBUG
    print(SA12, Text, n12, n, 0);
    printf("n0 = %d\n", n0); printf("n1 = %d\n", n1); printf("n2 = %d\n", n2); printf("n12 = %d\n", n12);
  #endif
  
  //LSD radix steps
  radixPass(SA12, Text, n12, sigma, 2);
  radixPass(SA12, Text, n12, sigma, 1);
  radixPass(SA12, Text, n12, sigma, 0);

  #if DEBUG
    printf("==\nRADIX\n==\n");
    print(SA12, Text, n12, n, 0);
  #endif

  //reduced text
  uint_t *TextU = (uint_t*) malloc((n12+3)*sizeof(uint_t));
  for(int i=0; i<n12+3; i++) TextU[i]=0;

  //middle position
  TextU[n1+1] = 0;
  uint_t name = 0, c0 = -1, c1 = -1, c2 = -1;
  //naming
  for(int i=0; i<n12; i++){
    int j=0;
    if(Text[SA12[i]] != c0 || Text[SA12[i]+1] != c1 || Text[SA12[i]+2] != c2){
      c0 = Text[SA12[i]]; c1 = Text[SA12[i]+1]; c2 = Text[SA12[i]+2];
      name++;
    }
    if(SA12[i]%3==1) TextU[SA12[i]/3] = name; //first half
    else TextU[SA12[i]/3 + (n1+1)] = name;    //second half
  }
  
  #if DEBUG
  for(int i=0; i<n12+1; i++)printf("%u ", TextU[i]);
  printf("\n");
  #endif

  //DC3 (recursive)
  if(name < n12){

    printf("====RECURSIVE====\n");
    #if DEBUG
      for(int i=0; i<n12+1; i++)printf("%d ", TextU[i]);
      printf("\n");
    #endif
    
    //recursive call
    DC3(SA12, TextU, n12+1, name+1);

    #if DEBUG
      printf("==\nBACK\n==\n");
      for(int i=0; i<n12+1; i++)printf("%d ", TextU[i]);
      printf("\n");
      print(SA12, TextU, n12+1, n12+1, 0);
    #endif

    //Map SA12 into SA (ignore first element of SA12, which is the middle 0)
    for(int i=0; i<n12; i++){
      if(SA12[i+1]<n1+1) SA12[i] = SA12[i+1]*3+1;  // i%3==1
      else SA12[i] = (SA12[i+1]-(n1+1))*3+2;  // i%3==2
    }

    #if DEBUG
      printf("==\nSA\n==\n");
      print(SA12, Text, n12, n, 1);
    #endif
  }

  //SA0
  int_t *SA0 = (int_t*) malloc(n0*sizeof(int_t));
  int i0=0;
  if(n%3==1) SA0[i0++]=n-1; //if last position is mod 0
  for(int i=0; i<n12; i++) if(SA12[i]%3==1) SA0[i0++]=SA12[i]-1;

  radixPass(SA0, Text, n0, sigma, 0);
  #if DEBUG
    print(SA0, Text, n0, n, 0);
  #endif

  //ISA
  int_t *ISA12 = (int_t*) malloc((n12+n0)*sizeof(int_t));
  for(int i=0; i<n12; i++) ISA12[SA12[i]] = i; 

  //merge SA12 and SA0
  int i=0; 
  i12=0; i0=0;
  while(i12<n12 && i0 < n0){

    if(Text[SA12[i12]] < Text[SA0[i0]])
      SA[i] = SA12[i12++];
    else if(Text[SA0[i0]] < Text[SA12[i12]])
      SA[i] = SA0[i0++];
    else{//first symbol is equal
      if(SA12[i12]%3==1){ //SA0 vs. SA1
        if(ISA12[SA12[i12]+1] < ISA12[SA0[i0]+1]) SA[i] = SA12[i12++];//use ISA12
        else SA[i] = SA0[i0++];
      }
      else{ //SA0 vs. SA2
        if(Text[SA12[i12]+1] < Text[SA0[i0]+1]) SA[i] = SA12[i12++];
        else if(Text[SA0[i0]+1] < Text[SA12[i12]+1]) SA[i] = SA0[i0++];
        else{//second symbol is (also) equal
          if(ISA12[SA12[i12]+2] < ISA12[SA0[i0]+2]) SA[i] = SA12[i12++];//use ISA12
          else SA[i] = SA0[i0++];
        }
      }
    }
    i++;
  }
  //remaining elements in SA0 and SA12
  while(i12<n12) SA[i++] = SA12[i12++];
  while(i0 < n0) SA[i++] = SA0[i0++];

  #if DEBUG
    printf("==\nSA (merged)\n==\n");
    print(SA, Text, n, n, 0);
  #endif

  free(SA0);
  free(ISA12);
  free(SA12);
  free(TextU);

  return 0;
}

/****************/

int main(int argc, char *argv[]){

  printf("sizeof(int_t) = %zu bytes\n", sizeof(int_t));
  unsigned char *Text;

  // intput data
  if(argc==2){
    Text = malloc((strlen(argv[1])+1)*sizeof(unsigned char));
    sscanf(argv[1], "%s", Text);	
  }
  else{
    //Text = "banaananaanana";
    //Text = "banana";
    Text = "mississippi";
  }	

  printf("Text = %s$\n", Text);
  int n = strlen(Text)+1;
  int i, j;

  // Text as int
  uint_t *uText = (uint_t*) malloc((n+2) * sizeof(uint_t)); 

  for(int i = 0; i < n; i++) uText[i] = Text[i];
  uText[n-1] = uText[n] = uText[n+1] =  '$';

  // allocate
  int_t *SA = (int_t *)malloc(n * sizeof(int_t));
  for(int i = 0; i < n; i++) SA[i]=0;

  // sort
  DC3(SA, uText, n, 255);
  #if DEBUG
    print(SA, uText, n, n, 1);
  #endif

  if(validate){
    int_t *SA2 = (int_t *)malloc(n * sizeof(int_t));
    naive(SA2, Text, n);

    for(int i=0; i<n; i++)
      if(SA[i]!=SA2[i]){
        printf("ERROR!!\n");
        #if DEBUG
          print(SA2, uText, n, n, 1);
        #endif
        break;
      }
    free(SA2);
  }

  /**/
  // deallocate
  free(SA);
  if(argc==2) free(Text);
  free(uText);

  return 0;
}


/****************/

typedef struct{
  char *suffix;
  int   pos;
} t_suffix;

int cmp(const void *p1, const void *p2) { 
  const t_suffix *a = p1;
  const t_suffix *b = p2;
  return strcmp(a->suffix, b->suffix) < 0? 0 : 1; 
}

int naive(int_t *SA, char *Text, int n){

  t_suffix *tmp = (t_suffix*) malloc(n*sizeof(t_suffix));

  for(int i=0; i<n; i++){
    tmp[i].suffix = &Text[i];
    tmp[i].pos = i;
  }

  qsort(tmp, n, sizeof(t_suffix), cmp);

  for(int i=0; i<n; i++)
    SA[i] = tmp[i].pos;

  free(tmp);

  return 0;
}

/****************/

int print(int *SA, uint_t *Text, int n, int m, int c){

  // output
  printf("i\tSA\tsuffixes\n");
  for(int i = 0; i < n; ++i) {
    //if(i%3==0) continue;
    printf("%d\t%d\t",i, SA[i]);
    for(int j = SA[i]; j < m; j++) {
      if(c==1)
        printf("%c", Text[j]);
      else
        printf("[%d]", Text[j]);
    }
    printf("\n");
    //printf("$$$\n");
  }

  return 0;
}

/****************/
