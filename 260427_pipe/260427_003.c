#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define READ 0
#define WRITE 1

volatile int pid = 0;
volatile int isExpired = 0;

void handler(int sig)
{
    isExpired = 1;
    kill(pid, SIGTERM);
}

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
        pid=f;
        signal(SIGALRM, handler);
        close(p[WRITE]);
        char buf[32];
        int bytes;
        alarm(5);
        while ((bytes = read(p[READ], buf, sizeof(buf) - 1)) > 0)
        {
            alarm(5); // reset timer
            buf[bytes] = '\0';
            int n = atoi(buf);
            s += n;
            printf("Somma parziale: %d\n", s);
        }
        alarm(0); // disattivo timer.
        if (isExpired)
            printf("[TIMEOUT] Somma parziale: %d\n", s);
        else
            printf("Somma totale: %d\n", s);
        wait(NULL);
        close(p[READ]);
    }
    return 0;
}