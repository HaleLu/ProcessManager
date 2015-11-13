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

//���ܣ�	˫������ĳ�ʼ��
//˵����	�����ڴ�ռ�
Status InitList_Du(DuLinkList& L);

//���ܣ�	˫�����������
//˵����	�ͷſռ�
Status DestroyList_Du(DuLinkList& L);

//���ܣ�	�������ڱ������н�����Ϣ��˫������
//ǰ�������������ѳ�ʼ��
//˵����	����API����EnumProcesses��ö�ٵ�ǰ���̵�PID��
//			������SaveProcess_Du��ListInsert_Du������
//			������Ĵ���ʱ����ڴ�ʹ���������˫��������
Status CreateRunningList_Du(DuLinkList& L);

//���ܣ�������Ԫ��e��ͬ��Ԫ�أ�cmp���������£�
//˵����˳�����
DuLinkList LocateElem_Du(DuLinkList& L, ElemType& e, int cmp(ElemType&, ElemType&), int *pos);

//���ܣ�����С˳�����Ԫ��e
//˵������cmp�����Ĵ�С˳����롣
int SortedListInsert_Du(DuLinkList& L, ElemType& e, int cmp(ElemType&, ElemType&));


typedef struct LNode
{
	ElemType data;
	struct LNode* next;
} *LinkList;

//���ܣ�	��������ĳ�ʼ��
//˵����	�����ڴ�ռ�
Status InitList_L(LinkList& L);

//���ܣ�	�������������
//˵����	�ͷſռ�
Status DestroyList_L(LinkList& L);

//���ܣ�	��������Ĳ���
//˵����	�����ҵ��Ķ�Ӧֵ��ǰ����ע����ǰ����������
LinkList LocateElem_L(LinkList& L, ElemType& e, int cmp(ElemType&, ElemType&), int *pos);



//���ܣ���������Ϣ����Ϊ�ṹ��
//˵��������API����PID�鵽���̵������Ϣ�����浽Ԫ��e��
Status SaveProcess(DWORD processID, ElemType& e);

//���ܣ��ȽϽ���ռ���ڴ��С
//˵��������ǰ�ߴ󷵻����������ߴ��򷵻ظ�������ȷ���0
int cmpMemory(ElemType& x, ElemType& y);

//���ܣ��ȽϽ����Ƿ�Ϊͬһ����
//˵����ͨ��PID�ͽ��̴���ʱ���ж��Ƿ�Ϊͬһ����
int cmpIfSame(ElemType& x, ElemType& y);

//���ܣ��ȽϽ��������Ƿ���ͬ
int cmpName(ElemType& x, ElemType& y);


//���ܣ��������������ƶ����
//˵����mode��ʾ�ƶ�ģʽ��ID_FINISHED��ID_RESTART��
//		modeΪ FINISHED ʱ��running_nodeΪ�����Ľ��̣�finished_nodeΪ�������ͷָ��,����ֵΪ���뵥�����ָ�룻
//		modeΪ RESTART ʱ��running_nodeΪ˫�������ͷָ�룬finished_nodeΪҪɾ������ǰ����ע����ǰ������������ֵΪ����˫�������ָ�룬pos�����Ӧλ��
void* ListMoveNode_Du(DuLinkList& running_node, LinkList& finished_node, int mode, int* pos);