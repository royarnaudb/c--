/*!
	\file compute-math-expr.c
	\brief
	This file contains the implementation of the function calculate, which is 
	responsible for calculating the result of a mathematical expression. It also
	contains some helper functions that are used by calculate(). calculate() uses
	the function 'parse_expr' to parse the expression and	then compute() the result.
*/

#include <stdio.h>
#include <math.h>
#include "parse-math-expr.h"
#include "compute-math-expr.h"

/*! \fn double calculate(char* status)
		\brief
		This function is responsible for calculating the result of a mathematical
		expression.	It uses the function 'parse_expr' to parse the expression and
		then computes the result based on the parsed operands and operators. The
		function also handles operator precedence and parentheses in the
		expression.
		
		\param status a pointer to a char variable that will be used to store the status of the parsing process.
		\return the result of the calculated expression as a double.
*/
double calculate(char* status)
{
	char window_at = 0;  // The maximum window_at is 2 (WINDOW_SIZE - 1)
	double operands[3] = {0,0,0}; // The maximum operands we can have at a time is 3 (WINDOW_SIZE)
	char operators[3] = {'+','+','+'};

	char parse_expr(double* operands, char* operators, char* window_at);
	char contains_nest_op(char* operators);
	void compute(char* operators, double* operands, char* window_at);

	do {
		*status = parse_expr(operands, operators, &window_at);

		if(*status == '>'){
			compute(operators, operands, &window_at);
		}
		else if(*status == 'n' || *status == 'c'){
			compute(operators, operands, &window_at);
			break; // break the loop, then return operands[0] as our result
		}
		else if(*status == 'o'){
			if(contains_nest_op(operators)){
				operators[window_at] = '*';
				if(window_at < 2)
					operands[window_at+1] = calculate(status);
				else{
					compute(operators, operands, &window_at);
					operands[window_at] = calculate(status);
				}
			}
			else
				operands[window_at] = calculate(status);
			compute(operators, operands, &window_at);

			if(*status == '\n')
				break; // break the loop, then return operands[0] as our result
		}

	} while(*status != 's');
	return operands[0];
}

char contains_nest_op(char* operators)
{
	if(	operators[0] == '(' || operators[1] == '(' || operators[2] == '(' )
		return 1;
	return 0;
}

/*! \fn	int highest_order_op(char* operators)
		\brief
		This function is responsible for determining the index of the operator with the
		highest precedence in the 'operators' array. It checks for multiplication and 
		division operators first, as they have higher precedence than addition and 
		subtraction. If it finds a multiplication or division operator, it returns its 
		index. If it doesn't find any, it returns 0 by default, which corresponds to the 
		first operator in the array.

		\param operators an array of char representing the operators in the current window.
		\return the index of the operator with the highest precedence (0, 1, or 2).
*/
int highest_order_op(char* operators)
{ // We check for '^' operator first, as it has the highest precedence
	if(operators[1] == '^')
		return 1;
	else if(operators[0] == '^')
		return 0;
	if(operators[0] == '*' || operators[0] == '/')
		return 0;
	else if(operators[1] == '*' || operators[1] == '/')
		return 1;
	return 0;
}

/*! \fn	void arithmetic_op(char operator, double* a, double* b, double* result)
		\brief
		This function performs the specified arithmetic operation on the two operands
		'a' and 'b', and stores the result in the variable pointed to by 'result'.
		The function takes an operator as input, which can be one of the following:
		'+', '-', '*', or '/'.
		Based on the operator, it performs the corresponding arithmetic operation.
		If an unexpected operator is provided, it prints a syntax error message.

		\param operator a char representing the arithmetic operator to be applied.
		\param a a pointer to the first operand (double).
		\param b a pointer to the second operand (double).
		\param result a pointer to a double variable where the result of the operation will be stored.
*/
void arithmetic_op(char operator, double* a, double* b, double* result)
{
	switch(operator){
		case '+':
			*result = *a + *b;
			break;
		case '-':
			*result = *a - *b;
			break;
		case '*':
			*result = *a * *b;
			break;
		case '/':
			*result = *a / *b;
			break;
		case '^':
			*result = pow(*a, *b);
			break;
		default:
			printf("SyntaxError: Unexpected operator '%c'!", operator);
			break;
	}
}

/*! \fn	void shift_window(char mode, char* window_at, char* operators, double* operands)
		\brief
		This function is responsible for shifting the window of operators and operands
		based on the specified mode. The mode determines how the operators and operands
		are shifted in the arrays. The function updates the 'window_at' variable to
		indicate the current position in the window after shifting. It also resets the
		last operand and operator in the window to their default values.

		\param mode a char representing the mode of shifting (1, 2, or 3).
		\param window_at a pointer to a char variable that indicates the current position in the window.
		\param operators an array of char representing the operators in the current window.
		\param operands an array of double representing the operands in the current window.
*/
void shift_window(char mode, char* window_at, char* operators, double* operands)
{
	switch(mode){
		case 1:
			operands[1]=0;
			operators[0]=operators[2];
			operators[1]='+';
			*window_at = 1;
			break;
		case 2:
			operands[1]=operands[2];
			operators[0]=operators[1];
			operators[1]=operators[2];
			*window_at = 2;
			break;
		case 3:
			operators[1]=operators[2];
			*window_at = 2;
			break;
		default:
			printf("Something went Wrong! '%i' shift_window mode is not defined!", mode);
			break;
	}
	operands[2]=0;
	operators[2]='+';
}

/*! \fn	void compute(char* operators, double* operands, char* window_at)
		\brief
		This function is responsible for computing the result of the current window of
		operators and operands. It determines the operator with the highest precedence
		and performs the corresponding arithmetic operations based on the operators in
		the window. The function also handles the shifting of the window after performing
		the operations to prepare for the next set of operators and operands.

		\param operators an array of char representing the operators in the current window.
		\param operands an array of double representing the operands in the current window.
		\param window_at a pointer to a char variable that indicates the current position in the window.
*/
void compute(char* operators, double* operands, char* window_at)
{
	int preced_id; // preced_id (precedence_index) stores the index at 'operators' from highest_order_op()

	int highest_order_op(char* operators);
	void arithmetic_op(char operator, double* a, double* b, double* result);
	void shift_window(char mode, char* window_at, char* operators, double* operands);

	preced_id = highest_order_op(operators);
	if(operators[2] == '+' || operators[2] == '-'){
		// if the last operator input is '+' or '-'
		if(preced_id == 0){
			arithmetic_op(operators[preced_id], &operands[0], &operands[1], &operands[0]);
			arithmetic_op(operators[1], &operands[0], &operands[2], &operands[0]);
			shift_window(1, window_at, operators, operands);
		}
		else if(preced_id == 1){
			arithmetic_op(operators[preced_id], &operands[1], &operands[2], &operands[1]);
			arithmetic_op(operators[0], &operands[0], &operands[1], &operands[0]);
			shift_window(2, window_at, operators, operands);
		}
	}
	else if(operators[2] == '*' || operators[2] == '/' || operators[2] == '^'){	// if the last operator input is '*' or '/'.
		if(preced_id == 0 && (operators[0] == '*' || operators[0] == '/')){
			arithmetic_op(operators[preced_id], &operands[0], &operands[1], &operands[0]);
			if(operators[1] == '*' || operators[1] == '/'){
				arithmetic_op(operators[1], &operands[0], &operands[2], &operands[0]);
				shift_window(1, window_at, operators, operands);
			}
			else shift_window(2, window_at, operators, operands);
		}
		else if(preced_id == 1 && (operators[1] == '*' || operators[1] == '/' || operators[1] == '^')){
			arithmetic_op(operators[preced_id], &operands[1], &operands[2], &operands[1]);
			shift_window(3, window_at, operators, operands);
		}
		else{
			arithmetic_op(operators[preced_id], &operands[0], &operands[1], &operands[0]);
			shift_window(2, window_at, operators, operands);
		}
	}
}