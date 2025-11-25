#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
	int fd, end_process, stringlen, read_bytes;
	char read_buffer[128];
	char end_str[5];

	printf("Клиент: Бесконечное общение с сервером пока не напишешь 'end'\n");

	fd = open(FIFO_FILE, O_CREAT|O_RDWR);
	strcpy(end_str, "end");

	while(1) {
		printf("Введитие сообщение:\n");
		fgets(read_buffer, sizeof(read_buffer), stdin);
		stringlen = strlen(read_buffer);
		read_buffer[stringlen - 1] = '\0';
		end_process = strcmp(read_buffer, end_str);

		if (end_process != 0) {
			write(fd, read_buffer, strlen(read_buffer));
			printf("Клиент: Отправка сообщения '%s'\n", read_buffer);
			read_bytes = read(fd, read_buffer, sizeof(read_buffer));
			read_buffer[read_bytes] = '\0';
			printf("Клиент: Получено сообщение от сервера - '%s'\n", read_buffer);
		} else {
			write(fd, read_buffer, strlen(read_buffer));
			printf("Клиент: Отправка сообщения '%s'\n", read_buffer);
			close(fd);
			break;
		}

	}
	return 0;

}
