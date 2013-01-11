#ifndef MYQQ_H
#define MYQQ_H

#include "user.h"

//数据类型宏 - 字符串
#define DATETYPE_FRIENDSLIST       "1"    //好友列表
#define DATETYPE_COMMUNICATE       "2"    //通信信息
#define DATETYPE_FRIENDSINFO       "3"    //好友信息
#define DATETYPE_OFFLINEMESSAGE    "4"    //离线消息

#define LOGIN_SUCCESS              "5"    //登陆成功
#define LOGIN_PASSWORD_WRONG       "6"    //密码错误

#define CREATEUSER_SUCCESS         "7"    //注册用户成功

#define MENU_ADDFRIEND             "8"    //添加新好友
#define MENU_SENDMESSAGE           "9"    //发送消息

//数据类型宏 - 整形
#define DATETYPE_FRIENDSLIST_I      1
#define DATETYPE_COMMUNICATE_I      2
#define DATETYPE_FRIENDSINFO_I      3
#define MENU_ADDFRIEND_I            8
#define MENU_SENDMESSAGE_I          9


struct arg_dis    //显示线程参数结构体
{
    char *name;    //返回选中用户名
    int *sign;     //功能选项
    int *num;      //选中用户序列号
    char (*friendslist)[USERNAME_SIZE];    //好友列表
    int *onlinesign;    //在线标志
};

struct arg_recv   //网络接收线程参数结构体
{
    int *fd;     //套接口描述符
    char *sender;   //消息发送者
    char *message;  //接收消息缓冲区
    char (*friendslist)[USERNAME_SIZE];  //好友列表
    int *message_sign;
    int *onlinesign;    //在线标识
};

struct arg_key    //按键检测线程参数结构体
{
    int *sign;    //功能选项
    int *num;     //选中用户序列号
    int *num_max;   //列表最大数
    int *logout;    //退出标识
    char *message;    //接收缓冲区
    int *message_sign;   //缓冲区空标识，0表示为空
};

struct arg_ser_newconnect   //处理新链接线程参数结构体
{
    struct User_List *user;    //用户链表
    char name[DATELEN];       //新链接的用户名
    char passwd[DATELEN];     //新链接的用户密码
    char message[DATELEN];    //消息缓冲区
    char temp[2][USERNAME_SIZE];
    int *fd;                  //链接的套接字描述符
    int *sumofcli;            //当前在线用户人数
};

struct arg_ser_key       //服务器按键检测线程参数结构体
{
    int *sign;     //功能选择
    int *num;      //选中对象序号
    int *num_max;     //列表最大对象数
    int *logout;      //退出标识
    char *message;     //消息缓冲区
    int *message_sign;     //缓冲区空标识，0表示为空
};

struct arg_ser_dis    //显示进程参数结构
{
    struct User_List *user;    //用户链表
    int *num;                 //被选中用户序号
    int *sumofuser;           //总用户数
    char *name_cur;           //返回被选中用户名
};



#endif










