#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct User_Type
{
    char name[15];
    char password[10];
}User_Type;

typedef struct User_List
{
    struct User_Type user;
    struct User_List *next;
}User_List;

int main(int argc,char *argv[])
{
	int fp,fp_acc,num;
	char date[1024],ip[128];
    pid_t pid;
    socklen_t len;
	struct sockaddr_in ser_addr,cli_addr;
    struct User_List USER_HEAD;
	if ((fp = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
        perror("fp");
        return 1;
	}
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(atoi(argv[1]));
    if ((bind(fp,(struct sockaddr *)&ser_addr,sizeof(ser_addr))) < 0)
    {
        perror("bind");
        return 1;
    }
    listen(fp,100);
    while(1)
    {
        if ((fp_acc = accept(fp,(struct sockaddr *)&cli_addr,&len)) < 0)
        {
            perror("fp_acc");
            break;
        }
        printf("IP：%s 链接.\n",inet_ntop(AF_INET,&cli_addr.sin_addr,ip,sizeof(ip)));
        pid = fork();
        if (pid < 0)
        {
            perror("pid");
            break;
        }
        else if (pid == 0)
        {
            while(1)
            {
                memset(date,0,sizeof(date));
                if ((num = recv(fp_acc,date,sizeof(date),0)) <= 0)
                {
                    printf("IP：%s 断开链接.\n",inet_ntop(AF_INET,&cli_addr.sin_addr,ip,sizeof(ip)));
                    close(fp_acc);
                    return 0;
                }
                printf("1234567890\n");
                printf("%s\n",date);
                send(fp_acc,date,strlen(date),0);
                perror("send");
                usleep(10);
            }
        }
    }
}
