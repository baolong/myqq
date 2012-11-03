#ifndef USER_H
#define USER_H

#include "user.h"
#define USERNAME_SIZE 15
#define USERPASSWD_SIZE 10
struct User_Type
{
    char name[USERNAME_SIZE];
    char password[USERPASSWD_SIZE];
};

struct User_List
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
int AddUser(struct User_List *user,char name[],char password[]);
int UserNameChange(struct User_List *user,char *oldname,char *newname,char *passwd);
int UserPasswdChange(struct User_List *user,char *name,char *passwd,char *oldpasswd);
int UserDel(struct User_List *user,char name[],char password[]);
int DisList(struct User_List *user);
int SaveList(struct User_List *user);
int LoadList(struct User_List *user);
#endif
