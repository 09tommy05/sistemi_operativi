/*
Realizzare funzioni per stringhe char *stringrev(char * str) (inverte ordine
caratteri) e int stringpos(char * str, char chr) (cerca chr in str e restituisce la
posizione)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *stringrev(char * s);
int stringpos(char * str, char chr);

int main(int argc, char **argv){
    if(argc < 2){
        printf("Inserisci una stringa come argomento.\n");
        return 1;
    }
    char *rev=stringrev(argv[1]);
    printf("reverse= %s\n",rev);
    if(argc > 2){
        int pos= stringpos(argv[1], argv[2][0]);
        if(pos==-1){
            printf("Non e' stato possibile trovare '%c' in \"%s\"\n", argv[2][0], argv[1]);
        }else{
            printf("Il carattere '%c' in \"%s\" si trova in posizione %d\n",argv[2][0], argv[1], pos);
        }
    }
    free(rev);
    return 0;
}

char *stringrev(char * s){
    int len= strlen(s);
    char *res;
    res=(char *)malloc(sizeof(s)*(len+1));
    int i=0;
    res[len]='\0';
    for(i=0;i<len;i++)
        res[len-i-1]=s[i]; 
    return res;
}

int stringpos(char * str, char chr){
    int pos=-1;
    int i=0;
    for(i=0;str[i]!='\0' && pos==-1;i++){
        if(str[i]==chr){
            pos=i;
        }
    }
    return pos;
}