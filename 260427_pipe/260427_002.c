/*
Creare un programma che prenda come argomento 'n' il numero di figli da
generare. Ogni figlio creato comunicherà al genitore (tramite pipe) un numero
casuale e il genitore calcolerà la somma di tutti questi numeri, inviandola a
stdout.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define READ 0
#define WRITE 1

int *pid = NULL;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: usage %s <n>\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    int (*p)[2] = malloc(N * sizeof(*p));
    pid = malloc(N * sizeof(int));
    int s = 0;
    for (int i = 0; i < N; i++)
    {
        if (pipe(p[i]) == -1)
        {
            perror("Pipe");
            return 1;
        }
        int f = fork();
        if (f == -1)
        {
            perror("Errore");
            return 2;
        }
        else if (f == 0)
        {
            pid[i] = getpid();
            for (int j = 0; j < N; j++)
            {
                if (j != i)
                {
                    close(p[j][READ]);
                    close(p[j][WRITE]);
                }
            }
            close(p[i][READ]);
            srand(time(NULL) & getpid());
            int r = rand() % 100;
            char buf[31];
            int len = snprintf(buf, sizeof(buf), "%d", r);
            write(p[i][WRITE], buf, len);
            printf("%d> ho inviato %s\n", getpid(), buf);
            close(p[i][WRITE]);
            exit(0);
        }
        else
        {
            close(p[i][WRITE]);
            char buf[31];
            waitpid(pid[i],NULL,0);
            int r = read(p[i][READ], buf, 31);
            int num = atoi(buf);
            s += num;
            close(p[i][READ]);
        }
    }

    printf("somma= %d\n", s);
    free(p);
    free(pid);
    return 0;
}