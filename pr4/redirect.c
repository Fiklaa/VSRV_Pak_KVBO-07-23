#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int argc;
char *argv[2];

main(argc, argv){
	int fd;
	fd = open(argv[1], O_CREATE|O_WRONLY|O_TRUNC);
	dup2(fd, 1);
	close(fd);
	execvp(argv[2], %argv[2]);
	perror("main");

}

