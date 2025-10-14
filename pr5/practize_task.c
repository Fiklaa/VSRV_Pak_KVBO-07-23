#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


bool is_prime(int n){
	if (n < 2) return false;
	if (n == 2) return true;
	if (n % 2 == 0) return false;

	for (int i = 3; i * i <= n; i+=2) {
		if (n % i == 0) return false;
	}
	
	return true;	
}


void find_numbers(int arr[], int size, int *count, int *min_num){
	*count = 0;
	*min_num = 999;

	for (int i = 0; i < size; i++){
		if (is_prime(arr[i])) {
			(*count)++;
			if (arr[i] < *min_num){
				*min_num = arr[i];
			}
		}
	}

	if (*count == 0) {
		*min_num = -1;
	}
}


int main(int argc, char *argv[]){
	
	if (argc < 2) {
		printf("\nИспользование: %s <число1> <число2> <число3> ...\n", argv[0]);
		printf("Пример: %s 1 2 3 4 5 6 7 8 9...\n", argv[0]);
		return 1;
	}

	int size = argc - 1;

	int *arr = (int*)malloc(size * sizeof(int));


	printf("Введенные числа: ");
	for (int i = 0; i < size; i++){
		arr[i] = atoi(argv[i+1]);
		printf("%d ", arr[i]);
	}
	printf("\n");

	int prime_count, min_prime;

	find_numbers(arr, size, &prime_count, &min_prime);

	printf("Кол-во простых чисел: %d\n", prime_count);
	if (prime_count > 0){
		printf("Наименьшее простое число: %d\n", min_prime);
	}else{
		printf("Простых чисел не найдено\n");
	}

	free(arr);

	return 0;
}

