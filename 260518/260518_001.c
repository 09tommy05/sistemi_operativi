#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <n_children>\n", argv[0]);
        exit(1);
    }
    int n_children = atoi(argv[1]);
    int **pipes_in = malloc(sizeof(int *) * n_children);
    int **pipes_out = malloc(sizeof(int *) * n_children);
    int *pids = malloc(sizeof(int) * n_children);
    printf("Creating %d processes\n", n_children);
    for (int i = 0; i < n_children; i++)
    {
        pipes_in[i] = malloc(sizeof(int) * 2);
        pipes_out[i] = malloc(sizeof(int) * 2);
        pipe(pipes_in[i]);
        pipe(pipes_out[i]);
        int child = fork();
        if (child == -1)
        {
            printf("Error creating child\n");
        }
        else if (child == 0)
        {
            srand(time(NULL));
            char buf[255];
            while (read(pipes_in[i][READ], buf, 255) > 0)
            {
                switch (buf[0])
                {
                case 'q':
                    close(pipes_in[i][READ]);
                    close(pipes_out[i][READ]);
                    close(pipes_in[i][WRITE]);
                    close(pipes_out[i][WRITE]);
                    exit(0);
                case 'i':
                    int pid = getpid();
                    printf("Child %d sending own pid....\n", (i + 1));
                    write(pipes_out[i][WRITE], &pid, sizeof(int));
                    break;
                case 'r':
                    int r = rand();
                    printf("Child %d computing random....\n", (i + 1));
                    write(pipes_out[i][WRITE], &r, sizeof(int));
                    break;
                default:
                    break;
                }
            }
            exit(0);
        }
        else
        {
            pids[i] = child;
        }
    }
    char command[255];
    do
    {
        printf("Next command: ");
        fgets(command, sizeof(command), stdin);
        if (command[0] != 'r' && command[0] != 'i' && command[0] != 'q')
        {
            printf("Wrong parameter. Allowed are 'r' and 'i' and 'q'\n");
            continue;
        }
        else if (command[0] == 'q')
        {
            for (int i = 0; i < n_children; i++)
                write(pipes_in[i][WRITE], command, 255);
        }
        else
        {
            char num[255];
            int i;
            for (i = 1; command[i] != '\0'; i++)
            {
                num[i - 1] = command[i];
            }
            num[i - 1] = '\0';
            int n = atoi(num);
            if (n > n_children || n < 1)
            {
                printf("Wrong target\n");
                continue;
            }
            write(pipes_in[n - 1][WRITE], command, 255);
            num[0] = '\0';
            int res;
            read(pipes_out[n - 1][READ], &res, sizeof(int));
            printf("Child %d told me: '%d'\n", pids[n - 1], res);
        }
    } while (command[0] != 'q');

    for (int i = 0; i < n_children; i++)
    {
        free(pipes_in[i]);
        free(pipes_out[i]);
    }
    free(pipes_in);
    free(pipes_out);
    free(pids);
    return 0;
}