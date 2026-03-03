/*! 
	\file	parse-math-expr.c
	\brief 
	This file contains the implementation of the function 'parse_expr' which 
	is used to parse a mathematical expression from an inputstream buffer. The function
	reads the expression character by character and stores the operands and operators 
	in separate arrays. The function also checks for syntax errors in the expression
	and returns appropriate status codes.The function 'parse_operand' is a helper 
	function that is used to parse an operand from the inputstream buffer.
*/

#include <math.h> // TODO: Make a util pow()
#include "io.h"
#include "var-in-expr.h"
#include "parse-math-expr.h"


int sign = 1;
/// *operand is Positive (default). 'sign' is used at the end of the function to determine the sign of *operand
int op_mode = 1;


/*! \fn char parse_operand(char c, double* operand)
		\brief This function is used to parse an operand from the inputstream buffer.
		It reads characters until it encounters a non-numeric character or a dot ('.')
		which indicates the start of the fractional part of the operand. The function also
		checks for syntax errors such as multiple dots in the operand or consecutive operators.
		The parsed operand is stored in the variable pointed to by 'operand'.
		
		\param c a char from the inputstream buffer.
		\param operand a pointer to a double variable where the parsed operand will be stored.
		\return a char indicating the status of the parsing process:
			- 's' for syntax error
			- 'o' for opening parenthesis
			- 'c' for closing parenthesis
			- 'n' for end of expression
			- '>' for continuing with the expression
*/ 
char parse_operand(char c, double* operand)
{	
	char hasFractionalPart = 0;
	/// 'hasFractionalPart' is set to 1 (True) when we encounter the character '.' in the inputstream buffer
	
	sign = 1; // set sign to default for the next operand
	
	// Set *operand to 0
	*operand = 0;

	if(found_var(c))
		*operand = value(c); // asign the value of the variable to *operand
	else if(c == '-')
		sign = -1;
	else if( c == '+')
		;
	else if( c >= '0' && c <= '9')
		*operand = (double)(c-'0');  // Computing the first significant digit
	else if( c == '.')
		hasFractionalPart = 1;  // The operand does not have an integer part (e.g. .1234 )
	else if(c == '(')
		return 'O'; // return OpeningParenthesis status. We have an operator before '('. (Example: 89 * (90+10) )
	else
		return 's';

	char is_unary_sign = (c == '+' || c == '-');
	c = get_char();
	char exponent = 0;  // Used for computing the fractional part of the operand

	// OpeningParenthesis after unary sign operator (Example: 89 * -(90+10) )
	if(c == '(' && is_unary_sign)
		return 'O';

	while( (c >= '0' && c <= '9') || c == '.'  ){
		if(c >= '0' && c <= '9'){
			if(hasFractionalPart == 1)
				*operand += (double)(c-'0') / (double)pow(10, ++exponent);
			else
				*operand = (double)((*operand)*10+ (c-'0'));
		}
		else if(hasFractionalPart == 1)  
			// next character is '.' but hasFractionalPart is already True then 
			//SyntaxError: We have two dots '.' in our operand. (e.g. 12.8.9 or .90.8 )
			return 's';
		else
			hasFractionalPart = 1;
		c = get_char();
	}
	*operand *= sign;
	
	return c;
}

/*! \fn char parse_expr(double* operands, char* operators, char* window_at)
		\brief This function is used to parse a mathematical expression from an inputstream buffer.
		It reads the expression character by character and stores the operands and operators in separate arrays.
		The function also checks for syntax errors in the expression and returns appropriate status codes.
		
		\param operands an array of double where the parsed operands will be stored.
		\param operators an array of char where the parsed operators will be stored.
		\param window_at a pointer to a char variable that keeps track of the number of operands and operators parsed so far.
		\return a char indicating the status of the parsing process:
			- 's' for syntax error
			- 'o' for opening parenthesis
			- 'c' for closing parenthesis
			- 'n' for end of expression
			- '>' for continuing with the expression
			- '.' for end of input (EOF)
*/
char parse_expr(double* operands, char* operators, char* window_at)
{
	char c = '\0';  // character from the buffer
	char parse_operand(char , double*);

	while((*window_at) < 3){
		do 
			c = get_char(); // we discard white spaces
		while( c == ' ' || c == '\t' || c == '\0');

		if (c == '\n' || c == EOF || c == ')')
			return c == '\n' ? 'n' : (c == EOF ? '.' : 'c'); // End status: We have reached the end of the expression (e.g. 34 + 78 - 90\n or 34 + 78 - 90 EOF )
		
		if (op_mode)
			c = parse_operand(c, &operands[*window_at]);
		op_mode = 1; 			// reset op_mode for the next operand
		
		if(c == 's')
			return 's'; // syntaxError status

		switch(c){
			case '^':
				operators[*window_at] = '^';
				break;
			case '+':
				operators[*window_at] = '+';
				break;
			case '-':
				operators[*window_at] = '-';
				break;
			case '*':
				operators[*window_at] = '*';
				break;
			case '/':
				operators[*window_at] = '/';
				break;
			case '(':
				operators[*window_at] = '(';
				return 'o'; 	//OpeningParenthesis status. The case of a part-operator parenthesis (e.g. 89(90+10) )
			case 'O':
				return 'O'; 	//OpeningParenthesis status. The case of part-operand parenthesis (e.g. 89 * (90+10) )
			case ')':
				return 'c'; 	//ClosingParenthesis status.
			case '\n':
				return 'n'; 	//End status: We have reached the end of the expression (e.g. 34 + 78 - 90\n )
			case EOF:
				return '.'; 	//End status: We have reached the end of the expression (e.g. 34 + 78 - 90 EOF )
		}
		*window_at += 1;
	}
	
	return '>'; 				//ContinuingWithExpression status
}