#include <string.h>
#include <stdio.h>
typedef struct
{
    char word[128];
    int length;
    int palindrome;
    int vowels;
} WordStats;

void analyze(char *s, WordStats *ws);