#include "net.h"

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
int Accept(int fp,struct sockaddr_in *addr)
{
    int fp_;
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
    if ((num = recv(fp,date,DATELEN*sizeof(char),0)) < 0)
        perror("recv");
    return num;
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
    return num;
}
