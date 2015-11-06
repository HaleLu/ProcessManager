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
#define PROCESS_HAS_FINISHED -22	//�����ѽ���

//������� 
#define LIST_IS_NULL -100	//���Ա�Ϊ��
#define HEAD_IS_NULL -101	//����Ϊ�� 

#define ID_FINISH 0
#define ID_RESTART 1

struct ElemType
{
	DWORD processID;
	TCHAR name[MAX_PATH];
	FILETIME creationTime;
	SIZE_T memorySize;
};

typedef struct LNode
{
	ElemType data;
	struct LNode* pre;
	struct LNode* next;
} LNode, *List;

Status InitList(List& L);
Status DestroyList(List& L);
Status CreateRunningList(List& L);
Status RefreshList(List&, List&);
int LocateElem(List& L, ElemType& e, int compare(ElemType&, ElemType&));
Status ListInsert(List& L, ElemType& e, int cmp(ElemType&, ElemType&));
Status ListMoveNode(List& from, List& to, LNode* node, int mode);
Status ListToView(List& L, CListCtrl& list_ctrl, Status translate(ElemType, CListCtrl&));
Status print(ElemType e);
int SaveProcess(DWORD processID, ElemType& e);

void AdjustList(List& L);
Status InsertRunningData(ElemType e, CListCtrl& list_ctrl);
Status InsertRunningData(ElemType e, int pos, CListCtrl& list_ctrl);
Status InsertFinishedData(ElemType e, CListCtrl& list_ctrl);
Status cmpMemory(ElemType& x, ElemType& y);
int cmpIfSame(ElemType& x, ElemType& y);
