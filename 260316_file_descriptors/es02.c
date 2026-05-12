/*
Scrivere un’applicazione che definisce una lista di argomenti validi e legge quelli passati
alla chiamata verificandoli e memorizzando le opzioni corrette, restituendo un errore in
caso di un’opzione non valida.
*/

#include <stdio.h>
#include <string.h>
#define DIM 3

char *args[] = {"-a", "-b", "-c"};

int check_args(char **, int, char[DIM][3]);
int isInOpt(char *, char[][3], int);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Nessun argomento passato\n");
        return 1;
    }
    char opt[DIM][3];
    int i = 0;
    for (i = 0; i < DIM; i++)
        opt[i][0] = '\0';
    int is_ok = check_args(argv, argc, opt);
    if (is_ok == 1)
    {
        printf("Gli argomenti sono stati passati correttamente:\n");
        for (i = 0; i < DIM; i++)
        {
            printf("%s ", opt[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Argomenti non validi.");
    }
    return 0;
}

int check_args(char **argv, int argc, char opt[DIM][3])
{
    int flag = 0;
    int i = 0, j = 0;
    int k = 0;
    for (i = 1; i < argc && flag != 1; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            if (strcmp(argv[i], args[j]) == 0)
            {
                flag=1;
                if (isInOpt(argv[i], opt, k) == 0)
                {
                    strcpy(opt[k], argv[i]);
                    ++k;
                }
            }
        }
        if(flag==1 && (i+1) < argc){
            flag=0;
        }
    }
    return flag;
}

int isInOpt(char *s, char arr[DIM][3], int k)
{
    int flag = 0;
    int i, j;
    for (i = 0; i <= k && !flag; i++)
    {
        if (strcmp(s, arr[i]) == 0)
        {
            flag = 1;
        }
    }
    return flag;
}