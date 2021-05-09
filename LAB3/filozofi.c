#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t monitor;
pthread_cond_t cond_queue[5];

char philosophers[5];
int forks[5];

void thinking(int seconds) {
    sleep(seconds);
}

void eating(int philosopher_id) {
    pthread_mutex_lock(&monitor);

    int i;
    
    philosophers[philosopher_id] = 'o';
    
    while (forks[philosopher_id] == 0 || forks[(philosopher_id + 1) % 5] == 0) {
        pthread_cond_wait(&cond_queue[philosopher_id], &monitor);
    }
    
    forks[philosopher_id] = 0;
    forks[(philosopher_id + 1) % 5] = 0;

    philosophers[philosopher_id] = 'X';

    for (i = 0; i < 5; i++) {
        printf("%c ", philosophers[i]);
    }
    printf("(%d)", philosopher_id + 1);
    printf("\n");


    pthread_mutex_unlock(&monitor);

    sleep(2);   //eating

    pthread_mutex_lock(&monitor);
    
    philosophers[philosopher_id] = 'O';
    forks[philosopher_id] = 1;
    forks[(philosopher_id + 1) % 5] = 1;

    pthread_cond_signal(&cond_queue[(philosopher_id - 1) % 5]);
    pthread_cond_signal(&cond_queue[(philosopher_id + 1) % 5]);

    for (i = 0; i < 5; i++) {
        printf("%c ", philosophers[i]);
    }
    printf("(%d)", philosopher_id + 1);
    printf("\n");

    pthread_mutex_unlock(&monitor);
}

void *philosopher_thread(void *thread_id_ptr) {
    while (1) {
        thinking(3);
        eating(*(int *)thread_id_ptr);
    }
}

int main() {
    pthread_t threads[5];
    int thread_id[5];
    int i;

    for (i = 0; i < 5; i++) {
        philosophers[i] = '0';
        forks[i] = 1;
    }

    pthread_mutex_init(&monitor, NULL);
    
    for (i = 0; i < 5; i++)
        pthread_cond_init(&cond_queue[i], NULL);

    for (i = 0; i < 5; i++) {
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, philosopher_thread, &thread_id[i]);
    }

    for (i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);
    
    pthread_mutex_destroy(&monitor);
    
    for (i = 0; i < 5; i++)
        pthread_cond_destroy(&cond_queue[i]);

    return 0;
}