/*循环队列的基本操作*/
#include <stdio.h>
#define MAX 8
typedef struct
{
    int base[MAX];
    int front;
    int rear;
} SqQueue;
/*********** 初始化 **************/
void InitQueue(SqQueue *Q)
{
    Q->front = 0;
    Q->rear = Q->front;
}
/******求队列长度******/
int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAX) % MAX;
}
/*****入队列****************/
void EnQueue(SqQueue *Q, int e)
{
    if ((Q->rear + 1) % MAX == Q->front)
    {
        printf("队列已满，无法入队!\n");
        return;
    }
    Q->base[Q->rear] = e;
    Q->rear = ((Q->rear + 1) % MAX);
}
/*******遍历队列******/
void traverse(SqQueue Q)
{
    int I, k;
    if (Q.front <= Q.rear)
        for (I = Q.front; I < Q.rear; I++)
            printf("%d ", Q.base[I]);
}
/**************出队************/
void DeQueue(SqQueue *Q, int *e)
{
    if (Q->rear == Q->front)
    {
        printf("队列为空，无法出队!\n");
    }
    *e = Q->base[Q->front];
    Q->front = ((Q->front + 1) % MAX);
}
int main()
{
    SqQueue Q;
    int j, m, x;
    InitQueue(&Q);
    printf("入队操作\n");
    for (j = 0; j < 7; j++)
        EnQueue(&Q, 2 * j);
    traverse(Q);
    m = QueueLength(Q);
    printf("\n长度 %d\n", m);
    printf("出队操作\n");
    for (j = 0; j < m; j++)
    {
        DeQueue(&Q, &x);
        printf("%d\n", x);
    }
}