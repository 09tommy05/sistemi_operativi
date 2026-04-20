#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int pid=-1;
    printf("pid > ");
    scanf("%d", &pid);
    int i=0;
    for(i=0;i<5;i++){
       kill(pid, SIGUSR1);
       sleep(1);
        if(i%2==0){
           kill(pid, SIGUSR2);
            sleep(1);
        }
    }
    return 0;
}