#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#pragma comment (lib, "psapi.lib")

#define Status int

//���������� 
#define OK 1 			//�ɹ� 
#define OUT_OF_RANGE -10	//�±�Խ��
#define ELEM_NOT_FOUND -11	//Ԫ��δ�ҵ� 
#define VISIT_FAILED -12	//Ԫ�ط���ʧ�� 
#define ENUM_PROCESSES_FAILED -20	//ö�ٵ�ǰ����ʧ�� 
#define OPEN_PROCESS_FAILED -21 	//�򿪽���ʧ�� 

//������� 
#define LIST_IS_NULL -100	//���Ա�Ϊ��
#define HEAD_IS_NULL -101	//����Ϊ�� 

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