/*
Dati due eseguibili come argomenti del tipo ls e wc si eseguono in due processi distinti: il primo
deve generare uno stdout redirezionato su un file temporaneo, mentre il secondo deve essere
lanciato solo quando il primo ha finito leggendo lo stesso file come stdin.
Ad esempio ./main ls wc deve avere lo stesso effetto di ls | wc.
*/

// esempio di avvio 260330_006 ls wc

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Errore: non ci sono abbastanza argomenti.");
        return 1;
    }
    char *temp_filename = "/tmp/260330_temp.txt";
    int out = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    int f = fork();
    int status;
    if (f < 0)
    {
        fprintf(stderr, "Errore: creazione processo.");
        return -1;
    }
    else if (f == 0)
    {
        dup2(out, 1);
        close(out);
        execlp(argv[1], argv[1], NULL);
        return 0;
    }
    else
    {
        waitpid(f, &status, 0);
        f = fork();
        if (f < 0)
        {
            fprintf(stderr, "Errore: creazione processo.");
            return -1;
        }
        else if (f == 0)
        {
            int in = open(temp_filename, O_RDONLY);
            dup2(in, 0);
            close(in);
            execlp(argv[2], argv[2], NULL);
            return 0;
        }
        else
        {
            waitpid(f, &status, 0);
            printf("Comando %s | %s eseguito con successo.\n", argv[1], argv[2]);
        }
    }
    return 0;
}