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
#define pf printf

typedef struct 
{
    char id[20];            //包编号
    char name[20];            //发送者姓名
    char hostname[20];         //机器名
    unsigned int cmdid;        //命令字
    char buf[1024];            //附加信息
}cmds;
void UDP_11(int *login,char *myname,char *append) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }
    if (append == NULL) 
    {
        append = "";
    }
    // 2. 设置广播属性
    int opt  = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    char buf[1024];
    struct sockaddr_in cliaddr;
    int len = sizeof(cliaddr);
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(UDP_PORT); // 接收端需要绑定9999端口
    // 只要主机在237网段, 并且绑定了9999端口, 这个接收端就能收到广播消息
    inet_pton(AF_INET, "192.168.1.255", &cliaddr.sin_addr.s_addr);//
    // 3. 通信
    int num = 0;
    //cmds *mymsg=(cmds*)malloc(sizeof(cmds));
    ipmsg_coding(buf,login,append,myname);//编码
    // 数据广播
    sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr*)&cliaddr, len);
   
    sleep(1);
    close(fd);
}
void ipmsg_coding(char *buffer, unsigned int cmd, char *append,char *myname)
{
    char hname[20];
    char input[20];
    //char myname[20];
    time_t h;
    time(&h);
    gethostname(hname, sizeof(hname));
    if (append == NULL) 
    {
        append = "";
    }
    //此处广播消息功能没有开发
    // printf("请输入你的用户名: \n");
    // scanf("%s", &myname);
    if(cmd == 1){
        pf("[%s]尊敬的用户:【%s 】您已经成功登录\n",get_time(2),myname);
    }
    sprintf(buffer, "1:%ld:%s:%s:%d:%s",h, myname, hname, cmd, append);
}
//将接收到的字符串分解为struct cmd结构体
void transcode(cmds *object, char *buffer, int len)
{
    sscanf(buffer, "1:%[^:]:%[^:]:%[^:]:%d:%[^\n]", &object->id, &object->name, &object->hostname, &object->cmdid, &object->buf);
}
