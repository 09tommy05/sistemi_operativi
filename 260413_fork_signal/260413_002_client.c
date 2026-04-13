/*
Scrivere due programmi distinti:
- Il primo programma deve comportarsi come un semplice server di notifiche: rimane in
esecuzione e, ogni volta che riceve un segnale da un processo esterno, stampa quale processo ha
inviato la richiesta.
- Il secondo programma deve comportarsi come un client, che invia una singola richiesta al
server specificando il PID del destinatario.
*/
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int pid=-1;
    printf("pid > ");
    scanf("%d", &pid);
    kill(pid, SIGALRM);
    return 0;
}