#include <stdio.h>
#include <stdlib.h>
#include "stats.h"

int my_strlen(char *);
int is_palindromo(char *,int);
int vocali(char *);

int main (int argc, char **argv){
    if(argc==1){
        printf("No args, use: %s args\n", argv[0]);
        return 1;
    }
    int i=1;
    WordStats *ws = malloc(sizeof(WordStats)*(argc-1));
    for(i=1;i<argc;i++){
        #ifdef DEBUG
        printf("[DEBUG] Sto processando la stringa: \"%s\"\n", argv[i]);
        #endif
        analyze(argv[i],&ws[i-1]);
        #ifdef DEBUG
        printf("[DEBUG] Ho finito di processare la stringa: \"%s\"\n", argv[i]);
        #endif
        printf("%s -> len:%d, palindromo:%s, vocali:%d\n",ws[i-1].word,ws[i-1].length, ws[i-1].palindrome? "si" : "no", ws[i-1].vowels);
    }
    free(ws);
    return 0;
}