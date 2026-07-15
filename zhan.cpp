/*顺序栈的基本操作*/
#include <stdio.h>
#define MAX 10
typedef struct
{
    int base;
    int top;
    int st[MAX];
} SqStack;
/*基本操作说明*/
void InitStack(SqStack *S);
void push(SqStack *S, int e);
void pop(SqStack *S, int *e);

/*基操作的算法描述*/
void InitStack(SqStack *S)
{
    S->top = 0;
    S->base = S->top;
}
void push(SqStack *S, int e)
{
    if (S->top == MAX)
    {
        printf("栈满\n");
        return;
    }
    S->st[S->top] = e;
    S->top += 1;
}
void pop(SqStack *S, int *e)
{
    if (S->top == 0)
    {
        printf("栈空\n");
        return;
    }
    S->top -= 1;
    *e = S->st[S->top];
}
void print(SqStack S)
{
    int I;
    printf("栈的元素如下:\n");
    for (I = S.top - 1; I >= 0; I--)
        printf("%d\n", S.st[I]);
}
int main()
{
    SqStack S;
    int i, x;
    printf("堆栈初始化\n");
    InitStack(&S); /*初始化*/
    printf("5个元素入栈:\n");
    for (i = 0; i < 5; i++) /*入栈*/
        push(&S, 2 * i + 1);
    print(S);
    printf("出栈:\n");
    pop(&S, &x); /*出栈*/
    printf("出栈元素为：%d\n", x);
    print(S);
}