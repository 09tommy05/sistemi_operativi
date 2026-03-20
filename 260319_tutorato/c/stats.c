#include "stats.h"

int my_strlen(char *s)
{
    int i = 0;
    for (i = 0; s[i] != '\0'; i++)
        ;
    return i;
}

int is_palindromo(char *s, int len)
{
    int i = 0;
    for (i = 0; i < len / 2; i++)
    {
        if (s[i] != s[len - 1 - i])
            return 0;
    }
    return 1;
}

int vocali(char *s)
{
    int i = 0;
    int ctr = 0;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' || s[i] == 'A' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U')
            ++ctr;
    }
    return ctr;
}

void analyze(char *s, WordStats *ws)
{
#ifdef DEBUG
    printf("[DEBUG] Calcolo la lunghezza di \"%s\"\n", s);
#endif
    ws->length = my_strlen(s);
#ifdef DEBUG
    printf("[DEBUG] Ho calcolato la lunghezza di \"%s\" (%d)\n", s, ws->length);
    printf("[DEBUG] Calcolo se \"%s\" e' palindroma\n", s);
#endif
    ws->palindrome = is_palindromo(s, ws->length);
#ifdef DEBUG
    printf("[DEBUG] Ho calcolato se \"%s\" e' palindroma (%s)\n", s, ws->palindrome == 1 ? "si" : "no");
    printf("[DEBUG] Calcolo il numero di vocali di \"%s\"\n", s);
#endif
    ws->vowels = vocali(s);
#ifdef DEBUG
    printf("[DEBUG] Ho calcolato il numeri di vocali di \"%s\" (%d)\n", s, ws->vowels);
#endif
    strcpy(ws->word, s);
}