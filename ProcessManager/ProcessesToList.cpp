#include "stdafx.h"
#include "ProcessesToList.h"
#include "Helper.h"

Status InitList_Du(DuLinkList& L)
{
	L = (DuLinkList)malloc(sizeof(DuLNode));
	memset((L), 0, sizeof(*L));
	return OK ;
}//InitList_Du

Status DestroyList_Du(DuLinkList& L)
{
	if (&L == NULL) exit(LIST_IS_NULL);
	if (L == NULL) exit(HEAD_IS_NULL);
	DuLinkList p = L;
	DuLinkList q;
	while (p != NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
	return OK ;
}//DestroyList_Du

Status CreateRunningList_Du(DuLinkList& L)
{
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;
	ElemType tmp_e;

	unsigned int i;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return ENUM_PROCESSES_FAILED ;
	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < cProcesses; i++)
	{
		if (SaveProcess(aProcesses[i], tmp_e) == OK)
			SortedListInsert_Du(L, tmp_e, cmpMemory);
	}

	return OK ;
}

DuLinkList LocateElem_Du(DuLinkList& L, ElemType& e, int compare(ElemType&, ElemType&), int *pos)
{
	DuLinkList p = L->next;
	int i = 0;
	for (; p != NULL; i++, p = p->next)
	{
		if (compare(p->data, e) == 0)
		{
			*pos = i;
			return p;
		}
			
	}
	return NULL;
}//LocateElem_Du

int SortedListInsert_Du(DuLinkList& L, ElemType& e, int cmp(ElemType&, ElemType&))
{
	unsigned int i = 0;
	DuLinkList p;
	int tmp;
	p = LocateElem_Du(L, e, cmpName, &tmp);
	if (p != NULL)
	{
		if (CompareFileTime(&e.creationTime, &p->data.creationTime) < 0)
		{
			memcpy(&p->data.creationTime, &e.creationTime, sizeof(FILETIME));
		}
		p->data.memorySize += e.memorySize;
		//根据p中进程内存的大小调整p的位置
		DuLinkList q = p->pre;
		while (q != L && p->data.memorySize > q->data.memorySize)
		{
			q = q->pre;
		}

		if (q != p->pre)
		{
			p->pre->next = p->next;
			if (p->next) p->next->pre = p->pre;
			p->pre = q;
			p->next = q->next;
			q->next = p;
			p->next->pre = p;
		}
		return 0;
	}

	p = L;
	while (p->next && cmp(e, p->next->data) < 0)
	{
		p = p->next;
		i++;
	}

	DuLinkList s = (DuLinkList)malloc(sizeof(DuLNode));
	memcpy(&s->data, &e, sizeof(ElemType));
	s->next = p->next;
	p->next = s;
	s->pre = p;
	if (s->next != NULL) s->next->pre = s;
	return i;
}//SortedListInsert_Du

void* ListMoveNode_Du(DuLinkList& running_node, LinkList& finished_node, int mode, int* pos)
{
	if (mode == ID_FINISH)
	{
		Helper helper;
		running_node->pre->next = running_node->next;
		if (running_node->next != NULL)running_node->next->pre = running_node->pre;
		LinkList to = (LinkList)malloc(sizeof(LNode));
		memcpy(&to->data, &running_node->data, sizeof(ElemType));
		helper.FileTimeAddRunningTime(&to->data.runningTime, to->data.creationTime);
		free(running_node);

		to->next = finished_node->next;
		finished_node->next = to;
		return to;
	}

	if (mode == ID_RESTART)
	{
		LinkList toDelete = finished_node->next;
		finished_node->next = toDelete->next;

		DuLinkList p = running_node;
		int i = 0;
		while (p->next && cmpMemory(toDelete->data, p->next->data) < 0)
		{
			p = p->next;
			i++;
		}
		DuLinkList to = (DuLinkList)malloc(sizeof(DuLNode));
		memcpy(&to->data, &toDelete->data, sizeof(ElemType));
		free(toDelete);

		to->next = p->next;
		p->next = to;
		to->pre = p;
		if (to->next != NULL) to->next->pre = to;
		*pos = i;
		return to;
	}

	return NULL ;
}//ListMoveNode_Du

Status SaveProcess(DWORD processID, ElemType& e)
{
	HMODULE hMods[1];
	HANDLE hProcess;
	DWORD cbNeeded;
	DWORD cExitCode;
	e.processID = processID;
	e.memorySize = 0;
	memset(&e.runningTime, 0, sizeof(FILETIME));

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (NULL == hProcess) return OPEN_PROCESS_FAILED ;
	GetExitCodeProcess(hProcess, &cExitCode);
	if (cExitCode != STILL_ACTIVE) return PROCESS_HAS_FINISHED;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		TCHAR szModName[MAX_PATH];
		if (GetModuleBaseName(hProcess, hMods[0], szModName, sizeof(szModName) / sizeof(TCHAR)))
		{
			memcpy(e.name, szModName, sizeof(szModName));
		}
	}

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
	e.memorySize = pmc.WorkingSetSize;

	FILETIME tempTime;
	GetProcessTimes(hProcess, &e.creationTime, &tempTime, &tempTime, &tempTime);

	CloseHandle(hProcess);
	return OK ;
}

int cmpMemory(ElemType& x, ElemType& y)
{
	if (x.memorySize == y.memorySize) return 0;
	return x.memorySize - y.memorySize;
}

int cmpIfSame(ElemType& x, ElemType& y)
{
	if (x.processID == y.processID && CompareFileTime(&x.creationTime, &y.creationTime) == 0) return 0;
	return -1;
}

int cmpName(ElemType& x, ElemType& y)
{
	Helper helper;
	char x_name[MAX_PATH];
	char y_name[MAX_PATH];
	strcpy(x_name, helper.WcharToChar(x.name));
	strcpy(y_name, helper.WcharToChar(y.name));
	if (strcmp(x_name, y_name) == 0)
		return 0;
	return -1;
}


Status InitList_L(LinkList& L)
{
	L = (LinkList)malloc(sizeof(LNode));
	memset((L), 0, sizeof(*L));
	return OK;
}//InitList_L

Status DestroyList_L(LinkList& L)
{
	if (&L == NULL) exit(LIST_IS_NULL);
	if (L == NULL) exit(HEAD_IS_NULL);
	LinkList p = L;
	LinkList q;
	while (p != NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
	return OK;
}//DestroyList_L

LinkList LocateElem_L(LinkList& L, ElemType& e, int compare(ElemType&, ElemType&), int *pos)
{
	LinkList p = L;
	int i = 0;
	for (; p->next != NULL; i++, p = p->next)
	{
		if (compare(p->next->data, e) == 0)
		{
			*pos = i;
			return p;
		}
	}
	return NULL;
}//LocateElem_L