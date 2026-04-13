/*
Scrivere un programma che crea un processo figlio. Il processo figlio deve rimanere in attesa di una
notifica esterna per terminare. Il processo padre deve:
- creare il figlio
- attendere un intervallo di tempo simulando delle operazioni interne
- inviare al figlio un segnale che ne autorizza la terminazione
Il figlio, alla ricezione della notifica, deve terminare la propria esecuzione.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig, siginfo_t *info, void *context)
{
    printf("[CHILD] signal recieved.\n");
    printf("[CHILD] Killing myself.\n");
    exit(0);
}
int main()
{
    int child = fork();
    if (child == 0)
    {
        printf("[CHILD] Child created.\n");
        printf("[CHILD] I'm alive.\n");
        struct sigaction sa;
        sa.sa_sigaction = handler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGUSR1, &sa, NULL);
        while (1)
        {
            pause();
        }
    }
    else
    {
        printf("[PARENT] executing operations...\n");
        sleep(5);
        printf("[PARENT] ended executing operations\n");
        printf("[PARENT] Killing child\n");
        kill(child, SIGUSR1);
        printf("[PARENT] Child killed\n");
    }
    return 0;
}