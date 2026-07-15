#include <stdio.h>
#include <stdlib.h>
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
typedef struct
{
    float real;
    float image;
} Elem;
typedef struct
{
    Elem *base;
    int length;
    int listsize;
} List;
void List_Init(List *L)
{
    L->length = 0;
    L->base = (Elem *)malloc(LIST_INIT_SIZE * sizeof(Elem));
    L->listsize = LIST_INIT_SIZE;
    printf("顺序表创建成功\n");
}
void List_Insert(List *L, int i, Elem e)
{
    if (i < 0 || i > L->length)
    {
        printf("插入位置非法/n");
        return;
    }
    Elem *newbase, *p, *q;
    if (L->length >= L->listsize) /*如果当前存储空间已满，增加分配*/
    {
        newbase = (Elem *)realloc(L->base, (L->listsize + LISTINCREMENT) * sizeof(Elem));
        if (!newbase)
        {
            printf("存储空间分配失败\n");
            return;
        }
        L->base = newbase;
        L->listsize += LISTINCREMENT;
    }
    q = &(L->base[i - 1]);
    for (p = &(L->base[L->length - 1]); p >= q; --p)
        *(p + 1) = *p;
    *q = e;
    ++L->length;
    printf("线性表插入成功\n");
    printf("插入的元素为%f+%fi\n", L->base[i - 1].real, L->base[i - 1].image);
}
void List_Delete(List *L, int i, Elem *e)
{
    if (i < 0 || i > L->length - 1)
    {
        printf("删除位置非法\n");
        return;
    }
    Elem *q, *p;
    p = &(L->base[i - 1]);
    q = L->base + L->length - 1;
    for (++p; p <= q; ++p)
        *(p - 1) = *p;
    --L->length;
    printf("线性表的第%d个元素已成功删除\n", i);
}
void TravelList(List *L)
{
    for (int j = 0; j < L->length; j++)
        printf("%f+%fi ", L->base[j].real, L->base[j].image);
    printf("\n");
}
int main()
{
    int i;
    List l;
    Elem item, item2;
    List_Init(&l);
    for (i = 0; i < 5; i++)
    {
        printf("请输入您要插入到线性表中的复数的实部：");
        scanf("%f", &item.real);
        printf("请输入您要插入到线性表中的复数的虚部：");
        scanf("%f", &item.image);
        List_Insert(&l, i, item);
    }
    TravelList(&l);
    printf("请输入您希望删除的元素序号：");
    scanf("%d", &i);
    List_Delete(&l, i, &item2);
    printf("返回被删除的元素%f+%fi\n", item2.real, item2.image);
}