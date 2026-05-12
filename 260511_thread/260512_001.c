#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int glob = 0;
pthread_mutex_t lock;

void *thr(void *arg)
{
    int N = *(int *)arg;
    for (int i = 0; i < N; i++)
    {
        pthread_mutex_lock(&lock);
        ++glob;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage %s <T> <N>\n", argv[0]);
        exit(1);
    }
    int T = atoi(argv[1]);
    int *N = malloc(sizeof(int));
    *N = atoi(argv[2]);

    pthread_t *t_ids = malloc(sizeof(pthread_t) * T);
    
    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < T; i++)
        pthread_create(&t_ids[i], NULL, thr, N);

    //aspetto che i thread finiscano prima di distruggere la mutex
    for(int i =0;i<T;i++)
        pthread_join(t_ids[i],NULL);
    
    pthread_mutex_destroy(&lock);

    printf("Valore atteso: %d\n", (*N*T));
    printf("Valore ottenuto= %d\n", glob);
    printf("Incrementi persi= %d\n", ((*N*T)- glob));
    free(N);
    free(t_ids);
    return 0;
}