#include <stdio.h>
#include "user.h"

int main()
{
    InitList(&user);
    printf("空否：%d\n",ListEmpty(user));
    char name[15],passwd[10];
    scanf("%s",name);
    scanf("%s",passwd);
    AddUser(&user,name,passwd);
    scanf("%s",name);
    scanf("%s",passwd);
    AddUser(&user,name,passwd);
    scanf("%s",name);
    scanf("%s",passwd);
    AddUser(&user,name,passwd);
    printf("空否：%d\n",ListEmpty(user));
    printf("%d",ListLength(&user));
    DisList(&user);
    printf("输入删除的账号密码:\n");
    scanf("%s",name);
    scanf("%s",passwd);
    UserDel(&user,name,passwd);
    DisList(&user);
    return 0;
}
