/*
Scrivere due programmi distinti:
- Il primo programma deve comportarsi come un semplice server di notifiche: rimane in
esecuzione e, ogni volta che riceve un segnale da un processo esterno, stampa quale processo ha
inviato la richiesta.
- Il secondo programma deve comportarsi come un client, che invia una singola richiesta al
server specificando il PID del destinatario.
*/
#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void myHandler(int sig, siginfo_t * info, void * context){
    printf("%d\n",info->si_pid);
}

int main(){
    printf("pid= %d\n",getpid());
    struct sigaction sa;
    sa.sa_sigaction = myHandler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM,&sa,NULL);
    while(1){
        pause();
    }
}