#include <stdio.h>
#include <unistd.h>

int main(){
	printf("\n Main process with %d pid, exec ls -l", getpid());
	execl("/bin/ls", "ls", "-l", NULL);
	printf("NEVER");

	return 0;
}

