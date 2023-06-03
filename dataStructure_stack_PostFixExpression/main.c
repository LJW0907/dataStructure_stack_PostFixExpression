#include "stack.h"

static char OPERATORS[] = "+-*/";
static int PRECEDENCE[] = { 1, 1, 2, 2 }; //������ �켱����

Stack operand_stack;
Stack operator_stack; //char���� ����ȴ�

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

int precedence(char op) { //op �������� �켱���� ��ȯ
	return PRECEDENCE[is_operator(op)];
}

char *convert(char *infix) {
	operator_stack = create();

	char *postfix = (char *)malloc(strlen(infix) + 1); //��ȯ�� postfix expression�� ����
	char *pos = postfix; //��� �ۼ��ؾ� ���� ��ġ�� �����ϴ� pos

	char *token = strtok(infix, " ");

	while (token != NULL) {
		if (token[0] >= '0' && token[0] <= '9') { //�ǿ�����
			sprintf(pos, "%s ", token);
			pos += (strlen(token) + 1);
		}
		else if (is_operator(token[0]) > -1) { //������
			pos = process_op(token[0], pos); //process_op �Լ��� �����ڸ� append�ϰ� �� �ּ� ��ȯ
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
	if (is_empty(operator_stack)) //operator_stack�� ����ٸ�
		push(operator_stack, op);
	else {
		char top_op = peek(operator_stack); //top�� �ִ� ������ �����ؿ�

		if (precedence(op) > precedence(top_op)) //���� �����ڰ� top�� �����ں��� �켱������ ���ٸ�
			push(operator_stack, op); //���� ������ ���ÿ� push
		else {
			while (!is_empty(operator_stack) && precedence(op) <= precedence(top_op)) {
				pop(operator_stack); //top_op�� ���� ������ �� ����
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

	char *token = strtok(expr, " "); //��� �����ڿ� �ǿ����ڰ� �������� ���еǾ� �ִٰ� ����

	while (token != NULL) {
		if (token[0] >= '0' && token[0] <= '9') { //�ǿ�����
			int value = atoi(token);
			push(operand_stack, value);
		}
		else if (is_operator(token[0]) > -1) { //������
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