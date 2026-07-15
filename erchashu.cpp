#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 20
#define OK 1
#define ERROR 0
#define OVERFLOW 0
typedef char TElemType;
typedef int Status;

typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

BiTree CreateBiTree()
{
    char ch;
    scanf(" %c", &ch);
    if (ch == '#')
        return NULL;

    BiTree T = (BiTNode *)malloc(sizeof(BiTNode));
    if (!T)
        exit(OVERFLOW);
    T->data = ch;
    T->lchild = NULL;
    T->rchild = NULL;
    T->lchild = CreateBiTree();
    T->rchild = CreateBiTree();

    return T;
}

void PreOrder(BiTree T)
{
    if (T == NULL)
        return;
    {
        printf("%c ", T->data);
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}

void InOrder(BiTree T)
{
    if (T == NULL)
        return;
    {
        InOrder(T->lchild);
        printf("%c ", T->data);
        InOrder(T->rchild);
    }
}

void PostOrder(BiTree T)
{
    if (T == NULL)
        return;
    {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%c ", T->data);
    }
}
void LevleOrder(BiTree T)
{                         /*层次遍历二叉树T，从第一层开始，每层从左到右*/
    BiTree Queue[MAX], b; /*用一维数组表示队列，front和rear分别表示队首和队尾指针*/
    int front, rear;
    front = rear = 0;
    if (T) /*若树非空*/
    {
        Queue[rear++] = T; /*根结点入队列*/
        while (front != rear)
        {                       /*当队列非空*/
            b = Queue[front++]; /*队首元素出队列，并访问这个结点*/
            printf("%2c", b->data);
            if (b->lchild != NULL)
                Queue[rear++] = b->lchild; /*左子树非空，则入队列*/
            if (b->rchild != NULL)
                Queue[rear++] = b->rchild; /*右子树非空，则入队列*/
        }
    }
} /*LevelOrder*/

int main()
{
    BiTree B, T;
    // BiTNode T;
    printf("\n请读入构造二叉树的字符序列:");
    B = CreateBiTree(); /*建立一棵二叉树T*/
                        // B=&T;
    printf("\n先序遍历结果: ");
    PreOrder(B);
    printf("\n中序遍历结果: ");
    InOrder(B);
    printf("\n后序遍历结果: ");
    PostOrder(B);
    printf("\n该二叉树的层次遍历是:");
    LevleOrder(B); /*层次遍历二叉树*/
    printf("\n");

    return 0;
}