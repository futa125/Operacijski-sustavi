#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

atomic_int *NUM, *ENTERING;
int THREAD_COUNT, ITERATION_COUNT;
int sum;

int max(atomic_int *array) {
   
    int i, maxNum = 0;

    for (i = 0; i < THREAD_COUNT; ++i) {
        if (*(array + i) > maxNum)
            maxNum = *(array + i);
    }

    return maxNum;
}

void *thread(void *threadIndex) {
   int i, j;
   int *threadIndexPtr = threadIndex;

   for (i = 0; i < ITERATION_COUNT; i++) {

        ENTERING[*threadIndexPtr] = 1;
        NUM[*threadIndexPtr] = max(NUM) + 1;
        ENTERING[*threadIndexPtr] = 0;

        for (j = 0; j < THREAD_COUNT; j++) {
            while (ENTERING[j] != 0);
            while (NUM[j] != 0 && (NUM[j] < NUM[*threadIndexPtr] || (NUM[j] == NUM[*threadIndexPtr] && j < *threadIndexPtr)));
        }

        sum += 1;

        NUM[*threadIndexPtr] = 0;
   }

   return NULL;
}

int main(int argc, char const *argv[]) {
    sum = 0;

    if (argc != 3) {
        printf("Insufficient arguments!\n");
        exit(1);
    }

    THREAD_COUNT = atoi(argv[1]);
    ITERATION_COUNT = atoi(argv[2]);

    int *id = (int *)malloc(THREAD_COUNT * sizeof(int));
    pthread_t threads[THREAD_COUNT];

    ENTERING = (atomic_int *)malloc(THREAD_COUNT * sizeof(atomic_int));
    NUM = (atomic_int *)malloc(THREAD_COUNT * sizeof(atomic_int));

    for (int i = 0; i < THREAD_COUNT; i++) {
        ENTERING[i] = 0;
        NUM[i] = 0;
    }
    
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