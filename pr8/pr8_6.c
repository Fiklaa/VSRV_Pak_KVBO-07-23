#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int fd[2];

	pipe(fd);

	if (fork() == 0) {
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);

		execlp("who", "who", NULL);
	} else {
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);

		execlp("wc", "wc", "-l", NULL);
	}
	return 0;
}
