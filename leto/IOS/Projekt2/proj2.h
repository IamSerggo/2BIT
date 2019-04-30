/**
 * Header file for the second project for the subject IOS
 * @brief: Modified synchronization problem - River Crossing Problem
 * @file : proj2.h
 * @date : 28.04.2019
 * @author : Radovan Babic, xbabic09
 */

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

//a constants for an error handling
#define SYS_ERR_ARGS 1
#define SYS_ERR_FILE 1
#define SYS_ERR_SEM 1
#define SYS_ERR_MEM 1
#define SYS_OK 0

//a constants for a semaphore states
#define LOCKED 0
#define UNLOCKED 1

//a struct for the input arguments
typedef struct {
    unsigned int P;
    unsigned int H;
    unsigned int S;
    unsigned int R;
    unsigned int W;
    unsigned int C;
} arg;

//a pointer to the output file
FILE *fp;

//the semaphores declaration
sem_t *mutex;
sem_t *writing;
sem_t *mole;
sem_t *boat_s;
sem_t *boat_h;
sem_t *boat_available;
sem_t *on_cruise;
sem_t *captain_write;

//a variables for a shared memory allocation using the shmget()
int shared_operation_id;
int shared_serfs_on_mole;
int shared_hackers_on_mole;
int shared_hackers_output;
int shared_serfs_output;

//the real shared memory variables created (attached) using the shmat()
int *operation_id;
int *serfs_on_mole;
int *hackers_on_mole;
int *hackers_output;
int *serfs_output;

/**
 * Checking if the inserted arguments values are correct according to the task
 *
 * @param arguments the arguments to be checked. If the arguments are not correct
 *                  the function will print an error message to the stderr and exit with an adequate error code.
 *
 * @return void
 */
void checkArgs(arg arguments);

/**
 * Parsing the input arguments into the arg struct
 * Function is checking if the arguments are in correct format
 *
 * @param argc the arguments counter
 * @param argv the array of the input arguments to be parsed
 *
 * @return arg the struct filled with the parsed arguments
 */
arg parseArgs(int argc, char* argv[]);

/**
 * Resources initialization
 *
 * File opening
 * Shared memory allocation and attachment
 * Semaphores initialization
 *
 * @param void
 *
 * @return void
 */
void initialization(void);

/**
 * Resources cleanup
 *
 * File closing
 * Shared memory deallocation and deattachment
 * Semaphores destruction
 *
 * @param void
 *
 * @return void
 */
void cleanup(void);

/**
 * Function for the HACKER process
 *
 * The processes are generated in the main function and each process is invoking this function
 * The hackers are coming to the mole, if the capacity is reached, they sleep themselves and then try again
 * If they reached the mole, they are either becoming a captain or a member
 *
 * The synchronization of the processes is done by the shared memory and the semaphores
 *
 * @param num the "serial number" of the hacker
 * @param arg the input arguemnts struct
 *
 * @return void
 */
void gen_hacker(int num, arg arguments);

/**
 * Function for the SERF process
 *
 * The processes are generated in the main function and each process is invoking this function
 * The serfs are coming to the mole, if the capacity is reached, they sleep themselves and then try again
 * If they reached the mole, they are either becoming a captain or a member
 *
 * The synchronization of the processes is done by the shared memory and the semaphores
 *
 * @param num the "serial number" of the serf
 * @param arg the input arguemnts struct
 *
 * @return void
 */
void gen_serf(int num, arg arguments);

/**
 * Generating random delay
 *
 * Usleep in miliseconds from range <0,time>
 *
 * @param time the maximum value (right part of range)
 *
 * @return void
 */
void generate_delay(int time);

/**
 * The main function of the program
 *
 * The input arguments are parsed using the parseArgs()
 * The shared memory and the semaphores are initialized using the initialize()
 * Then the parent and child processes are created using the fork()
 * The parent process is handling the Hackers
 * The child process is handling the Serfs
 * All resources are cleaned by the cleanup()
 * Possible errors are handled
 *
 * @param argc the argument counter
 * @param argv the arguments array
 *
 * @return int return code of the program
 *             1 if some error occurred
 *             0 if no error occurred and the program did all the work
 */
int main(int argc, char* argv[]);

#endif //PROJ2_H
