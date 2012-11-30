#ifndef USER_H
#define USER_H

#define DATELEN 128*2      //单词数据长度
#define USERNAME_SIZE 15     //用户名长度
#define USERPASSWD_SIZE 10      //密码长度

struct MessageLog
{
    char message[DATELEN];            //消息记录
    struct MessageLog *front;
    struct MessageLog *next;
};

struct Friend       //用户好友列表结构体
{
    char name[USERNAME_SIZE];          //好友用户名
    struct MessageLog messagelog;        //聊天记录
    struct Friend *front;
    struct Friend *next;
};

struct OffLineMessage       //离线消息结构体
{
    char message[DATELEN];        //消息
    char Sender[USERNAME_SIZE];       //发送者
    char SendTime[21];               //发送时间
    struct OffLineMessage *front;
    struct OffLineMessage *next;
};

struct User_Type    //用户信息结构体
{
    char name[USERNAME_SIZE];         //用户名
    char password[USERPASSWD_SIZE];       //用户密码
    int online;                           //在线状态
    unsigned int numoffriend;              //好友个数
    struct Friend friends;             //好友列表结构
    struct OffLineMessage offlinemessage;    //离线消息
    int socket;                    //用户对应套接字
};

struct User_List      //用户列表结构体
{
    struct User_Type user;          //用户列表结构
    struct User_List *front;
    struct User_List *next;
};

extern struct User_List list;

int InitList(struct User_List *user);
int ListEmpty(struct User_List *user);
int ListLength(struct User_List *user);
int UserChecking(struct User_List *user,char name[],char password[]);
int UserLogin(struct User_List *user,int fp);
int AddUser(struct User_List *user,char name[],char password[],unsigned int num,char friends[][USERNAME_SIZE]);
int UserNameChange(struct User_List *user,char *oldname,char *newname,char *passwd);
int UserPasswdChange(struct User_List *user,char *name,char *passwd,char *oldpasswd);
int SearchUser(struct User_List *user,char name[]);
int UserDel(struct User_List *user,char name[],char password[]);
int DisList(struct User_List *user);
int SaveList(struct User_List *user);
int LoadList(struct User_List *user);
int OnLine(struct User_List *user,char name[],int sign);
int AddFriend(struct User_List *user,char name[],char nameoffriend[]);
int CreateFriendList(struct Friend *friend,char name[][USERNAME_SIZE]);
int GetFriendList(struct User_List *user,char name[],char namelist[][USERNAME_SIZE]);
int Itoa(int num,char str[DATELEN]);
int Atoi(char str[]);
int InsertToMessagelog(struct Friend *friends,char name[USERNAME_SIZE],char message[DATELEN]);
int GetSocket(struct User_List *user,char name[USERNAME_SIZE]);
int SetSocket(struct User_List *user,char name[USERNAME_SIZE],int fd);
int GetName(struct User_List *user,int fd,char name[USERNAME_SIZE]);
int GetTime(char time_str[21]);
int InsertOffLineMessage(struct User_List *user,char buf[DATELEN],char receiver[USERNAME_SIZE],char sender[USERNAME_SIZE]);


#endif
