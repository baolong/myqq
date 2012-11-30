#include <user.h>
#include <net.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <sys/ioctl.h>

int GetSize(int *x,int *y);
int Windows_Client();


int main()
{
    int x,y;
    initscr();
    Windows_Client(&x,&y);
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
 * 函数功能：客户端界面窗口
 * 参数：当前窗口行数、列数
 * 返回值：成功 - 0
 *         失败 - -1
 *
 * *****************************/
int Windows_Client(int *x,int *y)
{
    if (OK != start_color())
        return -1;;
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    *x = *y = 0;
    GetSize(x,y);
    box(stdscr,0,0);        //外边框
    move(3,1);
    hline(ACS_HLINE,*x-2);
    move(4,*x/5);
    vline(ACS_VLINE,*y-5);
    move(4,*x*4/5+1);
    vline(ACS_VLINE,*y-5);
    move(*y-3,*x/5+1);
    hline(ACS_HLINE,*x*3/5);
    attroff(COLOR_PAIR(1));
    move(*y-2,*x/5+1);
    return 0;
}

/******************************
 *
 * 函数功能：在对应区域显示好友列表
 * 返回值：成功- 0
 *         失败 - -1
 *
 * ***************************/
int DisPlayFriendLIST(int x,int y,char friends[][USERPASSWD_SIZE,char name[USERNAME_SIZE]])
{
    
}

