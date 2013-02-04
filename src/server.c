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

    int sign_menu = 0;    //功能标识
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
    argv_dis->num_max = num_max;
    argv_dis->name_cur = name_cur;
    argv_dis->logout = &logout;

    char a[50][USERNAME_SIZE];
    WindowInit();
    InitList(user);
    if (0 == access("date",0))
        Ser_LoadList(user);
    else
    {
    strcpy(a[0],"peter");
    strcpy(a[1],"ken");
    strcpy(a[2],"tom");
    AddUser(user,"andy","1",3,a);
    memset(a,0x0,sizeof(a));
    strcpy(a[0],"andy");
    strcpy(a[1],"ken");
    strcpy(a[2],"tom");
    AddUser(user,"peter","1",3,a);
    memset(a,0x0,sizeof(a));
    strcpy(a[0],"andy");
    strcpy(a[1],"peter");
    strcpy(a[2],"tom");
    AddUser(user,"ken","1",3,a);
    memset(a,0x0,sizeof(a));
    strcpy(a[0],"andy");
    strcpy(a[1],"ken");
    strcpy(a[2],"peter");
    AddUser(user,"tom","1",3,a);
    }
    pthread_mutex_init(&mut,NULL);
//    user->front = NULL;
//    user->next = NULL;
    fd_ = SerNetInit();
    keypad(stdscr,1);
    pthread_create(&pth_dis,NULL,Display,argv_dis);    //创建显示进程
//    sleep(2);
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
    int nunoffriend = 0;
    struct User_List *cur,*recv_user;;
    struct arg_ser_newconnect *argv;
    char FriendList[FRIENDS_MAX][USERNAME_SIZE];
    char message[DATELEN];
    argv = (struct arg_ser_newconnect *)argv1;
   


loop:
    cur = argv->user;
    recv_user = argv->user;
    memset(FriendList,0x0,sizeof(FriendList));
    memset(argv->message,0x0,sizeof(argv->message));
    memset(argv->name,0x0,sizeof(argv->name));
    memset(argv->passwd,0x0,sizeof(argv->passwd));
    Recv(*argv->fd,argv->message);   //接收选项
    Recv(*argv->fd,argv->name);
    Recv(*argv->fd,argv->passwd);
    if (0 == strcmp(argv->message,"2"))   //注册用户
    {
        unsigned int numoffriend = 0;
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

        /**************
         * 发送好友列表
         * ************/
        GetFriendList(argv->user,argv->name,FriendList);  //获取用户好友列表 
        Send(*argv->fd,DATETYPE_FRIENDSLIST);   //发送信息类型——好友列表
        usleep(SENDDELAYTIME);
        memset(message,0x0,sizeof(message));
        sprintf(message,"%d",cur->user.numoffriend);
        Send(*argv->fd,message);
        usleep(SENDDELAYTIME);
        numoffriend = 1;
        while(numoffriend <= cur->user.numoffriend)
        {
            Send(*argv->fd,FriendList[numoffriend]);
            char online[2];
            online[0] = OnLine(argv->user,FriendList[numoffriend],1) + '0';
            online[1] = '\0';
            usleep(SENDDELAYTIME);    //发送后延时
            send(*argv->fd,online,sizeof(online),0);
            usleep(SENDDELAYTIME);
            numoffriend++;
        }

        /******************
         * 发送用户离线消息
         * ***************/
        SendOffLineMessage(cur);    //发送离线消息
        usleep(SENDDELAYTIME);     //发送后延时

        /**************
         * 正式进入服务
         * ***********/
        while(1)
        {
//            pthread_mutex_lock(&mut);
            num = 0;
            memset(receiver,0x0,sizeof(receiver));
            memset(message,0x0,sizeof(message));
            num = RecvMessage(argv->user,&cur->user.friends,argv->name,*argv->fd,message,receiver);
            if (MENU_ADDFRIEND_I == num)   //添加好友
            {
                recv(*argv->fd,receiver,USERNAME_SIZE*sizeof(char),0);
                AddFriend(argv->user,argv->name,receiver);   //本用户添加对方好友
                AddFriend(argv->user,receiver,argv->name);   //对方用户添加自己为好友
                SendFriendlist(argv->user,cur,argv->name,*argv->fd);  //发送好友列表
            }
            else if (MENU_DELFRIEND_I == num)    //删除好友
            {
                recv(*argv->fd,receiver,USERNAME_SIZE*sizeof(char),0);
                DelFriend(argv->user,argv->name,receiver);
                DelFriend(argv->user,receiver,argv->name);
                SendFriendlist(argv->user,cur,argv->name,*argv->fd);
            }
            else if (MENU_SENDMESSAGE_I == num)  //发送信息
            {
                if (1 == OnLine(argv->user,receiver,1))    //判断接收人是否在线
                {
                    SendMessage(argv->user,message,receiver,argv->name);   //在线则直接发送给用户
                    InsertToMessagelog(&cur->user.friends,receiver,message,MSGOWN_FRIENDS_I);   //将信息写入接收者聊天记录
                }
                else
                {
                    InsertOffLineMessage(argv->user,message,receiver,argv->name);     //不在线则存入用户离线消息列表
                    InsertToMessagelog(&cur->user.friends,receiver,message,MSGOWN_FRIENDS_I);   //将信息写入接收者聊天记录
                }
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
    argv2 = (struct arg_ser_dis *)argv1;
    int x = 0,y = 0;
    char userlist[200][USERNAME_SIZE];
    char friendlist[200][USERNAME_SIZE];
    int online_sign[200] = {0};
    char friend_cur[USERNAME_SIZE];
    int sumoffriends = 0;
    int dis_temp = 0;
    x = 0;
    clear();
    int x1,y1;
    leaveok(stdscr,1);
    noecho();
    while(1)
    {
        if (1 == *argv2->logout)     //关闭并保存数据
        {
            Ser_SaveList(argv2->user);
            endwin();
            exit(0);
        }
//        clear();
//        refresh();
        {
//            if (dis_temp == 0)
//                clear();   //清屏
            Ser_windows(&x,&y);   //初始化窗口界
            memset(userlist,0x0,sizeof(userlist));
            memset(friendlist,0x0,sizeof(friendlist));
            argv2->num_max[0] = GetUserList(argv2->user,userlist);    //获取用户列表
            GetOnline(argv2->user,online_sign);   //获取用户在线状态
            Ser_DisplayUserList(x,y,userlist,argv2->num[0],argv2->num_max[0],online_sign,argv2->name_cur);    //显示用户列表
            argv2->num_max[1] = GetFriendList(argv2->user,argv2->name_cur,friendlist);    //获取对应用户好友列表
            Ser_DisplayFriendList(x,y,friendlist,argv2->num[1],argv2->num_max[1],friend_cur);   //显示好友列表 
            Ser_DisPlayMsg(x,y,argv2->user,argv2->name_cur,friend_cur);
            MessageBox(x,y,"asd",1);
            usleep(100000);
        }
    }
    pthread_exit(NULL);
}

void *Keyboard(void *argv1)
{
    struct arg_ser_key *argv;
    int a = 0;
    char s[2];
    argv = (struct arg_ser_key *)argv1;
    KeyboardControl(argv->num,argv->num_max,argv->sign,argv->logout,argv->message,argv->message_sign,&a,s);
    pthread_exit(NULL);
}




