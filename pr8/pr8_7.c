#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	int fd[2];
	int numbers[] = {2, 5, 7, 3, 6, 15};
	int num_count = 6;

	pipe(fd);

	if (fork() == 0) {
		close(fd[1]);
		int number;
		
		printf("Дочерний процесс: начинаю работать с числами из родитльского процесса!\n");
		while (read(fd[0], &number, sizeof(number)) > 0) {
			int square = number * number;
			printf("Дочерни процесс: квадрат числа %d = %d\n", number, square);
		}
		close(fd[0]);
		printf("Дочерний процесс: Все числа отработаны. Завершение процесса\n");

	} else {
		close(fd[0]);
		printf("Родительскийй процесс: Начинаю отправку числа\n");

		for (int i = 0; i < num_count; i++) {
			write(fd[1], &numbers[i], sizeof(numbers[i]));
			printf("Родительский процесс: отправил число %d\n", numbers[i]);
			sleep(1);			
		}
		close(fd[1]);
		wait(NULL);
		printf("Родительский процесс: дочерний процесс завершился\n");
	}
	return 0;	
	
}
