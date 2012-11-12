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
 * 函数功能：获取账号个数
 * 返回值：账号个数
 *
 * *********************************/
int ListLength(struct User_List *user)
{
    int num = 0;
    while(user->next != NULL)
    {
        num++;
        user = user->next;
    }
    return(num);
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
int AddUser(struct User_List *user,char name[],char password[],unsigned int num,char friends[][USERNAME_SIZE])
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
    new->user.numoffriend = num;
    if (num > 0)
        CreateFriendList(&new->user.friends,friends);
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

/***********************************
 *
 * 函数功能：查找是否有该用户
 * 返回值：存在该用户 - 0
 *         不存在该用户 - 1
 *
 * ********************************/
int SearchUser(struct User_List *user,char name[])
{
    if (NULL != user->next)
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))
                return 0;
            user = user->next;
        }
    }
    return 1;
}

/************************************
 *
 * 函数功能：删除账号
 * 返回值：  成功 - 1
 *           不存在该用户 - 2
 *           密码错误 - 3
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
 * 返回值：成功 - 1
 *         无数据 - 0
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
    char num_[5];
    struct Friend *temp;
    sum = ListLength(user);
    sum_[1] = sum%10+48;
    sum_[0] = sum/10%10+48;
    sum_[2] = '\0';
    fwrite(sum_,sizeof(sum_),1,fp);
    if (NULL == user->next)
        return 1;
    else
    {
        user = user->next;
        while(user != NULL)
        {
            temp = &user->user.friends;
            Itoa(user->user.numoffriend,num_);
            fwrite(user->user.name,sizeof(user->user.name),1,fp);
            fwrite(user->user.password,sizeof(user->user.password),1,fp);
            fwrite(num_,sizeof(num_),1,fp);
            if (NULL != temp->next)
            {
                temp = temp->next;
                while(NULL != temp)
                {
                    fwrite(temp->name,USERNAME_SIZE*sizeof(char),1,fp);
                    temp = temp->next;
                }
            }
            user = user->next;
        }
    }
    fclose(fp);
    return 1;
}

int LoadList(struct User_List *user)
{
    FILE *fp;
    printf("LoadList:%d\n",user);
    char name[USERNAME_SIZE] = "",passwd[USERPASSWD_SIZE] = "";
    if ((fp = fopen("date","r+")) < 0)
    {
        perror("fp");
        return 1;
    }
    int sumofuser = 0;
    char sumofuser_[3];
    int sumoffriends = 0,numoffriends = 0;
    char sumoffriends_[5];
    memset(&sumofuser_, 0x0, sizeof(sumofuser_));
    memset(&sumoffriends_, 0x0, sizeof(sumoffriends_));
    char nametemp[10][USERNAME_SIZE];
    memset(&nametemp,0x0,sizeof(nametemp));
    fread(sumofuser_, sizeof(sumofuser_), 1, fp);
    sumofuser = atoi(sumofuser_);
    printf("sumofusers:%d\n",sumofuser);
//    user = user->next;
    while(sumofuser > 0)
    {
        fread(name, USERNAME_SIZE*sizeof(char), 1, fp);          //读取用户名
        fread(passwd, USERPASSWD_SIZE*sizeof(char), 1, fp);        //读取用户密码
        fread(sumoffriends_, sizeof(sumoffriends_), 1, fp);    //读取用户好友个数字符串
        printf("name:%s - %s sumoffriends_:%s\n",name,passwd,sumoffriends_);
        sumoffriends = atoi(sumoffriends_);       //将用户好友个数转换成整型
        numoffriends = 0;
        while(numoffriends < sumoffriends)          //循环读取用户好友用户名
        {
            fread(nametemp[numoffriends],USERNAME_SIZE*sizeof(char),1,fp);
            printf("%dfriend's name:%s\n",numoffriends,nametemp[numoffriends]);
            numoffriends++;
        }
        AddUser(user,name,passwd,sumoffriends,nametemp);
        sumofuser--;
    }
    fclose(fp);
    return 0;
}

/**************************************
 *
 * 函数功能：获取、设置用户在线状态
 * 参数：user - 用户列表
 *       name - 欲改变状态的用户名
 *       sign - 功能标识，
 *              值为1则查询状态，
 *              值为2则设置为在线，
 *              值为3则设置为离线。
 * 返回值：成功 - 当前在线状态
 *         失败 - -1
 *
 * *************************************/
int OnLine(struct User_List *user,char name[],int sign)
{
    if (NULL != user->next)
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))
            {
                if (sign == 1)
                    return user->user.online;
                else if (sign == 2)
                {
                    user->user.online = 1;
                    return user->user.online;
                }
                else if (sign == 3)
                {
                    user->user.online = 0;
                    return user->user.online;
                }
            }
            else
                user = user->next;
        }
    }
    return -1;
}

/************************************************
 *
 * 函数功能：添加好友
 * 参数：user - 账号列表
 *       name - 欲添加好友的用户名
 *       nameoffriend - 好友用户名
 * 返回值：成功 - 当前好友个数
 *         用户列表为空 - -1
 *         没有nameoffriend这个用户 - 0
 *
 * ********************************************/
int AddFriend(struct User_List *user,char name[],char nameoffriend[])
{
    if (SearchUser(user,nameoffriend))
    {
        printf("没有该用户.\n");
        return 0;
    }
    if (NULL != user->next)
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))
            {
                printf("X");
                struct Friend *cur;
                cur = &(user->user.friends);
                {
                    if (NULL != cur->next)
                        while(NULL != cur->next)
                            cur = cur->next;
                    struct Friend *new;
                    new = (struct Friend *)malloc(sizeof(struct Friend));
                    strcpy(new->name,nameoffriend);
                    new->front = cur;
                    new->next = NULL;
                    cur->next = new;
                    user->user.numoffriend++;
                }
                return user->user.numoffriend;
            }
            else
                user = user->next;
        }
        return 0;
    }
    return -1;
}

/**********************************************
 *
 * 函数功能：将二维数组转化为好友列表
 * 返回值：成功 - 好友个数
 *
 * ******************************************/
int CreateFriendList(struct Friend *friend,char name[][USERNAME_SIZE])
{
    int num = 0;
    while(0 != strcmp(name[num],""))
    {
        struct Friend *newfriend;
        newfriend = (struct Friend *)malloc(sizeof(struct Friend));
        strcpy(newfriend->name,name[num]);
        newfriend->front = friend;
        newfriend->next = NULL;
        friend->next = newfriend;
        num++;
    }
    printf("添加了%d个好友\n",num);
    return num;
}
/***************************************************
 *
 * 函数功能：获取用户好友列表
 * 返回值：成功 - 好友个数
 *         失败 - -1
 *
 * **********************************************/
int GetFriendList(struct User_List *user,char name[],char namelist[][USERNAME_SIZE])
{
    int num = 0;
    struct Friend *temp;
    if (NULL != user->next)
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))
            {
                temp = &user->user.friends;
                if (NULL != temp->next)
                {
                    temp = temp->next;
                    while(NULL != temp)
                    {
                        strcpy(namelist[num],temp->name);
                        num++;
                        temp = temp->next;
                    }
                    return user->user.numoffriend;
                }
                return -1;
            }
            else
                user = user->next;
        } 
    }
    return -1;
}

int Itoa(int num,char str[])
{
    str[0] = num%10+'0';
//    str[1] = num/10%10+'0';
//    str[2] = num/100%10+'0';
//    str[3] = num/1000%10+'0';
//    str[4] = num/10000%10+'0';
//    str[5] = num/100000%10+'0';
    str[1] = '\0';
    return 0;
}

int Atoi(char str[])
{
//    int a = 0;
//    a = strlen(str);
    int num = 0;
    num += (str[0] - '0');
//    num += (str[1] - '0')*10;
 //   num += (str[2] - '0')*100;
//    num += (str[3] - '0')*1000;
//    num += (str[4] - '0')*10000;
//    num += (str[5] - '0')*100000;
/*    while('\0' != str)
    {
        num = num*10 + (*str - '0');
        str++;
    }*/
    printf("str = %s   int = %d\n",str,num);
    return num;
}
