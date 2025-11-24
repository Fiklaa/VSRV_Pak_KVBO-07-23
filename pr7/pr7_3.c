#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

volatile sig_atomic_t timeout = 0;

void hendler_alarm(int sig) {
	printf("Нет ввода! Ответ неверный >;(\n");
       	_exit(0);

}

int main() {

	char text[100];
	char *result;

	signal(SIGALRM, hendler_alarm);

	printf("Вопрос от Жака Фреско, на раздумье даётся 10 секунд: Что?\n");
	fflush(stdout);

	alarm(10);

	result = fgets(text, sizeof(text), stdin);
	
	int saved_err = errno;

	alarm(0);

	if (timeout || (result == NULL && saved_err == EINTR)) {
		
		printf("\nНет ввода\n");
		if (result == NULL) {
			printf("fgets вернула NULL, errno = %d (%s)\n", errno, strerror(errno));
		}
	} else if (result == NULL) {
		printf("Ошибка ввода: %s \n", strerror(errno));
	}else {
		printf("правильный ответ, СПАСИБО!\nВы ввели %s\n", text);
	}
	return 0;
}
