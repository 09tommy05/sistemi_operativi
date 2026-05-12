#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define N_THREAD 4

int sum = 0;
pthread_mutex_t lock;

typedef struct
{
    int *v;
    int start;
    int size;
} thr_args;

void *thr(void *args)
{
    thr_args *a = (thr_args *)args;
    int *v    = a->v;
    int start = a->start;
    int size = a->size;
    int end=start+size;
    int s = 0;

    for(int i=start;i<end;i++){
        s+=v[i];
    }

    pthread_mutex_lock(&lock);
    sum += s;
    pthread_mutex_unlock(&lock);
    free(a);
    return NULL;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    if (argc < 2)
    {
        printf("Usage %s <size>\n", argv[0]);
        exit(1);
    }
    int size = atoi(argv[1]);
    if (size < N_THREAD)
    {
        printf("Size should be at least %d\n", N_THREAD);
        exit(1);
    }
    int *v = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        v[i] = rand() % 69 + 1;
        printf("%d ", v[i]);
    }
    printf("\n");
    int size_arr_thr = size / N_THREAD;
    int start = 0;
    pthread_t thr_ids[N_THREAD];
    pthread_mutex_init(&lock,NULL);
    for (int i = 0; i < N_THREAD; i++)
    {
        thr_args *args = malloc(sizeof(thr_args));
        args->v = v;
        args->start = start;
        //così non è equamente distribuito, ma scarica il resto all'ultimo, (da migliorare con int remainder = size % N_THREAD;)
        if(i==N_THREAD-1 && (size_arr_thr+start) < size){
            args->size= size-start;
        }else
            args -> size = size_arr_thr;
        pthread_create(&thr_ids[i], NULL, thr, args);
        start += size_arr_thr;
    }

    for(int i=0;i<N_THREAD;i++){
        pthread_join(thr_ids[i],NULL);
    }
    pthread_mutex_destroy(&lock);
    free(v);
    printf("somma= %d\n",sum);
    return 0;
}