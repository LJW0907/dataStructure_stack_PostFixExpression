#include "stack.h"

static char OPERATORS[] = "+-*/";
static int PRECEDENCE[] = { 1, 1, 2, 2 }; //연산자 우선순위

Stack operand_stack;
Stack operator_stack; //char들이 저장된다

int is_operator(char ch);
int eval(char *expr);
int eval_op(char op);

int precedence(char op);
int is_operator(char ch);
char *convert(char *infix);
char* process_op(char op, char* pos);

int main() {
	char expr[] = "2 - 10 / 5 * 6 + 4";
	char *expr_conv;
	
	expr_conv = convert(expr);
	//int answer = eval(expr);

	//printf("%d", answer);
}

int precedence(char op) { //op 연산자의 우선순위 반환
	return PRECEDENCE[is_operator(op)];
}

char *convert(char *infix) {
	operator_stack = create();

	char *postfix = (char *)malloc(strlen(infix) + 1); //변환된 postfix expression이 저장
	char *pos = postfix; //어디에 작성해야 할지 위치를 저장하는 pos

	char *token = strtok(infix, " ");

	while (token != NULL) {
		if (token[0] >= '0' && token[0] <= '9') { //피연산자
			sprintf(pos, "%s ", token);
			pos += (strlen(token) + 1);
		}
		else if (is_operator(token[0]) > -1) { //연산자
			pos = process_op(token[0], pos); //process_op 함수가 연산자를 append하고 끝 주소 반환
		}
		else {
			terminate("Syntax Error: invalid character encountered.");
		}
		token = strtok(NULL, " ");
	}

	while (!is_empty(operator_stack)) {
		char op = (char)pop(operator_stack);
		sprintf(pos, "%c ", op);
		pos += 2;
	}
	*pos = '\0';

	return postfix;	
}

char* process_op(char op, char* pos) {
	if (is_empty(operator_stack)) //operator_stack이 비었다면
		push(operator_stack, op);
	else {
		char top_op = peek(operator_stack); //top에 있는 연산자 복사해옴

		if (precedence(op) > precedence(top_op)) //다음 연산자가 top의 연산자보다 우선순위가 높다면
			push(operator_stack, op); //다음 연산자 스택에 push
		else {
			while (!is_empty(operator_stack) && precedence(op) <= precedence(top_op)) {
				pop(operator_stack); //top_op랑 같기 때문에 걍 버림
				sprintf(pos, "%c ", top_op);
				pos += 2;

				if (!is_empty(operator_stack))
					top_op = (char)peek(operator_stack);
			}
			push(operator_stack, op);
		}
	}
	return pos;
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