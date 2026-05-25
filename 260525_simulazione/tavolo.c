#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>
#include "gioco.h"

#define READ 0
#define WRITE 1

void sort(int *);
void sort_pipes_by_pids(int *, int **piepes, int);
int getMaxIdx(int *, int);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: %s <gameSettings> <checkerPID>\n", argv[0]);
        return 40;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        printf("File %s doesn't exists\n", argv[1]);
        return 40;
    }

    // controllo che il PID esista
    if (kill(atoi(argv[2]), 0) == -1)
    {
        printf("PID %s doesn't exists\n", argv[2]);
        return 40;
    }

    // prendo <n> dal file
    char buf[32];
    int n = 0;
    int bytesRead = read(fd, buf, sizeof(buf));
    buf[bytesRead] = 0;
    n = atoi(buf);
    if (n < 2 || n > MAX_PLAYERS)
    {
        printf("<n> must be between 2 and %d.\n", MAX_PLAYERS);
        return 1;
    }
    int **pipes = malloc(sizeof(int *) * n);
    int *pids = malloc(sizeof(int) * n);
    key_t key = ftok(argv[0], n);
    int queueId = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        pipe(pipes[i]);
        int f = fork();
        if (f < 0)
        {
            printf("Error creating child %d\n", i);
            return 1;
        }
        else if (f == 0)
        {
            close(pipes[i][READ]);
            player_main(pipes[i][WRITE], atoi(argv[2]), argv[1], queueId);
            exit(0);
        }
        else
        {
            close(pipes[i][WRITE]);
            pids[i] = f;
            // creo la mano di carte da mandare al figlio
            hand_t mano;
            mano.mtype = f;
            for (int j = 0; j < NUM_CARDS; j++)
                mano.cards[j] = (rand() % 9) + 1;
            sort(mano.cards);
            // mando la mano di carte al figlio tramite queue
            int esito = msgsnd(queueId, &mano, sizeof(mano.cards), 0);
        }
    }
    sort_pipes_by_pids(pids, pipes, n);
    // numero di match
    int *cards = malloc(sizeof(int) * n);
    int max;
    for (int i = 0; i < NUM_CARDS; i++)
    {
        printf("[TAVOLO] Mano %d\n\n", (i + 1));
        for (int j = 0; j < n; j++)
        {
            kill(pids[j], SIGUSR1);
            int end = read(pipes[j][READ], buf, sizeof(buf));
            buf[end] = 0;
            cards[j] = atoi(buf);
            printf("[TAVOLO] %d ha inviato %d\n", pids[j], cards[j]);
        }
        // prendi il max
        max = getMaxIdx(cards, n);
        fprintf(stderr, "[TAVOLO] ha vinto %d con la carta %d\n", pids[max], cards[max]);
        kill(pids[max], SIGUSR2);
    }
    union sigval value;
    for (int i = 0; i < n; i++)
    {
        if(max==i)
            value.sival_int = 1;
        else
            value.sival_int = 0;
        sigqueue(pids[i], SIGTERM, value);
    }
    for (int i = 0; i < n; i++)
    {
        close(pipes[i][WRITE]);
        close(pipes[i][READ]);
        free(pipes[i]);
    }
    msgctl(queueId, IPC_RMID,NULL);
    free(pipes);
    free(pids);
    free(cards);
    close(fd);
    return 0;
}

void sort_pipes_by_pids(int *pids, int **pipes, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (pids[i] > pids[j])
            {
                int temp = pids[i];
                pids[i] = pids[j];
                pids[j] = temp;
                temp = pipes[i][0];
                int temp1 = pipes[i][1];
                pipes[i][0] = pipes[j][0];
                pipes[i][1] = pipes[j][1];
                pipes[j][0] = temp;
                pipes[j][1] = temp1;
            }
        }
    }
}

void sort(int *v)
{
    for (int i = 0; i < NUM_CARDS - 1; i++)
    {
        for (int j = i + 1; j < NUM_CARDS; j++)
        {
            if (v[i] > v[j])
            {
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }
}

int getMaxIdx(int *v, int n)
{
    int max = 0;
    for (int i = 1; i < n; i++)
    {
        //>= perché ciò implica che anche il pid sia maggiore quello dopo perché sono stati ordinati, quindi se sono uguali prendo quello con pid più alto.
        if (v[i] >= v[max])
        {
            max = i;
        }
    }
    return max;
}