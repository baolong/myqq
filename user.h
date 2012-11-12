#ifndef USER_H
#define USER_H

#include "net.h"
#include "user.h"
#include <time.h>

#define DATELEN 128*2
#define USERNAME_SIZE 15     //用户名长度
#define USERPASSWD_SIZE 10      //密码长度

struct MessageLog
{
    char message[DATELEN];
    char user[USERNAME_SIZE];
    struct MessageLog *front;
    struct MessageLog *next;
};

struct Friend       //用户好友列表结构体
{
    char name[USERNAME_SIZE];
    struct MessageLog messagelog;
    struct Friend *front;
    struct Friend *next;
};

struct OffLineMessage       //离线消息结构体
{
    char message[DATELEN];
    char Sender[USERNAME_SIZE];
    char Recviver[USERNAME_SIZE];
    char SendTime[21];
    struct OffLineMessage *front;
    struct OffLineMessage *next;
};

struct User_Type    //用户信息结构体
{
    char name[USERNAME_SIZE];
    char password[USERPASSWD_SIZE];
    int online;
    unsigned int numoffriend;
    struct Friend friends;
};

struct User_List      //用户列表结构体
{
    struct User_Type user;
    struct User_List *front;
    struct User_List *next;
};

extern struct User_List list;

int InitList(struct User_List *user);
int ListEmpty(struct User_List user);
int ListLength(struct User_List *user);
int UserChecking(struct User_List *user,char name[],char password[]);
int AddUser(struct User_List *user,char name[],char password[],unsigned int num,char friends[][USERNAME_SIZE]);
int UserNameChange(struct User_List *user,char *oldname,char *newname,char *passwd);
int UserPasswdChange(struct User_List *user,char *name,char *passwd,char *oldpasswd);
int UserDel(struct User_List *user,char name[],char password[]);
int DisList(struct User_List *user);
int SaveList(struct User_List *user);
int LoadList(struct User_List *user);
int OnLine(struct User_List *user,char name[],int sign);
int AddFriend(struct User_List *user,char name[],char nameoffriend[]);
int SearchUser(struct User_List *user,char name[]);
int Itoa(int num,char str[]);
int Atoi(char str[]);
int CreateFriendList(struct Friend *friend,char name[][USERNAME_SIZE]);


#endif
