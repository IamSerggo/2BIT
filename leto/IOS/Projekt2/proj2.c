/**
 * Source code file for the second project for the subject IOS
 * @brief: Modified synchronization problem - River Crossing Problem
 * @file : proj2.c
 * @date : 28.04.2019
 * @author : Radovan Babic, xbabic09
 */

#include "proj2.h"

void checkArgs(arg arguments){
    bool isCorrect = true;

    if (arguments.P < 2 || arguments.P % 2 != 0) {
        fprintf(stderr, "1. argument incorrect! P >= 2 && P %% 2 == 0\n");
        isCorrect = false;
    }

    if (arguments.H > 2000) {
        fprintf(stderr, "2. argument incorrect! H >= 0 && H <= 2000\n");
        isCorrect = false;
    }

    if (arguments.S > 2000) {
        fprintf(stderr, "3. argument incorrect! S >= 0 && S <= 2000\n");
        isCorrect = false;
    }

    if (arguments.R > 2000) {
        fprintf(stderr, "4. argument incorrect! R >= 0 && R <= 2000\n");
        isCorrect = false;
    }

    if (arguments.W < 20 || arguments.W > 2000) {
        fprintf(stderr, "5. argument incorrect! W >= 20 && W <= 2000\n");
        isCorrect = false;
    }

    if (arguments.C < 5) {
        fprintf(stderr, "6. argument incorrect! C >= 5\n");
        isCorrect = false;
    }

    if (!isCorrect) exit(SYS_ERR_ARGS);
}

arg parseArgs(int argc, char* argv[]){
    if (argc != 7) {
        fprintf(stderr, "Wrong number of arguments inserted!\n");
        exit(SYS_ERR_ARGS);
    }

    char *err = NULL;
    bool isCorrect = true;

    arg arguments = {
        .P = 0,
        .H = 0,
        .S = 0,
        .R = 0,
        .W = 0,
        .C = 0
    };

    arguments.P = strtoul(argv[1], &err, 10);
    if ((*err) != 0) {
        fprintf(stderr, "1. argument is not a valid number!\n");
        isCorrect = false;
    }

    arguments.H = strtoul(argv[2], &err, 10);
    if ((*err) != 0) {
        fprintf(stderr, "2. argument is not a valid number!\n");
        isCorrect = false;
    }

    arguments.S = strtoul(argv[3], &err, 10);
    if ((*err) != 0) {
        fprintf(stderr, "3. argument is not a valid number!\n");
        isCorrect = false;
    }

    arguments.R = strtoul(argv[4], &err, 10);
    if ((*err) != 0) {
        fprintf(stderr, "4. argument is not a valid number!\n");
        isCorrect = false;
    }

    arguments.W = strtoul(argv[5], &err, 10);
    if ((*err) != 0) {
        fprintf(stderr, "5. argument is not a valid number!\n");
        isCorrect = false;
    }

    arguments.C = strtoul(argv[6], &err, 10);
    if ((*err) != 0) {
        fprintf(stderr, "6. argument is not a valid number!\n");
        isCorrect = false;
    }

    if (isCorrect) checkArgs(arguments);
    else exit(SYS_ERR_ARGS);

    return arguments;
}

void initialization(void){
    int error = SYS_OK;

    if ( (fp = fopen("proj2.out", "w")) == NULL ){
        fprintf(stderr, "Error while opening output file!\n");
        error = SYS_ERR_FILE;
    }

    if((mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
       (writing = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
       (mole = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
       (boat_s = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
       (boat_h = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
       (on_cruise = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
       (captain_write = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
       (boat_available = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||

       sem_init(writing, 1, UNLOCKED) == -1 ||
       sem_init(on_cruise, 1, LOCKED) == -1 ||
       sem_init(mole, 1, UNLOCKED) == -1 ||
       sem_init(boat_h, 1, LOCKED) == -1 ||
       sem_init(boat_s, 1, UNLOCKED) == -1 ||
       sem_init(boat_available, 1, UNLOCKED) == -1 ||
       sem_init(captain_write, 1, LOCKED) == -1 ||
       sem_init(mutex, 1, UNLOCKED) == -1 ){
        error = SYS_ERR_SEM;
    }
    else if(  (shared_hackers_on_mole = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
              (shared_serfs_on_mole = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
              (shared_operation_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
              (shared_hackers_output = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
              (shared_serfs_output = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||

              (hackers_on_mole = (int *) shmat(shared_hackers_on_mole, NULL, 0)) == NULL ||
              (serfs_on_mole = (int *) shmat(shared_serfs_on_mole, NULL, 0)) == NULL ||
              (operation_id = (int *) shmat(shared_operation_id, NULL, 0)) == NULL ||
              (hackers_output = (int *) shmat(shared_hackers_output, NULL, 0)) == NULL ||
              (serfs_output = (int *) shmat(shared_serfs_output, NULL, 0)) == NULL) {
        error = SYS_ERR_MEM;
    }

    if (error != SYS_OK) {
        fprintf(stderr, "Error while semaphores init and/or shared memory allocation!!\n");
        cleanup();
        exit(SYS_ERR_MEM);
    }
}

void cleanup(void){
    int error = SYS_OK;

    fclose(fp);

    if(sem_destroy(writing) == -1 ||
       sem_destroy(mutex) == -1 ||
       sem_destroy(mole) == -1 ||
       sem_destroy(boat_s) == -1 ||
       sem_destroy(boat_h) == -1 ||
       sem_destroy(on_cruise) == -1 ||
       sem_destroy(captain_write) == -1 ||
       sem_destroy(boat_available) == -1){
        error = SYS_ERR_SEM;
    }

    if(shmctl(shared_hackers_on_mole, IPC_RMID, NULL) == -1 ||
       shmctl(shared_serfs_on_mole, IPC_RMID, NULL) == -1 ||
       shmctl(shared_operation_id, IPC_RMID, NULL) == -1 ||
       shmctl(shared_hackers_output, IPC_RMID, NULL) == -1 ||
       shmctl(shared_serfs_output, IPC_RMID, NULL) == -1 ||

       shmdt(hackers_on_mole) == -1 ||
       shmdt(serfs_on_mole) == -1 ||
       shmdt(hackers_output) == -1 ||
       shmdt(serfs_output) == -1 ||
       shmdt(operation_id) == -1 ){
        error =  SYS_ERR_MEM;
    }

    if (error != SYS_OK){
        fprintf(stderr, "Unable to do memory cleanup!\n");
        exit(SYS_ERR_MEM);
    }
}

void gen_hacker(int num, arg arguments){
    int my_id = num+1;
    unsigned int mole_occupation = 0;

    bool isCaptain = false;
    bool onlyHackers = false;

    //hacker process is started
    sem_wait(writing);
    fprintf(fp, "%d: HACK %d: starts\n", ++(*operation_id), my_id);
    fflush(fp);
    sem_post(writing);

    //hacker process is trying to get to the mole
    while (true){
        sem_wait(mole);
        sem_wait(mutex);

        mole_occupation = (*hackers_output) + (*serfs_output); //checking if there si space on the mole
        if (mole_occupation >= arguments.C){
            //hacker is leaving the mole because it's full
            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: leaves queue: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
            fflush(fp);
            sem_post(writing);

            sem_post(mutex);
            sem_post(mole);

            generate_delay(arguments.W); //generating delay and letting other hackers to try to get to the mole

            //hacker is back and will try to get to the mole again
            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: is back\n", ++(*operation_id), my_id);
            fflush(fp);
            sem_post(writing);

            continue;
        }
        else{ //hacker enters the mole
            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: waits: %d: %d\n", ++(*operation_id), my_id, ++(*hackers_output), (*serfs_output));
            fflush(fp);
            (*hackers_on_mole)++;
            sem_post(writing);

            //hacker is checking if he can be the captain
            if ( (*hackers_on_mole) >= 2 && (*serfs_on_mole) >= 2 ){ //if true, a group of 2 serfs and 2 hackers will go to the boat
                isCaptain = true;
                onlyHackers = false;
                (*hackers_on_mole) -= 2; //decrementing these values so that the next hacker will now that these persons don't need captain
                (*serfs_on_mole) -= 2;   //because they already have one
            }
            else if ( (*hackers_on_mole) >= 4 ) { //if true, a group of 4 hackers will go to the boat
                isCaptain = true;
                onlyHackers = true;
                (*hackers_on_mole) -= 4; //same decrementing logic here
            }

            break;
        }
    }
    sem_post(mutex);
    sem_post(mole);

    if (isCaptain){ //hacker is going to be the captain of the boat
        sem_wait(boat_available); //semaphore to be sure that even if 2 serfs can be captains, only one boat will be on river at the time
        if (onlyHackers) { //group of 4 hackers
            sem_wait(mutex);

            sem_post(boat_h);
            sem_post(boat_h);
            sem_post(boat_h);

            //hackers boarding
            (*hackers_output) -= 4;
            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: boards: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
            fflush(fp);
            sem_post(writing);

            sem_post(mutex);
        }
        else { //group of 2 hackers and 2 serfs
            sem_wait(mutex);

            sem_post(boat_h);
            sem_post(boat_s);
            sem_post(boat_s);

            //members boarding
            (*hackers_output) -= 2;
            (*serfs_output) -= 2;
            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: boards: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
            fflush(fp);
            sem_post(writing);

            sem_post(mutex);
        }

        generate_delay(arguments.R); //simulation of the boat cruise, when captain is woken, he will let the 3 members exit
        sem_post(on_cruise);
        sem_post(on_cruise);
        sem_post(on_cruise);

        //waiting for members to exit the boat
        sem_wait(captain_write);
        sem_wait(captain_write);
        sem_wait(captain_write);

        //captain exits the boat
        sem_wait(writing);
        fprintf(fp, "%d: HACK %d: captain exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
        fflush(fp);
        sem_post(writing);

        sem_post(boat_available); //next captain can take the boat
        exit(0); //successful exit of process
    }

    //here the hacker is not the captain, just the member
    sem_wait(boat_h); //boarding the boat

    sem_wait(on_cruise); //waiting for captain to wake up

    sem_wait(mutex);

    //hacker exits the boat
    sem_wait(writing);
    fprintf(fp, "%d: HACK %d: member exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
    fflush(fp);
    sem_post(writing);
    sem_post(captain_write); //hacker let know the captain that he exited the boat

    sem_post(mutex);
    exit(0);
}

void gen_serf(int num, arg arguments){
    int my_id = num+1;
    unsigned int mole_occupation = 0;

    bool isCaptain = false;
    bool onlySerfs = false;

    //serf process is started
    sem_wait(writing);
    fprintf(fp, "%d: SERF %d: starts\n", ++(*operation_id), my_id);
    fflush(fp);
    sem_post(writing);

    //serf process is trying to get to the mole
    while (true){
        sem_wait(mole);
        sem_wait(mutex);

        mole_occupation = (*hackers_output) + (*serfs_output); //checking if there si space on the mole
        if (mole_occupation >= arguments.C){
            //serf is leaving the mole because it's full
            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: leaves queue: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
            fflush(fp);
            sem_post(writing);

            sem_post(mutex);
            sem_post(mole);

            generate_delay(arguments.W); //generating delay and letting other serfs to try to get to the mole

            //serf is back and will try to get to the mole again
            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: is back\n", ++(*operation_id), my_id);
            fflush(fp);
            sem_post(writing);

            continue;
        }
        else{
            //serf enters the mole
            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: waits: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), ++(*serfs_output));
            fflush(fp);
            (*serfs_on_mole)++;
            sem_post(writing);

            //serf is checking if he can be the captain
            if ( (*hackers_on_mole) >= 2 && (*serfs_on_mole) >= 2 ){ //if true, a group of 2 serfs and 2 hackers will go to the boat
                isCaptain = true;
                onlySerfs = false;
                (*hackers_on_mole) -= 2; //decrementing these values so that the next serf will now that these persons don't need captain
                (*serfs_on_mole) -= 2;   //because they already have one
            }
            else if ( (*serfs_on_mole) >= 4 ) { //if true, a group of 4 serfs will go to the boat
                isCaptain = true;
                onlySerfs = true;
                (*serfs_on_mole) -= 4; //same decrementing logic here
            }
            break;
        }
    }
    sem_post(mutex);
    sem_post(mole);

    if (isCaptain){ //serf is going to be the captain of the boat
        sem_wait(boat_available); //semaphore to be sure that even if 2 serfs can be captains, only one boat will be on river at the time
        if (onlySerfs) { //group of 4 serfs
            sem_wait(mutex);

            sem_post(boat_s);
            sem_post(boat_s);
            sem_post(boat_s);

            //serfs boarding
            (*serfs_output) -= 4;
            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: boards: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
            fflush(fp);
            sem_post(writing);

            sem_post(mutex);
        }
        else { //group of 2 serfs 2 hackers
            sem_wait(mutex);

            sem_post(boat_s);
            sem_post(boat_h);
            sem_post(boat_h);

            //members boarding
            (*hackers_output) -= 2;
            (*serfs_output) -= 2;
            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: boards: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
            fflush(fp);
            sem_post(writing);

            sem_post(mutex);
        }

        generate_delay(arguments.R); //simulation of the boat cruise, when captain is woken, he will let the 3 members exit
        sem_post(on_cruise);
        sem_post(on_cruise);
        sem_post(on_cruise);

        //waiting for members to exit the boat
        sem_wait(captain_write);
        sem_wait(captain_write);
        sem_wait(captain_write);

        //captain exits the boat
        sem_wait(writing);
        fprintf(fp, "%d: SERF %d: captain exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
        fflush(fp);
        sem_post(writing);

        sem_post(boat_available); //next captain can take the boat
        exit(0); //successful exit of process
    }

    //here the serf is not the captain, just the member
    sem_wait(boat_s); //boarding the boat

    sem_wait(on_cruise); //waiting for captain to wake up

    sem_wait(mutex);

    //serf exits the boat
    sem_wait(writing);
    fprintf(fp, "%d: SERF %d: member exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_output), (*serfs_output));
    fflush(fp);
    sem_post(writing);
    sem_post(captain_write); //serf let know the captain that he exited the boat

    sem_post(mutex);

    exit(0);
}

void generate_delay(int time){
    int gen_time = (random() % (time + 1));
    usleep(gen_time * 1000);
}

int main(int argc, char* argv[]) {
    arg arguments = parseArgs(argc, argv);

    initialization();

    pid_t hackers[arguments.P];
    pid_t serfs[arguments.P];

    pid_t main_pid;
    pid_t hacker_pid;
    pid_t serf_pid;

    setbuf(fp, NULL); //operations not printed in right order fix

    main_pid = fork();
    if (main_pid > 0){ //parent process for hackers generating
        for (unsigned int i = 0; i < arguments.P; i++) {
            if (arguments.H != 0) generate_delay(arguments.H);

            hacker_pid = fork();

            if (hacker_pid  > 0){ //parent process will keep info about pid for further waiting
                hackers[i] = hacker_pid;
            }
            else if (hacker_pid  == 0){ //child process will generate hackers
                gen_hacker(i, arguments);
                exit(0);
            }
            else{ //fork error
                fprintf(stderr, "Error while forking!\n");
                cleanup();
                return(SYS_ERR_MEM);
            }
        }

        for (unsigned int i = 0; i < arguments.P; i++) { //waiting for all hackers to end
            waitpid(hackers[i], NULL, 0);
        }

    }
    else if (main_pid == 0){ // child process for serfs generating
        for (unsigned int i = 0; i < arguments.P; i++) {
            if (arguments.S != 0) generate_delay(arguments.S);

            serf_pid = fork();

            if (serf_pid > 0){ //parent process will keep info about pid for further waiting
                serfs[i] = serf_pid;
            }
            else if (serf_pid == 0){ //child process will generate serfs
                gen_serf(i, arguments);
                exit(0);
            }
            else{ //fork error
                fprintf(stderr, "Error while forking!\n");
                cleanup();
                return(SYS_ERR_MEM);
            }
        }

        for (unsigned int i = 0; i < arguments.P; i++) { //waiting for all serfs to end
            waitpid(serfs[i], NULL, 0);
        }
    }
    else { //fork error
        fprintf(stderr, "Error while forking!\n");
        cleanup();
        return(SYS_ERR_MEM);
    }

    waitpid(main_pid, NULL, 0); //waiting for main process to end (this includes all subprocesses)

    cleanup();
    return SYS_OK;
}
