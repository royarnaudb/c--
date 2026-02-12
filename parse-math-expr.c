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

#include <stdio.h>
#include <math.h>
#include "parse-math-expr.h"

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
	char hasFractionalPart = 0; // Initialized to False
	double sign = 1;
	/// 'hasFractionalPart' is set to True when we encounter the character '.' in the inputstream buffer
	/// *operand is Positive (default). 'sign' is used at the end of the function to determine the sign of *operand
	
	// Set *operand to 0, we compute the operand by adding the significant digits incremetally to this variable
	*operand = 0;

	if(c == '-')
		sign = -1;
	else if( c == '+')
		;
	else if( c >= '0' && c <= '9')
		*operand = (double)(c-'0');  // Computing the first significant digit
	else if( c == '.')
		hasFractionalPart = 1;  // The operand does not have an integer part (e.g. .1234 )
	else if( c == '*' || c == '/')
		return 's'; // SyntaxError: Two operators cannot be consecutive (e.g. 34 + * 78 )
	else if(c == '(')
		return 'o'; // return OpeningParenthesis status. We have an operator before '('. (Example: 89 * (90+10) )
	else
		return c;

	c = getchar();
	char exponent = 0;  // Used for computing the fractional part of the operand

	while( (c >= '0' && c<='9') || c == '.'  ){
		if (c >= '0' && c<='9'){
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
		c = getchar();
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
*/
char parse_expr(double* operands, char* operators, char* window_at)
{
	char c;  // character from the buffer
	char parse_operand(char , double*);

	while( (*window_at) < 3){
		c = getchar();
		c = parse_operand(c, &operands[*window_at]);

		if(c == 's')
			return 's'; // syntaxError status

		while( c == ' ' || c == '\t') // we discard white spaces
			c = getchar();

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
				return 'o'; //OpeningParenthesis status (e.g. 89(90+10) )
			case 'o':
				return 'o'; //OpeningParenthesis status. We have an operator before the parathensis '(' (e.g. 89 * (90+10) )
			case ')':
				return 'c'; //ClosingParenthesis status.
			case '\n':
				return 'n'; //End status: We have reached the end of the expression (e.g. 34 + 78 - 90\n )
			default:
				return 's'; //SyntaxError status: We have an invalid character in the expression (e.g. 34 + 78 @ 90 )
		}
		*window_at += 1;
	}
	return '>'; //ContinuingWithExpression status
}