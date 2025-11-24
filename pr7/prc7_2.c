#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler_zerodiv(int signum);

int main() {
	int result;
	int v1, v2;
	void (*sigHandlerReturn)(int);
	sigHandlerReturn = signal(SIGFPE, handler_zerodiv);
	if (sigHandlerReturn == SIG_ERR) {
		perror("Signal error: \n");
		return 1;
	}
	v1 = 1;
	v2 = 0;
	result = v1/v2;
	printf("Результат деления на 0 = %d\n", result);
	return 0;
}

void handler_zerodiv(int signum) {
	if (signum == SIGFPE) {
		printf("Получено SIGFPE, Деление на 0 нельзя так делать))\n");
		exit(0);
	} else {
		printf("Получен %d сигнал\n", signum);
		return;
	}

}
