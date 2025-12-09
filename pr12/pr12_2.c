#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

void* timed_func(void* arg) {
    struct timeval now;
    struct timespec timeout;
    
    pthread_mutex_lock(&a_mutex);
    gettimeofday(&now, NULL);
    
    timeout.tv_sec = now.tv_sec + 2;
    timeout.tv_nsec = now.tv_usec * 1000;
    
    if (timeout.tv_nsec >= 1000000000) {
    	timeout.tv_sec += timeout.tv_nsec / 1000000000;
        timeout.tv_nsec %= 1000000000;
    }

    printf("Поток: жду сигнал (таймаут 2 сек)...\n");
    
    int rc = pthread_cond_timedwait(&got_request, &a_mutex, &timeout);
    
    if (rc == ETIMEDOUT) {
        printf("Поток: таймаут! Сигнал не поступил.\n");
    } else {
        printf("Поток: получил сигнал вовремя!\n");
    }
    
    pthread_mutex_unlock(&a_mutex);
    return NULL;
}

int main() {
    pthread_t t;
    
    if (pthread_create(&t, NULL, timed_func, NULL) != 0) {
        perror("Ошибка создания потока");
        return 1;
    }
    
    if (pthread_join(t, NULL) != 0) {
        perror("Ошибка ожидания потока");
        return 1;
    }

    return 0;
}
