#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PHILO 5
#define DELAY 30000
#define FOOD 50

pthread_mutex_t forks[PHILO];
pthread_mutex_t table_mutex;
pthread_cond_t cond_var;    
pthread_t phils[PHILO];
void *philosopher(void *id);
int food_on_table();
int try_get_forks(int, int, int);
void put_forks(int, int);
pthread_mutex_t foodlock;

int sleep_seconds = 0;

int main(int argn, char **argv) {
    int i;

    if (argn == 2)
        sleep_seconds = atoi(argv[1]);

    pthread_mutex_init(&foodlock, NULL);
    pthread_mutex_init(&table_mutex, NULL);
    pthread_cond_init(&cond_var, NULL);
    
    for (i = 0; i < PHILO; i++)
        pthread_mutex_init(&forks[i], NULL);
    
    for (i = 0; i < PHILO; i++)
        pthread_create(&phils[i], NULL, philosopher, (void *)i);
    
    for (i = 0; i < PHILO; i++)
        pthread_join(phils[i], NULL);
    
    pthread_mutex_destroy(&table_mutex);
    pthread_cond_destroy(&cond_var);
    
    return 0;
}

void *philosopher(void *num) {
    int id;
    int left_fork, right_fork, f;
    int got_forks = 0;

    id = (int)num;
    printf("Philosopher %d sitting down to dinner.\n", id);
    right_fork = id;
    left_fork = id + 1;

    if (left_fork == PHILO)
        left_fork = 0;

    while ((f = food_on_table())) {
        if (id == 1)
            sleep(sleep_seconds);

        printf("Philosopher %d: get dish %d.\n", id, f);
        
        got_forks = 0;
        while (!got_forks) {
            got_forks = try_get_forks(id, left_fork, right_fork);
            if (!got_forks) {

                pthread_mutex_lock(&table_mutex);
                pthread_cond_wait(&cond_var, &table_mutex);
                pthread_mutex_unlock(&table_mutex);
            }
        }

        printf("Philosopher %d: eating.\n", id);
        usleep(DELAY * (FOOD - f + 1));
        
        put_forks(left_fork, right_fork);
        
        pthread_cond_broadcast(&cond_var);
    }
    printf("Philosopher %d is done eating.\n", id);
    return (NULL);
}

int try_get_forks(int phil, int left_fork, int right_fork) {
    int got_left = 0;
    int got_right = 0;
    
    pthread_mutex_lock(&table_mutex);
    
    if (pthread_mutex_trylock(&forks[left_fork]) == 0) {
        got_left = 1;
        
        if (pthread_mutex_trylock(&forks[right_fork]) == 0) {
            got_right = 1;
        } else {
            pthread_mutex_unlock(&forks[left_fork]);
        }
    }
    
    pthread_mutex_unlock(&table_mutex);
    
    if (got_left && got_right) {
        printf("Philosopher %d: got left fork %d and right fork %d\n", 
               phil, left_fork, right_fork);
        return 1;
    }
    
    return 0;
}

void put_forks(int left_fork, int right_fork) {
    pthread_mutex_unlock(&forks[left_fork]);
    pthread_mutex_unlock(&forks[right_fork]);
}

int food_on_table() {
    static int food = FOOD;
    int myfood;

    pthread_mutex_lock(&foodlock);
    if (food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock(&foodlock);
    return myfood;
}
