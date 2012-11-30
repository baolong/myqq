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
        perror("bind");
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
        perror("fp");
        return 1;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    setsockopt(fp,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
    inet_pton(AF_INET,IP,&addr.sin_addr);
    if ((fp = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("fp");
        return 1;
    }
    if (connect(fp,(struct sockaddr *)&addr,sizeof(addr)) < 0)
    {
        perror("connect");
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
int Accept(int fp,struct sockaddr_in *cli_addr)
{
    int fp_;
    char ip[128];
    memset(ip,0x0,sizeof(ip));
    socklen_t len;
    if ((fp_ = accept(fp,(struct sockaddr *)cli_addr,&len)) < 0)
    {
        perror("fp_");
        return 1;
    }
    printf("IP:%链接.\n",inet_ntop(AF_INET,&cli_addr->sin_addr,ip,sizeof(ip)));
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
    if ((num = recv(fp,date,DATELEN*sizeof(char),0)) < 0)
        perror("recv");
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
    if ((num = send(fp,date,strlen(date),0)) < 0)
        perror("send");
    printf("发送了:%d\n",num);
    return num*DATELEN;
}


/****************************************
 *
 * 函数功能：服务器端接收客户端信息
 * 返回值：成功 - 接收到信息的字节数
 *         失败 - -1
 *
 * ************************************/
int RecvMseeage(struct User_List *user,struct Friend *friends,char sender[USERNAME_SIZE],int fp)
{
    unsigned int num = 0;
    char receiver[USERNAME_SIZE],buf[DATELEN];
    memset(receiver,0x0,sizeof(receiver));
    memset(buf,0x0,sizeof(buf));
    if ((num = recv(fp,receiver,sizeof(receiver),0)) < 0)     //接收信息接收人用户名
    {
        perror("receiver num");
        return -1;
    }
    receiver[num] = '\0';
    if ((num = recv(fp,buf,sizeof(buf),0)) < 0)     //接收信息内容
    {
        perror("recvbuf num");
        return -1;
    }
    buf[num] = '\0';
    printf("接收到信息：%s - %s\n",receiver,buf);
    printf("fd: %d\n",GetSocket(user->next,receiver));
    InsertToMessagelog(friends,sender,buf);    //将信息写入发送者的聊天记录
    printf("在线状态：%d\n",OnLine(user,receiver,1));
    printf("链表：%s\n",user->next->next->user.name);
    SendMessage(user,buf,sender);
    if (OnLine(user,receiver,1))      //判断接收人是否在线
        SendMessage(user,buf,receiver);     //在线则直接发送给用户
    else
    {
        InsertOffLineMessage(user,buf,receiver,sender);   //离线则存入用户离线消息列表 
        printf("插入离线消息：%s\n",user->next->next->user.offlinemessage.next->message);
    }
    return num;
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
int SendMessage(struct User_List *user,char message[DATELEN],char name[USERNAME_SIZE])
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
    printf("message:%s\n",message);
    if ((fd = GetSocket(user,name)) < 0)
        return -1;
    if ((num = send(fd,message,strlen(message),0)) > 0)
        return 0;
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
        printf("发送离线消息：%s -%s\n",user->user.name,offline->message);
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
    printf("没有离线消息\n");
    return 0;
}







