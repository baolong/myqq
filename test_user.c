/**********************************************
 *
 * 账号密码测试程序.
 *
 * ********************************************/
#include <stdio.h>
#include "user.h"
#include "net.h"

char name[15],passwd[10];

int Add(struct User_List *user)
{
    char temp[10][USERNAME_SIZE];
    printf("添加\n");
    scanf("%s",name);
    scanf("%s",passwd);
    AddUser(user,name,passwd,0,temp);
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
        printf("1.添加   2. 删除  3.退出   4.登陆   5.修改账号名  6.修改密码  7.在线状态  8.好友管理\n");
        scanf("%d",&a);
        if (a == 1)
            Add(&list);
        else if (a == 2)
            Del(&list);
        else if (a == 3)
        {
            SaveList(&list);
            break;
        }
        else if (a ==4)
        {
            scanf("%s",name);
            scanf("%s",pass);
            printf("Checking:%s\n",UserChecking(&list,name,pass)?"登陆成功":"登陆错误");
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
        else if (a == 7)
        {
            int b;
            while(1)
            {
                printf("1.查询状态 2.设置成在线 3.设置成离线 4.返回\n");
                scanf("%d",&b);
                if (b == 1)
                {
                    printf("输入用户名：");
                    scanf("%s",name);
                    printf("用户%s当前%s状态\n",name,(OnLine(&list,name,1))?"在线":"离线");
                }
                else if (b == 2)
                {
                    printf("输入用户名：");
                    scanf("%s",name);
                    printf("已设置用户%s为%s状态\n",name,(OnLine(&list,name,2))?"在线":"离线");
                }
                else if (b == 3)
                {
                    printf("输入用户名：");
                    scanf("%s",name);
                    printf("已设置用户%s为%s状态\n",name,(OnLine(&list,name,3))?"在线":"离线");
                }
                else if (b == 4)
                    break;
            }
        }
        else if (a == 8)
        {
            int b,num=0;
            char namef[15];
            char fri[10][USERNAME_SIZE];
            while(1)
            {
                b = 0;
                num = 0;
                memset(&fri,0x0,sizeof(fri));
                printf("输入用户名：");
                scanf("%s",name);
                printf("1. 添加好友  2.显示好友\n");
                scanf("%d",&b);
                if (b == 1)
                {
                    printf("输入添加的好友名：");
                    scanf("%s",namef);
                    AddFriend(&list,name,namef);
                }
                else if (b == 2)
                {
                    num = GetFriendList(&list,name,fri);
                    b = 0;
                    while(0 != strcmp(fri[b],""))
                    {
                        printf("第%d个好友：%s\n",num,fri[b]);
                        b++;
                    }
                }
                else if (b == 3)
                    break;
            }
        }
    }
    return 0;
}

