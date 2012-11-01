#ifndef USER_H
#define USER_H

#include "user.h"

struct User_Type
{
    char name[15];
    char password[10];
};

struct User_List
{
    struct User_List *front;
    struct User_Type user;
    struct User_List *next;
};

extern struct User_List user;

int InitList(struct User_List *user);
int ListEmpty(struct User_List user);
int ListLength(struct User_List *user);
int Password(struct User_List user,char name[],char password[]);
int AddUser(struct User_List *user,char name[],char password[]);
int UserDel(struct User_List *user,char name[],char password[]);
int DisList(struct User_List *user);

#endif
