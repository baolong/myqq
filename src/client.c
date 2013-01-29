#include "user.h"
#include "net.h"
#include "windows.h"
#include <pthread.h>
#include <stdlib.h>
#include "include.h"
#include <curses.h>

void *Keyboard(void *argv1);
void *Display(void *argv1);
void *RecvMsg(void *argv1);

int main()
{
    struct arg_key argv_key1,*argv_key;
    struct arg_dis argv_dis1,*argv_dis;
    struct arg_recv argv_recv1,*argv_recv;
    struct Friend Friendlist,*friendlist;
    friendlist = &Friendlist;
    int fd = 0;
    int x = 0,y = 0;
    pthread_t pth_t,pth_dis,pth_recv;
    char name_loacl[USERNAME_SIZE];
    char message_recv[DATELEN];    //信息接收缓冲区
    char message_send[DATELEN];   //信息发送缓冲区
    char name[USERNAME_SIZE];     //用户名缓冲区
    char passwd[USERPASSWD_SIZE];   //用户密码缓冲区
    char friendslist[USER_MAX][USERNAME_SIZE];   //好友列表
    char sender[USERNAME_SIZE];
    char receiver[USERNAME_SIZE];

    int onlinesign[USER_MAX];   //好友在线状态
    int message_sign = 1;  //发送缓冲区标识，0表示缓冲区为空
    int sign = 0;   //功能选择标识
    int num[4] = {0};   //各模块的被选中对象的编号
    int num_max[4] = {0};   //各模块对大对象数
    int logout  = 0;   //退出标识
    int num_temp = 0; 
    int sumoffriends = 0;
    int loginsign = 0;
    //进行相关的初始化
    argv_key = &argv_key1;
    argv_dis = &argv_dis1;
    argv_recv = &argv_recv1;

    argv_key->num = num;
    argv_key->num_max = num_max;
    argv_key->sign = &sign;
    argv_key->logout = &logout;
    argv_key->message = message_send;
    argv_key->message_sign = &message_sign;

    argv_dis->name = name;
    argv_dis->name_loacl = name_loacl;
    argv_dis->sign = &sign;
    argv_dis->num = num;
    argv_dis->sumoffriends = &num_max[0];
    while(num_temp < FRIENDS_MAX)
    {
        argv_dis->friendslist[num_temp] = friendslist[num_temp];
        num_temp++;
    }
    argv_dis->onlinesign = onlinesign;
    
    argv_recv->fd = &fd;
    argv_recv->sender = sender;
    argv_recv->message = message_recv;
    argv_recv->sumoffriends = &num_max[0];

    num_temp = 0;
    while(num_temp < FRIENDS_MAX)
    {
        argv_recv->friendslist[num_temp] = friendslist[num_temp];
        num_temp++;
    }
    argv_recv->onlinesign = onlinesign;
    argv_recv->message_sign = &message_sign;

    WindowInit();
    fd = CliNetInit();

loop:
//    getch();
//    Cli_Windows(&x,&y);    //初始化窗口界面
    GetSize(&x,&y);
    loginsign = 1;
    memset(message_recv,0x0,sizeof(message_recv));
    memset(name_loacl,0x0,sizeof(name_loacl));
    memset(passwd,0x0,sizeof(passwd));
    loginsign  = Cli_Welcome();    //进入欢迎界面
    message_recv[0] = loginsign + '0';
    message_recv[1] = '\0';
    Send(fd,message_recv);    //发送选项
    usleep(SENDDELAYTIME);
    if (1 == loginsign)    //登陆
        Cli_Login(x,y,name_loacl,passwd);   //进入登陆界面，并返回用户名、密码
    else if (2 == loginsign)    //注册
        Cli_Apply(x,y,name_loacl,passwd);   //进入注册界面，并返回用户名、密码
    else
    {
        endwin();
        return 0;
    }
    Send(fd,name_loacl);    //发送用户名
    usleep(SENDDELAYTIME);
    Send(fd,passwd);    //发送密码
    usleep(SENDDELAYTIME);
    memset(message_recv,0x0,sizeof(message_recv));   //初始化message
    Recv(fd,message_recv);    //接收提示信息
    if (0 == strcmp(message_recv,CREATEUSER_SUCCESS))   //注册成功，返回登录
    {
        goto loop;
    }
    if (0 == strcmp(message_recv,LOGIN_PASSWORD_WRONG))
        goto loop;
    if (0 == strcmp(message_recv,LOGIN_SUCCESS))    //登录成功
    {
        int num = 0;
        clear();
        pthread_create(&pth_dis,NULL,Display,argv_dis);   //创建显示线程
        pthread_create(&pth_t,NULL,Keyboard,argv_key);  //创建键盘控制线程
        pthread_create(&pth_recv,NULL,RecvMsg,argv_recv);  //创建接收信息线程
        while(1)
        {
            if (message_sign == 1)
            {
                num++;
//                Send(fd,MENU_SENDMESSAGE);
                send(fd,MENU_SENDMESSAGE,3*sizeof(char),0);
                usleep(SENDDELAYTIME);
//                Send(fd,"2");
                send(fd,"2",USERNAME_SIZE*sizeof(char),0);
                usleep(SENDDELAYTIME);
//                Send(fd,message_send);
                send(fd,message_send,DATELEN*sizeof(char),0);
                usleep(SENDDELAYTIME);
/*                move(y-4,18);
                printw("                         ");
                move(y-4,18);
                printw("发送成功:%d - %s",num,message_send);
                refresh();*/
                message_sign = 0;
            }
            usleep(SENDDELAYTIME*1000);

/*            move(31,20);
            printw("dis:%d - num[0]:%d - num[1]:%d - num[2]:%d - sign:%d - logout:%d",dis,num[0],num[1],num[2],sign,logout);
            refresh();
            usleep(1000);*/
        }
    }
}

/***************************
 *
 * 函数功能：客户端键盘控制线程
 *
 * *************************/
void *Keyboard(void *argv1)
{
    struct arg_key *argv;
    argv = (struct arg_key *)argv1;  //初始化线程参数结构体
    KeyboardControl(argv->num,argv->num_max,argv->sign,argv->logout,argv->message,argv->message_sign);
}

/*************************
 *
 * 函数功能：客户端显示线程
 *
 * *********************/
void *Display(void *argv1)
{
    struct arg_dis *argv;
    argv = (struct arg_dis *)argv1;   //初始化线程参数结构体
    int x = 0,y = 0;    //屏幕最大尺寸变量
    int x1 = 0,y1 = 0;
    noecho();
    leaveok(stdscr,1);
    while(1)
    {
        clear();
        Cli_Windows(&x,&y);   //客户端界面框架
        Cli_DisplayFriendList(x,y,argv->friendslist,argv->num[0],*argv->sumoffriends,argv->name,argv->onlinesign);   //显示好友列表 
//        Cli_DisPlayMsg(x,y,argv->user,argv->name_loacl,argv->name);
        leaveok(stdscr,1);
        getyx(stdscr,y1,x1);
        move(1,2);
        printw("sign - 0  -  1 -  2 - 3");
        move(2,2);
        printw("                            ");
        move(2,2);
        printw(" %d -  %d - %d - %d -",*argv->sign,argv->num[0],argv->num[1],argv->num[2]);
        refresh();
        move(y1,x1);
        usleep(100000);
    } 
}

/*****************************
 *
 * 函数功能：客户端接收信息线程
 * 
 * *************************/
void *RecvMsg(void *argv1)
{
    struct arg_recv *argv;
    argv = (struct arg_recv *)argv1;
    int datetype = 0;
    int numoffriend = 0;
    while(1)
    {
        memset(argv->message,0x0,DATELEN*sizeof(char));
        Recv(*argv->fd,argv->message);     //接收数据类型
        datetype = atoi(argv->message);   //将字符串转换为整形
        if (DATETYPE_FRIENDSLIST_I == datetype)      //接收好友列表
        {
            memset(argv->message,0x0,DATELEN*sizeof(char));
            Recv(*argv->fd,argv->message);
            *argv->sumoffriends = atoi(argv->message);
            numoffriend = 0;
            while(numoffriend < *argv->sumoffriends)
            {
                Recv(*argv->fd,argv->friendslist[numoffriend + 1]);  //接收好友列表
                numoffriend++;
            }
        }
        else if (DATETYPE_FRIENDSINFO_I == datetype)  //好友信息
        {
            
        }
        else if (DATETYPE_COMMUNICATE_I == datetype)  //通信信息
        {
            Recv(*argv->fd,argv->message);
        }
    }
}





