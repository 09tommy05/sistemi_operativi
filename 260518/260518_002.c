#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIG_CREATE (SIGRTMIN + 1)
#define SIG_PRINT (SIGRTMIN + 3)
#define SIG_KILLD (SIGRTMIN + 4)
#define SIG_QUIT SIGTERM

#define MAX_CHILDREN 64

int my_depth = 0;
pid_t my_pid = 0;
pid_t parent = 0;

pid_t children[MAX_CHILDREN];
int nchildren = 0;
sigset_t set;

/**
 * Forward signal to all children with the given integer value as payload.
 * @param sig Signal to forward
 * @param val Integer value to send with the signal
 */
void forward(int sig, int val)
{
    union sigval sv;
    sv.sival_int = val;

    for (int i = 0; i < nchildren; ++i)
    {
        pid_t pid = children[i];

        if (pid <= 0)
            continue;

        if (sigqueue(pid, sig, sv) == -1)
        {
            fprintf(stderr, "sigqueue failed for pid %d: ", pid);
            perror(NULL);
        }
    }
}

/**
 * Terminate all children and wait for them to exit.
 */
void terminate_children(void)
{
    forward(SIG_QUIT, 0);
    for (int i = 0; i < nchildren; i++)
    {
        if (children[i] > 0)
        {
            if (waitpid(children[i], NULL, 0) == -1)
                perror("waitpid (terminate_children)");
            children[i] = 0;
        }
    }
    nchildren = 0;
}

/**
 * Initialize signal mask
 */
void setup_signals(sigset_t *mask)
{
    sigemptyset(mask);
    sigaddset(mask, SIG_CREATE);
    sigaddset(mask, SIG_PRINT);
    sigaddset(mask, SIG_KILLD);
    sigaddset(mask, SIG_QUIT);
    sigprocmask(SIG_BLOCK, mask, NULL);
}

/**
 * Main loop for child processes: wait for signals and handles them.
 * @param depth Depth of the current process in the tree
 * @param par PID of the parent process
 */
void child_loop(int depth, pid_t par)
{
    my_depth = depth;
    my_pid = getpid();
    parent = par;
    nchildren = 0;

    printf("I'm new child at level %d with id = %d\n", depth, (int)my_pid);
    fflush(stdout);

    while (1)
    {
        siginfo_t info;
        int sig = sigwaitinfo(&set, &info);
        if (sig == -1)
        {
            perror("sigwaitinfo");
            continue;
        }

        if (sig == SIG_CREATE)
        {
            int n = info.si_value.sival_int;
            if (n - 1 == my_depth)
            {
                pid_t pid = fork();
                if (pid == 0)
                {
                    child_loop(my_depth + 1, my_pid);
                    exit(0);
                }
                else if (pid > 0)
                {
                    children[nchildren++] = pid;
                }
            }
            else
            {
                forward(SIG_CREATE, n);
            }
        }

        if (sig == SIG_KILLD)
        {
            /* TODO */
        }

        if (sig == SIG_QUIT)
        {
            terminate_children();
            exit(0);
        }

        if (sig == SIG_PRINT)
        {
            forward(SIG_PRINT, 0);
            for (int t = 0; t < my_depth; t++)
                printf("\t");
            printf("[ID %d - Parent: %d] depth %d\n",
                   (int)my_pid, (int)parent, my_depth);
            fflush(stdout);
            continue;
        }
    }
}

/**
 * Create children at given depth
 * @param n Depth at which the new child should be created
 */
void root_create(int n)
{
    if (n == 1)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            child_loop(1, getppid());
            exit(0);
        }
        else if (pid > 0)
        {
            children[nchildren++] = pid;
        }
    }
    else
    {
        forward(SIG_CREATE, n);
    }
}

/**
 * Kill command
 */
void root_kill(int n)
{
    /* TODO */
}

/**
 * Print command
 */
void root_print(void)
{
    printf("Printing Tree:\n");
    printf("[ID %d - Parent: 0] depth 0\n", (int)getpid());
    fflush(stdout);
    for (int i = 0; i < nchildren; i++)
        if (children[i] > 0)
            kill(children[i], SIG_PRINT);
}

/**
 * Quit command
 */
void root_quit(void)
{
    printf("Quitting...\n");
    fflush(stdout);
    terminate_children();
    exit(0);
}

int main(void)
{
    char line[64];

    setup_signals(&set);

    while (1)
    {
        usleep(100000); // Prompt order
        printf("\nNext command: ");
        fflush(stdout);

        int r = read(STDIN_FILENO, line, sizeof(line) - 1);
        line[r] = '\0';
        int n = 0;

        switch (line[0])
        {
        case 'c':
            if (isdigit((unsigned char)line[1]))
            {
                n = atoi(line + 1);
                root_create(n);
            }
            else
            {
                printf("Invalid command. Usage: c<n> to create.\n");
            }
            break;
        case 'k':
            if (isdigit((unsigned char)line[1]))
            {
                n = atoi(line + 1);
                root_kill(n);
            }
            else
            {
                printf("Invalid command. Usage: k<n> to kill.\n");
            }
            break;
        case 'p':
            root_print();
            break;
        case 'q':
            root_quit();
            break;
        default:
            printf("Commands: c<n>  k<n>  p  q\n");
        }
    }
    return 0;
}
