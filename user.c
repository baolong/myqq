#include "user.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct User_List user;

int InitList(struct User_List *user)
{
    user = (struct User_List *)malloc(sizeof(struct User_List));
    user->front = NULL;
    user->next = NULL;
    return 0;
}

/***********************************
 *
 * 函数功能：判断链表是否为空
 * 返回值：空返回1，否则返回0
 *
 * *******************************/
int ListEmpty(struct User_List user)
{
    return (user.next == NULL);
}

/************************************
 *
 * 函数功能：获取链成员个数
 * 返回值：成员个数
 *
 * *********************************/
int ListLength(struct User_List *user)
{
    int n = 0;
    while(user->next != NULL)
    {
        n++;
        user = user->next;
    }
    return(n);
}

/************************************************
 *
 *函数功能：验证账号密码函数
 *返回值：  账号密码正确返回1，否则返回0
 *
 * *********************************************/
int Password(struct User_List user,char name[],char password[])
{
    struct User_List *p = user.next;
    while(p->next != NULL && !strcmp(p->user.name,name))
        p = p->next;
    if (strcmp(p->user.password,password) == 0)
        return 1;
    else
        return 0;
}

/**************************************
 *
 * 函数功能：新增账号
 * 返回值：  成功返回1，用户名存在返回2
 *
 * **********************************/
int AddUser(struct User_List *user,char name[],char password[])
{
    struct User_List *new;
    new = (struct User_List *)malloc(sizeof(struct User_List));
    while(user->next != NULL)
    {
        if (!strcmp(user->user.name,name))
            return 2;
        user = user->next;
    }
    strcpy(new->user.name,name);
    strcpy(new->user.password,password);
    new->front = user;
    new->next = NULL;
    user->next = new;
    return 1;
}

/************************************
 *
 * 函数功能：删除账号
 * 返回值：  成功返回1，不存在该用户
 *           返回2，密码错误返回3
 *
 * ********************************/
int UserDel(struct User_List *user,char name[],char password[])
{
    struct User_List *p = user,*del;
    del->front = NULL;
    del->next = NULL;
    while (0 != strcmp(p->user.name,name))
    {
        if (p->next == NULL)
            return 2;
        p = p->next;
    }
    if (0 == strcmp(p->user.password,password))
    {
        del = p;
        p->front->next = p->next;
        free(del);
    }
    return 0;
}

/***********************************
 *
 * 函数功能：遍历链表所有成员
 * 返回值：成功返回1，无数据返回0
 *
 * ********************************/
int DisList(struct User_List *user)
{
    int num = 0;
    user = user->next;
    while(user != NULL)
    {
        num++;
        printf("第%d个数据：%s-%s\n",num,user->user.name,user->user.password);
        user = user->next;
    }
    if (num == 0)
        return 1;     
    return 0;
}

int SaveList(struct User_List user)
{
    FILE *fp;
    if ((fp = fopen("user","rb+")) == 0)
    {
        perror("fp");
        return 1;
    }

}
