#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int PROCESS_COUNT, ITERATION_COUNT;
int *sum;

void process() {
    int i;

    for (i = 0; i < ITERATION_COUNT; i++) {
        *sum += 1;
    }

}

int main(int argc, char const *argv[]) {

    if (argc != 3) {
        printf("Insufficient arguments!\n");
        exit(1);
    }

    PROCESS_COUNT = atoi(argv[1]);
    ITERATION_COUNT = atoi(argv[2]);

    int shmId = shmget(IPC_PRIVATE, sizeof(int), 0600);

    if (shmId == -1) {
        exit(1);
    }

    sum = (int *) shmat(shmId, NULL, 0);
    *sum = 0;

    int i;

    for (i = 0; i < PROCESS_COUNT; i++) {
        if (fork() == 0) {
            process(i);
            exit(0);
        }
    } 

    for (int j = 0; j < PROCESS_COUNT; j++) {
        wait(NULL);
    }

    printf("A=%d\n", *sum);

    (void) shmdt((char *) sum);
    (void) shmctl(shmId, IPC_RMID, NULL);

    return 0;
}