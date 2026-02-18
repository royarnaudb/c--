#include <stdio.h>
#include "compute-math-expr.h"
#include "io.h"

char status ='\0';

int main(int argc, char* argv[])
{
	double calculate();
	char set_inputstream(char* from_file);

	if(argc > 1){
		if(!set_inputstream(argv[1])){
			fprintf(stderr, "Error: Could not open input file '%s'\n", argv[1]);
			return -1;
		}
	}
	else{
		set_inputstream(NULL); // set inputstream to stdin
	}

	printf(">>> ");

	do {
		printf("%.3lf\n", calculate());
		if(status == 'n' || status == 'c'){
			status = '\0'; // set status for the next calculation
			printf(">>> ");
		}
	} while(status != '.' && status != 's');

	return 0;
} //Next step: creating a graphical user interface!