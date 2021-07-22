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

//Ordena os sufixos e cria os vetores SA e LCP
int *Naive(char *String, int N)
{

    int *SA = (int *)malloc(N * sizeof(int));
    int i = 0;

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

    return SA;
}

int *MM90(char *String, int N)
{
    int i, j, k, H, saH, pos, aux;

    int *SA = (int *)malloc(N * sizeof(int));
    int *ISA = (int *)malloc(N * sizeof(int));   //ISA[SA[i]]==i
    int *Count = (int *)malloc(N * sizeof(int)); //Counter
    int *SB = (int *)malloc(N * sizeof(int));    //Start of Bucket
    bool *B = (bool *)malloc(N * sizeof(bool));  //Bucket

    int *SA2H = (int *)malloc(N * sizeof(int));
    int *SB2H = (int *)malloc(N * sizeof(int));

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
        bucket[(int)String[i]]++; //Conta cada letra
    }
    aux = 0;
    for (i = 0; i < 256; i++)
        if (bucket[i] != 0) //Letra da String
        {
            aux += bucket[i];
            bucket[i] = aux - bucket[i]; //Posição inicial no vetor SA de acordo com o bucket
            B[bucket[i]] = true;
        }
    for (i = 0; i < N; i++) //SA organizado pelo primeiro caracter
    {
        j = (int)String[i];
        SA[bucket[j]] = i;
        ISA[i] = bucket[j];
        bucket[j]++;
        Count[i] = SB[i] = 0;
        SA2H[i] = 0;
    }
    //////////////////////
    if (DEBUG)
    {
        printf("\nBH\tSOB\tSA\tISA\n");
        for (int i = 0; i < N; i++)
        {
            printf("%d\t", B[i]);
            printf("%d\t", SB[i]);
            printf("%d\t", SA[i]);
            printf("%d\n", ISA[i]);
        }
    }
    ////////////////////
    for (H = 1; H < N; H *= 2)
    {
        ///ORGANIZA BUCKETS//
        int nbucket = 0;
        for (j = 0, i = 0; i < N; i++)
        {
            SB2H[i] = 0;
            Count[i] = 0;
            if (B[i])
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

            if (B[pos] == 0 && SB2H[pos] != SB2H[pos - 1])
                B[pos] = 1;
        }

        for (j = 0, i = 0; i < N; i++)
        {
            //update SA
            SA[i] = SA2H[i];
        }
        /////////////////////
        if (DEBUG)
        {
            printf("\nH=%d\nBH\tSOB\tSA\tISA\n", H);
            for (int i = 0; i < N; i++)
            {
                printf("%d\t", B[i]);
                printf("%d\t", SB[i]);
                printf("%d\t", SA[i]);
                printf("%d\n", ISA[i]);
            }
        }
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

    int *SA_MM90 = MM90(String, N);
    int *SA_Naive = Naive(String, N);

    //TESTES
    //*
    printf("\n%s\n", String);
    int flag = 1;
    for (int i = 0; i < N; i++)
    {
        printf("%d\t%d\n", SA_Naive[i], SA_MM90[i]);
        if (SA_MM90[i] != SA_Naive[i])
            flag = 0;
    }
    if (flag)
        printf("GREAT!\n");
    else
        printf("THIS IS BAD!\n");

    //SALVA AS INFORMAÇÕES EM UM ARQUIVO
    FILE *arq = fopen("SA.bin", "wb");
    fwrite(&N, sizeof(int), 1, arq);
    fwrite(String, sizeof(char), N, arq);
    fwrite(SA_MM90, sizeof(int), N, arq);
    fclose(arq);

    //< LIBERA MEMÓRIA ALOCADA >//
    free(SA_MM90);
    free(SA_Naive);
    free(String);

    return 0;
}