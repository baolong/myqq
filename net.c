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
int SerNetInit(void)
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
int CliNetInit(void)
{
    int fp;
    char IP_addr[10];
    struct sockaddr_in addr;
    if ((fp = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("fp");
        return 1;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
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
//    date[num] = '\0';
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
    return num*DATELEN;
}


/****************************************
 *
 * 函数功能：服务器端接收客户端信息
 * 返回值：成功 - 接收到信息的字节数
 *         失败 - -1
 *
 * ************************************/
int RecvMseeage(struct Friend *friends,char sender[USERNAME_SIZE],int fp)
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
    if ((num = recv(fp,buf,sizeof(buf),0)) < 0)     //接收信息内容
    {
        perror("recvbuf num");
        return -1;
    }
    InsertToMessagelog(friends,sender,buf);    //将信息写入发送者的聊天记录
    if (OnLine(user,name,1))      //判断接收人是否在线
        SendMessage(fp,buf,receiver);     //在线则直接发送给用户
    else
                                    //离线则存入用户离线消息列表 
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
    if ((fd = GetSocket(user,name)) < 0)
        return -1;
    if ((num = send(fd,message,sizeof(message),0)) > 0)
        return 0;
    return -1;
}








