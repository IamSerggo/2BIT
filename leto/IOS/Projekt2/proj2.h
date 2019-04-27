//
// Created by radovan on 23.4.2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>

#ifndef PROJ2_H
#define PROJ2_H

#define SYS_ERR_ARGS 1
#define SYS_ERR_FILE 1
#define SYS_ERR_SEM 1
#define SYS_ERR_MEM 1
#define SYS_OK 0

#define LOCKED 0
#define UNLOCKED 1

typedef struct {
    int P;
    int H;
    int S;
    int R;
    int W;
    int C;
} arg;

sem_t *writing;
sem_t *mole;
sem_t *boat_s;
sem_t *boat_h;
sem_t *mutex;
sem_t *boat_available;
sem_t *on_cruise;
sem_t *captain_write;

pid_t captain;

int shared_operation_id;
int shared_serf_id; //TODO ?
int shared_hacker_id; //TODO ?
int shared_serfs_on_mole;
int shared_hackers_on_mole;
int shared_mole_cap;
int shared_hackers_output;
int shared_serfs_output;

int *operation_id;
int *serfs_on_mole;
int *hackers_on_mole;
int *mole_cap;
int *hackers_output;
int *serfs_output;

void cleanup(void);
void generate_delay(int time);

FILE *fp;

#endif //PROJ2_H
