#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdatomic.h>

sem_t *K;           //BSem
sem_t *santa;       //BSem
sem_t *reindeer;    //OSem
sem_t *elves;       //OSem

atomic_int *elf_queue;
atomic_int *reindeer_queue;


void elf_process(int id) {
    sem_wait(K);
    *elf_queue += 1;
    printf("Broj vilenjaka koji čekaju u redu: %d\n", *elf_queue);
    if (*elf_queue == 3) {
        sem_post(santa);
    }
    sem_post(K);
    sem_wait(elves);
    exit(0);
}


void reindeer_process(int id) {
    sem_wait(K);
    *reindeer_queue += 1;
    printf("Broj sobova koji čekaju u redu: %d\n", *reindeer_queue);
    if (*reindeer_queue == 10) {
        sem_post(santa);
    }
    sem_post(K);
    sem_wait(reindeer);
    exit(0);
}


void north_pole_process(int id) {
    srand(time(NULL));
    int perc_odds;
    pid_t pid;

    while (1) {

        sleep((rand() % 3) + 1);

        perc_odds = (rand() % (100 + 1));

        if (perc_odds > 50 && *reindeer_queue != 10) {
            pid = fork();
            if (pid == 0) {
                reindeer_process(0);
                exit(0);
            }
        }

        perc_odds = (rand() % (100 + 1));

        if (perc_odds > 50) {
            pid = fork();
            if (pid == 0) {
                elf_process(0);
                exit(0);
            }
        }
    }

    exit(0);
}


void santa_process(int id) {
    while (1) {
        printf("Djed Božićnjak spava.\n");
        sem_wait(santa);
        printf("Djed Božićnjak se probudio.\n");
        sem_wait(K);
        
        if (*reindeer_queue == 10 && *elf_queue > 0) {
            sem_post(K);
            printf("Pokloni su ukrcani i spremni za razvoženje.\n");
            sleep(2);
            sem_wait(K);

            for(int i = 0; i < 10; i++)
                sem_post(reindeer);

            *reindeer_queue = 0;

            printf("Svi sobovi su otišli na godišnji odmor.\n");
        }

        if (*reindeer_queue == 10) {
            sem_post(K);
            printf("Djed Božićnjak je nahranio sobove.\n");
            sleep(2);
            sem_wait(K);

        }

        while (*elf_queue >= 3) {
            sem_post(K);
            printf("Djed Božićnjak riješio je problem od vilenjaka.\n");
            sleep(2);
            sem_wait(K);

            for(int i = 0; i < 3; i++)
                sem_post(elves);

            *elf_queue -= 3;
        }
        sem_post(K);
    }
    exit(0);
}


int main() {
    signal(SIGCHLD, SIG_IGN);

    int ID;

    ID = shmget(IPC_PRIVATE, sizeof(sem_t) * 4 + sizeof(int) * 2, 0600);

    K = shmat(ID, NULL, 0);

    shmctl(ID, IPC_RMID, NULL);

    santa = (sem_t *) (K + 1);
    reindeer = (sem_t *) (santa + 1);
    elves = (sem_t *) (reindeer + 1);
    elf_queue = (atomic_int *) (elves + 1);
    reindeer_queue = (atomic_int *) (elf_queue + 1);

    sem_init(K, 1, 1);
    sem_init(santa, 1, 0);
    sem_init(reindeer, 1, 0);
    sem_init(elves, 1, 0);
    *elf_queue = 0;
    *reindeer_queue = 0;
    
    if (fork() == 0) {
        north_pole_process(0);
        exit(0);
    }

    if (fork() == 0) {
        santa_process(0);
        exit(0);
    }

    while (1) {
        wait(NULL);
    }

    sem_destroy(K);
    sem_destroy(santa);
    sem_destroy(reindeer);
    sem_destroy(elves);

    shmdt(K);

    return 0;
}
