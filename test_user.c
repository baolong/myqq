#include <stdio.h>
#include "user.h"

char name[15],passwd[10];

int Add(struct User_List *user)
{
    printf("添加\n");
    scanf("%s",name);
    scanf("%s",passwd);
    AddUser(user,name,passwd);
    return 0;
}
int Del(struct User_List *user)
{
    printf("删除\n");
    scanf("%s",name);
    scanf("%s",passwd);
    printf("删除：%d\n",UserDel(user,name,passwd));
    return 0;
}

int main()
{
    InitList(&list);
    printf("%s-%s\n",list.user.name,list.user.password);
    int a = 0;
    char name[15],pass[10],newname[15],newpasswd[10];
    printf("是否为空%d\n",ListEmpty(list));
    printf("0.加载 1.初始化\n");
    scanf("%d",&a);
    if (a == 0)
        LoadList(&list);
    while(1)
    {
        a = 0;
        DisList(&list);
        printf("1.添加   2. 删除  3.退出   4.登陆   5.修改账号名  6.修改密码\n");
        scanf("%d",&a);
        if (a == 1)
            Add(&list);
        else if (a == 2)
            Del(&list);
        else if (a == 3)
        {
            printf("保存：%d\n",SaveList(&list));
            break;
        }
        else if (a ==4)
        {
            scanf("%s",name);
            scanf("%s",pass);
            printf("Checking:%d\n",UserChecking(&list,name,pass));
        }
        else if (a == 5)
        {
            printf("账号名：\n");
            scanf("%s",name);
            printf("密码：\n");
            scanf("%s",passwd);
            printf("新账号名：\n");
            scanf("%s",newname);
            UserNameChange(&list,name,newname,passwd);
            printf("修改成功!\n");
        }
        else if (a == 6)
        {
            printf("账号名：\n");
            scanf("%s",name);
            printf("密码：\n");
            scanf("%s",passwd);
            printf("新密码：\n");
            scanf("%s",newpasswd);
            UserPasswdChange(&list,name,passwd,newpasswd);
        }

    }
    return 0;
}

