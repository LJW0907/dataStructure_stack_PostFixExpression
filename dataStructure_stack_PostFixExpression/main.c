#include "stack.h"

static char OPERATORS[] = "+-*/";
Stack operand_stack;

int is_operator(char ch);
int eval(char *expr);
int eval_op(char op);

int main() {
	char expr[] = "5 3 *";

	int answer = eval(expr);

	printf("%d", answer);
}

int is_operator(char ch) {
	for (int i = 0; i < strlen(OPERATORS); i++)
		if (OPERATORS[i] == ch)
			return i;
	return -1;
}

int eval(char *expr) {
	operand_stack = create();

	char *token = strtok(expr, " "); //모든 연산자와 피연산자가 공백으로 구분되어 있다고 가정

	while (token != NULL) {
		if (token[0] >= '0' && token[0] <= '9') { //피연산자
			int value = atoi(token);
			push(operand_stack, value);
		}
		else if (is_operator(token[0]) > -1) { //연산자
			int result = eval_op(token[0]);
			push(operand_stack, result);
		}
		else {
			terminate("Syntax Error: invalid character encountered.");
		}
		token = strtok(NULL, " ");
	}

	if (is_empty(operand_stack))
		terminate("Syntax Error: Stack empty in eval.");

	int answer = pop(operand_stack);

	if (is_empty(operand_stack))
		return answer;
	else {
		terminate("Syntax Error: Stack should be empty");
		return -1;
	}
}

int eval_op(char op) {
	if (is_empty(operand_stack))
		terminate("Syntax Error: Stack empty in eval_op.");

	int rhs = pop(operand_stack);

	if (is_empty(operand_stack))
		terminate("Syntax Error: Stack empty in eval_op.");

	int lhs = pop(operand_stack);
	int result = 0;

	switch (op) {
	case '+': result = lhs + rhs; break;
	case '-': result = lhs - rhs; break;
	case '*': result = lhs * rhs; break;
	case '/': result = lhs / rhs; break;
	}
	return result;
}