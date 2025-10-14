#include <stdio.h>
#include <stdlib.h>

int main(){
	int value = 123;

	const double maxTemp = 3.1415;

	printf("\nПример статического выделения памяти: %d %f\n", value, maxTemp);


	int *ptr = NULL;

	ptr = (int*) calloc(10, sizeof(int));

	ptr[0] = 1;

	printf("\nПервое значение динамического массива: %d\n", ptr[0]);
	
	printf("\nВторое значение динамического массива (заданно по умолчанию 0 с помощью calloc): %d\n", ptr[1]);

	free(ptr);

	ptr = NULL;

	return 0;
}
