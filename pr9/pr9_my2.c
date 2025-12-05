#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_TEXT 100

struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    key_t key = ftok("msgfile", 65);
    int msgid;
    struct message msg;
    
    if ((msgid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс
        printf("Дочерний процесс: Ожидание сообщений...\n");
        
        
        while (1) {
            if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) < 0) {
                perror("msgrcv");
                exit(1);
            }
            
            printf("Дочерний процесс получил: %s\n", msg.msg_text);
            
            if (strcmp(msg.msg_text, "exit") == 0) {
                printf("Дочерний процесс завершает работу\n");
                break;
            }
        }
    } else {
        // Родительский процесс
        char input[MAX_TEXT];
        msg.msg_type = 1;
        
        printf("Родительский процесс: Введите сообщения (для выхода введите 'exit'):\n");
        
        while (1) {
            printf("> ");
            fgets(input, MAX_TEXT, stdin);
            input[strcspn(input, "\n")] = 0;
            
            strcpy(msg.msg_text, input);
            
            if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) < 0) {
                perror("msgsnd");
                exit(1);
            }
            
            printf("Родительский процесс отправил: %s\n", input);
            
            if (strcmp(input, "exit") == 0) {
                printf("Родительский процесс завершает работу\n");
                break;
            }
        }
        
        wait(NULL);
        
        if (msgctl(msgid, IPC_RMID, NULL) < 0) {
            perror("msgctl");
            exit(1);
        }
        printf("Очередь сообщений удалена\n");
    }
    
    return 0;
}
