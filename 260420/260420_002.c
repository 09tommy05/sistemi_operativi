#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

void intHandler(int sig, siginfo_t *info, void *context);
void termHandler(int sig, siginfo_t *info, void *context);
void usrHandler(int sig, siginfo_t *info, void *context);
int log_fd = 0;
volatile int i = 0;
int main()
{
    printf("PID: %d\n", getpid());
    FILE *in = fopen("/tmp/config.txt", "r");
    if (in == NULL)
    {
        printf("Error");
        return 2;
    }
    char app_name[255];
    int process;

    if (fgets(app_name, 255, in) != NULL)
    {
        process = atoi(app_name);
        if (fgets(app_name, 255, in) == NULL)
        {
            printf("Empty file\n");
            return 1;
        }
    }
    else
    {
        printf("Empty filee\n");
        return 1;
    }
    fclose(in);
    printf("Processi: %d\nApp: %s\n", process, app_name); // punto 1
    if (unlink("/tmp/log.txt") == -1)
    { // punto 2
        if (errno != ENOENT)
        {
            perror("unlink");
            return 3;
        }
    }
    // punto 3
    log_fd = open("/tmp/log.txt", O_CREAT | O_WRONLY, 0644);
    char buf[255] = "LOG STARTED: ";
    strcat(buf, app_name);
    write(log_fd, buf, sizeof(buf) - 1);
    buf[strlen(buf) - 1] = '\0';
    printf("Ho scritto \"%s\" sul file\n", buf);

    // punto 4
    struct sigaction sa;
    sa.sa_sigaction = intHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    // punto 8
    setpgid(0, 0);
    // punto 5
    struct sigaction sa_term;
    sa_term.sa_sigaction = termHandler;
    sa_term.sa_flags = 0;
    sigemptyset(&sa_term.sa_mask);
    sigaction(SIGTERM, &sa_term, NULL);
    for (i = 0; i < process; i++)
    {
        int f = fork();
        if (f == 0)
        {
            dup2(log_fd, 1);
            printf("Figlio %d avviato, PID= %d\n", i, getpid());
            struct sigaction sa_usr;
            sa_usr.sa_sigaction = usrHandler;
            sa_usr.sa_flags = 0;
            sigemptyset(&sa_usr.sa_mask);
            sigaction(SIGUSR1, &sa_usr, NULL);
            sleep(i * 10);
            printf("Figlio %d terminato\n", i);
            return i;
        }
    }
    // sempre punto 8
    if (getpgrp() == getpid())
    {
        char buf[255];
        snprintf(buf, sizeof(buf), "PGID confermato: %d\n", getpgrp());
        write(log_fd, buf, sizeof(buf));
    }
    int pid;
    int status;
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
        {
            int code = WEXITSTATUS(status);
            char buf[255];
            snprintf(buf, sizeof(buf), "Figlio PID=%d uscito con codice %d\n", pid, code);
            write(log_fd, buf, sizeof(buf));
        }
    }

    while (1)
    {
        pause();
    }
    close(log_fd);
    return 0;
}

void intHandler(int sig, siginfo_t *info, void *context)
{
    char buf[255] = "SIGINT received, ignoring\n";
    write(log_fd, buf, sizeof(buf));
}

void termHandler(int sig, siginfo_t *info, void *context)
{
    char buf[255] = "Shutting down\n";
    write(log_fd, buf, sizeof(buf));
    kill(0, SIGUSR1);
    int pid;
    int status;
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
        {
            int code = WEXITSTATUS(status);
            char buf[255];
            snprintf(buf, sizeof(buf), "Figlio PID=%d uscito con codice %d\n", pid, code);
            write(log_fd, buf, sizeof(buf));
        }
    }
    close(log_fd);
    exit(0);
}
void usrHandler(int sig, siginfo_t *info, void *context)
{
    char buf[255];
    snprintf(buf, sizeof(buf), "Figlio %d: ricevuto SIGUSR1\n", i);
    write(log_fd, buf, sizeof(buf));
}