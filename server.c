/*****************************************
 *
 * 聊天程序服务器程序
 *
 * **************************************/
#include <stdio.h>
#include "user.h"
#include "net.h"
#include <unistd.h>
#include <pthread.h>

int Running(struct User_List *user,int fd,struct User_List *cur,char name[USERNAME_SIZE],char passwd[USERPASSWD_SIZE]);

int main()
{
    int fd,fd_;
    int sign = 0;
    pid_t pid;
    struct User_List *p = &list;     //用户列表
    struct User_List *Cur = &list;;     //存储用户当前节点
    struct sockaddr_in cli_addr;
    char message[DATELEN];
    char name[USERPASSWD_SIZE];
    char passwd[USERPASSWD_SIZE];
    char temp[10][USERNAME_SIZE];
    InitList(p);
    fd = SerNetInit();
    while(1)
    {
        memset(&cli_addr,0x0,sizeof(struct sockaddr_in));
        memset(message,0x0,DATELEN*sizeof(char));
        memset(name,0x0,sizeof(name));
        memset(passwd,0x0,sizeof(passwd));
        if ((fd_ = Accept(fd,&cli_addr)) > 0)
        {
            pid = fork();
            if (pid > 0)
            {
                printf("创建新进程\n");
loop:           Recv(fd_,message);     //接收选项
                printf("接收到sign = %s\n",message);
                Recv(fd_,name);     //接收新用户名
                printf("1\n");
                Recv(fd_,passwd);   //接收新用户密码
                printf("2\n");
                if (0 == strcmp(message,"1"))            //新建账户
                {
                    printf("3\n");
                    AddUser(&list,name,passwd,0,temp);    //添加用户
                    printf("4\n");
                    Send(fd_,"Create Success");    //发送成功信号
                    printf("5\n");
                    goto loop;
//                  break;    //退出，重新登陆
                }
                else if (0 == strcmp(message,"2"))      //登陆已有账号
                {
                    if (!UserChecking(p,name,passwd))
                        return 0;
//                  goto LOGIN_AGAIN;
                    else
                    {
                        OnLine(p,name,2);
                        Send(fd_,"Login Success");
                        while(0 != strcmp(Cur->user.name,name) && NULL != Cur->next)
                            Cur = Cur->next;
                        Running(p,fd_,Cur,name,passwd);
                    }
                }
            }
        }
    }
}


int Running(struct User_List *user,int fd,struct User_List *cur,char name[USERNAME_SIZE],char passwd[USERPASSWD_SIZE])
{
    char FriendList[100][USERNAME_SIZE];
    memset(FriendList,0x0,sizeof(FriendList));
//    GetFriendList(user,name,FriendList);
//    Send(fd,FriendList);
    while(1)
    {
        RecvMseeage(user,&cur->user.friends,name,fd);
    }
}






