// Come punto 1 ma in più salvando i flussi di stdout e stderr in un unico file
// 260330_005 bin1 bin2c

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    char *args[] = {NULL};
    int i = 1;
    int out = open("./out.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    int err = open("./err.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    dup2(out, 1);
    dup2(err, 2);
    close(out);
    close(err);
    for (i = 1; i < argc; i++)
    {
        int f = fork();
        if (f == 0)
        {
            execl(argv[i], argv[i], NULL);
        }
    }
    wait(NULL);
    return 0;
}