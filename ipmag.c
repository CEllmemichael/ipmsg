#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include "tools.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "tcp.h"
#include "udp_11.h"
#include "tcp_re.h"
#include "userlist.h"
#define pf printf
//#include<string>
#include<string.h>

typedef struct Linkss{
    char  name;
    char  ip;
    char  hname;
    struct Linkss *next;
}linkss;
typedef struct LS
{
	char mode[15];	// 文件的模式
	int dir_num;	// 是否目录或目录中包含目录的数量
	char user[20];	// 文件的用户名
	char group[20]; // 文件的组名
	long size;		// 文件的字节数
	char time[30];	// 文件的最后修改时间
	int st_mode;	// 文件类型和权限
	char name[20];	// 文件名
} LS;
typedef struct 
{
    char id[20];            //包编号
    char name[20];            //发送者姓名
    char hostname[20];         //机器名
    unsigned int cmdid;        //命令字
    char buf[1024];            //附加信息
	char ip[20]; //
}cmds;
typedef struct{
    char name[20]; //
    char ip_address[20]; //
    char hostname[20]; //
}udplist;

// 开始运行
void *start_run(void *arg);
// 客户端上传
void cs_up(int *clifd);
// 客户端下载
void cs_down(int *clifd);
// 返回文件列表
void cs_list(int *clifd);
//udp接受线程
void *Udp_res(char *myname); 
//tcp接受线程
void *Tcp_res(void *p);
void *Mainmenu(char *name);
//主菜单
void udp_mag();
void udp_msg(char *myname);
void mainquit(char *name);
void display(linkss *p){
    linkss* temp=p;//将temp指针重新指向头结点
    //只要temp指针指向的结点的next不是Null，就执行输出语句。
    while (temp->next) {
        temp=temp->next;
        printf("%s\t",&temp->name);
        printf("%s\t",&temp->ip);
        printf("%s\t",&temp->hname);
        printf("\n");
    }
    printf("\n");
}

linkss * initLink(){
    linkss * p=(linkss*)malloc(sizeof(linkss));//创建一个头结点
    linkss * temp=p;//声明一个指针指向头结点，用于遍历链表
    //生成链表
    linkss *a=(linkss*)malloc(sizeof(linkss));//创建一个结点
	strcpy(&a->name, "name");
	strcpy(&a->ip, "ip");
	strcpy(&a->hname, "hname");
    a->next=NULL;
    temp->next=a;
    temp=temp->next;
    return p;
}
linkss * insertElem(linkss * p,char *ip,char *name,char *hname){
    linkss * temp=p;//创建临时结点temp
    //首先找到要插入位置的上一个结点
    for (int i=1; i<50; i++) {
        if (temp->next==NULL) {
        	//创建插入结点c
    		linkss * c=(linkss*)malloc(sizeof(linkss));
    		strcpy(&c->name, name);
			strcpy(&c->ip, ip);
			strcpy(&c->hname, hname);
            c->next=temp->next;
    		temp->next=c;
    		return  p;
        }
        temp=temp->next;
    }
}
linkss * delElem(linkss * p,int add){
    linkss * temp=p;
    //遍历到被删除结点的上一个结点
    for (int i=1; i<add; i++) {
        temp=temp->next;
    }
    linkss * del=temp->next;//单独设置一个指针指向被删除结点，以防丢失
    temp->next=temp->next->next;//删除某个结点的方法就是更改前一个结点的指针域
    free(del);//手动释放该结点，防止内存泄漏
    return p;
}
int selectElem(linkss * p,char *elem){
    linkss * t=p;
    int i=1;
    while (t->next) {
        t=t->next;
        if (t->name==elem) {
            return i;
        }
        i++;
    }
    return -1;
}
// 主函数




int main()
{
    pthread_t id, id2,menu;//创建3个线程
    int a=0,b=0,i=0;
	
	char myname[20];
	printf("请输入你的用户名: \n");
    scanf("%s", &myname);
    int statue = 1;//登录  1
    UDP_11(statue,myname);
	pthread_create(&id, NULL, Tcp_res, (char *)myname );
    pthread_create(&menu, NULL, Mainmenu, (char *)myname );
    pthread_create(&id2, NULL, Udp_res, (void *)124 );
    pthread_join(id, &a);
    pthread_join(menu, &i);
    pthread_join(id2, &b);
}
void *Tcp_res(void *p)//tcp接受进程
{
    pthread_t id, id2,tcp;//创建两个线程
	int a=0,i=0;
    
    pf("[%s] 服务器创建socket...\n", get_time(2));
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sockfd)
	{
		perror("socket");
		return -1;
	}

	pf("[%s] 准备地址...\n", get_time(2));
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	// 端口 IP 自行修改
	addr.sin_port = htons(9009);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	socklen_t len = sizeof(addr);

	pf("[%s] 绑定socket与地址...\n", get_time(2));
	if (bind(sockfd, (struct sockaddr *)&addr, len))
	{
		perror("bind");
		return -1;
	}

	pf("[%s] 设置监听...\n", get_time(2));
	if (listen(sockfd, 5))
	{
		perror("listen");
		return -1;
	}

	pf("[%s] 等待客户端连接...\n", get_time(2));
	for (;;)
	{
		struct sockaddr_in addrcli = {};
		// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		int *clifd = (int*)malloc(sizeof(int));
		*clifd = accept(sockfd, (struct sockaddr *)&addrcli, &len);
		if (0 > *clifd)
		{
			perror("accept");
			continue;
		}

		pthread_t pid;
		// 创建线程函数 int pthread_create(pthread_t *restrict tidp,const pthread_attr_t *restrict_attr,void*（*start_rtn)(void*),void *restrict arg);
		//第一个参数为指向线程标识符的指针。
		//第二个参数用来设置线程属性。
		//第三个参数是线程运行函数的地址。
		//最后一个参数是运行函数的参数。
		pthread_create(&pid, NULL, start_run, (void *)clifd);
	}

	return 0;
}

void *Udp_res(char *myname) //udp接受线程
{   //printf("udp\n"); 
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }
	linkss *p=initLink();
    // 2. 通信的套接字和本地的IP与端口绑定
    struct sockaddr_in addr;
    struct sockaddr_in cliaddr;
    int  len;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_PORT);    // 大端
    addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(0);
    }

    char buf[1024];
    cmds *remsg=(cmds*)malloc(sizeof(cmds));
    // 3. 通信
    while(1)
    {	int a;
        // 接收广播消息
        memset(buf, 0, sizeof(buf));
        // 阻塞等待数据达到
		//len = sizeof(struct sockaddr);
        recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, sizeof(struct sockaddr));
		transcode(remsg,buf, sizeof(buf));
		scanf(remsg->ip,"%s",inet_ntoa(cliaddr.sin_addr));
		char hname[20];
		gethostname(hname, sizeof(hname));
		if (remsg->hostname !=hname||1){
			//printf("recvfrom: %d\n",remsg->id);
			if (remsg->cmdid ==0){
				printf("recvfrom: %d\n",remsg->id);
				a = selectElem(p, remsg->name);
				p = delElem(p,a);
				printf("[%s]您的好友： %s[%s]已经下线\n",get_time(2), remsg->name,inet_ntoa(cliaddr.sin_addr));
				display(p);
			}	
			if (remsg->cmdid ==1){
				printf("recvfrom: %d\n",remsg->id);
				printf("[%s]您的好友： %s[%s]已经上线\n",get_time(2), remsg->name,inet_ntoa(cliaddr.sin_addr));
				p=insertElem(p,inet_ntoa(cliaddr.sin_addr),remsg->name,remsg->hostname);
				display(p);
			}
			if(remsg->cmdid ==2){
				printf("recvfrom: %d\n",remsg->id);
				printf("[%s]您的好友： %s[%s]在局域网内广播：\n%s\n",get_time(2), remsg->name,inet_ntoa(cliaddr.sin_addr),remsg->buf);
				sleep(2);
			}
			if(remsg->cmdid ==3&remsg->name == myname){
				printf("recvfrom: %d\n",remsg->id);
				printf("[%s]您的好友： %s[%s]对您私聊：\n%s\n",get_time(2), remsg->name,inet_ntoa(cliaddr.sin_addr),remsg->buf);
				sleep(2);
			}
		} 
		
        //recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        //ip_address = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        
        
        

    }
    close(fd);

	pthread_exit(200);//线程结束，返回一个200的值
}

void *Mainmenu(char *myname)
{
    for (;;)
	{
        sleep(1);
		system("clear");
		pf("*** 局域网客户端主界面 ***\n");
		pf("     1、收发文件\n");
		pf("     2、局域网广播消息\n");
		pf("     3、单独发送消息\n");
		pf("     0、退出\n");
		pf("--------------------------\n");
		// 我的自定义函数 在tools.c里面 get_cmd
		switch (get_cmd('0', '3'))
		{
			case '1':
				tcp("114.55.237.32");
                break;
				//return;22
			case '2':
                //tcp_re();
            	udp_msg(myname);
				break;
			case '3':
				//tcp_re();
				udp_mag();
				break;
			case '0':
				mainquit(myname);
				return;
		}
	}
}

void udp_msg(char *myname){
	char msg[1024];
	printf("局域网内广播>\n");
	printf("请输入您要发送的消息>\n");
	scanf("%s", &msg);
	UDP_11(2,myname,msg);
    return;
}
void udp_mag(){
	char msg[1024];
	char hname[1024];
	printf("局域网内单独发送>\n");
	printf("请输入对方昵称：>\n");
	scanf("%s", &hname);
	for (int i = 0; i<6;i++) 
	{
		//printf("输入abcd结束通信>\n");
		printf("请输入您要发送的消息[只能发送5次]>\n");
		//fgets(msg, 20, stdin);
		scanf("%c", &msg);
		fflush(stdin);
		if(msg[0]=='a'&msg[3]=='c'){
			break;
		}
		UDP_11(3,hname,msg);
	}
    return;
}
void mainquit(char *myname)
{
    int statue = 0;//退出 0
	usleep(10000);
    UDP_11(statue,myname);
	pf("[%s] 程序退出\n", get_time(2));
	return;
}