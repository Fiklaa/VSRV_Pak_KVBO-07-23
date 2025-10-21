#include <stdio.h>

void never_call(){
	printf("You will never see this line!\n");
	return;
}

int main(int argc, char **argv){
	printf("Started and finished!\n");
	return 0;
}
