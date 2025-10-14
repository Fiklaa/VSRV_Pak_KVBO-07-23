#include <stdio.h>

int main(){
	int value = 300;

	double value2 = 3.4;

	printf("\n%d - значение переменной value\n", value);
	printf("\n%p - адрес переменной value\n", &value);
	
	printf("\n%f - значение переменной value2\n", value2);
	printf("\n%p - адрес переменной value2\n", &value2);
	
	void *Vptr_value;

	Vptr_value = &value;

	printf("\nДля значения переменной int value\n");
	printf("\n%p - значение переменной Vptr_value\n", Vptr_value);
	printf("\n%p - адрес переменной Vptr_value\n", &Vptr_value);


	Vptr_value = &value2;


	printf("\nДля значения переменной double value2\n");
	printf("\n%p - значение переменной Vptr_value\n", Vptr_value);
	printf("\n%p - адрес переменной Vptr_value\n", &Vptr_value);

	return 0;
}
