#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int DEBUG = 0;

//Função de comparação para o qsort
int comp_str(const void *a, const void *b)
{
    const char **stra = (const char **)a;
    const char **strb = (const char **)b;
    return strcmp(*stra, *strb);
}

//qsort return SA
int *Naive(char *String, int N, int *LCP)
{
    //      SA
    int *SA = (int *)malloc(N * sizeof(int));
    int i;

    char **suffixes = (char **)malloc(N * sizeof(char *));
    for (i = 0; i < N; i++)
    {
        suffixes[i] = (char *)malloc(N * sizeof(char));
        strcpy(suffixes[i], String + i);
    }

    qsort(suffixes, N, sizeof(char *), comp_str);

    for (i = 0; i < N; i++)
    {
        SA[i] = N - strlen(suffixes[i]);
        free(suffixes[i]);
    }
    free(suffixes);

    //      LCP

    LCP[0] = 0;
    for (i = 1; i < N; i++)
    {
        int lcp = 0;
        while (String[SA[i] + lcp] == String[SA[i - 1] + lcp])
        {
            lcp++;
        }
        LCP[i] = lcp;
    }

    //      DEBUG
    if (DEBUG)
    {
        printf("NAIVE Test\ni\tSA\tLCP\tSuffixes\n");
        for (i = 0; i < N; i++)
        {
            printf("%d\t%d\t%d\t%s\n", i, SA[i], LCP[i], String + SA[i]);
        }
        printf("\n");
    }

    return SA;
}

//Mambers & Myers sorting suffix array
int *MM90(char *String, int N, int *LCP)
{
    int i, j, k, H, saH, pos, aux;

    int *SA = (int *)malloc(N * sizeof(int));
    int *ISA = (int *)malloc(N * sizeof(int));   //ISA[SA[i]]==i
    int *Count = (int *)malloc(N * sizeof(int)); //Counter
    int *SB = (int *)malloc(N * sizeof(int));    //Start of Bucket
    bool *B = (bool *)malloc(N * sizeof(bool));  //Bucket

    int *SA2H = (int *)malloc(N * sizeof(int)); //stage H sort result
    int *SB2H = (int *)malloc(N * sizeof(int)); //Index

    for (int i = 0; i < N; i++)
    {
        SA[i] = ISA[i] = Count[i] = SB[i] = 0;
        B[i] = false;
    }

    //K=0//BUCKET SORT//////
    int bucket[256];
    for (i = 0; i < 256; i++)
        bucket[i] = 0;
    for (i = 0; i < N; i++)
    {
        bucket[(int)String[i]]++; //Count any char
    }
    aux = 0;
    for (i = 0; i < 256; i++)
        if (bucket[i] != 0) //char of string
        {
            aux += bucket[i];
            bucket[i] = aux - bucket[i]; //accumulated sum
            B[bucket[i]] = true;
        }
    for (i = 0; i < N; i++) //First char sort in SA
    {
        j = (int)String[i];
        SA[bucket[j]] = i;
        ISA[i] = bucket[j];
        bucket[j]++;

        //initiated
        SA2H[i] = Count[i] = SB[i] = 0;
        if (B[i])
            LCP[i] = 0;
        else
            LCP[i] = -1;
    }
    //////////////////////
    if (DEBUG)
    {
        printf("\nB\tSB\tSA\tISA\tLCP\tString\n");
        for (int i = 0; i < N; i++)
        {
            printf("%d\t", B[i]);
            printf("%d\t", SB[i]);
            printf("%d\t", SA[i]);
            printf("%d\t", ISA[i]);
            printf("%d\t", LCP[i]);
            printf("%s\n", String + SA[i]);
        }
    }
    ////////////////////
    for (H = 1; H < N; H *= 2)
    {
        ///Count number of buckets//
        int nbucket = 0;
        for (j = 0, i = 0; i < N; i++)
        {
            SB2H[i] = 0;
            Count[i] = 0;
            if (B[i]) //first suffix bucket
            {
                nbucket++;
                j = i;
            }
            //update start of bucket
            SB[i] = j;
        }
        if (nbucket == N)
            break; //Complete
        ////////////////////
        SA2H[0] = N - 1;
        for (i = 0; i < N; i++)
        {
            saH = SA[i] - H;
            if (saH < 0)
                continue;

            pos = SB[ISA[saH]] + Count[SB[ISA[saH]]];
            Count[SB[ISA[saH]]]++;
            //induce suffix
            SA2H[pos] = saH;
            ISA[saH] = pos;

            SB2H[pos] = SB[i]; //bucket id

            if (B[pos] == 0 && SB2H[pos] != SB2H[pos - 1]) //new bucket
            {
                B[pos] = 1;
                LCP[pos] = H;
            }
            else if (B[pos] == 0)
            {
                LCP[pos] = H;
            }
        }

        for (j = 0, i = 0; i < N; i++)
        {
            //update SA
            SA[i] = SA2H[i];
        }
        /////////////////////
        if (DEBUG)
        {
            printf("\nH=%d\nB\tSB\tSA\tISA\tLCP\tString\n", H);
            for (int i = 0; i < N; i++)
            {
                printf("%d\t", B[i]);
                printf("%d\t", SB[i]);
                printf("%d\t", SA[i]);
                printf("%d\t", ISA[i]);
                printf("%d\t", LCP[i]);
                printf("%s\n", String + SA[i]);
            }
        }
    }

    //final LCP
    for (i = 1; i < N; i++)
    {
        j = LCP[i] * 2;
        while (LCP[i] + 1 < j &&
               String[SA[i] + LCP[i]] == String[SA[i - 1] + LCP[i]]) //lcp(sa(i), sa(i-1))++
            LCP[i]++;
    }

    ////////////////////////////////////////////////////////////////
    free(ISA);
    free(Count);
    free(SB);
    free(B);
    free(SA2H);
    free(SB2H);

    return SA;
}

int main(int argc, char *argv[])
{
    char *String = (char *)malloc(13 * sizeof(char));
    strcpy(String, "mississippi$");
    if (argc == 2)
    {
        String = (char *)malloc((1 + strlen(argv[1])) * sizeof(char));
        strcpy(String, argv[1]);
        strcat(String, "$");
    }
    int N = strlen(String);

    //NAIVE & MM90
    int *LCP = (int *)malloc(N * sizeof(int));
    int *LCP_Naive = (int *)malloc(N * sizeof(int));

    int *SA_Naive = Naive(String, N, LCP_Naive);
    int *SA = MM90(String, N, LCP);

    //TEST
    printf("\n%s\n", String);
    int flag = 1;
    for (int i = 0; i < N; i++)
    {
        if (DEBUG)
            printf("%d\t%d\t%d\t%d\t%d\\n", i, SA_Naive[i], SA[i], LCP_Naive[i], LCP[i]);
        if (SA[i] != SA_Naive[i] || LCP_Naive[i] != LCP[i])
        {
            flag = 0;
            printf("\nsuffix %d\n", i);
            if (!DEBUG)
                break;
        }
    }
    if (flag)
        printf("GREAT!\n");
    else
        printf("THIS IS BAD!\n");

    //SAVE
    FILE *arq = fopen("SA.bin", "wb");
    fwrite(&N, sizeof(int), 1, arq);
    fwrite(String, sizeof(char), N, arq);
    fwrite(SA, sizeof(int), N, arq);
    fclose(arq);

    //< FREE >//
    free(LCP_Naive);
    free(SA_Naive);
    free(LCP);
    free(SA);
    free(String);

    return 0;
}