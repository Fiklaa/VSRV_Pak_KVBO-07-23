#include <stdio.h>
#include <pthread.h>

pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

void* my_thread(void* arg) {
    pthread_mutex_lock(&a_mutex);
    printf("Поток %ld: ждёт сигнал...\n", (long)arg);
    
    pthread_cond_wait(&got_request, &a_mutex);
        
    printf("Поток %ld: получил сигнал!\n", (long)arg);
    pthread_mutex_unlock(&a_mutex);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, my_thread, (void*)1);
    pthread_create(&t2, NULL, my_thread, (void*)2);
    
    sleep(1);
    printf("Типо сервер: отправляю сигнал...\n");
    pthread_cond_signal(&got_request);
    
    sleep(1);
    printf("Типо сервер: отправляю ещё один сигнал...\n");
    pthread_cond_signal(&got_request);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    return 0;
}
