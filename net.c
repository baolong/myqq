#include "net.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


/*********************************
 *
 * 函数功能：服务器网络初始化
 * 返回值：成功 - 侦听套接字描述符
 *         socket失败 - 1
 *         bind失败 - 2
 *
 * ******************************/
int SerNetInit()
{
    int fp;
    struct sockaddr_in addr;
    if ((fp = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("fp");
        return 1;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    int i = 1;
    setsockopt(fp,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
    if ((bind(fp,(struct sockaddr *)&addr,sizeof(addr))) < 0)
    {
//        perror("bind");
        return 2;
    }
    listen(fp,CLIENTNUM);
    return fp;
}

/****************************************
 *
 * 函数功能：客户端网络初始化
 * 返回值：成功 - 链接后的套接字描述符
 *         socket失败 - 1
 *         connect失败 - 2
 *
 ***************************************/
int CliNetInit()
{
    int fp;
    int i = 1;
    struct sockaddr_in addr;
    if ((fp = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
//        perror("fp");
        return 1;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    setsockopt(fp,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
    inet_pton(AF_INET,IP,&addr.sin_addr);
    if ((fp = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
//        perror("fp");
        return 1;
    }
    if (connect(fp,(struct sockaddr *)&addr,sizeof(addr)) < 0)
    {
//        perror("connect");
        return 2;
    }
    return fp;
}


/********************************
 *
 * 函数功能：接受一个链接
 * 返回值：成功 - 链接描述符
 *         失败 - 1
 *
 * ****************************/
int Accept(int fp,struct sockaddr_in cli_addr)
{
    int fp_;
    socklen_t len;
    len = 0;
    if ((fp_ = accept(fp,(struct sockaddr *)&cli_addr,&len)) < 1)
        return 1;
    return fp_;
}


/*********************************
 *
 * 函数功能：接收数据
 * 返回值：成功 - 接收到的字节数
 *         失败 - 1
 *
 ********************************/
int Recv(int fp,char *date)
{
    unsigned int num = 0;
    if ((num = recv(fp,date,DATELEN*sizeof(char),0)) < 0);
//        perror("recv");
    move(40,20);
    printw("接收到：%s",date);
    move(40,40);
    refresh();
    date[num] = '\0';
    return num*DATELEN;
}

/********************************
 *
 * 函数功能：发送数据
 * 返回值：成功 - 发送的字节数
 *         失败 - 1
 *
 ********************************/
int Send(int fp,char *date)
{
    unsigned int num = 0;
    if ((num = send(fp,date,strlen(date),0)) < 0);
//        perror("send");
    move(20,10);
    printw("发送了:%d",num);
    refresh();
    return num*DATELEN;
}


/****************************************
 *
 * 函数功能：服务器端接收客户端信息
 * 返回值：发送信息 - 0
 *         添加好友 - 1
 *         错误 - -1
 *
 * ************************************/
int RecvMessage(struct User_List *user,struct Friend *friends,char sender[USERNAME_SIZE],int fp,char buf[DATELEN],char receiver[USERNAME_SIZE])
{
    unsigned int num = 0;
    char sign[2];
    memset(receiver,0x0,sizeof(receiver));
    memset(buf,0x0,sizeof(buf));
    if (recv(fp,sign,sizeof(sign),0) <= 0)   //接收功能选择
        return -1;
    move(17,30);
    printw("测试10");
    refresh();
    sleep(3);
    if (0 == strcmp(sign,"1"))   //如果为1，则为添加好友，返回1
        return 1;
    move(18,30);
    printw("测试11");
    refresh();
    sleep(3);
    if ((num = recv(fp,receiver,sizeof(receiver),0)) < 0)     //接收信息接收人用户名
        return -1;
    receiver[num] = '\0';
    if ((num = recv(fp,buf,sizeof(buf),0)) < 0)     //接收信息内容
        return -1;
    buf[num] = '\0';
    InsertToMessagelog(friends,sender,buf);    //将信息写入发送者的聊天记录
//    if (OnLine(user,receiver,1))      //判断接收人是否在线
//        SendMessage(user,buf,receiver);     //在线则直接发送给用户
//    else
//        InsertOffLineMessage(user,buf,receiver,sender);   //离线则存入用户离线消息列表 
    return 0;
}
/*****************************************
 *
 * 函数功能：服务器端将信息传给接收者用户
 * 参数：Friends - 该用户的好友列表
 *       fp - 套接口描述符
 *       message = 消息结构体
 *       name - 接收消息的用户名
 * 返回值：成功 - 0
 *
 * *************************************/
int SendMessage(struct User_List *user,struct Friend *friends,char message[DATELEN],char name[USERNAME_SIZE])
{
    unsigned int num = 0;
    int fd;
/*    while(NULL != user->next)
    {
        if (0 == strcmp(user->name,name))
            break;
        else
            user = user->next;
    }*/
    if ((fd = GetSocket(user,name)) < 0)
        return -1;
    if ((num = send(fd,message,strlen(message),0)) > 0)
    {
        InsertToMessagelog(friends,name,message);   //将信息写入接收者聊天记录
        return 0;
    }
    return -1;
}

/*******************************************
 *
 * 函数功能：发送并删除用户离线消息
 * 参数：user - 欲检测离线消息的用户节点
 * 返回值： 成功 - 返回离线消息数
 *          没有离线消息 - 0
 *
 * ****************************************/
int SendOffLineMessage(struct User_List *user)
{
    int fd;
    int num = 0;
    struct OffLineMessage *offline = &user->user.offlinemessage;
    struct OffLineMessage *del;
    fd = user->user.socket;
    if (NULL != offline->next)
    {
        offline = offline->next;
        del = offline;
//        printf("发送离线消息：%s -%s\n",user->user.name,offline->message);
        Send(fd,offline->message);
        if (NULL != offline->next)
        {
            offline->front->next = offline->next;
            offline->next->front = offline->front;
        }
        else
            offline->front->next = NULL;
        free(del);
        return num;
    }
//    printf("没有离线消息\n");
    return 0;
}

/**********************************
 *
 * 函数功能：检测接收到信息的
 *           第一个字节，以判断功能
 * 返回值：返回第一个字节对应整数值
 *
 * ******************************/
int JudgeFirstWord(char str[DATELEN])
{
    int num = 0;
    num = str[0] - '0';
    return num;
}





