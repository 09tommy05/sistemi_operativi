/*
Scrivere una funzione che permetta di eseguire un comando passato come stringa, attendere il
completamento e restituire il codice di terminazione. La funzione deve essere poi utilizzata
all’interno di un programma di test.
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int exec_comm(char *);

int main(){
    int whoami = exec_comm("whoami");
    printf("\nCodice ritorno: %d\n",whoami);
    return 0;
}

int exec_comm(char *command){
    int outcome=system(command);     
    return WEXITSTATUS(outcome);
}