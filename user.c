#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include "user.h"
#include "net.h"
#include <time.h>

struct User_List list;

/*************************************
 *
 * 函数功能：初始化用户列表结构
 * 
 * **********************************/
int InitList(struct User_List *user)
{
    user = (struct User_List *)malloc(sizeof(struct User_List));
    user->front = NULL;
    user->next = NULL;
    return 0;
}

/***********************************
 *
 * 函数功能：判断链表是否为空
 * 返回值：空返回1，否则返回0
 *
 * *******************************/
int ListEmpty(struct User_List *user)
{
    return (user->next == NULL);                   //如果链表头指针为空则该链表为空
}

/************************************
 *
 * 函数功能：获取账号个数
 * 返回值：账号个数
 *
 * *********************************/
int ListLength(struct User_List *user)
{
    int num = 0;
    while(user->next != NULL)
    {
        num++;
        user = user->next;
    }
    return(num);
}

/************************************************
 *
 *函数功能：验证账号密码函数
 *返回值：  账号密码正确返回1，否则返回0
 *
 * *********************************************/
int UserChecking(struct User_List *user,char name[],char password[])
{
    while(user->next != NULL && 0 != strcmp(user->user.name,name))
        user = user->next;
    if (strcmp(user->user.password,password) == 0)
        return 1;
    else
        return 0;
}

/***************************
 *
 * 函数功能：用户登录
 * 返回值：成功 - 1
 *         账号密码错误 - 0
 *         用户链表为空 - -1
 *
 * ***********************/
int UserLogin(struct User_List *user,int fp)
{
    char name[USERNAME_SIZE],passwd[USERPASSWD_SIZE];
    int sign = 0;
    memset(name,0x0,sizeof(name));
    memset(passwd,0x0,sizeof(passwd));
    if (ListEmpty(user))
        return -1;
    else
    {
        Recv(fp,name);
        usleep(10);
        Recv(fp,passwd);
        usleep(10);
        if ((sign = UserChecking(user,name,passwd)) == 1)
        {
            Send(fp,"登录成功");
            usleep(10);
            return 1;
        }
        else
            return sign;
    }
}

/**************************************
 *
 * 函数功能：新增账号
 * 返回值：  成功返回1，用户名存在返回2
 *
 * **********************************/
int AddUser(struct User_List *user,char name[],char password[],unsigned int num,char friends[][USERNAME_SIZE])
{
    struct User_List *new;                     //定义新链表指针
    new = (struct User_List *)malloc(sizeof(struct User_List));
    while(user->next != NULL)                        //将原用户链表指针指向最后一个节点
    {
        if (!strcmp(user->user.name,name))
            return 2;
        user = user->next;
    }
    strcpy(new->user.name,name);
    strcpy(new->user.password,password);
    new->user.numoffriend = num;
    if (num > 0)                           //如果用户好友个数大于0，则添加好友信息
        CreateFriendList(&new->user.friends,friends);
    new->front = user;                 //将新链表指针和用户链表指针结合
    new->next = NULL;
    user->next = new;
    return 1;
}

/********************************
 *
 * 函数功能：修改账号名
 * 返回值：成功 - 0
 *         密码错误 - 1
 *         没有该账号 - 2
 *         数据库为空 - 3
 *
 *******************************/
int UserNameChange(struct User_List *user,char *oldname,char *newname,char *passwd)
{
    if (NULL != user->next)             //如果链表的第一个节点的next为空，则链表为空
    {
        while(NULL != user->next)         //寻找欲修改的用户节点
        {
            if (0 != strcmp(user->user.name,oldname))
                user = user->next;
            else
                break;
        }
        if (0 == strcmp(user->user.name,oldname))
        {
            if (0 == strcmp(user->user.password,passwd))                 //判断密码是否正确
            {
                strcpy(user->user.name,newname);
                return 0;
            }
            else
                return 1;
        }
        else
            return 2;
    }
    else
        return 3;
}

/********************************
 *
 * 函数功能：修改密码
 * 返回值：成功 - 0
 *         密码错误 - 1
 *         没有该账号 - 2
 *         数据库为空 - 3
 *
 *******************************/
int UserPasswdChange(struct User_List *user,char *name,char *oldpasswd,char *newpasswd)
{
    if (NULL != user->next)              //如果链表第一个节点的next为空，则链表为空
    {
        while(NULL != user->next)               //寻找欲修改密码的节点
        {
            if (0 != strcmp(user->user.name,name))
                user = user->next;
            else
                break;
        }
        if (0 == strcmp(user->user.name,name))
        {
            if (0 == strcmp(user->user.password,oldpasswd))    //检验旧密码
            {
                strcpy(user->user.password,newpasswd);            //修改密码
                return 0;
            }
            else
                return 1;
        }
        else
            return 2;
    }
    else
        return 3;
}

/***********************************
 *
 * 函数功能：查找是否有该用户
 * 返回值：存在该用户 - 0
 *         不存在该用户 - 1
 *
 * ********************************/
int SearchUser(struct User_List *user,char name[])
{
    if (NULL != user->next)        //若链表第一个节点的next为空，则用户列表为空
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))     //循环，直到用户名与name相等
                return 0;
            user = user->next;
        }
    }
    return 1;
}

/************************************
 *
 * 函数功能：删除账号
 * 返回值：  成功 - 1
 *           不存在该用户 - 2
 *           密码错误 - 3
 *
 * ********************************/
int UserDel(struct User_List *user,char name[],char password[])
{
    struct User_List *del;      //定义新节点，存放欲删除用户节点
    del->front = NULL;
    del->next = NULL;
//    user = user->next;
    if (NULL != user->next)    //若用户链表第一个节点的next,则用户链表为空
    {
        while (user != NULL)              //循环，直到用户节点名与name相同
        {
            if (strcmp(user->user.name,name) == 0)
            {
                if (0 == strcmp(user->user.password,password))      //检验用户密码
                {
                    user = user->front;
                    del = user->next;
                    user->next = del->next;
                    free(del);                           //释放欲删除用户节点空间
                    DisList(user);
                    return 1;
                }
            }
            user = user->next;
        }
    } 
    return 0;
}

/***********************************
*
 * 函数功能：遍历链表所有成员
 * 返回值：成功 - 1
 *         无数据 - 0
 *
 * ********************************/
int DisList(struct User_List *user)
{
    int num = 0;
    if (NULL != user->next )
    {
        user = user->next;
        while(user != NULL)
        {
            num++;
            printf("第%d个账号：%s - %s\n",num,user->user.name,user->user.password);
            user = user->next;
        }
    }
    if (num == 0)
        return 1;     
    return 0;
}

/*******************************
 *
 * 函数功能：保存账号数据
 * 返回值：成功返回0，没有数据返回1
 *
 * ****************************/
int SaveList(struct User_List *user)
{
    FILE *fp;
    if (NULL == (fp = fopen("date","w+")))             //打开date文件
    {
        perror("fp");
        return 3;
    }
    int sum = 0;      //用户个数 整形
    char sum_[3];     //用户个数 字符串
    char num_[5];     //用户好友个数 
    struct Friend *temp;    //用户好友列表指针
    sum = ListLength(user);    //获取用户总数
//    sum_[1] = sum%10+48;      
//    sum_[0] = sum/10%10+48;
//    sum_[2] = '\0';
    sprintf(sum_,"%d",sum);
    fwrite(sum_,sizeof(sum_),1,fp);
    if (NULL == user->next)
        return 1;
    else
    {
        user = user->next;
        while(user != NULL)
        {
            temp = &user->user.friends;
            sprintf(num_,"%d",user->user.numoffriend);
            fwrite(user->user.name,sizeof(user->user.name),1,fp);
            fwrite(user->user.password,sizeof(user->user.password),1,fp);
            fwrite(num_,sizeof(num_),1,fp);
            if (NULL != temp->next)
            {
                temp = temp->next;
                while(NULL != temp)
                {
                    fwrite(temp->name,USERNAME_SIZE*sizeof(char),1,fp);
                    temp = temp->next;
                }
            }
            user = user->next;
        }
    }
    fclose(fp);
    return 1;
}

int LoadList(struct User_List *user)
{
    FILE *fp;
    printf("LoadList:%d\n",user);
    char name[USERNAME_SIZE] = "",passwd[USERPASSWD_SIZE] = "";
    if ((fp = fopen("date","r+")) < 0)
    {
        perror("fp");
        return 1;
    }
    int sumofuser = 0;
    char sumofuser_[3];
    int sumoffriends = 0,numoffriends = 0;
    char sumoffriends_[5];
    memset(&sumofuser_, 0x0, sizeof(sumofuser_));
    char nametemp[10][USERNAME_SIZE];
    fread(sumofuser_, sizeof(sumofuser_), 1, fp);
    sumofuser = atoi(sumofuser_);
    printf("sumofusers:%d\n",sumofuser);
//    user = user->next;
    while(sumofuser > 0)
    {
        memset(&sumoffriends_, 0x0, sizeof(sumoffriends_));
        memset(&nametemp,0x0,sizeof(nametemp));
        fread(name, USERNAME_SIZE*sizeof(char), 1, fp);          //读取用户名
        fread(passwd, USERPASSWD_SIZE*sizeof(char), 1, fp);        //读取用户密码
        fread(sumoffriends_, sizeof(sumoffriends_), 1, fp);    //读取用户好友个数字符串
        printf("name:%s - %s sumoffriends_:%s\n",name,passwd,sumoffriends_);
        sumoffriends = atoi(sumoffriends_);       //将用户好友个数转换成整型
        numoffriends = 0;
        while(numoffriends < sumoffriends)          //循环读取用户好友用户名
        {
            fread(nametemp[numoffriends],USERNAME_SIZE*sizeof(char),1,fp);
            printf("%dfriend's name:%s\n",numoffriends,nametemp[numoffriends]);
            numoffriends++;
        }
        AddUser(user,name,passwd,sumoffriends,nametemp);
        sumofuser--;
    }
    fclose(fp);
    return 0;
}

/**************************************
 *
 * 函数功能：获取、设置用户在线状态
 * 参数：user - 用户列表
 *       name - 欲改变状态的用户名
 *       sign - 功能标识，
 *              值为1则查询状态，
 *              值为2则设置为在线，
 *              值为3则设置为离线。
 * 返回值：成功 - 当前在线状态
 *         失败 - -1
 *
 * *************************************/
int OnLine(struct User_List *user,char name[],int sign)
{
    if (NULL != user->next)
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))
            {
                if (sign == 1)
                    return user->user.online;
                else if (sign == 2)
                {
                    user->user.online = 1;
                    return user->user.online;
                }
                else if (sign == 3)
                {
                    user->user.online = 0;
                    return user->user.online;
                }
            }
            else
                user = user->next;
        }
    }
    return -1;
}

/************************************************
 *
 * 函数功能：添加好友
 * 参数：user - 账号列表
 *       name - 欲添加好友的用户名
 *       nameoffriend - 好友用户名
 * 返回值：成功 - 当前好友个数
 *         用户列表为空 - -1
 *         没有nameoffriend这个用户 - 0
 *
 * ********************************************/
int AddFriend(struct User_List *user,char name[],char nameoffriend[])
{
    if (SearchUser(user,nameoffriend))
    {
        printf("没有该用户.\n");
        return 0;
    }
    if (NULL != user->next)
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))
            {
                printf("X");
                struct Friend *cur;
                cur = &(user->user.friends);
                {
                    if (NULL != cur->next)
                        while(NULL != cur->next)
                            cur = cur->next;
                    struct Friend *new;
                    new = (struct Friend *)malloc(sizeof(struct Friend));
                    strcpy(new->name,nameoffriend);
                    new->front = cur;
                    new->next = NULL;
                    cur->next = new;
                    user->user.numoffriend++;
                }
                return user->user.numoffriend;
            }
            else
                user = user->next;
        }
        return 0;
    }
    return -1;
}

/**********************************************
 *
 * 函数功能：将二维数组转化为好友列表
 * 返回值：成功 - 好友个数
 *
 * ******************************************/
int CreateFriendList(struct Friend *friend,char name[][USERNAME_SIZE])
{
    int num = 0;
    while(0 != strcmp(name[num],""))
    {
        struct Friend *newfriend;
        newfriend = (struct Friend *)malloc(sizeof(struct Friend));
        strcpy(newfriend->name,name[num]);
        newfriend->front = friend;
        newfriend->next = NULL;
        friend->next = newfriend;
        friend = friend->next;
        num++;
    }
    printf("添加了%d个好友\n",num);
    return num;
}
/***************************************************
 *
 * 函数功能：获取用户好友列表
 * 返回值：成功 - 好友个数
 *         失败 - -1
 *
 * **********************************************/
int GetFriendList(struct User_List *user,char name[],char namelist[][USERNAME_SIZE])
{
    int num = 0;
    struct Friend *temp;
    if (NULL != user->next)
    {
        while(NULL != user)
        {
            if (!strcmp(user->user.name,name))
            {
                temp = &user->user.friends;
                if (NULL != temp->next)
                {
                    temp = temp->next;
                    while(NULL != temp)
                    {
                        strcpy(namelist[num],temp->name);
                        num++;
                        temp = temp->next;
                    }
                    return user->user.numoffriend;
                }
                return -1;
            }
            else
                user = user->next;
        } 
    }
    return -1;
}

int Itoa(int num,char str[DATELEN])
{
    str[0] = num%10;
    str[1] = '\0';
    return 0;
}

int Atoi(char str[])
{
    int num = 0,a = 0;
//    while('\0' != str[a])
        num += (str[a] - '0');
    return num;
}

/***************************************
 *
 * 函数功能：将信息写入聊天记录
 * 返回值：成功 - 1
 *         失败 - -1
 *
 * ************************************/
int InsertToMessagelog(struct Friend *friends,char name[USERNAME_SIZE],char message[DATELEN])
{
    struct MessageLog *newmessage,*temp;
    newmessage = (struct MessageLog *)malloc(sizeof(struct MessageLog));
    newmessage->front = NULL;
    newmessage->next = NULL;
    strcpy(newmessage->message,message);
    if (NULL != friends->next)
    {
        while(NULL != friends)
        {
            if (0 == strcmp(friends->name,name))
            {
                temp = &friends->messagelog;
                while(NULL != temp->next)
                    temp = temp->next;
                newmessage->front = temp;
                temp->next = newmessage;
                newmessage->next = NULL;
                return 1;
            }
            else
                friends = friends->next;
        }
    }
    return -1;
}

/***************************************
 *
 * 函数功能：通过 用户名获取对应套接字描述符
 * 返回值：成功 - 套接字描述符
 *         用户不在线 - -1
 *         用户列表为空 - -2
 *
 * ***********************************/
int GetSocket(struct User_List *user,char name[USERNAME_SIZE])
{
    int fd;
    while(NULL != user->next)           //找到用户对应节点
    {
        if (0 == strcmp(user->user.name,name))
            break;
        else
            user = user->next;
    }
    if (OnLine(user,name,1))                //判断该用户是否在线
        return user->user.socket;
    else
        return -1;
}

/*************************************
 *
 * 函数功能：通过用户名设置对应套接字描述符
 * 参数：user - 用户列表
 *       name - 用户名
 *       fd - 套接字描述符
 *
 * *********************************/
int SetSocket(struct User_List *user,char name[USERNAME_SIZE],int fd)
{
    while(NULL != user->next)
    {
        if (0 == strcmp(user->user.name,name))
            break;
        else
            user = user->next;
    }
    user->user.socket = fd;           //设置套接字描述符
    user->user.online = 1;       //设置为在线状态
    return 0;
}

/*************************************
 *
 * 函数功能：通过套接字描述符获取对应用户名
 * 参数：user - 用户列表
 *       fd -  用户套接字描述符
 *       name - 接收用户名的字符串
 * 返回值：成功 - 0
 *         失败 - -1
 *
 * *********************************/
int GetName(struct User_List *user,int fd,char name[USERNAME_SIZE])
{
    while(NULL != user->next)
    {
        if (user->user.socket == fd)
        {
            strcpy(name,user->user.name);
            return 0;
        }
        else
            user = user->next;
    }
    return -1;
}

/***********************************
 *
 * 函数功能：获取当前时间，并转化为字符串格式
 * 参数：timestr - 接收时间的字符串
 * 返回值：成功 -0
 *         失败 - -1
 *
 * *******************************/
int GetTime(char time_str[21])
{
    time_t time_;
    struct tm *time_tm;
    time_ = time(NULL);
    time_tm = localtime(&time_);
    strftime(time_str,21,"%F %T",time_tm);
    return 0;
}

/***************************************
 *
 * 函数功能：将信息存入离线信息列表
 * 参数：user - 用户列表
 *       buf - 欲插入的离线消息
 *       receiver - 接收者用户名
 *       sender - 发送者用户名
 * 返回值：成功 - 0
 *         失败 - -1
 *
 * ************************************/
int InsertOffLineMessage(struct User_List *user,char buf[DATELEN],char receiver[USERNAME_SIZE],char sender[USERNAME_SIZE])
{
    struct OffLineMessage *new;
    char time_str[21];
    memset(time_str,0x0,sizeof(time_str));
    new = (struct OffLineMessage *)malloc(sizeof(struct OffLineMessage));
    while(NULL != user)              //寻找接收者节点
    {
        if (0 == strcmp(user->user.name,receiver))
            break; 
        else
            user = user->next;
    }
    strcpy(new->message,buf);      //将信息存入新节点
    strcpy(new->Sender,sender);     //写入发送者用户名
    GetTime(time_str);          //获取当前时间
    strcpy(new->SendTime,time_str);     //写入当前时间
    new->next = NULL;
    new->front = &user->user.offlinemessage;
    return 0;
}

/***************************************
 *
 * 函数功能：提取离线信息,并删除该离线消息
 * 参数：user - 要获取信息的用户节点
 *       sender - 接收发送者用户名
 *       buf - 接收信息
 * 返回值：成功 - 0
 *         没有离线消息 - 1
 *
 * ***********************************/
int GetOffLineMessage(struct User_List *user,char sender[USERNAME_SIZE],char buf[USERNAME_SIZE])
{
    struct OffLineMessage *cur,*del;
    cur = &user->user.offlinemessage;
    if (NULL != cur->next)
    {
        cur = cur->next;
        strcpy(sender,cur->Sender);
        strcpy(buf,cur->message);
        del = cur;
        cur->front->next = cur->next;
        cur->next->front = cur->front;
        free(del);
        return 0;
    }
    else
        return -1;
}





