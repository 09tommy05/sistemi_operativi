#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Dynamic_strings_vec
{
    int dim;
    char **v;
    int *occ;
} Dynamic_strings_vec;

void vec_push(Dynamic_strings_vec *ds, char *str);
void vec_free(Dynamic_strings_vec *ds);
int is_in_list(char *, Dynamic_strings_vec *ds);
void to_low(char *s);
void sort(Dynamic_strings_vec *ds);
void vec_print(Dynamic_strings_vec ds);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("./%s <nome_file>\n", argv[0]);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL)
    {
        printf("Il file \"%s\" non esiste\n", argv[1]);
        fclose(in);
        return 2;
    }

    Dynamic_strings_vec ds;
    ds.dim = 0;
    ds.v = NULL;
    ds.occ = NULL;
    char buffer[255];
    buffer[0] = '\0';
    while (fgets(buffer, 255, in) != NULL)
    {
        char *token = strtok(buffer, " ");
        while (token != NULL)
        {
            to_low(token);
            if (strcmp(token, "\n") != 0 && strcmp(token, " ") != 0)
            {
                if (is_in_list(token, &ds) == 0)
                { // se non in lista lo aggiungo.
                    vec_push(&ds, token);
                }
            }
            token = strtok(NULL, " ");
        }
    }
    vec_print(ds);
    vec_free(&ds);
    fclose(in);
    return 0;
}
void vec_push(Dynamic_strings_vec *ds, char *str)
{
    int len;
    int dim = ds->dim;
    int *tmp_occ = (int *)malloc(sizeof(int *) * (dim));
    char **tmp = (char **)malloc(sizeof(char *) * (dim));
    int i = 0;
    for (i = 0; i < dim; i++)
    {
        len = strlen(ds->v[i]) + 1;
        tmp[i] = (char *)malloc(len * sizeof(char));
        strcpy(tmp[i], ds->v[i]);
        tmp_occ[i] = ds->occ[i];
    }
    vec_free(ds);
    ds->dim++;
    ds->v = (char **)malloc(sizeof(char *) * ds->dim);
    ds->occ = (int *)malloc(sizeof(int *) * ds->dim);
    for (i = 0; i < dim; i++)
    {
        len = strlen(tmp[i]) + 1;
        ds->v[i] = (char *)malloc(len * sizeof(char));
        ds->occ[i] = tmp_occ[i];
        strcpy(ds->v[i], tmp[i]);
    }
    len = strlen(str) + 1; //+1 per \0
    ds->v[dim] = (char *)malloc(len * sizeof(char));
    strcpy(ds->v[dim], str);
    ds->occ[dim] = 1;
    for (i = 0; i < dim; i++)
    {
        free(tmp[i]);
    }
    free(tmp);
    free(tmp_occ);
}
void vec_free(Dynamic_strings_vec *ds)
{
    int i = 0;
    for (i = 0; i < ds->dim; i++)
    {
        free(ds->v[i]);
    }
    free(ds->v);
    free(ds->occ);
}

void to_low(char *s)
{
    int i = 0;
    for (i = 0; s[i] != '\0'; i++)
        s[i] = tolower(s[i]);
}

int is_in_list(char *s, Dynamic_strings_vec *ds)
{
    int i = 0;
    for (i = 0; i < ds->dim; i++)
    {
        if (strcmp(ds->v[i], s) == 0)
        {
            ds->occ[i]++;
            return 1;
        }
    }
    return 0;
}

void sort(Dynamic_strings_vec *ds)
{
    int i = 0, j = 0;
    for (i = 0; i < ds->dim - 1; i++)
    {
        for (j = i + 1; j < ds->dim; j++)
        {
            if (ds->occ[i] < ds->occ[j])
            {
                // scambio occorrenze
                int tmp = ds->occ[i];
                ds->occ[i] = ds->occ[j];
                ds->occ[j] = tmp;
                // scambio stringhe:
                int len = strlen(ds->v[i]) + 1;
                char *temp = (char *)malloc(sizeof(char) * len);
                strcpy(temp, ds->v[i]);
                int len2 = strlen(ds->v[j]) + 1;
                free(ds->v[i]);
                ds->v[i] = (char *)malloc(sizeof(char) * len2);
                strcpy(ds->v[i], ds->v[j]);
                free(ds->v[j]);
                ds->v[j] = (char *)malloc(sizeof(char) * len);
                strcpy(ds->v[j], temp);
            }
        }
    }
}

void vec_print(Dynamic_strings_vec ds)
{
    int i = 0;
    int dim = 10 < ds.dim ? 10 : ds.dim;
    for (i = 0; i < dim; i++)
    {
        printf("%d. %s --> %d\n", (i + 1), ds.v[i], ds.occ[i]);
    }
    putchar('\n');
}