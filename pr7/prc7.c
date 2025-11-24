#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigtstp_handler(int sig){
	printf("Полученг CTRL+Z - SIGTSTP");
	printf("\nЗавершение работы ффайла\n");
	sleep(2);
	signal(SIGTSTP, SIG_DFL);
	raise(SIGTSTP);
}

int main() {
	signal(SIGTSTP, sigtstp_handler);
	printf("PID - %d\n", getpid());
	printf("Напишите {CTRL+Z} для остановки процесса\n");
	
	fflush(stdout);
	int counter = 0;
	while(1) {
		printf("Работа программы... (итерация: %d)\n", counter++);
		sleep(3);
	}
	return 0;	
}
