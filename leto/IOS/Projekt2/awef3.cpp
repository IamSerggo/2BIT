//
// Created by radovan on 27.4.2019.
//

void gen_hacker(int num, arg arguments){
    int my_id = num+1;
    bool isCaptain = false;
    bool hackersBoat = false;

    sem_wait(writing);
    fprintf(fp, "%d: HACK %d: starts\n", ++(*operation_id), my_id);
    fflush(fp);
    sem_post(writing);

    while (true){
        sem_wait(mutex);
        sem_wait(mole);
        if ( (*hackers_on_mole) + (*serfs_on_mole) > (*mole_cap) ){
            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: leaves queue: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
            fflush(fp);
            sem_post(writing);

            sem_post(mole);
            sem_post(mutex);

            generate_delay(arguments.W);

            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: is back\n", ++(*operation_id), my_id);
            fflush(fp);
            sem_post(writing);

            continue;
        }
        else {
            sem_wait(writing);
            fprintf(fp, "%d: HACK %d: waits: %d: %d\n", ++(*operation_id), my_id, ++(*hackers_on_mole), (*serfs_on_mole));
            fflush(fp);
            sem_post(writing);
            sem_post(mutex);

            sem_post(mole);
            break;
        }
    }

    if (isBoatAvailable){
        sem_wait(mutex);
        if ( (*hackers_on_mole) >= 4){
            isCaptain = true;
            hackersBoat = true;
            isBoatAvailable = false;
            sem_post(boat_h);
            sem_post(boat_h);
            sem_post(boat_h);
            sem_post(boat_h);
        }
        else if ( (*hackers_on_mole) >= 2 && (*serfs_on_mole) >= 2 ){
            isCaptain = true;
            hackersBoat = false;
            isBoatAvailable = false;
            sem_post(boat_s);
            sem_post(boat_s);
            sem_post(boat_h);
            sem_post(boat_h);
        }
        sem_post(mutex);
    }

    sem_wait(boat_h);
    printf("hacker %d on boat\n", my_id);
    if (isCaptain) {
        sem_wait(boat_available);

        sem_wait(mutex);
        if (hackersBoat) (*hackers_on_mole) -= 4;
        else{
            (*hackers_on_mole) -= 2;
            (*serfs_on_mole) -= 2;
        }
        sem_post(mutex);

        sem_wait(writing);
        fprintf(fp, "%d: HACK %d: boards: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
        fflush(fp);
        sem_post(writing);

        captain = getpid();

        sem_post(on_cruise);
        sem_post(on_cruise);
        sem_post(on_cruise);
        //printf("hack %d before delay\n", my_id);
        generate_delay(arguments.R);
        //printf("hack %d after delay\n", my_id);

        sem_wait(captain_write);
        sem_wait(captain_write);
        sem_wait(captain_write);
        sem_wait(writing);
        fprintf(fp, "%d: HACK %d: captain exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
        fflush(fp);
        sem_post(writing);
        sem_post(boat_available);
        isBoatAvailable = true;
    }
    else{
        sem_wait(on_cruise);

        //printf("hack %d before wait\n", my_id);
        waitpid(captain, NULL, 0);
        //printf("hack %d after wait\n", my_id);

        sem_wait(writing);
        fprintf(fp, "%d: HACK %d: member exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
        fflush(fp);
        sem_post(writing);
        sem_post(captain_write);
    }
}

void gen_serf(int num, arg arguments){
    int my_id = num+1;
    bool isCaptain = false;
    bool serfBoat = false;

    sem_wait(writing);
    fprintf(fp, "%d: SERF %d: starts\n", ++(*operation_id), my_id);
    fflush(fp);
    sem_post(writing);

    while (true){
        sem_wait(mutex);
        sem_wait(mole);
        if ( (*hackers_on_mole) + (*serfs_on_mole) > (*mole_cap) ){
            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: leaves queue: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
            fflush(fp);
            sem_post(writing);

            sem_post(mole);
            sem_post(mutex);

            generate_delay(arguments.W);

            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: is back\n", ++(*operation_id), my_id);
            fflush(fp);
            sem_post(writing);

            continue;
        }
        else {
            sem_wait(writing);
            fprintf(fp, "%d: SERF %d: waits: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), ++(*serfs_on_mole));
            fflush(fp);
            sem_post(writing);
            sem_post(mutex);

            sem_post(mole);
            break;
        }
    }

    if (isBoatAvailable){
        sem_wait(mutex);
        if ( (*serfs_on_mole) >= 4){
            isCaptain = true;
            serfBoat = true;
            isBoatAvailable = false;
            sem_post(boat_s);
            sem_post(boat_s);
            sem_post(boat_s);
            sem_post(boat_s);
        }
        else if ( (*hackers_on_mole) >= 2 && (*serfs_on_mole) >= 2 ){
            isCaptain = true;
            serfBoat = false;
            isBoatAvailable = false;
            sem_post(boat_s);
            sem_post(boat_s);
            sem_post(boat_h);
            sem_post(boat_h);
        }
        sem_post(mutex);
    }

    sem_wait(boat_s);

    if (isCaptain) {
        sem_wait(boat_available);

        sem_wait(mutex);
        if (serfBoat) (*serfs_on_mole) -= 4;
        else{
            (*serfs_on_mole) -= 2;
            (*hackers_on_mole) -= 2;
        }
        sem_post(mutex);

        sem_wait(writing);
        fprintf(fp, "%d: SERF %d: boards: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
        fflush(fp);
        sem_post(writing);

        captain = getpid();

        sem_post(on_cruise);
        sem_post(on_cruise);
        sem_post(on_cruise);
        //printf("serf %d before delay\n", my_id);
        generate_delay(arguments.R);
        //printf("serf %d after delay\n", my_id);

        sem_wait(captain_write);
        sem_wait(captain_write);
        sem_wait(captain_write);
        sem_wait(writing);
        fprintf(fp, "%d: SERF %d: captain exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
        fflush(fp);
        sem_post(writing);
        sem_post(boat_available);
        isBoatAvailable = true;

    }
    else{
        sem_wait(on_cruise);
        //printf("serf %d before wait\n", my_id);
        waitpid(captain, NULL, 0);
        //printf("serf %d after wait\n", my_id);

        sem_wait(writing);
        fprintf(fp, "%d: SERF %d: member exits: %d: %d\n", ++(*operation_id), my_id, (*hackers_on_mole), (*serfs_on_mole));
        fflush(fp);
        sem_post(writing);
        sem_post(captain_write);
    }
}

(*boat_wait_h) += (*hackers_on_mole);
(*boat_wait_s) += (*serfs_on_mole);

if ( (*boat_wait_h) >= 2 && (*boat_wait_s) >= 2 ){
isCaptain = true;
onlyHackers = false;
(*boat_wait_h) -= 2;
(*boat_wait_s) -= 2;
}
else if ( (*boat_wait_h) >= 4 ) {
isCaptain = true;
onlyHackers = true;
(*boat_wait_h) -= 4;
}