#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef int Item; //왜 이렇게 함? : 이렇게 하면 다른 자료형에서 쓸 일이 생겨도 'int'만 'float'로 바꾸면 돼!
typedef struct stack_type *Stack;

Stack create();
void destroy(Stack s);
void make_empty(Stack s);
bool is_empty(Stack s);
void push(Stack s, Item i);
Item pop(Stack s);
Item peek(Stack s);
void terminate(const char *message);
int read_line(FILE *fp, char* str, int limit);

