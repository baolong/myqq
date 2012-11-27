#include <stdio.h>
#include "user.h"
#include "net.h"
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd;
    int sign = 0;
    char message[DATELEN];
    char name[USERNAME_SIZE];
    char passwd[USERPASSWD_SIZE];
    memset(message,0x0,sizeof(message));
    memset(name,0x0,sizeof(name));
    memset(passwd,0x0,sizeof(passwd));
    fd = CliNetInit();       //客户端网络初始化
loop:    printf("1.创建新账户.   2.登陆\n");
    scanf("%s",message);
//    Itoa(sign,message);
    Send(fd,message);
    sign = message[0] - '0';
    usleep(10);      //挂起10微秒
    if (1 == sign)
    printf("请天下以下信息:\n");
    printf("请输入用户名：\n");
    scanf("%s",name);
    printf("请输入密码：\n");
    scanf("%s",passwd);
    Send(fd,name);
    printf("1\n");
    usleep(10);
    Send(fd,passwd);
    printf("2\n");
    usleep(10);
    Recv(fd,message);
    if (1 == sign)
    {
        if (0 == strcmp(message,"Create Success"))
        {
            printf("注册成功，请输入账号密码登陆:\n");
            goto loop;
//            goto Login;
        }
        else
        {
            printf("注册失败，请重新输入用户名密码：\n");
            goto loop;
//            goto Login;
        }
    }
    else if (2 == sign)
    {
        int sign = 0;
        printf("1 接收 2.发送\n");
        scanf("%d",&sign);
        while(1)
        {
            memset(message,0x0,sizeof(message));
            if (sign == 2)
            {
                Send(fd,"c");
                usleep(10);
                scanf("%s",message);
                Send(fd,message);
                usleep(10);
                memset(message,0x0,sizeof(message));
            }
            Recv(fd,message);
            printf("接收到:%s\n",message);
        }
    }
}






