#include <stdio.h>

int main(){
	int value = 300;

	double value2 = 3.4;
	
	int *wild_ptr = NULL;
	
	for (int i = 0; i < 2; i++){
		
		if (wild_ptr != NULL){
			printf("\nДикий указатель уже инициализирован: %p\n", wild_ptr);
			printf("\nАдрес указателя: %p\n", &wild_ptr);
		}else{
			printf("\nУказатель не иницализирован!!\n");
			wild_ptr = &value;
			printf("\nТеперь инициализирован)\n");
			printf("\n%d - значение переменной value\n", value);
			printf("\n%p - адрес переменной value\n", &value);

		}

	}


	printf("\n%d - значение переменной value\n", value);
	printf("\n%p - адрес переменной value\n", &value);
	
	
	return 0;
}
