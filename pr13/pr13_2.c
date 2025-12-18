#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define MAX_FOOD 1000
#define MIN_FOOD 100
#define CATS_COUNT 5
#define OWNER_PORTION 150
#define MIN_EAT_PORTION 10
#define MAX_EAT_PORTION 50

int feeder_food = MAX_FOOD;
bool running = true;

pthread_mutex_t feeder_mutex = PTHREAD_MUTEX_INITIALIZER;

void* owner_thread(void* arg) {
    printf("Хозяин: начал работу\n");
    
    while (running) {
        sleep(1);
        
        pthread_mutex_lock(&feeder_mutex);
        
        int current_food = feeder_food;
        int to_add = OWNER_PORTION;
        
        if (current_food + to_add > MAX_FOOD) {
            to_add = MAX_FOOD - current_food;
        }
        
        if (to_add > 0) {
            feeder_food += to_add;
            printf("Хозяин: добавил %d грамм корма. Всего: %d грамм\n", 
                   to_add, feeder_food);
        } else {
            printf("Хозяин: кормушка полна (%d грамм)\n", feeder_food);
        }
        
        pthread_mutex_unlock(&feeder_mutex);
    }
    
    printf("Хозяин: завершил работу\n");
    return NULL;
}

void* cat_thread(void* arg) {
    int cat_id = *(int*)arg;
    printf("Кот %d: начал есть\n", cat_id);
    
    while (running) {
        usleep(rand() % 900000); // от 0 до 0.9 секунды
        
        pthread_mutex_lock(&feeder_mutex);
        
        int portion = MIN_EAT_PORTION + rand() % (MAX_EAT_PORTION - MIN_EAT_PORTION + 1);
        
        if (portion > feeder_food) {
            portion = feeder_food;
        }
        
        if (portion > 0) {
            feeder_food -= portion;
            printf("Кот %d: съел %d грамм корма за один прием. Осталось: %d грамм\n", 
                   cat_id, portion, feeder_food);
            
            if (feeder_food < MIN_FOOD) {
                printf("Кот %d: Внимание! Корма осталось мало: %d грамм\n", 
                       cat_id, feeder_food);
            }
        } else {
            printf("Кот %d: Кормушка пуста, жду...\n", cat_id);
        }
        
        pthread_mutex_unlock(&feeder_mutex);
        
        usleep(rand() % 800000); // от 0 до 0.8 секунды
    }
    
    printf("Кот %d: закончил есть\n", cat_id);
    return NULL;
}

int main() {
    srand(time(NULL));
    
    pthread_t owner;
    pthread_t cats[CATS_COUNT];
    int cat_ids[CATS_COUNT];
    
    printf("=== Программа 'Покорми кота' ===\n");
    printf("Начальное количество корма: %d грамм\n", feeder_food);
    printf("Максимум корма: %d грамм\n", MAX_FOOD);
    printf("Минимум корма: %d грамм\n", MIN_FOOD);
    printf("Количество котов: %d\n", CATS_COUNT);
    printf("\n");
    
    if (pthread_create(&owner, NULL, owner_thread, NULL) != 0) {
        perror("Ошибка создания потока хозяина");
        return 1;
    }
    
    for (int i = 0; i < CATS_COUNT; i++) {
        cat_ids[i] = i + 1;
        if (pthread_create(&cats[i], NULL, cat_thread, &cat_ids[i]) != 0) {
            perror("Ошибка создания потока кота");
            return 1;
        }
    }
    
    printf("Программа работает 20 секунд...\n\n");
    sleep(20);
    
    running = false;
    
    pthread_join(owner, NULL);
    for (int i = 0; i < CATS_COUNT; i++) {
        pthread_join(cats[i], NULL);
    }
    
    printf("\n=== Программа завершена ===\n");
    printf("Итоговое количество корма в кормушке: %d грамм\n", feeder_food);
    
    pthread_mutex_destroy(&feeder_mutex);
    
    return 0;
}
