#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_PRODUCERS 2
#define NUM_TASKS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int task_count = 0;


void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_TASKS; i++) {
        usleep(rand() % 500000 + 100000);
        
        pthread_mutex_lock(&mutex);
        task_count++;
        printf("Генератор %d создал задачу. Всего задач: %d\n", id, task_count);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        
        while (task_count == 0) {
            printf("Обработчик: нет задач, жду...\n");
            pthread_cond_wait(&cond, &mutex);
        }
        
        task_count--;
        printf("Обработчик: выполнил одну задачу. Осталось: %d\n", task_count);
        
        pthread_mutex_unlock(&mutex);
        
        usleep(300000);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t prod_threads[NUM_PRODUCERS], cons_thread;
    int ids[NUM_PRODUCERS];
    
    pthread_create(&cons_thread, NULL, consumer, NULL);
    
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i;
        pthread_create(&prod_threads[i], NULL, producer, &ids[i]);
    }
    
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    
    sleep(2);
    printf("Все задачи созданы и обработаны.\n");
    
    return 0;
}
