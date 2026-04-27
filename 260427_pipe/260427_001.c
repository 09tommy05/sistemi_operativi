/*
Impostare una comunicazione bidirezionale tra due processi con due livelli di
complessità:
○ Alternando almeno due scambi (P1 → P2, P2 → P1, P1 → P2, P2 → P1)
○ Estendendo il caso a mo’ di “ping-pong”, fino a un messaggio convenzionale
di “fine comunicazione”
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define READ 0
#define WRITE 1

int main()
{
    int pipe1[2], pipe2[2];
    char buf[50];
    pipe(pipe1);
    pipe(pipe2);
    int p2 = !fork();
    if (p2)
    {
        close(pipe1[WRITE]);
        close(pipe2[READ]);
        for (int i = 0; i < 4; i++)
        {
            int r = read(pipe1[READ], &buf, 50); // Read from pipe
            if (strcmp(buf, "FIN") == 0)
            {
                printf("P2 received: ‘%s’\n", buf);
                for(int j=0;j<3;j++){
                    write(pipe2[WRITE], "PONG",5);
                    sleep(1);
                }
                write(pipe2[WRITE], "FIN", 4); // Writes to pipe
                break;
            }
            printf("P2 received: ‘%s’\n", buf);
            write(pipe2[WRITE], "PONG", 5); // Writes to pipe
        }
        close(pipe1[READ]);
        close(pipe2[WRITE]);
    }
    else
    {
        close(pipe1[READ]);
        close(pipe2[WRITE]);

        for (int i = 0; i < 3; i++)
        {
            write(pipe1[WRITE], "PING", 5); // Writes to pipe
            int r = read(pipe2[READ], &buf, 50);    // Read from pipe
            printf("P1 received: ‘%s’\n", buf);
        }

        write(pipe1[WRITE], "FIN", 4); // Writes to pipe
        close(pipe1[WRITE]);
        do{
            int r = read(pipe2[READ], &buf, 50);
            printf("P1 received: ‘%s’\n", buf);
        }while(strcmp("FIN",buf)!=0);
        close(pipe2[READ]);
    }
    while (wait(NULL) > 0);
    return 0;
}