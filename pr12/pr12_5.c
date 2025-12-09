#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_HANDLER_THREADS 3

pthread_mutex_t list_mutex;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

int num_requests = 0;

struct request {
    int number;
    struct request* next;
};

struct request* requests = NULL;
struct request* last_request = NULL;

void add_request(int request_num) {
    struct request* a_request = (struct request*)malloc(sizeof(struct request));
    if (!a_request) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }
    a_request->number = request_num;
    a_request->next = NULL;

    pthread_mutex_lock(&list_mutex);

    if (num_requests == 0) {
        requests = a_request;
        last_request = a_request;
    } else {
        last_request->next = a_request;
        last_request = a_request;
    }
    num_requests++;

    printf("Добавлен запрос %d (всего в очереди: %d)\n", request_num, num_requests);
    
    pthread_mutex_unlock(&list_mutex);
    
    pthread_mutex_lock(&cond_mutex);
    pthread_cond_signal(&got_request);
    pthread_mutex_unlock(&cond_mutex);
}

struct request* get_request() {
    struct request* a_request = NULL;
    
    pthread_mutex_lock(&list_mutex);
    
    if (num_requests > 0) {
        a_request = requests;
        requests = a_request->next;
        if (requests == NULL) {
            last_request = NULL;
        }
        num_requests--;
    }
    
    pthread_mutex_unlock(&list_mutex);
    
    return a_request;
}

void handle_request(struct request* a_request, int thread_id) {
    if (a_request) {
        printf("Поток %d обработал запрос %d\n", thread_id, a_request->number);
    }
}

void* handle_requests_loop(void* data) {
    int thread_id = *((int*)data);
    printf("Поток %d запущен\n", thread_id);
    
    while (1) {
        pthread_mutex_lock(&list_mutex);
        int has_requests = (num_requests > 0);
        pthread_mutex_unlock(&list_mutex);
        
        if (has_requests) {
            struct request* req = get_request();
            if (req) {
                handle_request(req, thread_id);
                free(req);
            }
        } else {
            pthread_mutex_lock(&cond_mutex);
            printf("Поток %d: нет задач, жду...\n", thread_id);
            pthread_cond_wait(&got_request, &cond_mutex);
            pthread_mutex_unlock(&cond_mutex);
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_HANDLER_THREADS];
    int thread_ids[NUM_HANDLER_THREADS];
    
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&list_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    
    srand(time(NULL));
    
    for (int i = 0; i < NUM_HANDLER_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, handle_requests_loop, &thread_ids[i]);
    }
    
    sleep(1);
    
    for (int i = 0; i < 10; i++) {
        add_request(i);
        if (rand() % 4 == 0) {
            usleep(100000);
        }
    }
    
    sleep(2);
    
    printf("Все запросы обработаны. Завершаем.\n");
    
    for (int i = 0; i < NUM_HANDLER_THREADS; i++) {
        pthread_cancel(threads[i]);
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&list_mutex);
    pthread_mutex_destroy(&cond_mutex);
    pthread_cond_destroy(&got_request);
    
    return 0;
}
