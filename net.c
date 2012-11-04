#include <stdio.h>
#include <stdlib.h>
#include <sys/type.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "user.h"
#include "net.h"

#define DATELEN 128*2
/*********************************
 *
 * 函数功能：服务器网络初始化
 * 返回值：成功 - 0
 *         socket失败 - 1
 *         bind失败 - 2
 *
 * ******************************/
int NetInit(void)
{
    int fp = NULL;
    struct sockaddr_in addr = NULL;
    if ((fp = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("fp");
        return 1;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_famliy = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    if ((bind(fp,(struct sockaddr *)addr,sizeof(addr))) < 0)
    {
        perror("bind");
        return 2;
    }
    listen(fp,CLIENTNUM);
    return 0;
}

/********************************
 *
 * 函数功能：接受一个链接
 * 返回值：成功 - 链接描述符
 *         失败 - 1
 *
 * ****************************/
int Accept(int fp,struct sockaddr_in *addr)
{
    int fp_ = NULL;
    socklen_t len;
    if ((fp_ = accept(fp,(struct sockaddr *)addr,&len)) < 0)
    {
        perror("fp_");
        return 1;
    }
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
    if ((num = recv(fp,date,sizeof(date),0)) < 0)
    {
        perror("recv");
        return 1;
    }
    return num;
}

int Send(int fp,char *date)
{
    unsigned int num = 0;
    if ((
}
