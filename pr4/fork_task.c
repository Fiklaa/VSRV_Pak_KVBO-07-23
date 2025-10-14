#include <stdio.h>
#include <unistd.h>

void Child_process(){
	for (int i = 0; i<10; i++){
		printf("%d\n", i);
	}
	return;
}

void Parent_process(){
	for (int a = 0; a < 100; a = a+10){
		printf("%d\n", a);
		sleep(1);
	}
	return;
}


int main(){
	int pid;
	printf("\n I am the original process pid %d and ppid %d \n", getpid(), getppid());

	pid = fork();

	if (pid != 0){
		printf("Parent process with pid %d and ppid %d \n", getpid(), getppid());
		Parent_process();
	}else{
		printf("Child process with pid %d and ppid %d \n", getpid(), getppid());
		Child_process();
	}

	printf("Process %d terminated\n", getpid());
}

