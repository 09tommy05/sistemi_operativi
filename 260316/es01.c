#include <stdio.h>

int slen(char *);

int main(int argc, char **argv){
    if(argc <= 1){
        printf("Manca l'argomento\n");
        return 1;
    }
    int len=0;
    len=slen(argv[1]);
    printf("Lunghezza: %d", len);
    return 0;
}

int slen(char *s){
    int i=0;

    for(i=0;s[i]!='\0';i++);
    return i;
}
