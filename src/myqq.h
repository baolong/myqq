#ifndef MYQQ_H
#define MYQQ_H

#include "user.h"

#define DATETYPE_FRIENDSLIST       "1" 
#define DATETYPE_COMMUNICATE       "2"
#define DATETYPE_FRIENDSINFO       "3"
#define DATETYPE_OFFLINEMESSAGE    "4"

#define LOGIN_SUCCESS              "5"
#define LOGIN_PASSWORD_WRONG       "6"

#define CREATEUSER_SUCCESS         "7"

#define MENU_ADDFRIEND             "8"
#define MENU_SENDMESSAGE           "9"

#define DATETYPE_FRIENDSLIST_I      1
#define DATETYPE_COMMUNICATE_I      2
#define DATETYPE_FRIENDSINFO_I      3
#define MENU_ADDFRIEND_I            8
#define MENU_SENDMESSAGE_I          9


struct arg_dis
{
    char *name;    //返回选中用户名
    int *sign;     //功能选项
    int *num;      //选中用户序列号
    char (*friendslist)[USERNAME_SIZE];    //好友列表
    int *onlinesign;    //在线标志
};

struct arg_recv
{
    int *fd;     //套接口描述符
    char *sender;   //消息发送者
    char *message;  //接收消息缓冲区
    char (*friendslist)[USERNAME_SIZE];  //好友列表
    int *message_sign;
    int *onlinesign;    //在线标识
};

struct arg_key
{
    int *sign;    //功能选项
    int *num;     //选中用户序列号
    int *num_max;   //列表最大数
    int *logout;    //退出标识
    char *message;    //接收缓冲区
    int *message_sign;   //缓冲区空标识，0表示为空
};

struct arg_ser_newconnect
{
    struct User_List *user;
    char name[DATELEN];
    char passwd[DATELEN];
    char message[DATELEN];
    char temp[2][USERNAME_SIZE];
    int *fd;
    int *sumofcli;
};

struct arg_ser_key
{
    int *sign;
    int *num;
    int *num_max;
    int *logout;
    char *message;
    int *message_sign;
};

struct arg_ser_dis    //显示进程参数结构
{
    struct User_List *user;
    int *num;
    int *sumofuser;
    char *name_cur;
};



#endif










