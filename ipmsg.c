#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "tools.h"
#include "udp_11.h"
#include "tcp.h"
#include "tcp_re.h"
#define pf printf
#define UDP_PORT 6550
#define TCP_PORT 9009
char cmd[20] = {};
typedef struct 
{
    char id[20];            //包编号
    char name[20];            //发送者姓名
    char hostname[20];         //机器名
    unsigned int cmdid;        //命令字
    char buf[1024];            //附加信息
}cmds;
typedef struct{
    char name[20]; //
    char ip_address[20]; //
    char hostname[20]; //
}udplist;

void mainmenu(void);
void mainquit(void);
void downloads(void);
void showlists(void);
void *start_run(void *arg);
int tcp_re();
void *Udp_res(void *p); //udp接受线程
void *funs_1(void *p) //参数的值为 123
{
	int i = 0;
	mainmenu();
	pthread_exit(200);//线程结束，返回一个200的值
}
void *tcp_re_main(void *p) //tcp接受
{
	tcp_re();
    //printf("tcp_re");
	pthread_exit(200);//线程结束，返回一个200的值
}
int smain()
{
	pthread_t id, id2,tcp;//创建两个线程
	int a=0,i=0;
    char ip_address = "172.27.99.10";
    int sinin = 0;
    UDP_11(sinin);
    tcp_re();
    //pthread_create(&tcp, NULL, tcp_re_main, (void *)124 );
	//pthread_create(&id, NULL, Udp_res, (void *)123 );
	//pthread_create(&id2, NULL, funs_1, (void *)124 );
    
	pthread_join(id, &a);//接收来自线程ID为id的返回的值
    pthread_join(id2, &i);//接收来自线程ID为id的返回的值
	//printf("a=%d \n",a);
	return 0;
}

void *Udp_res(void *p) //udp接受线程
{   //printf("udp\n"); 
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

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
    {
        // 接收广播消息
        memset(buf, 0, sizeof(buf));
        // 阻塞等待数据达到
        recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, NULL);
        //recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        //ip_address = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        transcode(remsg,buf, sizeof(buf));
        printf(": %d\n",remsg->id);
        printf("[%s]您的好友： %s[%s]已经上线\n",get_time(2), remsg->name,inet_ntoa(cliaddr.sin_addr));

    }
    close(fd);

	pthread_exit(200);//线程结束，返回一个200的值
}

void *mainmenu(void*p)
{
    for (;;)
	{
        sleep(1);
		system("clear");
		pf("*** 局域网客户端主界面 ***\n");
		pf("     1、显示列表\n");
		pf("     2、选择通信对象\n");
		pf("     0、退出\n");
		pf("--------------------------\n");
		// 我的自定义函数 在tools.c里面 get_cmd
		switch (get_cmd('0', '2'))
		{
			case '1':
				tcp("114.55.237.32");
                break;
				//return;
			case '2':
                //tcp_re();
            	downloads();
				break;
			case '0':
				mainquit();
				return;
		}
	}
}
void mainquit(void)
{
    char sinin[20] = "out";
	usleep(10000);
    UDP_11(sinin);
	pf("[%s] 程序退出\n", get_time(2));
	return;
}
void downloads(void){
    return;
}
void showlists(void){
    return;
}
int tcp_re(void){
    return;
}
// 主函数
int main()//tcp_re()
{	printf("0000000000000000");
	pf("[%s] 服务器创建socket...\n", get_time(2));
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sockfd)
	{
		perror("socket");
		return -1;
	}
	printf("0000000000000000");
	pf("[%s] 准备地址...\n", get_time(2));
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	// 端口 IP 自行修改
	addr.sin_port = htons(TCP_PORT);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t len = sizeof(addr);
	printf("0000000000000000");
	pf("[%s] 绑定socket与地址...\n", get_time(2));
	if (bind(sockfd, (struct sockaddr *)&addr, len))
	{
		perror("bind");
		return -1;
	}
	printf("0000000000000000");
	pf("[%s] 设置监听...\n", get_time(2));
	if (listen(sockfd, 5))
	{
		perror("listen");
		return -1;
	}

	pf("[%s] 等待客户端连接...\n", get_time(2));
	printf("0000000000000000");
	for (;;)
	{
		printf("0000000000000000");
		struct sockaddr_in addrcli = {};
		// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		int *clifd = (int*)malloc(sizeof(int));
		printf("0000000000000000");
		*clifd = accept(sockfd, (struct sockaddr *)&addrcli, &len);
		printf("11111111111111111111");
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

// 开始运行
void *start_run(void *arg)
{	
	printf("start_run\n");
	int *clifd = (int *)arg;
	char up[20] = "我想上你";
	char down[20] = "我想下你";
	char see[20] = "我想看你";
	char quit[20] = "我要走了";
	int c_size = 0;
	for (;;)
	{
		c_size = read(*clifd, cmd, sizeof(cmd));
		if(-1 == c_size)
		{
			pf("[%s] read函数出错！\n", get_time(2));
		}
		
		if (strcmp(up, cmd) == 0)
		{
			pf("[%s] 收到客户端的上传指令\n", get_time(2));
			c_up(clifd);
			memset(cmd, 0, 20);
		}
		else if (strcmp(down, cmd) == 0)
		{
			pf("[%s] 收到客户端的下载指令\n", get_time(2));
			c_down(clifd);
			memset(cmd, 0, 20);
		}
		else if (strcmp(see, cmd) == 0)
		{
			pf("[%s] 收到客户端的目录指令\n", get_time(2));
			rc_list(clifd);
			memset(cmd, 0, 20);
		}
		else if (strcmp(quit, cmd) == 0)
		{
			pf("[%s] 收到服务端的退出指令\n", get_time(2));
			pthread_exit(0);
			return (void *)NULL;
		}
		
	}
	//char *str = "我死了";
	//pthread_exit(str);
}