#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
 
int main() {
    const char *shm_name = "/my_shared_memory";
    const int SIZE = 4096;

    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    
    ftruncate(shm_fd, SIZE);
 
    pid_t pid = fork();
    
    if (pid == 0) {
        char *shared_mem = (char*)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        printf("Child process:\nChild read: ");
        printf("%s",shared_mem);
        munmap(shared_mem, SIZE);
    } else {
        char *shared_mem = (char *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        const char *message = "msg = \"Hello from parent!\"\n";
        
	sprintf(shared_mem, "%s", message);
        munmap(shared_mem, SIZE);
        
	wait(NULL);  
    }

    shm_unlink(shm_name); 
    return 0;
}

