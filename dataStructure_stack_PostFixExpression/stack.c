
#include "stack.h"

#define INIT_CAPACITY 100

void reallocate(Stack s);

struct stack_type {
	Item *contents; //stack 역할을 할 배열
	int top; //contents의 top 역할
	int size; //contents의 size
};

static void terminate(const char *message) {//예외 처리 함수
	printf("%s\n", message);
	exit(1);
}

Stack create() { //Stack 객체 생성
	Stack s = (Stack)malloc(sizeof(struct stack_type));

	if (s == NULL)
		terminate("Error in create: stack could not be created");

	s->contents = (Item*)malloc(sizeof(Item) * INIT_CAPACITY);

	if (s->contents == NULL) {
		free(s);
		terminate("Error in create: stack could not be created.");
	}
	s->top = -1;
	s->size = INIT_CAPACITY;

	return s;
}

void destroy(Stack s) { //Stack 객체 해제
	free(s->contents);
	free(s);
}

void make_empty(Stack s) { //Stack 초기화
	s->top = -1;
}

bool is_empty(Stack s) { //empty 여부 확인
	return s->top == -1;
}

int is_full(Stack s) { //is_full
	return s->top == s->size - 1;
}

void push(Stack s, Item i) { //push
	if (is_full(s))
		reallocate(s);
	s->top++;
	s->contents[s->top] = i;
}

Item pop(Stack s) { //pop
	if (is_empty(s))
		terminate("Error in pop: stack is empty.");
	s->top--;

	return s->contents[s->top + 1];
}

Item peek(Stack s) { //peek
	if (is_empty(s))
		terminate("Error in peek: stack is empty.");

	return s->contents[s->top];
}

void reallocate(Stack s) {
	Item *tmp = (Item*)malloc(sizeof(Item) * 2 * s->size);

	if (tmp == NULL)
		terminate("Error in create: stack could not be created.");

	for (int i = 0; i < s->size; i++)
		tmp[i] = s->contents[i];

	free(s->contents);

	s->contents = tmp;
	s->size *= 2;
}

int read_line(FILE *fp, char* str, int limit) { //단점을 모두 보완한 문자열 입력 함수
	int ch, i = 0;
	char tmp = ' ';

	while ((ch = fgetc(fp)) != '\n' && ch != EOF) //getchar()의 반환형은 int다!! 명심
		if (i < limit - 1) {
			if (tmp != ' ' || ch != ' ') {
				str[i++] = tmp = ch;
			}
		}

	if (str[i - 1] == ' ')
		i--;

	str[i] = '\0';
	return i; //문자열 길이 반환
}