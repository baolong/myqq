#include "net.h"
#include <stdio.h>

int main()
{
    int fp,fp_,n=0;
    struct sockaddr_in addr;
    printf("1.server   2.client\n");
    scanf("%d",&n);
    if (n == 1)
    {
        fp = SerNetInit();
        char date[DATELEN];
        fp_ = Accept(fp,&addr);
        while(1)
        {
            memset(date,0x0,sizeof(date));
            printf("recv:%d\n",Recv(fp_,date));
            printf("接收到:%s\n",date);
        }
        close(fp_);
    }
    else if (n == 2)
    {
        char date[DATELEN];
        fp = CliNetInit();
        while(1)
        {
            memset(date,0x0,sizeof(date));
            scanf("%s",date);
            printf("send:%d\n",Send(fp,date));
        }
    }
}
