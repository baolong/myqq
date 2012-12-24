#include "user.h"
#include "net.h"
#include "windows.h"
#include <pthread.h>

void *Keyboard(void *argv1);

struct arg_dis
{
    struct User_List *user;
    char *name;
    int *sign;
    int *dis;
    int *num;
};

struct arg_key
{
    int *sign;
    int *dis;
    int *num;
    int *max_num;
    int *logout;
};

int main()
{
    struct arg_key argv1,*argv;
    int fd = 0;
    int x = 0,y = 0;
    pthread_t pth_t;
    char message[DATELEN];
    char name[USERNAME_SIZE];
    char passwd[USERPASSWD_SIZE];
    char *passwd_temp[USERPASSWD_SIZE];

    int sign = 0;   //功能选择标识
    int dis = 0;    //显示与否的信号
    int num[3] = {0};   //各模块的被选中对象的编号
    int max_num[3] = {0};   //各模块对大对象数
    int logout  = 0;   //退出标识

    //进行相关的初始化
    argv = &argv1;
    argv->dis = &dis;
    argv->num = num;
    argv->max_num = max_num;
    argv->sign = &sign;
    argv->logout = &logout;

    WindowInit();
    fd = CliNetInit();

loop:
//    getch();
    Cli_Windows(&x,&y);    //初始化窗口界面
    sign = 0;
    memset(message,0x0,sizeof(message));
    memset(name,0x0,sizeof(name));
    memset(passwd,0x0,sizeof(passwd));
    sign  = Cli_Welcome();    //进入欢迎界面
    message[0] = sign + '0';
    message[1] = '\0';
    Send(fd,message);    //发送选项
    move(1,10);
    printw("sign：%d",sign);
    refresh();
    if (1 == sign)    //登陆
        Cli_Login(x,y,name,passwd);   //进入登陆界面，并返回用户名、密码
    else if (2 == sign)    //注册
        Cli_Apply(x,y,name,passwd);   //进入注册界面，并返回用户名、密码
    else
    {
        endwin();
        return 0;
    }
    Send(fd,name);    //发送用户名
    usleep(SENDDELAYTIME);
    Send(fd,passwd);    //发送密码
    usleep(SENDDELAYTIME);
    memset(message,0x0,sizeof(message));   //初始化message
    Recv(fd,message);    //接收提示信息
    if (0 == strcmp(message,"Create Success"))   //注册成功，返回登录
    {
        goto loop;
    }
    if (0 == strcmp(message,"Password Wrong"))
        goto loop;
    if (0 == strcmp(message,"Login Success"))    //登录成功
    {
        pthread_create(&pth_t,NULL,Keyboard,argv);  //创建键盘控制线程
        while(1)
        {
            move(31,20);
            printw("dis:%d - num[0]:%d - num[1]:%d - num[2]:%d - sign:%d - logout:%d",dis,num[0],num[1],num[2],sign,logout);
            refresh();
            usleep(1000);
        }
//        while(2)    //正式进入聊天服务
    }
}

void *Keyboard(void *argv1)
{
    struct arg_key *argv;
    argv = (struct arg_key *)argv1;
    KeyboardControl(argv->dis,argv->num,argv->max_num,argv->sign,argv->logout);
}

void *Display(void *argv1)
{
    struct arg_dis *argv;
    struct User_List *user;
    argv = (struct arg_dis *)argv1;
    user = (struct User_List *)argv->user;
    int x = 0,y = 0;
    char friendslist[200][USERNAME_SIZE];
    int onlinesign[200];
    
    Cli_Windows(&x,&y);
    
}


