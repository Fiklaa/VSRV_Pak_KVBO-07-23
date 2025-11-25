#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1



int main() {
	char *msg = "Привет от родительского процесса!";	
	int fd[2], bytesRead;
	char buffer[128];

	pipe(fd);

	if (fork() == 0) {
		printf("Работа дочернего процесса - %d!\n", getpid());
		close(fd[READ]);
		write(fd[WRITE], msg, strlen(msg)+1);
		close(fd[WRITE]);
	} else {
		printf("Работа родительского процесса - %d\n", getpid());
		close(fd[WRITE]);
		bytesRead = read(fd[READ], buffer, sizeof(buffer));
		printf("Передано от родительского процесса - '%s'\n", buffer);
		close(fd[READ]);
	}
	printf("Завершение работы программы %d!\n", getpid());
	return 0;

}
