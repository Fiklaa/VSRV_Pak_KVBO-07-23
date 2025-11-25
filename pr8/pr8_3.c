#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
	const char *fifo_path = "/tmp/my_fifo";
	char buffer[128];
	mkfifo(fifo_path, 0666);

	if (fork() == 0) {
		int fd = open(fifo_path, O_RDONLY);
		read(fd, buffer, sizeof(buffer));
		printf("Получено в дочернем процессе от родителя: '%s'\n", buffer);
		close(fd);
	} else {
		int fd = open(fifo_path, O_WRONLY);
		char *msg = "Привет от род. процесса! :)";
		write(fd, msg, strlen(msg)+1);
		close(fd);
		wait(NULL);
	}
	
	unlink(fifo_path);

	printf("Завершение работы %d процесса\n", getpid());
	return 0;
}
