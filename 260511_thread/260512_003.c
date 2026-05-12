#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define _USE_MATH_DEFINES
#include <fcntl.h>
#include <math.h>
#include <string.h>

int out = 0;
pthread_mutex_t lock;

typedef struct
{
    int i;
    double num;
} thr_args;

void *thr(void *args)
{
    thr_args *a = (thr_args *)args;
    double num = a->num;
    char buf[256];
    double res[3];
    char *oper[3] = {"square", "sqrt", "times PI"};

    res[0] = num * num;
    res[1] = sqrt(num);
    res[2] = num * M_PI;
    for (int i = 0; i < 3; i++)
    {
        strcpy(buf, "");
        pthread_mutex_lock(&lock);
        snprintf(buf, sizeof(buf), "[THREAD %d] Operation %d (%s): START - input: %f\n", a->i, i, oper[i], a->num);
        write(out, buf, strlen(buf));
        strcpy(buf, "");
        snprintf(buf, sizeof(buf), "[THREAD %d] Operation %d (%s): END - result: %f\n", a->i, i, oper[i], res[i]);
        write(out, buf, strlen(buf));
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    if (argc < 2)
    {
        printf("Usage %s <N>\n", argv[0]);
        exit(1);
    }
    int N = atoi(argv[1]);
    pthread_mutex_init(&lock, NULL);
    pthread_t *thr_ids = malloc(sizeof(pthread_t) * N);
    out = open("file.log", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    thr_args *ta = malloc(sizeof(thr_args) * N);

    for (int i = 0; i < N; i++)
    {
        ta[i].num = rand() % 100 + 1.0;
        ta[i].i = i+1;
        pthread_create(&thr_ids[i], NULL, thr, &ta[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(thr_ids[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    free(ta);
    free(thr_ids);
    close(out);
    return 0;
}