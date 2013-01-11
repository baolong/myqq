/*****************************************
 *
 * 聊天程序服务器程序
 *
 * **************************************/
#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "net.h"
#include <unistd.h>
#include <pthread.h>

struct arg
{
    struct User_List *user;
    char name[USERNAME_SIZE];
    char passwd[USERPASSWD_SIZE];
    char message[DATELEN];
    char temp[10][USERNAME_SIZE];
    int fd_;
};

int Running(struct User_List *user,int fd,struct User_List *cur,char name[USERNAME_SIZE],char passwd[USERPASSWD_SIZE]);
void *new(void *argv);

int main()
{
    struct arg argv1[100],*argv[100];
    int fd,fd_[100];
    int sumofcli = 0;
    int sign = 0;
    int mid;
    pthread_t pth_t[100];
    struct User_List *p = &list;     //用户列表
    struct sockaddr_in cli_addr[100];
    char message[100][DATELEN];
    char name[100][USERPASSWD_SIZE];
    char passwd[100][USERPASSWD_SIZE];
    char temp[100][USERNAME_SIZE];
    InitList(p);
    fd = SerNetInit();
    memset(fd_,0x0,sizeof(fd_));
    while(sumofcli < 100)
    {
        argv[sumofcli] = &argv1[sumofcli];
        memset(&cli_addr[sumofcli],0x0,sizeof(struct sockaddr_in));
        memset(message[sumofcli],0x0,DATELEN*sizeof(char));
        memset(name[sumofcli],0x0,sizeof(name));
        memset(passwd[sumofcli],0x0,sizeof(passwd));
        if ((fd_[sumofcli] = Accept(fd,&cli_addr[sumofcli])) > 0)
        {
            printf("循环次数：%d\n",sumofcli);
            argv[sumofcli]->user = p;
            strcpy(argv[sumofcli]->name,name[sumofcli]);
            strcpy(argv[sumofcli]->passwd,passwd[sumofcli]);
            strcpy(argv[sumofcli]->temp[0],temp[0]);
            argv[sumofcli]->fd_ = fd_[sumofcli];
            mid = pthread_create(&pth_t[sumofcli],NULL,new,argv[sumofcli]);
            printf("sumofcli %d\n",sumofcli);
            sumofcli++;
        }
    }
}


int Running(struct User_List *user,int fd,struct User_List *cur,char name[USERNAME_SIZE],char passwd[USERPASSWD_SIZE])
{
    int num = 0;
    char FriendList[100][USERNAME_SIZE];
    memset(FriendList,0x0,sizeof(FriendList));
//    GetFriendList(user,name,FriendList);
//    Send(fd,FriendList);
    while(1)
    {
        num = RecvMseeage(user,&cur->user.friends,name,fd);
        if (num <= 0)
            break;
    }
}

void *new(void *argv1)
{
    int num = 0;
    struct User_List *cur;
    struct arg *argv;
    argv = argv1;
    cur = argv->user;
    printf("创建新线程\n");
    memset(argv->message,0x0,DATELEN*sizeof(char));
loop:   Recv(argv->fd_,argv->message);     //接收选项
    printf("接收到sign = %s\n",argv->message);
    num = Recv(argv->fd_,argv->name);     //接收新用户名
    printf("1\n");
    Recv(argv->fd_,argv->passwd);   //接收新用户密码
    printf("2\n");
    if (0 == strcmp(argv->message,"1"))            //新建账户
    {
        printf("3\n");
        AddUser(argv->user,argv->name,argv->passwd,0,argv->temp);    //添加用户
        printf("4\n");
        Send(argv->fd_,"Create Success");    //发送成功信号
        printf("5\n");
        goto loop;
//      break;    //退出，重新登陆
    }
    else if (0 == strcmp(argv->message,"2"))      //登陆已有账号
    {
        if (!UserChecking(argv->user,argv->name,argv->passwd))
            return 0;
//      goto LOGIN_AGAIN;
        else
        {


            printf("OnLine Setting:%s\n",argv->name);
            printf("状态：%d\n",OnLine(argv->user,argv->name,2));
            printf("fd_ = %d\n",argv->fd_);
            SetSocket(argv->user,argv->name,argv->fd_);
            DisList(argv->user);
            Send(argv->fd_,"Login Success");
            while(0 != strcmp(cur->user.name,argv->name) && NULL != cur->next)
                cur = cur->next;
            printf("cur :%s\n",cur->user.name);
            SendOffLineMessage(cur);
            Running(argv->user,argv->fd_,cur,argv->name,argv->passwd);
            OnLine(argv->user,argv->name,3);     //设置为离线
            SetSocket(cur,argv->name,0);   //清楚套接字描述符
            close(argv->fd_);
        }
    }
}




