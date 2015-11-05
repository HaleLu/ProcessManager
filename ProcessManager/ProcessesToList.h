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

//错误代码 
#define LIST_IS_NULL -100	//线性表为空
#define HEAD_IS_NULL -101	//链表为空 

struct ElemType {
	DWORD processID;
	TCHAR name[MAX_PATH];
	FILETIME creationTime;
	SIZE_T memorySize;
};

typedef struct 	LNode {
	ElemType data;
	struct LNode *pre;
	struct LNode *next;
} LNode, *List;

Status InitList(List &L);
Status DestroyList(List &L);
Status CreateList(List &L);
Status Refresh(List& L);
int LocateElem(List& L, ElemType e, int compare(ElemType, ElemType));
Status ListInsert(List &L, ElemType e);
Status ListToView(List& L, CListCtrl &list_ctrl, Status translate(ElemType, CListCtrl&));
Status print(ElemType e);
int SaveProcess(DWORD processID, ElemType &e);

void GenerateList(List &L);
void AdjustList(List &L);

Status InsertData(ElemType e, CListCtrl &list_ctrl);