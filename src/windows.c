#include "user.h"
#include "net.h"
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <locale.h>

/*int ColorInit();
int GetSize(int *x,int *y);
int Cli_Windows(int *x,int *y);
int Ser_windows(int *x,int *y);
int Cli_DisPlayFriendList(int x,int y,char friends[][USERNAME_SIZE],int num,int sum,char name[USERNAME_SIZE],int sign[100]);
int Cli_DisPlayUserDate(int x,int y,struct User_List *user,char name[USERNAME_SIZE]);*/
/*
int main()
{
    InitList(&list);
    struct User_List *p;
    char name[USERNAME_SIZE];
    char f[10][USERNAME_SIZE];
    int sign1[100] = {0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};
    char a[][USERNAME_SIZE] = {{"不选择"},{"1"},{"2"},{"3"},{"4"},{"原谅我当天不"},{"如果还"},{"asa"},{"8"},{"9"},{"10"},{"11"},{"12"},{"13"},{"14"},{"15"},{"16"},{"17"},{"18"},{"19"},{"20"},{"21"},{"22"},{"23"},{"24"},{"25"},{"26"},{"27"},{"28"},{"29"},{"30"},{"31"},{"32"},{"33"},{"34"},{"35"},{"36"},{"37"},{"38"},{"39"},{"40"},{"41"},{"42"},{"43"},{"44"}};
    int x,y;
    char str[50];
    int num = 0,sign = 0;
    p = &list;
    AddUser(p,"你好","你好",0,f);
    AddUser(p,"我好","我好",0,f);
    AddUser(p,"大家好","大家好",0,f);
    setlocale(LC_ALL,"");
    initscr();
    ColorInit();
    refresh();
    while(1)
    {
        clear();
        refresh();
        Windows_Client(&x,&y);
        Cli_DisPlayFriendList(x,y,a,num,45,name,sign1);
        Cli_DisPlayUserDate(x,y,p,"大家好");
        move(y-2,18);
        refresh();
        scanw("%s",str);
        move(y-2,18);
        printw("                                  ");
        if (num == 44)
            sign = 1;
        if (num == 0)
            sign = 0;
        if (sign == 0)
            num++;
        else
            num--;
        refresh();
    }
    getch();
    endwin();
    return 0;
}*/

/******************************
 *
 * 函数功能：窗口初始化
 *
 * ***************************/
int WindowInit()
{
    setlocale(LC_ALL,"");
    initscr();
    ColorInit();
}

/****************************
 *
 * 函数功能：登陆界面
 *
 *
 * **************************/
int Cli_Login(int x,int y,char name[USERNAME_SIZE],char passwd[USERPASSWD_SIZE])
{
    clear();
    leaveok(stdscr,0);
    attron(COLOR_PAIR(1));
    box(stdscr,0,0);
    move(y/2-2,x/2-10);
    printw("账号:");
    move(y/2,x/2-10);
    printw("密码:");
    move(y/2-2,x/2-4);
    scanw("%s",name);
    move(y/2,x/2-4);
    scanw("%s",passwd);
    move(10,20);
    printw("%s - %s",name,passwd);
    refresh();
    return 0;
}

/****************************
 *
 * 函数功能：注册新账号
 *
 *
 * **************************/
int Cli_Apply(int x,int y,char name[USERNAME_SIZE],char passwd[USERPASSWD_SIZE])
{
    char passwd1[USERPASSWD_SIZE];
    char passwd2[USERPASSWD_SIZE];
loop:    clear();
    leaveok(stdscr,0);
    attron(COLOR_PAIR(1));
    box(stdscr,0,0);
    move(y/2-2,x/2-10);
    printw("新用户名:");
    move(y/2,x/2-10);
    printw("密码:");
    move(y/2+2,x/2-10);
    printw("确认密码:");
    move(y/2-2,x/2);
    scanw("%s",name);
    move(y/2,x/2);
    scanw("%s",passwd1);
    move(y/2+2,x/2);
    scanw("%s",passwd2);
    if (0 == strcmp(passwd1,passwd2))
        strcpy(passwd,passwd1);
    else
    {
        move(y/2+4,x/2-18);
        printw("两次密码不相同,请重新输入,按任意键继续");
        getch();
        goto loop;
    }
    move(10,20);
    printw("%s - %s",name,passwd);
    refresh();
    return 0;
}

/********************************
 *
 * 函数功能：客户端欢迎界面
 * 返回值：1 - 登陆
 *         2 - 注册新账号
 *
 * ****************************/
int Cli_Welcome()
{
    int x,y;
    int key;
    int sign = 1;
    keypad(stdscr,1);
    leaveok(stdscr,1);
    while(1)
    {
        clear();
        refresh();
        attron(COLOR_PAIR(1));
        GetSize(&x,&y);
        box(stdscr,0,0);
        attroff(COLOR_PAIR(2));
        if (sign == 1)
            attron(COLOR_PAIR(3));
        else
            attron(COLOR_PAIR(4));
        move(y/2-2,x/2-4);
        printw("登    陆");
        if (sign == 1)
            attroff(COLOR_PAIR(3));
        else
            attroff(COLOR_PAIR(4));
        if (sign == 2)
            attron(COLOR_PAIR(3));
        else
            attron(COLOR_PAIR(4));
        move(y/2,x/2-4);
        printw("新建账号");
        if (sign == 2)
            attroff(COLOR_PAIR(3));
        else
            attroff(COLOR_PAIR(4));
        if (sign == 3)
            attron(COLOR_PAIR(3));
        else
            attron(COLOR_PAIR(4));

        move(y/2+2,x/2-4);
        printw("退    出");
        if (sign == 3)
            attroff(COLOR_PAIR(3));
        else
            attroff(COLOR_PAIR(4));
        refresh();
        key = 0;
        key = getch();
        switch(key)
        {
            case KEY_DOWN:
                if (sign < 3)
                    sign++;
                break;
            case KEY_UP:
                if (sign > 1)
                    sign--;
                break;
            case 108:
            case 10:
            case 109:
            {
                if (1 == sign)
                    return 1;
                else if (2 == sign)
                    return 2;
                else
                    return 3;
            }
        }
    }
    leaveok(stdscr,0);
    return 0;
}

/*********************************
 *
 * 函数功能：获取窗口大小
 * 参数：x - 窗口行数
 *       y - 窗口列数
 * 返回值：成功 - 0
 *         错误 - 1
 *
 * ******************************/
int GetSize(int *x,int *y)
{
    struct winsize size;
    if (0 == isatty(STDOUT_FILENO))
        return 1;
    if (ioctl(STDOUT_FILENO,TIOCGWINSZ,&size) < 0)
    {
        perror("ioctl TIOCGWINSZ error");
        return 1;
    }
    *y = size.ws_row;
    *x = size.ws_col;
    return 0;
}

/********************************
 *
 * 函数功能：颜色初始化
 *
 * *****************************/
int ColorInit()
{
    if (OK != start_color())
        return -1;
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_BLACK,COLOR_GREEN);
    init_pair(3,COLOR_RED,COLOR_GREEN);
    init_pair(4,COLOR_GREEN,COLOR_BLACK);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);
    init_pair(6,COLOR_GREEN,COLOR_BLACK);
    init_pair(7,COLOR_GREEN,COLOR_BLACK);
}
/********************************
 *
 * 函数功能：客户端界面窗口
 * 参数：当前窗口行数、列数
 * 返回值：成功 - 0
 *         失败 - -1
 *
 * *****************************/
int Cli_Windows(int *x,int *y)
{
    int x1,y1;
    getyx(stdscr,y1,x1);
    attron(COLOR_PAIR(1));
    *x = *y = 0;
    GetSize(x,y);
    box(stdscr,0,0);        //外边框
    move(1,*x/2-10);
    printw("多人聊天软件");
    move(2,*x-23);
    printw("时间:2012.12.05 23:00");
    move(3,1);
    hline(ACS_HLINE,*x-2);
    move(4,1);
    printw("   好友列表");
    move(5,1);
    hline('-',15);
    move(4,16);
    vline(ACS_VLINE,*y-5);
    move(4,*x - 16);
    vline(ACS_VLINE,*y-5);
    move(4,*x-12);
    printw("好友信息");
    move(5,*x-15);
    hline('-',14);
    move(*y-3,17);
    hline(ACS_HLINE,*x - 33);
    attroff(COLOR_PAIR(1));
//    refresh();
//    move(*y-2,33);
    move(y1,x1);
    return 0;
}

/*******************************
 *
 * 函数功能：服务器界面窗口a
 *
 * ****************************/
int Ser_windows(int *x,int *y)
{
    int x1,y1;
    getyx(stdscr,y1,x1);   //获取光标当前位置
    attron(COLOR_PAIR(1));
    *x =*y = 0;
    GetSize(x,y);
    box(stdscr,0,0);
    move(1,*x/2-14);
    printw("多人聊天软件服务器端");
    move(2,*x-23);
    printw("时间:2012.12.05 23:00");
    move(3,1);
    hline(ACS_HLINE,*x-2);
    move(4,1);
    printw("   用户列表");
//    move(6,1);
//    printw("你好我是谁你是");
    move(5,1);
    hline('-',15);
    move(4,16);
    vline(ACS_VLINE,*y-5);
    move(4,*x - 20);
    vline(ACS_VLINE,*y-5);
    move(4,*x-15);
    printw("服务器信息");
    move(6,*x-19);
    printw("用户总数：123");
    move(7,*x-19);
    printw("在线用户数：123");
    move(11,*x-19);
    hline(ACS_HLINE,18);
    move(12,*x - 14);
    printw("实时动态");
    move(13,*x - 19);
    hline('-',18);
    move(5,*x-19);
    hline('-',18);
    move(4,32);
    vline(ACS_VLINE,*y-5);
    move(8,33);
    hline(ACS_HLINE,*x - 33 - 20);
    move(4,20);
    printw("好友列表");
//    move(6,17);
//    printw("你好我是谁你是");
    move(4,(*x-33-19)/2 - 10 + 33);
    printw("用户信息");
    move(5,33);
    hline('-',*x - 53);
    move(5,17);
    hline('-',15);
    move(*y-3,33);
    hline(ACS_HLINE,*x - 53);
    attroff(COLOR_PAIR(1));
    move(y1,x1);   //恢复光标
    return 0;
}

/******************************
 *
 * 函数功能：在对应区域显示好友列表
 * 参数：x,y - 窗口尺寸
 *       user - 用户列表
 *       num - 被选中好友编号（从1开始）
 *       sum - 总用户数
 *       name - 返回选中好友用户名
 *       sign - 好友在线标识 为1则在线，否则离线
 * 返回值：成功- 0
 *         失败 - -1
 *
 * ***************************/
int Cli_DisplayFriendList(int x,int y,char friends[][USERNAME_SIZE],int num,int sum,char name[USERNAME_SIZE],int sign[100])
{
    int num_ = 6;     //用于一屏显示好友个数的计数
    int temp = 0;   //好友在好友列表中的序号
    int len;
    int x1,y1;
//    char a[5];
    char name_temp[USERNAME_SIZE];
    if (num >= sum)
        return 0;
    if (num > (y - 7))    //如果选中的好友序号大于一屏所能显示的好友数
        temp = num - (y -7);    //则将该好友显示在最低
    while(num_ <= (y - 3))     
    {
        if (strcmp(friends[temp],"") != 0)
        {
//            len = 0;
//            strcpy(a,"");
            strcpy(name_temp,"");
            len = strlen(friends[temp]);
//            len = wcswidth(friends[temp],1000);
//            a[0] = len/10+'0';
//            a[1] = len%10+'0';
//            a[2] = '\0';
//            mvaddstr(num_,40,a);
            strcpy(name_temp,friends[temp]);
            while(len <= 13)
            {
                name_temp[len] = ' ';
                len++;
            }
            name_temp[len] = '\0';
            getyx(stdscr,y1,x1);   //获取光标当前坐标
            if (temp == num)      //如果是被选中的用户，则反色显示
            {
                attron(COLOR_PAIR(2));
                mvaddstr(num_,2,name_temp);
                attroff(COLOR_PAIR(2));
                strcpy(name,friends[temp]);
            }
            else     //如果好友不是被选中的用户
            {
                if (sign[temp] == 1)
                {
                    attron(COLOR_PAIR(1));
                    mvaddstr(num_,2,name_temp);
                    attroff(COLOR_PAIR(1));
                }
                else
                {
                    attron(COLOR_PAIR(3));
                    mvaddstr(num_,2,name_temp);
                    attroff(COLOR_PAIR(3));
                }
            }
            move(y1,x1);
            temp++;
            num_++;
        }
        else
            break;
    }
//    refresh();
}

/*************************************
 *
 * 函数功能：显示好友信息
 * 参数：x,y - 窗口大小
 *       user - 用户链表
 *       name - 好友用户名
 * 返回值：成功- 0
 *         失败 - -1
 *
 * **********************************/
int Cli_DisplayUserDate(int x,int y,struct User_List *user,char name[USERNAME_SIZE])
{
    int cur = 5;
    int x1,y1;
    if (NULL != user->next)
    {
        user = user->next;
        getyx(stdscr,y1,x1);    //获取光标当前坐标
        while(NULL != user)
        {
            if (0 == strcmp(name,user->user.name))
                mvaddstr(cur,x - 15,user->user.name);
            user = user->next;
            cur++;
        }
        move(y1,x1);
//        refresh();
    }
    return 0;
}

/************************************
 *
 * 函数功能：服务器显示用户列表
 * 参数：x,y - 窗口大小
 *       list - 用户列表
 *       num - 被选中用户编号
 *       sum - 总用户数
 *       sign - 用户在线标识
 *       name - 返回当前选择用户名
 * ***********************************/
int Ser_DisplayUserList(int x,int y,char list[][USERNAME_SIZE],int num,int sum,int sign[200],char name[USERNAME_SIZE])
{
    int cur = 6;
    int temp = 0;
    int len = 0;
    int x1,y1;
    char name_temp[USERNAME_SIZE];
    num -= 1;
    strcpy(list[0],"整体动态");
    if (num >= sum)
        return 0;
    if (num > (y - 8))    //如果选中的好友序号大于一屏所能显示的好友数
        temp = num - (y -8);    //则将该好友显示在最低
    while(cur <= (y - 2))     
    {
        if (strcmp(list[temp],"") != 0)
        {
            strcpy(name_temp,"");
            len = strlen(list[temp]);
            strcpy(name_temp,list[temp]);
            while(len <= 13)
            {
                name_temp[len] = ' ';
                len++;
            }
            name_temp[len] = '\0';
            getyx(stdscr,y1,x1);   //获取光标当前坐标
            if (temp == num)      //如果是被选中的用户，则反色显示
            {
                attron(COLOR_PAIR(2));
                mvaddstr(cur,2,name_temp);
                attroff(COLOR_PAIR(2));
                len =strlen(list[temp]);
                strcpy(name,list[temp]);
                name[len] = '\0';
            }
            else     //如果好友不是被选中的用户
            {
                if (sign[temp] == 1)   //在线用户
                {
                    attron(COLOR_PAIR(4));
                    mvaddstr(cur,2,name_temp);
                    attroff(COLOR_PAIR(4));
                }
                else      //离线用户
                {
                    attron(COLOR_PAIR(5));
                    mvaddstr(cur,2,name_temp);
                    attroff(COLOR_PAIR(5));
                }
            }
            move(y1,x1);    //恢复光标
            temp++;
            cur++;
        }
        else
            break;
    }

}

/************************************
 *
 * 函数功能：服务器端显示用户好友列表
 * 参数：x,y - 窗口大小
 *       friends - 好友列表
 *       num - 当前被选中好友编号
 *       sum - 好友总数
 *       name - 返回好友用户名
 *
 * ********************************/
int Ser_DisplayFriendList(int x,int y,char friends[][USERNAME_SIZE],int num,int sum,char name[USERNAME_SIZE])
{
    int cur = 6;
    int temp = 0;
    int len = 0;
    int x1,y1;
    char name_temp[USERNAME_SIZE];
    num -= 1;
    if (num >= sum)
        return 0;
    if (num > (y - 8))    //如果选中的好友序号大于一屏所能显示的好友数
        temp = num - (y -8);    //则将该好友显示在最低
    while(cur <= (y - 2))     
    {
        if (strcmp(friends[temp],"") != 0)
        {
            strcpy(name_temp,"");
            len = strlen(friends[temp]);
            strcpy(name_temp,friends[temp]);
            while(len <= 13)
            {
                name_temp[len] = ' ';
                len++;
            }
            name_temp[len] = '\0';
            getyx(stdscr,y1,x1);    //获取光标当前坐标
            if (temp == num)      //如果是被选中的用户，则反色显示
            {
                attron(COLOR_PAIR(2));
                mvaddstr(cur,17,name_temp);
                attroff(COLOR_PAIR(2));
                len =strlen(friends[temp]);
                strcpy(name,friends[temp]);
                name[len] = '\0';
            }
            else     //如果好友不是被选中的用户
            {
                attron(COLOR_PAIR(3));
                mvaddstr(cur,17,name_temp);
                attroff(COLOR_PAIR(3));
            }
            move(y1,x1);    //恢复光标
            temp++;
            cur++;
        }
        else
            break;
    } 
}

/*************************************
 *
 * 函数功能：键盘控制
 * 参数：num - 选择对象编号
 *       sign - 功能选择 从0开始
 *       logout - 退出登录标识,为真有效
 * 
 * *********************************/
int KeyboardControl(int *num,int *max_num,int *sign,int *logout,char *message,int *message_sign)
{
    int key = 0;
    int x,y;
    keypad(stdscr,0);
    while(1)
    {
        GetSize(&x,&y);
        if (0 != *sign)   //若功能不为聊天输入模式，则检测按键
        {
            key = 0;
            noecho();
            key = getch();
            echo();
            switch(key)
            {
                case KEY_UP:
                    if (0 != num[*sign])
                        num[*sign]--;
                    break;
                case KEY_DOWN:
                    num[*sign]++;
                    break;
                case '1':
                    *sign = 0;
                    break;
                case '2':
                    *sign = 1;
                    break;
                case '3':
                    *sign = 2;
                    break;
                case '4':
                    *sign = 3;
                    break;
                case 27:
                    *logout = 1;
            //     return 0;
            }
            usleep(1000);
        }
        else     //若功能选择为聊天呼入模式，则获取用户输入
        {

            while(0 != *message_sign);   //等待发送缓冲区为空
            echo();
            move(y-2,17);
            getstr(message);
            *message_sign = 1;
//            scanf("%s",message);
            noecho();
            if (0 == strcmp(message,"q"))
                *sign = 1;
            else
            {
//                *message_sign = 1;
                move(y-4,17);
                printw("                    ");
                move(y-4,17);
                printw("信息：%s",message);
                move(y-2,17);
//                printw("                            ");
                move(y-2,17);
                refresh();
            }
        }
    }
    return 0;
}


