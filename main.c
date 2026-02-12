#include <stdio.h>
#include "compute-math-expr.h"

int main(void)
{
	double calculate(char* status);
	char status ='\0';

	double result = calculate(&status);

	if(status == 's'){
		printf("\nSYNTAX ERROR\n");
		return -1;
	}

	printf("\n%.3lf\n", result);

	return 0;
}
//Next step: creating a graphical user interface!