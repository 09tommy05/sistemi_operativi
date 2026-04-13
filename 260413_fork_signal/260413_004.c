/*
Si consideri un sistema composto da due processi: un processo principale e un processo watchdog.
Il processo watchdog deve controllare che il processo principale sia ancora attivo. Mentre il processo
principale deve inviare periodicamente una notifica al watchdog per indicare che è ancora in
esecuzione. Il watchdog deve:
- attendere le notifiche del processo principale
- resettare un timer interno ad ogni notifica ricevuta
- terminare il processo principale se non riceve notifiche entro un intervallo di tempo prefissato
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int timer = 6;

void handler(int sig, siginfo_t *info, void *context)
{
    timer = 6;
    write(STDOUT_FILENO, "[WATCHDOG] Il padre è ancora vivo.\n", 36);
}

int main()
{
    pid_t parent_pid=getpid();
    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    int watchdog = fork();
    if (watchdog == 0)
    {
        while (1)
        {
            if (timer < 0)
            {
                printf("[WATCHDOG] uccido il padre.\n");
                kill(parent_pid, SIGKILL);
                exit(0);
            }
            sleep(1);
            --timer;
        }
    }
    else
    {
        srand(time(NULL));

        while (1)
        {
            printf("[PARENT] mando segnale.\n");
            kill(watchdog, SIGUSR1);
            int s=rand()%10;
            //int s = 5;
            printf("[PARENT] aspetto %d secondi\n", s);
            sleep(s);
        }
    }
    return 0;
}