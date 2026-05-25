#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(){
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGRTMIN, &sa, NULL);

    printf("%d\n", getpid());
    while(1);
    return 0;
}