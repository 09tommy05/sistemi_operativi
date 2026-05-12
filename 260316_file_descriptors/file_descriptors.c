#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Argomenti insufficienti\n");
        return 1;
    }
    int mode = atoi(argv[3]);
    FILE *in = fopen(argv[1], "r");
    int out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (in == NULL)
    {
        printf("File di input non trovato.\n");
        return 2;
    }
    if (out == -1)
    {
        printf("Problemi con il file di output\n");
        return 3;
    }
    if (mode == 0)
    {
        char buffer[255];
        while (fgets(buffer, 255, in) != NULL)
        {
            write(out, buffer, strlen(buffer));
        }
    }
    else if (mode == 1)
    {
        char c = '\0';
        while ((c = fgetc(in)) != EOF)
        {
            char s[2];
            s[0] = c;
            s[1] = '\0';
            write(out, s, strlen(s));
        }
    }
    else
    {
        printf("Opzione non valida.\n");
        return 4;
    }
    printf("File %s copiato in %s in modalita' %d\n", argv[1], argv[2], mode);
    fclose(in);
    close(out);
    return 0;
}