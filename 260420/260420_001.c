/*
Contare SIGUSR1 e SIGUSR2 ricevuti dall'eseterno con ctr distinti. Il numero massimo di mittenti deve essere gestibile impostato con un  #define.
* All'avvio deve mostrare il proprio PID
* Alla ricezione di uno dei due seganli deve mostrare un feedback con il codice del segnale ricevuto e il riferimento del mittente.
* Alla ricezione di un segnale SIGINT o SIGTERM il programma deve terminare mostrando un report dei conteggi suddiviso per mittente, ad esempio un elenco con PID mittente, numero SIGUSR1 ricevuti e SIGUSR1
*/
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define MAX 60
#define COL 3

typedef struct Counter
{
    int pid;
    int usr1;
    int usr2;
} Counter;

Counter c[MAX];

void sigHandler(int sig, siginfo_t *info, void *context);
void endPrg(int sig);
void init();
int is_in_counter(int);
int main()
{
    printf("PID --> %d\n", getpid());
    init();
    struct sigaction sa;
    sa.sa_sigaction = sigHandler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    signal(SIGINT, endPrg);
    signal(SIGTERM, endPrg);
    while (1)
    {
        pause();
    }
}

void init()
{
    int i = 0;
    for (i = 0; i < MAX; i++)
    {
        c[i].pid = -1;
        c[i].usr1 = 0;
        c[i].usr2 = 0;
    }
}

void sigHandler(int sig, siginfo_t *info, void *context)
{
    if (sig == SIGUSR1)
    {
        int res = is_in_counter(info->si_pid);
        if (res > -1)
        {
            c[res].pid = info->si_pid;
            c[res].usr1++;
        }
        printf("Ricevuto SIGUSR1 (%d), da %d\n", sig, info->si_pid);
    }
    else if (sig == SIGUSR2)
    {
        int res = is_in_counter(info->si_pid);
        if (res > -1)
        {
            c[res].pid = info->si_pid;
            c[res].usr2++;
        }
        printf("Ricevuto SIGUSR1 (%d), da %d\n", sig, info->si_pid);
    }
}

void endPrg(int sig)
{
    int i = 0;
    while (i < MAX && c[i].pid != -1)
    {
        printf("pid: %d, %d, %d\n", c[i].pid, c[i].usr1, c[i].usr2);
        ++i;
    }
    exit(0);
}

int is_in_counter(int pid)
{
    int i = 0;
    int res = -1;
    while (i < MAX && c[i].pid != -1 && res == -1)
    {
        if (pid == c[i].pid)
            res = i;
        ++i;
    }
    if (i < MAX && res==-1)
    {
        res = i;
    }
    return res;
}