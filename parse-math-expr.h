char parse_operand(char c, double* number);
char parse_expr(double* operands, char* operators, char* window_at);
extern int sign; // sign is used to determine the sign of the operand we are parsing in parse_operand()
extern int op_mode;