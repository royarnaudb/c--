#include <stdio.h>
#include "compute-math-expr.h"
#include "io.h"

extern char status; // the status variable defined at compute-math-expr.c

int main(int argc, char* argv[])
{
	double calculate();
	char set_inputstream(char* from_file);
	double result = 0;

	if(argc > 1){
		if(!set_inputstream(argv[1])){
			fprintf(stderr, "Error: Could not open input file '%s'\n", argv[1]);
			return -1;
		}
	}
	else{
		set_inputstream(NULL); // set inputstream to stdin
	}

	char* prompt = (argc > 1) ? "" : ">>> ";
	
	do {
		printf("%s", prompt);
		
		result = calculate();
		
		if(status == '.' && argc <= 1)
			printf("END!\n");
		else
			printf("%.3lf\n", result);

		if(status == 'n' || status == 'c')
			status = '\0'; // set status for the next calculation
	} while(status != '.' && status != 's');

	return 0;
} //Next step: creating a graphical user interface!