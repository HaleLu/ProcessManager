#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#pragma comment (lib, "psapi.lib")

#define Status int

//函数返回码 
#define OK 1 			//成功 
#define OUT_OF_RANGE -10	//下标越界
#define ELEM_NOT_FOUND -11	//元素未找到 
#define VISIT_FAILED -12	//元素访问失败 
#define ENUM_PROCESSES_FAILED -20	//枚举当前进程失败 
#define OPEN_PROCESS_FAILED -21 	//打开进程失败 
#define PROCESS_HAS_FINISHED -22	//进程已结束

//错误代码 
#define LIST_IS_NULL -100	//线性表为空
#define HEAD_IS_NULL -101	//链表为空 

#define ID_FINISH 0
#define ID_RESTART 1

struct ElemType
{
	DWORD processID;
	TCHAR name[MAX_PATH];
	FILETIME creationTime;
	FILETIME runningTime;
	SIZE_T memorySize;
};

typedef struct DuLNode
{
	ElemType data;
	bool flag;
	struct DuLNode* pre;
	struct DuLNode* next;
} *DuLinkList;

//功能：	双向链表的初始化
//说明：	申请内存空间
Status InitList_Du(DuLinkList& L);

//功能：	双向链表的销毁
//说明：	释放空间
Status DestroyList_Du(DuLinkList& L);

//功能：	创建用于保存运行进程信息的双向链表
//前置条件：链表已初始化
//说明：	调用API，即EnumProcesses，枚举当前进程的PID，
//			并调用SaveProcess_Du和ListInsert_Du函数，
//			将具体的创建时间和内存使用情况存入双向链表中
Status CreateRunningList_Du(DuLinkList& L);

//功能：查找与元素e相同的元素（cmp函数条件下）
//说明：顺序查找
DuLinkList LocateElem_Du(DuLinkList& L, ElemType& e, int cmp(ElemType&, ElemType&), int *pos);

//功能：按大小顺序插入元素e
//说明：按cmp函数的大小顺序插入。
int SortedListInsert_Du(DuLinkList& L, ElemType& e, int cmp(ElemType&, ElemType&));


typedef struct LNode
{
	ElemType data;
	struct LNode* next;
} *LinkList;

//功能：	单向链表的初始化
//说明：	申请内存空间
Status InitList_L(LinkList& L);

//功能：	单向链表的销毁
//说明：	释放空间
Status DestroyList_L(LinkList& L);

//功能：	单向链表的查找
//说明：	返回找到的对应值的前驱（注意是前驱！！！）
LinkList LocateElem_L(LinkList& L, ElemType& e, int cmp(ElemType&, ElemType&), int *pos);



//功能：将进程信息保存为结构体
//说明：调用API根据PID查到进程的相关信息并保存到元素e中
Status SaveProcess(DWORD processID, ElemType& e);

//功能：比较进程占用内存大小
//说明：返回前者大返回正数，后者大则返回负数，相等返回0
int cmpMemory(ElemType& x, ElemType& y);

//功能：比较进程是否为同一进程
//说明：通过PID和进程创建时间判断是否为同一进程
int cmpIfSame(ElemType& x, ElemType& y);

//功能：比较进程名称是否相同
int cmpName(ElemType& x, ElemType& y);


//功能：在两个链表中移动结点
//说明：mode表示移动模式（ID_FINISHED或ID_RESTART）
//		mode为 FINISHED 时，running_node为结束的进程，finished_node为单链表的头指针,返回值为插入单链表的指针；
//		mode为 RESTART 时，running_node为双向链表的头指针，finished_node为要删除结点的前驱（注意是前驱！！）返回值为插入双向链表的指针，pos保存对应位置
void* ListMoveNode_Du(DuLinkList& running_node, LinkList& finished_node, int mode, int* pos);