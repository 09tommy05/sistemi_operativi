#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Dynamic_strings_vec
{
    int dim;
    char **v;
} Dynamic_strings_vec;

void vec_push(Dynamic_strings_vec *ds, char *str);
void vec_free(Dynamic_strings_vec *ds);
void vec_print(Dynamic_strings_vec ds);

int main()
{
    Dynamic_strings_vec ds;
    ds.dim = 0;
    ds.v=NULL;
    char buffer[255];
    buffer[0] = '\0';
    while (buffer[0] != '0')
    {
        printf("##############################\n\n");
        vec_print(ds);
        printf("##############################\n\n");
        printf("Stringa: ");
        scanf("%s", buffer);
        fflush(stdin);
        vec_push(&ds, buffer);
    }
    vec_free(&ds);
    return 0;
}
void vec_push(Dynamic_strings_vec *ds, char *str)
{
    int len;
    int dim = ds->dim;
    char **tmp = (char **)malloc(sizeof(char *) * (dim));
    int i = 0;
    for (i = 0; i < dim; i++)
    {
        len = strlen(ds->v[i]) + 1;
        tmp[i] = (char *)malloc(len * sizeof(char));
        strcpy(tmp[i], ds->v[i]);
    }
    vec_free(ds);
    ds->dim++;
    ds->v = (char **)malloc(sizeof(char *) * ds->dim);
    for (i = 0; i < dim; i++)
    {
        len=strlen(tmp[i])+1;
        ds->v[i] = (char *)malloc(len * sizeof(char));
        strcpy(ds->v[i], tmp[i]);
    }
    len = strlen(str) + 1; //+1 per \0
    ds->v[dim] = (char *)malloc(len * sizeof(char));
    strcpy(ds->v[dim], str);
    for (i = 0; i < dim; i++)
        free(tmp[i]);
    free(tmp);
}
void vec_free(Dynamic_strings_vec *ds){
    int i=0;
    for(i=0;i< ds->dim;i++){
        free(ds->v[i]);
    }
    free(ds->v);
}

void vec_print(Dynamic_strings_vec ds){
    if(ds.v==NULL){
        printf("Non ci sono stringhe.\n");
        return;
    }
    int i=0;
    printf("Ci sono %d stringhe:\n", ds.dim);
    for(i=0;i< ds.dim;i++){
        printf("%d. %s\n",(i+1),ds.v[i]);
    }
    putchar('\n');
}