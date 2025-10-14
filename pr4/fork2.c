#include <stdio.h>
#include <unistd.h>

int main() {
	int pid;
	printf("\nI'm the original process with pid %d and ppid %d\n", getpid(), getppid());
	pid = fork();

	if (pid != 0) {
		printf("\nI'm the parent process with pid %d and ppid %d\n", getpid(), getppid());
		printf("\nMy child process pid %d\n", pid);
	}else{
		printf("\nbefore sleep\n");
		sleep(5);
		printf("\nI'm the child process with pid %d and ppid %d\n", getpid(), getppid());
		printf("\nWas deserted\n");
	}
	printf("\n pid %d terminated\n", getpid());
	return 0;
}

