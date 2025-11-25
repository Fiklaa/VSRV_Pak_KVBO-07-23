#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define FIFO_FILE "/tmp/fifo_twoway"

void reverse_str(char *);

int main() {
	int fd;
	char read_buffer[128];
	char end[10];
	int to_end;
	int read_bytes;

	mkfifo(FIFO_FILE, S_IFIFO|0640);
	strcpy(end, "end");
	fd = open(FIFO_FILE, O_RDWR);

	while(1) {
		read_bytes = read(fd, read_buffer, sizeof(read_buffer));
		read_buffer[read_bytes] = "\0";
		printf("Сервер: Получена строка '%s'\n", read_buffer);
		to_end = strcmp(read_buffer, end);

		if (to_end == 0) {
			close(fd);
			break;
		}
		reverse_str(read_buffer);
		printf("Сервер: Отправаляю клиенту зеркальную строку '%s'\n", read_buffer);
		write(fd, read_buffer, strlen(read_buffer));
		sleep(2);
	}
	return 0;
}

void reverse_str(char *str) {
	int last, limit, first;
	char temp;
	last = strlen(str) - 1;
	limit = last/2;
	first = 0;

	while (first < last) {
		temp = str[first];
		str[first] = str[last];
		str[last] = temp;
		first++;
		last--;
	}
	return;
}

