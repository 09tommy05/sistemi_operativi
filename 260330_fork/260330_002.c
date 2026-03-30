/*
Scrivere un programma che esegua due comandi distinti di sistema (‘whoami’ e ‘date’). Le
esecuzioni devono avvenire in modo indipendente e il programma deve attendere il
completamento di entrambe prima di terminare.
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
    int whoami = system("whoami");
    printf("\nCodice di ritorno whoami: %d\n\n", WEXITSTATUS(whoami));
    int date = system("date");
    printf("\nCodice di ritorno date: %d\n", WEXITSTATUS(date));
    return 0;
}