#include "stdafx.h"
#include "ProcessesToList.h"

Status InitList(List& L)
{
	L = (List)malloc(sizeof(LNode));
	memset((L), 0, sizeof(*L));
	return OK ;
}//InitList

Status DestroyList(List& L)
{
	if (&L == NULL) exit(LIST_IS_NULL);
	if (L == NULL) exit(HEAD_IS_NULL);
	List p = L;
	List q;
	while (p != NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
	free(&L);
	return OK ;
}//DestroyList

Status CreateRunningList(List& L)
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
			ListInsert(L, tmp_e, cmpMemory);
	}

	return OK ;
}

Status RefreshList(List& running_list, List& finished_list)
{
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;
	ElemType tmp_e;
	DWORD cExitCode;

	//移动running_list里结束的进程至finished_list并将running_list按内存重新排序
	List p = running_list->next;
	List q, tmp;
	while (p != NULL)
	{
		Status res = SaveProcess(p->data.processID, tmp_e);
		if (res == OPEN_PROCESS_FAILED || res == PROCESS_HAS_FINISHED || LocateElem(running_list, tmp_e, cmpIfSame) == 0)
		{
			q = p;
			p = p->next;
			ListMoveNode(running_list, finished_list, q, ID_FINISH);
		}
		else
		{
			tmp = p;
			q = p->pre;
			p->data.memorySize = tmp_e.memorySize;
			while (q != running_list && p->data.memorySize > q->data.memorySize)
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
			p = tmp->next;
		}
	}

	//比对当前运行的进程，新增进程Insert进running_list
	unsigned int i;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return ENUM_PROCESSES_FAILED;
	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < cProcesses; i++)
	{
		if (SaveProcess(aProcesses[i], tmp_e) == OK)
		{
			if (LocateElem(running_list, tmp_e, cmpIfSame) == 0)
			{
				ListInsert(running_list, tmp_e, cmpMemory);
			}
		}
		
	}

	return OK ;
}

int LocateElem(List& L, ElemType& e, int compare(ElemType&, ElemType&))
{
	int i;
	List p = L->next;
	for (i = 1; p != NULL; i++ , p = p->next)
	{
		if (compare(p->data, e) == 0) 
			return i;
	}
	return 0;
}//LocateElem

Status ListInsert(List& L, ElemType& e, int cmp(ElemType&, ElemType&))
{
	if (&L == NULL) exit(LIST_IS_NULL);
	if (L == NULL) exit(HEAD_IS_NULL);
	List p = L;
	while (p->next && cmp(e, p->next->data) < 0)
	{
		p = p->next;
	}

	List s = (List)malloc(sizeof(LNode));
	memcpy(&s->data, &e, sizeof(ElemType));
	s->next = p->next;
	p->next = s;
	s->pre = p;
	if (s->next != NULL) s->next->pre = s;
	return OK ;
}//ListInsert


Status ListMoveNode(List& from, List& to, LNode* node, int mode)
{
	if (mode == ID_FINISH)
	{
		node->pre->next = node->next;
		if (node->next != NULL)node->next->pre = node->pre;
		node->pre = to;
		node->next = to->next;
		to->next = node;
		if (node->next != NULL) node->next->pre = node;
	}

	if (mode == ID_RESTART)
	{
		node->pre->next = node->next;
		if (node->next != NULL)node->next->pre = node->pre;
		List p = to;
		while (p->next && cmpMemory(node->data, p->next->data) < 0)
		{
			p = p->next;
		}

		node->next = p->next;
		p->next = node;
		node->pre = p;
		if (node->next != NULL) node->next->pre = node;
	}

	return OK ;
}//ListMoveNode

Status ListToView(List& L, CListCtrl& list_ctrl, Status translate(ElemType, CListCtrl&))
{
	if (L == NULL) exit(HEAD_IS_NULL);
	List p = L->next;
	while (p != NULL)
	{
		if (translate(p->data, list_ctrl) != OK) return VISIT_FAILED ;
		p = p->next;
	}
	return OK ;
}//ListTraverse

Status SaveProcess(DWORD processID, ElemType& e)
{
	HMODULE hMods[1];
	HANDLE hProcess;
	DWORD cbNeeded;
	DWORD cExitCode;
	e.processID = processID;

	e.memorySize = 0;

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
