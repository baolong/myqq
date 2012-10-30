#include <stdio.h>
#include <string.h>
#include "user.h"

typedef struct User_Type
{
    char name[15];
    char password[10];
};

typedef struct User_List
{
    struct User_Type user;
    struct User_List *next;
};

struct Usr_List *user;

void InitList(User_List *&user)
{
    user = (User_List *)malloc(sizeof(User_List));
    user->next = NULL;
}

int ListEmpty(User_List *user)
{
    return (user->next == NULL);
}

int ListLength(User_List *user)
{
    User_List *p = user;
    int n = 0;
    while(p->next != NULL)
    {
        n++;
        p = p->next;
    }
    return(n);
}


/************************************************
 *
 *函数功能：验证账号密码函数
 *返回值：  账号密码正确返回1，否则返回0
 *
 * *********************************************/
int Password(User_List *user,char name[],char password[])
{
    User_List *p = user->next;
    while(p != NULL && p->user.name != name)
        p = p->next;
    if (strcmp(p->password,password) == 0)
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
int AddUser(User_List *&user,char name[],char password[])
{
    User_List *p = user,*new;
    while(p->next != NULL)
    {
        if (!(strcmp(p->user.name,name)))
            return 2; 
        p = p->next;
    }
    strcpy(new->user.name,name);
    strcpy(new->user.password,password);
    new = (User_List *)malloc(sizeof(User_List));
    p->next = new;
    new->next = NULL;
    return 1;
}

/************************************
 *
 * 函数功能：删除账号
 * 返回值：  成功返回1，否则返回0
 *
 * ********************************/
int main(User_List *&user,char name[],char password[])
{
    User_List *p = user,*del;
    while(!strcmp(p->user.name,name))
    {
        
    }
}
