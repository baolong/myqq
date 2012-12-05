#include "user.h"
#include "net.h"
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <locale.h>

int ColorInit();
int GetSize(int *x,int *y);
int Windows_Client();
int Cli_DisPlayFriendList(int x,int y,char friends[][USERNAME_SIZE],int num,int sum,char name[USERNAME_SIZE],int sign[100]);
int Cli_DisPlayUserDate(int x,int y,struct User_List *user,char name[USERNAME_SIZE]);

int main()
{
    InitList(&list);
    struct User_List *p;
    char name[USERNAME_SIZE];
    char f[10][USERNAME_SIZE];
    int sign1[100] = {0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};
    char a[][USERNAME_SIZE] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"原谅我当天不"},{"如果还"},{"asa"},{"8"},{"9"},{"10"},{"11"},{"12"},{"13"},{"14"},{"15"},{"16"},{"17"},{"18"},{"19"},{"20"},{"21"},{"22"},{"23"},{"24"},{"25"},{"26"},{"27"},{"28"},{"29"},{"30"},{"31"},{"32"},{"33"},{"34"},{"35"},{"36"},{"37"},{"38"},{"39"},{"40"},{"41"},{"42"},{"43"},{"44"}};
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
//        refresh();
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
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(4,COLOR_GREEN,COLOR_BLACK);
    init_pair(5,COLOR_GREEN,COLOR_BLACK);
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
int Windows_Client(int *x,int *y)
{
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
    move(*y-2,18);
//    refresh();
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
int Cli_DisPlayFriendList(int x,int y,char friends[][USERNAME_SIZE],int num,int sum,char name[USERNAME_SIZE],int sign[100])
{
    int num_ = 5;     //用于一屏显示好友个数的计数
    int temp = 0;   //好友在好友列表中的序号
    int len;
    char a[5];
    char name_temp[USERNAME_SIZE];
    if (num >= sum)
        return 0;
    if (num > (y - 7))    //如果选中的好友序号大于一屏所能显示的好友数
        temp = num - (y -7);    //则将该好友显示在最低
    while(num_ <= (y - 2))     
    {
        if (strcmp(friends[temp],"") != 0)
        {
//            len = 0;
            strcpy(a,"");
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
int Cli_DisPlayUserDate(int x,int y,struct User_List *user,char name[USERNAME_SIZE])
{
    int cur = 5;
    if (NULL != user->next)
    {
        user = user->next;
        while(NULL != user)
        {
            if (0 == strcmp(name,user->user.name))
                mvaddstr(cur,x - 15,user->user.name);
            user = user->next;
            cur++;
        }
//        refresh();
    }
    return 0;
}

/*************************************
 *
 * 函数功能：客户端键盘控制
 * 参数：num - 当前选择用户编号
 *       sign - 功能选择
 *              0为好友列表控制
 *              1为进入聊天模式
 *       logout - 退出登录标识,为真有效
 * 
 * *********************************/
int Cli_KeyboardControl(int num,int sign,int logout)
{
    int key = 0;
    while(1)
    {
        key = getch();
        switch(key)
        {
            case KEY_UP:
                num--;
                break;
            case KEY_DOWN:
                num++;
                break;
            case '\t':
                sign = ~sign;
                break;
            case 27:
                logout = 1;
                break;
        }
    }
    return 0;
}



