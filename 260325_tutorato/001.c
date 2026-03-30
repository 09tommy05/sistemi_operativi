#include <stdio.h>
#define DIM 10

void arr_min_max(int *, int, int *, int *);
void arr_print(int *, int);
void arr_reverse(int *, int);
void arr_rotate(int *, int, int);

int main()
{
    int v[DIM];
    int i = 0;
    for (i = 0; i < DIM; i++)
        v[i] = i + 1;
    arr_print(v, DIM);
    int min, max;
    min = v[0];
    max = v[0];
    arr_min_max(v, DIM, &min, &max);
    printf("Min: %d\nMax: %d\n", min, max);
    arr_reverse(v, DIM);
    arr_print(v, DIM);
    arr_rotate(v, DIM, 3);
    arr_print(v, DIM);
    return 0;
}

void arr_print(int *v, int N)
{
    int i = 0;
    for (i = 0; i < N; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

void arr_min_max(int *v, int N, int *min, int *max)
{
    int i = 1;
    for (i = 1; i < N; i++)
    {
        if (v[i] < *min)
            *min = v[i];
        else if (v[i] > *max)
            *max = v[i];
    }
}

void arr_reverse(int *v, int N)
{
    int i = 0;
    int tmp;
    for (i = 0; i < N / 2; i++)
    {
        tmp = v[i];
        v[i] = v[N - 1 - i];
        v[N - 1 - i] = tmp;
    }
}

void arr_rotate(int *v, int N, int k)
{
    int i, j;
    int dim = k % N;
    for (i = 0; i < dim; i++)
    {
        int prev = v[0];
        int prev2 = prev;
        for (j = N - 1; j >= 0; j--)
        {
            prev2 = v[j];
            v[j] = prev;
            prev = prev2;
        }
    }
}