#ifndef GIOCO_H
#define GIOCO_H

#include <sys/types.h>

#define MAX_PLAYERS 10
#define NUM_CARDS   10

typedef struct {
    long mtype;
    int  cards[NUM_CARDS];
} hand_t;

/*
 * Player entry point, called by child processes after fork().
 *
 *   pipe_write_fd : write end of pipe to table (played card)
 *   checker_pid   : PID of the external checker process
 *   settings_path : path to the gameSettings file
 *   queue_id      : message queue ID (already opened by the table)
 */
void player_main(int pipe_write_fd, pid_t checker_pid,
                 const char *settings_path, int queue_id);

#endif