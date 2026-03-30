// Avendo come argomenti dei “binari”, si eseguono con exec ciascuno in un sottoprocesso
//260330_004 bin1 bin2c

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int i = 1;
    for (i = 1; i < argc; i++)
    {
        int f = fork();
        if(f==0)
            execl(argv[i], argv[i], NULL);
    }
    wait(NULL);
    return 0;
}