#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int THREAD_COUNT, ITERATION_COUNT;
int sum = 0;

void *thread() {
    int i;

    for (i = 0; i < ITERATION_COUNT; i++) {
        sum++;
    }

    return NULL;
}

int main(int argc, char const *argv[]) {

    if (argc != 3) {
        printf("Insufficient arguments!\n");
        exit(1);
    }

    THREAD_COUNT = atoi(argv[1]);
    ITERATION_COUNT = atoi(argv[2]);

    int *id = malloc(THREAD_COUNT * sizeof(int));
    pthread_t *threads = malloc(THREAD_COUNT * sizeof(pthread_t));

    int i;

    for (i = 0; i < THREAD_COUNT; i++) {
        
        id[i] = i;
        
        if (pthread_create(&threads[i], NULL, thread, &id[i])) {
            printf("Couldn't create new thread!\n");
            exit(1);
        }
    }

    int j;

    for (j = 0; j < THREAD_COUNT; j++) {
        pthread_join(threads[j], NULL);
    }

    printf("A=%d\n", sum);

    return 0;
}