#include "user.h"
#include "net.h"
#include "windows.h"
#include <unistd.h>
#include <pthread.h>

#define USER_MAX 100    //最大在线人数
#define FRIENDS_MAX 100    //最大好友数
#define SENDDELAYTIME 10   //发送信息后延迟时间 单位毫秒
struct arg
{
    struct User_List *user;
    char name[USERNAME_SIZE];
    char passwd[USERPASSWD_SIZE];
    char message[DATELEN];
    char temp[2][USERNAME_SIZE];
    int fd;
};

int main()
{
    struct arg argv1[USER_MAX],*argv[USER_MAX];
    int fd_ = 0,fd[USER_MAX] = {0};
    int sumofcli = 0;
    char User_List *user = &list;
    struct sockaddr_in cli_addr[USER_MAX];
    char message[USER_MAX][DATELEN];
    char name[USER_MAX][USERNAME_SIZE];
    char passwd[USER_MAX][USERPASSWD_SIZE];
    char temp[2][USERNAME_SIZE];
    
    //各类初始化
    InitList(user);
    fd_ = SerNetInit();
    WindowInit();
ALLOW: 
    while(sumofcli < USER_MAX)    //如果在线用户小于最大限制数，则继续接受用户连接
    {
        argv[sumofcli] = &argv1[sumofcli];
        //对相关数据结构进行初始化
        memset(&cli_addr[sumofcli],0x0,sizeof(struct sockaddr_in));
        memset(message[sumofcli],0x0,DATELEN*sizeof(char));
        memset(name[sumofcli],0x0,USERNAME_SIZE*sizeof(char));
        memset(passwd[sumofcli],0x0,USERPASSWD_SIZE*sizeof(char));
        if ((fd[sumofcli] = Accept(fd_,&cli_addr[sumofcli])) > 0)
        {
            argv[sumofcli]->user = user;
            argv[sumofcli]->fd = fd[sumofcli];
            pthread_create(&pth_t[sumofcli],NULL,NewUserConnect,argv[sumofcli]);   //为用户创建新线程
            sumofcli++;   //在线用户数加一
        }
    }
    while(USER_MAX >=  sumofcli)   //死循环，等待在线用户数小于最大限制数
        goto ALLOW;    //则继续接受用户连接
    return 0;
}

void *NewUserConnect(void argv1)
{
    int num = 0;
    struct User_List *cur;
    struct arg *argv;
    char FriendList[FRRIENDS_MAX][USERNAME_SIZE];
    argv = argv1;
    cur = argv->user;
    memset(argv,0x0,sizeof(struct arg));
loop:
    Recv(argv->fd,argv->message);   //接收选项
    Recv(argv->fd,argv->name);
    Recv(argv->fd,argv->passwd);
    if (0 == strcmp(argv->message,"1"))   //注册用户
    {
        AddUser(argv->user,argv->name,argv->passwd,0,argv->temp);
        Send(argv->fd,"Create Success");
        goto loop;     //退出，重新登陆
    }
    else if (0 == strcmp(argv->message,"2"))    //登陆账户
    {
        char receiver[USERNAME_SIZE];
        char message[DATELEN];
        unsigned int numoffriend = 0;
        OnLine(argv->user,argv->name,2);  //设置用户为在线状态
        SetSocket(argv->user,argv->name,argv->fd);   //设置用户套接字描述符
        Send(argv->fd,"Login Success");   //发送登陆成功信息
        while(0 != strcmp(cur->user.name,argv->name) && NULL != cur->next)
            cur = cur->next;
        GetFriendList(argv->user,argv->name,FriendList);  //获取用户好友列表
        while(numoffriend < cur->user.numoffriend)    //逐一发送好友用户名
        {
            Send(argv->fd,FriendList[numoffriend]);
            numoffriend++;
            usleep(SENDDELAYTIME);    //发送后延时
        }
        Send(argv->fd,"FriendList success");   //发送好友列表发送完成信号
        usleep(SENDDELAYTIME);
        SendOffLineMessage(cur);    //发送离线消息
        while(1)
        {
            memset(receiver,0x0,sizeof(receiver));
            memset(message,0x0,sizeof(message));
            num = RecvMessage(argv->user,&cur->user.friends,argv->name,argv->fd,message,receiver);
            if (1 == num)   //添加好友
            {
                Recv(argv->fd,receiver);   //接收欲添加的好友用户名
                AddFriend(argv->user,argv->name,recviver);   //添加好友
                GetFriendList(argv->user,argv->name,FriendList);  //获取用户好友列表
                while(numoffriend < cur->user.numoffriend)    //逐一发送好友用户名
                {
                    Send(argv->fd,FriendList[numoffriend]);
                    numoffriend++;
                    usleep(SENDDELAYTIME);    //发送后延时
                }
                Send(argv->fd,"FriendList success");   //发送好友列表发送完成信号 
            }
            else if (0 == num)  //发送信息
            {
                RecvMessage(argv->user,&cur->user,friends,argv->name,argv->fd,message,receiver);
                if (OnLine(argv->user,receiver,1))    //判断接收人是否在线
                    SendMessage(argv->user,message,receiver);   //在线则直接发送给用户
                else
                    InsertOffLineMessage(argv->user,message,receiver,argv->name);     //不在线则存入用户离线消息列表
            }
            else   //出错,则自动断开链接
                break;
        }

        close(argv->fd);    //关闭用户套接字描述符
        OnLine(argv->user,argv->name,3);   //设置用户为离线状态
        SetSocket(argv->user,argc->name,0);   //清除套接字描述符
        sumofcli--;
    }
}








