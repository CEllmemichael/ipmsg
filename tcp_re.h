#ifndef TOOL_H
#define TOOL_H
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
void *start_run(void *arg);
// 客户端上传
void cs_up(int *clifd);
// 客户端下载
void cs_down(int *clifd);
// 返回文件列表
void cs_list(int *clifd);

#endif//TOOL_h