#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main()
{
    int p[2];
    if (pipe(p) == -1)
    {
        perror("Pipe");
        return 1;
    }
    int s = 0;
    int f = fork();
    if (f == -1)
    {
        perror("Fork");
        return 1;
    }
    else if (f == 0)
    {
        close(p[READ]);
        int n = 0;

        printf("> ");
        while (scanf("%d", &n) != EOF)
        {
            char buf[32];
            sprintf(buf, "%d", n);
            write(p[WRITE], buf, strlen(buf));
        }
        close(p[WRITE]);
        exit(0);
    }
    else
    {
        close(p[WRITE]);
        char buf[32];
        int bytes;
        while ((bytes = read(p[READ], buf, sizeof(buf) - 1)) > 0)
        {
            buf[bytes] = '\0'; // terminatore stringa
            int n = atoi(buf);
            s += n;
            printf("Somma parziale: %d\n", s);
        }
        wait(NULL);
        close(p[READ]);
    }
    printf("Somma totale: %d\n", s);
    return 0;
}