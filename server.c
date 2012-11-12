/*****************************************
 *
 * 聊天程序服务器程序
 *
 * **************************************/
#include "user.h"
#include "net.h"
#include <unistd.h>

int main()
{
    int fp,fp_;
    pid_t pid;
    struct sockaddr_in addr;
    InitList(&list);
    fp = SerNetInit();
    while(1)
    {
        if ((fp_ = Accept(fp,&addr)) > 0)
        {
            pid = fork();
            if (pid < 0)
                perror("pid");
            if (pid > 0)
            {
                
            }
        } 
    }
}

int communicating(struct User_List *user)
{
    
}
