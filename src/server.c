#include "windows.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <curses.h>
#include "include.h"

pthread_mutex_t mut;

void *Display(void *argv1);
void *NewUserConnect(void *argv1);
void *Keyboard(void *argv1);

int main()
{
    struct arg_ser_newconnect argv1[USER_MAX],*argv[USER_MAX];
    struct arg_ser_key argv_key1,*argv_key;
    struct arg_ser_dis argv_dis1,*argv_dis;
    struct  User_List *user = &list;
    struct sockaddr_in cli_addr[USER_MAX];

    argv_key = &argv_key1;
    argv_dis = &argv_dis1;
    pthread_t pth_t[USER_MAX],pth,pth_dis;

    int sign_menu = 1;    //功能标识
    int num[4] = {0};     //各模块被选中成员编号
    int num_max[4] = {0};   //各模块最大成员数
    int logout = 0;   //退出标识
    int sumofuser = 0;
    int fd_ = 0,fd[USER_MAX] = {0};
    int sumofcli = 0;
    int message_sign = 0;

    char name_cur[USERNAME_SIZE];
    char message[USER_MAX][DATELEN];
    char name[USER_MAX][USERNAME_SIZE];
    char passwd[USER_MAX][USERPASSWD_SIZE];
    char temp[2][USERNAME_SIZE];
    char message_send[DATELEN];
    //各类初始化
    argv_key->num = num;
    argv_key->num_max = num_max;
    argv_key->sign = &sign_menu;
    argv_key->logout = &logout;
    argv_key->message = message_send;
    argv_key->message_sign = &message_sign;

    argv_dis->user = user;
    argv_dis->num = num;
    argv_dis->sign = &sign_menu;
    argv_dis->sumofuser = &sumofuser;
    argv_dis->name_cur = name_cur;

    WindowInit();
    InitList(user);
    pthread_mutex_init(&mut,NULL);
//    user->front = NULL;
//    user->next = NULL;
    fd_ = SerNetInit();
    keypad(stdscr,1);
    pthread_create(&pth_dis,NULL,Display,argv_dis);    //创建显示进程
    sleep(2);
    pthread_create(&pth,NULL,Keyboard,argv_key);    //创建键盘控制进程
ALLOW: 
    while(sumofcli < USER_MAX)    //如果在线用户小于最大限制数，则继续接受用户连接
    {
        argv[sumofcli] = &argv1[sumofcli];
        //对相关数据结构进行初始化
        memset(&cli_addr[sumofcli],0x0,sizeof(struct sockaddr_in));
        memset(message[sumofcli],0x0,DATELEN*sizeof(char));
        memset(name[sumofcli],0x0,USERNAME_SIZE*sizeof(char));
        memset(passwd[sumofcli],0x0,USERPASSWD_SIZE*sizeof(char));

        if ((fd[sumofcli] = Accept(fd_,cli_addr[sumofcli])) > 1)
        {
            argv[sumofcli]->user = user;
            argv[sumofcli]->fd = &fd[sumofcli];
            pthread_create(&pth_t[sumofcli],NULL,NewUserConnect,argv[sumofcli]);   //为用户创建新线程
            sumofcli++;   //在线用户数加一
        }
    }
    while(USER_MAX >=  sumofcli)   //死循环，等待在线用户数小于最大限制数
        goto ALLOW;    //则继续接受用户连接
    return 0;
}

void *NewUserConnect(void *argv1)
{
    int num = 0;
    char a[50][USERNAME_SIZE];
    struct User_List *cur;
    struct arg_ser_newconnect *argv;
    char FriendList[FRIENDS_MAX][USERNAME_SIZE];
    char message[DATELEN];
    argv = (struct arg_ser_newconnect *)argv1;
    int z = 0;
    char x[3];
    strcpy(a[0],"3");
    AddUser(argv->user,"2","2",1,a);
    memset(a,0x0,sizeof(a));
    strcpy(a[0],"4");
    strcpy(a[1],"5");
    AddUser(argv->user,"3","5",2,a);
    while(z < 50)
    {
        x[1] = z%10 + '0';
        x[0] = z/10 + '0';
        strcpy(a[z],x);
        z++;
    }
    AddUser(argv->user,"4","5",50,a);    
    AddUser(argv->user,"5","5",50,a);
    AddUser(argv->user,"6","5",50,a);
    AddUser(argv->user,"7","5",50,a);
    AddUser(argv->user,"8","5",50,a);
    AddUser(argv->user,"9","5",50,a);
    AddUser(argv->user,"10","5",50,a);
    AddUser(argv->user,"11","5",50,a);
    AddUser(argv->user,"12","5",50,a);
    AddUser(argv->user,"13","5",50,a);
    AddUser(argv->user,"14","5",50,a);
    AddUser(argv->user,"15","5",50,a);
    AddUser(argv->user,"16","5",50,a);
    AddUser(argv->user,"17","5",50,a);
    AddUser(argv->user,"18","5",50,a);
    AddUser(argv->user,"19","5",50,a);
    AddUser(argv->user,"20","5",50,a);
    AddUser(argv->user,"21","5",50,a);
    AddUser(argv->user,"22","5",50,a);
    AddUser(argv->user,"23","5",50,a);
    AddUser(argv->user,"24","5",50,a);
    AddUser(argv->user,"25","5",50,a);
    AddUser(argv->user,"26","5",50,a);
    AddUser(argv->user,"27","5",50,a);
    AddUser(argv->user,"28","5",50,a);
    AddUser(argv->user,"29","5",50,a);
    AddUser(argv->user,"30","5",50,a);
    AddUser(argv->user,"31","5",50,a);
    AddUser(argv->user,"32","5",50,a);
    AddUser(argv->user,"33","5",50,a);
    AddUser(argv->user,"34","5",50,a);
    AddUser(argv->user,"35","5",50,a);
    AddUser(argv->user,"36","5",50,a);
    AddUser(argv->user,"37","5",50,a);
    AddUser(argv->user,"38","5",50,a);
    AddUser(argv->user,"39","5",50,a);
    AddUser(argv->user,"40","5",50,a);
    AddUser(argv->user,"41","5",50,a);
    AddUser(argv->user,"42","5",50,a);
    AddUser(argv->user,"43","5",50,a);
    AddUser(argv->user,"44","5",50,a);
    AddUser(argv->user,"45","5",50,a);
    AddUser(argv->user,"46","5",50,a);
    AddUser(argv->user,"47","5",50,a);



loop:
    cur = argv->user;
    memset(FriendList,0x0,sizeof(FriendList));
    memset(argv->message,0x0,sizeof(argv->message));
    memset(argv->name,0x0,sizeof(argv->name));
    memset(argv->passwd,0x0,sizeof(argv->passwd));
    Recv(*argv->fd,argv->message);   //接收选项
    Recv(*argv->fd,argv->name);
    Recv(*argv->fd,argv->passwd);
    if (0 == strcmp(argv->message,"2"))   //注册用户
    {
        pthread_mutex_lock(&mut);
        AddUser(argv->user,argv->name,argv->passwd,0,argv->temp);
        pthread_mutex_unlock(&mut);

        Send(*argv->fd,CREATEUSER_SUCCESS);
        goto loop;     //退出，重新登陆
    }
    else if (0 == strcmp(argv->message,"1"))    //登陆账户
    {
//        pthread_mutex_lock(&mut);
        if (1 != UserChecking(cur,argv->name,argv->passwd))
        {
            Send(*argv->fd,LOGIN_PASSWORD_WRONG);
            goto loop;
        }
        char receiver[USERNAME_SIZE];
        char message[DATELEN];
        unsigned int numoffriend = 0;
        OnLine(argv->user,argv->name,2);  //设置用户为在线状态
        SetSocket(argv->user,argv->name,*argv->fd);   //设置用户套接字描述符
        Send(*argv->fd,LOGIN_SUCCESS);   //发送登陆成功信息
        usleep(SENDDELAYTIME);
        while(0 != strcmp(cur->user.name,argv->name) && NULL != cur->next)
            cur = cur->next;
        GetFriendList(argv->user,argv->name,FriendList);  //获取用户好友列表
//        Send(*argv->fd,FriendList);
//        Send(*argv->fd,DATETYPE_FRIENDSLIST);   //发送信息类型——好友列表
        usleep(SENDDELAYTIME);
//        send(*argv->fd,FriendList,sizeof(FriendList),0);  //发送好友列表
        usleep(SENDDELAYTIME);    //发送后延时
//        SendOffLineMessage(cur);    //发送离线消息
        usleep(SENDDELAYTIME);     //发送后延时
//        pthread_mutex_unlock(&mut);
        while(1)
        {
//            pthread_mutex_lock(&mut);
            memset(receiver,0x0,sizeof(receiver));
            memset(message,0x0,sizeof(message));
            num = RecvMessage(argv->user,&cur->user.friends,argv->name,*argv->fd,message,receiver);
            if (MENU_ADDFRIEND_I == num)   //添加好友
            {
                Recv(*argv->fd,receiver);   //接收欲添加的好友用户名
                AddFriend(argv->user,argv->name,receiver);   //添加好友
                GetFriendList(argv->user,argv->name,FriendList);  //获取用户好友列表
                Send(*argv->fd,DATETYPE_FRIENDSLIST);   //发送信息类型——好友列表
                usleep(SENDDELAYTIME);
                send(*argv->fd,FriendList,sizeof(FriendList),0);  //发送好友列表
                usleep(SENDDELAYTIME);    //发送后延时
            }
            else if (MENU_SENDMESSAGE_I == num)  //发送信息
            {
                move(32,30);
                printw("                                ");
                move(32,30);
                printw("接收到：%s - %d - %s - %s Online:%d",cur->user.name,num,receiver,message,OnLine(argv->user,receiver,1));
                refresh();
                if (1 == OnLine(argv->user,receiver,1))    //判断接收人是否在线
                    SendMessage(argv->user,&cur->user.friends,message,receiver);   //在线则直接发送给用户
                else
                    InsertOffLineMessage(argv->user,message,receiver,argv->name);     //不在线则存入用户离线消息列表
            }
            else  //出错,则自动断开链接
            {
//                pthread_mutex_unlock(&mut);
                break;
            }
//            pthread_mutex_unlock(&mut);
        }

//        pthread_mutex_lock(&mut);
//        OnLine(argv->user,argv->name,3);
        SetSocket(argv->user,argv->name,0);   //清除套接字描述符
        close(*argv->fd);    //关闭用户套接字描述符
        OnLine(argv->user,argv->name,3);     //设置用户为离线状态

//        pthread_mutex_unlock(&mut);
//        sumofcli--;
    }
    pthread_exit(NULL);
}

void *Display(void *argv1)
{
    struct arg_ser_dis *argv2;
    struct User_List *user;
    int x = 0,y = 0;
    char userlist[200][USERNAME_SIZE];
    char friendlist[200][USERNAME_SIZE];
    int online_sign[200] = {0};
    char friend_cur[USERNAME_SIZE];
    int sumoffriends = 0;
    int dis_temp = 0;
    x = 0;
    sleep(1);
    clear();
    int x1,y1;
    leaveok(stdscr,1);
    noecho();
    while(1)
    {
        clear();
        refresh();
        argv2 = (struct arg_ser_dis *)argv1;
        {
//            if (dis_temp == 0)
//                clear();   //清屏
            Ser_windows(&x,&y);   //初始化窗口界面
            memset(userlist,0x0,sizeof(userlist));
            memset(friendlist,0x0,sizeof(friendlist));
            *argv2->sumofuser = GetUserList(argv2->user,userlist);    //获取用户列表
            GetOnline(argv2->user,online_sign);   //获取用户在线状态
            Ser_DisplayUserList(x,y,userlist,argv2->num[1],*argv2->sumofuser,online_sign,argv2->name_cur);    //显示用户列表
            sumoffriends = GetFriendList(argv2->user,argv2->name_cur,friendlist);    //获取对应用户好友列表
            Ser_DisplayFriendList(x,y,friendlist,argv2->num[2],sumoffriends,friend_cur);   //显示好友列表 
            Ser_DisPlayMsg(x,y,argv2->user,argv2->name_cur,friend_cur);
            getyx(stdscr,y1,x1);
            move(1,2);
            printw("sign - num: 0 - 1 - 2 - 3");
            move(2,2);
            printw("                                                                                    ");
            move(2,2);
            printw(" %1d -        %1d - %1d - %1d - %1d - %1d - %s",*argv2->sign,argv2->num[0],argv2->num[1],argv2->num[2],argv2->num[3],*argv2->sumofuser,friendlist[2]);
            refresh();
            move(y1,x1);
            usleep(100000);
        }
    }
    pthread_exit(NULL);
}

void *Keyboard(void *argv1)
{
    struct arg_ser_key *argv;
    argv = (struct arg_ser_key *)argv1;
    KeyboardControl(argv->num,argv->num_max,argv->sign,argv->logout,argv->message,argv->message_sign);
    pthread_exit(NULL);
}




