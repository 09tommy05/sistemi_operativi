/*
Scrivere un programma che, stampi il proprio PID e rimanga in esecuzione in attesa di eventi
esterni. Quando il processo riceve un determinato segnale definito dall’utente, deve aggiornare
un contatore interno e stampare a video quante volte tale segnale è stato ricevuto fino a quel
momento.
Il programma non deve occupare inutilmente la CPU durante l’attesa.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int ctr = 0;

void myHandler(int sigNum)
{
    ++ctr;
    printf("Il segnale e' stato ricevuto %d volte\n", ctr); // CI VUOLE IL \n, ODIO SIGNAL.
}

int main()
{
    printf("%d\n", getpid());
    int sig=-1;
    printf("0 - SIGCHLD\n");
    printf("1 - SIGCONT\n");
    printf("2 - SIGINT\n");
    printf("3 - SIGSYS\n");
    printf("4 - SIGTERM\n");
    printf("5 - SIGUSR1\n");
    printf("6 - SIGTSTP\n");
    printf("7 - SIGALRM\n");
    printf("> ");
    scanf("%d",&sig);
    switch(sig){
        case 0:
            signal(SIGCHLD, myHandler);
            break;
        case 1:
            signal(SIGCONT, myHandler);
            break;
        case 2:
            signal(SIGINT, myHandler);
            break;
        case 3:
            signal(SIGSYS, myHandler);
            break;
        case 4:
            signal(SIGTERM, myHandler);
            break;
        case 5:
            signal(SIGUSR1, myHandler);
            break;
        case 6:
            signal(SIGTSTP, myHandler);
            break;
        case 7:
            signal(SIGALRM, myHandler);
            break;
        default:
            printf("Opzione non valida");
            return 2;
    }
    while (1)
    {
        pause();
    }
}