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
    int in = open(argv[1], O_RDONLY);
    FILE *out = fopen(argv[2], "w");
    if (in == -1)
    {
        printf("File di input non trovato.\n");
        return 2;
    }
    if (out == NULL)
    {
        printf("Problemi con il file di output\n");
        return 3;
    }
    if (mode == 0)
    {
        char buffer[255];
        int bytesRead;
        while ((bytesRead = read(in, buffer, sizeof(buffer))) > 0)
        {
            buffer[bytesRead] = '\0';
            fprintf(out, "%s", buffer);
        }
    }
    else if (mode == 1)
    {
        char content[1];
        int bytesRead;
        while ((bytesRead = read(in, content, 1)) > 0)
        {
            fprintf(out, "%c", content[0]);
        }
    }
    else
    {
        printf("Opzione non valida.\n");
        return 4;
    }
    printf("File %s copiato in %s in modalita' %d\n", argv[1], argv[2], mode);
    close(in);
    fclose(out);
    return 0;
}