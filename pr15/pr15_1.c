#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_ITERATIONS 5

sem_t semA, semB, semC, semModule;

void* produceA(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sleep(1);
        printf("Деталь A произведена (итерация %d)\n", i + 1);
        sem_post(&semA);
    }
    return NULL;
}

void* produceB(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sleep(2);
        printf("Деталь B произведена (итерация %d)\n", i + 1);
        sem_post(&semB);
    }
    return NULL;
}

void* produceC(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sleep(3);
        printf("Деталь C произведена (итерация %d)\n", i + 1);
        sem_post(&semC);
    }
    return NULL;
}

void* assembleModule(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sem_wait(&semA);
        sem_wait(&semB);
        printf("Модуль (A+B) собран (итерация %d)\n", i + 1);
        sem_post(&semModule);
    }
    return NULL;
}

void* assembleProduct(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sem_wait(&semModule);
        sem_wait(&semC);
	printf("Изделие (модуль + C) собрано (итерация %d)\n", i + 1);
        printf("---\n");
    }
    return NULL;
}

int main() {
    pthread_t threadA, threadB, threadC, threadModule, threadProduct;

    sem_init(&semA, 0, 0);
    sem_init(&semB, 0, 0);
    sem_init(&semC, 0, 0);
    sem_init(&semModule, 0, 0);

    pthread_create(&threadA, NULL, produceA, NULL);
    pthread_create(&threadB, NULL, produceB, NULL);
    pthread_create(&threadC, NULL, produceC, NULL);
    pthread_create(&threadModule, NULL, assembleModule, NULL);
    pthread_create(&threadProduct, NULL, assembleProduct, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    pthread_join(threadModule, NULL);
    pthread_join(threadProduct, NULL);

    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);
    sem_destroy(&semModule);

    printf("Производство завершено.\n");
    return 0;
}
