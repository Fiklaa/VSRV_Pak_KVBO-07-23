#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int number;
    int ready;
    int processed;
    pthread_mutex_t mutex;
    pthread_cond_t cond_even;
    pthread_cond_t cond_odd;
    pthread_cond_t cond_new_number;
} SharedData;

void* number_generator(void* arg) {
    SharedData* data = (SharedData*)arg;
    
    while (1) {
        pthread_mutex_lock(&data->mutex);
        
        while (data->ready == 1 && data->processed == 0) {
            pthread_cond_wait(&data->cond_new_number, &data->mutex);
        }
        
        int num = rand() % 100;
        data->number = num;
        data->ready = 1;
        data->processed = 0;
        
        printf("Генератор: Сгенерировано число: %d\n", num);
        
        if (num % 2 == 0) {
            pthread_cond_signal(&data->cond_even);
        } else {
            pthread_cond_signal(&data->cond_odd);
        }
        
        pthread_mutex_unlock(&data->mutex);
        
        sleep(1);
    }
    
    return NULL;
}

void* even_processor(void* arg) {
    SharedData* data = (SharedData*)arg;
    
    while (1) {
        pthread_mutex_lock(&data->mutex);
        
        while (data->ready == 0 || data->number % 2 != 0 || data->processed == 1) {
            pthread_cond_wait(&data->cond_even, &data->mutex);
        }
        
        if (data->ready == 1 && data->number % 2 == 0 && data->processed == 0) {
            int num = data->number;
            int result = num * num;
            printf("Обрабочтик 1: Четное число %d, квадрат: %d\n", num, result);
            
            data->processed = 1;
            
            pthread_cond_signal(&data->cond_new_number);
        }
        
        pthread_mutex_unlock(&data->mutex);
    }
    
    return NULL;
}

void* odd_processor(void* arg) {
    SharedData* data = (SharedData*)arg;
    
    while (1) {
        pthread_mutex_lock(&data->mutex);
        
        while (data->ready == 0 || data->number % 2 == 0 || data->processed == 1) {
            pthread_cond_wait(&data->cond_odd, &data->mutex);
        }
        
        if (data->ready == 1 && data->number % 2 != 0 && data->processed == 0) {
            int num = data->number;
            int result = num * num * num;
            printf("Обработчикк 2: Нечетное число %d, куб: %d\n", num, result);
            
            data->processed = 1;
            
            pthread_cond_signal(&data->cond_new_number);
        }
        
        pthread_mutex_unlock(&data->mutex);
    }
    
    return NULL;
}

int main() {
    srand(time(NULL));
    
    SharedData data;
    data.number = 0;
    data.ready = 0;
    data.processed = 0;
    
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond_even, NULL);
    pthread_cond_init(&data.cond_odd, NULL);
    pthread_cond_init(&data.cond_new_number, NULL);
    
    pthread_t generator_thread, even_thread, odd_thread;
    
    pthread_create(&generator_thread, NULL, number_generator, &data);
    pthread_create(&even_thread, NULL, even_processor, &data);
    pthread_create(&odd_thread, NULL, odd_processor, &data);
    
    sleep(10);
    
    printf("\nПрограмма завершена после 10 секунд работы.\n");
    
    
    return 0;
}
