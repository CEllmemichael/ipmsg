#ifndef TOOL_H
#define TOOL_H
#define pf printf
#define UDP_PORT 6550
#define TCP_PORT 8000

// 菜单
void menu(void);
// 上传
void upload(void);
// 下载
void download(void);
// 显示服务器目录和文件
void s_list(void);
// 显示客户端目录和文件
void c_list(void);
// 退出程序
void quit(void);

// 主函数
int tcps(char ip[20]);




#endif//TOOL_h