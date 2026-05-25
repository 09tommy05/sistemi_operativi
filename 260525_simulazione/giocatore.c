#define _POSIX_C_SOURCE 200809L
#include "gioco.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>

static volatile int wins = 0;
static volatile int lower = 0;
static volatile int fd;
static int cards[NUM_CARDS];
static char settings_path_glob[127];

void turnHandler(int sig, siginfo_t *si, void *ucontext)
{
    char buf[16];
    int len = snprintf(buf, sizeof(buf), "%d", cards[lower]);
    write(fd, buf, len);
    ++lower;
}

void winHandler(int sig, siginfo_t *si, void *ucontext)
{
    ++wins;
}

void endHandler(int sig, siginfo_t *si, void *ucontext)
{
    if (si->si_value.sival_int == 1)
    {
        int fd = open(settings_path_glob, O_WRONLY);
        char buf[32];
        int len = sprintf(buf, "%d - %d", getpid(), wins);
        write(fd, buf, len);
        printf("[GIOCATORE %d] ho vinto con %d vittorie\n", getpid(), wins);
        close(fd);
    }
    exit(0);
}

void player_main(int pipe_write_fd, pid_t checker_pid, const char *settings_path, int queue_id)
{
    fd = pipe_write_fd;
    hand_t mano;
    msgrcv(queue_id, &mano, sizeof(mano.cards), getpid(), 0);
    for (int i = 0; i < NUM_CARDS; i++)
    {
        cards[i] = mano.cards[i];
    }
    strcpy(settings_path_glob, settings_path);
    struct sigaction sa_turn, sa_win, sa_end;
    sa_turn.sa_sigaction = turnHandler;
    sa_win.sa_sigaction = winHandler;
    sa_end.sa_sigaction = endHandler;
    sa_end.sa_flags = sa_win.sa_flags = sa_turn.sa_flags = SA_SIGINFO;
    sigemptyset(&sa_turn.sa_mask);
    sigemptyset(&sa_win.sa_mask);
    sigemptyset(&sa_end.sa_mask);
    sigaction(SIGUSR1, &sa_turn, NULL);
    sigaction(SIGUSR2, &sa_win, NULL);
    sigaction(SIGTERM, &sa_end, NULL);
    printf("[GIOCATORE %d] Le mie carte sono: ", getpid());
    for (int i = 0; i < NUM_CARDS; i++)
    {
        printf("%d ", mano.cards[i]);
    }
    printf("\n");
    union sigval value;
    value.sival_int = mano.cards[NUM_CARDS - 1];
    sigqueue(checker_pid, SIGRTMIN, value);
    while (1)
    {
        pause();
    }
}