#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid;
    char *shm_ptr;
    
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    
    if ((shm_ptr = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс
        printf("Дочерний процесс: Чтение данных...\n");
        sleep(1);  // ждем пока родитель запишет данные
        printf("Дочерний процесс прочитал: %s\n", shm_ptr);
        
        shmdt(shm_ptr);
    } else {
        // Родительский процес
        char message[] = "Привет от родительского процесса!";
        printf("Родительский процесс: Запись данных...\n");
        strcpy(shm_ptr, message);
        printf("Родительский процесс записал: %s\n", message);
        
        wait(NULL);
        
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        printf("Разделяемая память удалена\n");
    }
    
    return 0;
}
