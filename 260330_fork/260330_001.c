/*
Scrivere un programma che esegua un comando di sistema per mostrare il contenuto di una
directory in formato dettagliato. Il programma deve attendere il completamento dell’operazione
e stampare il codice di terminazione del comando.
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
    int outcome = system("ls -la");
    printf("\nCodice di ritorno: %d\n", WEXITSTATUS(outcome));
    return 0;
}