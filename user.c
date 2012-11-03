#include "user.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>


struct User_List list;

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
int UserChecking(struct User_List *user,char name[],char password[])
{
    while(user->next != NULL && 0 != strcmp(user->user.name,name))
        user = user->next;
    if (strcmp(user->user.password,password) == 0)
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

/********************************
 *
 * 函数功能：修改账号名
 * 返回值：成功 - 0
 *         密码错误 - 1
 *         没有该账号 - 2
 *         数据库为空 - 3
 *
 *******************************/
int UserNameChange(struct User_List *user,char *oldname,char *newname,char *passwd)
{
    if (NULL != user->next)
    {
        while(NULL != user->next)
        {
            if (0 != strcmp(user->user.name,oldname))
                user = user->next;
            else
                break;
        }
        if (0 == strcmp(user->user.name,oldname))
        {
            if (0 == strcmp(user->user.password,passwd))
            {
                strcpy(user->user.name,newname);
                return 0;
            }
            else
                return 1;
        }
        else
            return 2;
    }
    else
        return 3;
}

/********************************
 *
 * 函数功能：修改密码
 * 返回值：成功 - 0
 *         密码错误 - 1
 *         没有该账号 - 2
 *         数据库为空 - 3
 *
 *******************************/
int UserPasswdChange(struct User_List *user,char *name,char *oldpasswd,char *newpasswd)
{
    if (NULL != user->next)
    {
        while(NULL != user->next)
        {
            if (0 != strcmp(user->user.name,name))
                user = user->next;
            else
                break;
        }
        if (0 == strcmp(user->user.name,name))
        {
            if (0 == strcmp(user->user.password,oldpasswd))
            {
                strcpy(user->user.password,newpasswd);
                return 0;
            }
            else
                return 1;
        }
        else
            return 2;
    }
    else
        return 3;
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
    struct User_List *del;
    del->front = NULL;
    del->next = NULL;
//    user = user->next;
    if (NULL != user->next)
    {
        while (user != NULL)
        {
            if (strcmp(user->user.name,name) == 0)
            {
                if (0 == strcmp(user->user.password,password))
                {
                    user = user->front;
                    del = user->next;
                    user->next = del->next;
                    free(del);
                    DisList(user);
                    return 1;
                }
            }
            user = user->next;
        }
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
    if (NULL != user->next )
    {
        user = user->next;
        while(user != NULL)
        {
            num++;
            printf("第%d个账号：%s - %s\n",num,user->user.name,user->user.password);
            user = user->next;
        }
    }
    if (num == 0)
        return 1;     
    return 0;
}

/*******************************
 *
 * 函数功能：保存账号数据
 * 返回值：成功返回0，没有数据返回1
 *
 * ****************************/
int SaveList(struct User_List *user)
{
    FILE *fp;
    if (NULL == (fp = fopen("date","w+")))
    {
        perror("fp");
        return 3;
    }
    int sum = 0;
    char sum_[3];
    sum = ListLength(user);
    printf("sum = %d\n",sum);
    sum_[1] = sum%10+48;
    sum_[0] = sum/10%10+48;
    sum_[2] = '\0';
    printf("sum_ = %s\n",sum_);
    fwrite(sum_,sizeof(sum_),1,fp);
    if (NULL == user->next)
        return 1;
    else
    {
        user = user->next;
        while(user != NULL)
        {
            printf("测试\n");
            fwrite(user->user.name,USERNAME_SIZE*sizeof(char),1,fp);
            fwrite(user->user.password,USERPASSWD_SIZE*sizeof(char),1,fp);
            printf("成功写：%s\n",user->user.name);
            user = user->next;
        }
    }
    fclose(fp);
    return 1;
}

int LoadList(struct User_List *user)
{
    FILE *fp;
    char name[USERNAME_SIZE],passwd[USERPASSWD_SIZE];
    if ((fp = fopen("date","r+")) < 0)
    {
        perror("fp");
        return 1;
    }
    int sum = 0;
    char sum_[3];
    fread(sum_,sizeof(sum_),1,fp);
    sum = atoi(sum_);
    printf("sum_ = %d\n",sum);
    int a=0;
//    user = user->next;
    while(sum > 0)
    {
        fread(name,USERNAME_SIZE*sizeof(char),1,fp);
        fread(passwd,USERPASSWD_SIZE*sizeof(char),1,fp);
        AddUser(user,name,passwd);
        a++;
        printf("LoadList%d - %s\n",a,name);
        sum--;

    }
    fclose(fp);
    return 0;
}
