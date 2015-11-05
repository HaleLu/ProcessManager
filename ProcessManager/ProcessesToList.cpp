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

Status CreateList(List& L)
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
			ListInsert(L, tmp_e);
	}

	return OK ;
}

Status Refresh(List& L)
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
			ListInsert(L, tmp_e);
	}

	return OK ;
}

int LocateElem(List& L, ElemType e, int compare(ElemType, ElemType))
{
	if (L == NULL) exit(HEAD_IS_NULL);
	int i;
	List p = L->next;
	for (i = 1; p != NULL; i++ , p = p->next)
	{
		if (compare(p->data, e) == 0) return i;
	}
	return 0;
}//LocateElem

Status ListInsert(List& L, ElemType e)
{
	if (&L == NULL) exit(LIST_IS_NULL);
	if (L == NULL) exit(HEAD_IS_NULL);
	List p = L;
	while (p->next && e.memorySize < p->next->data.memorySize)
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

Status ListToView(List& L, CListCtrl &list_ctrl, Status translate(ElemType, CListCtrl&))
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

Status print(ElemType e)
{
	printf("Process ID: %u", e.processID);
	_tprintf(TEXT("\t%s"), e.name);
	_tprintf(TEXT("\t%.1fM"), e.memorySize * 1.0 / 1024 / 1024);

//	printFileTime(e.creationTime);

	SYSTEMTIME currentSystemTime;
	GetSystemTime(&currentSystemTime);
	FILETIME currentTime;
	SystemTimeToFileTime(&currentSystemTime, &currentTime);

	FILETIME runningTime;

	if (currentTime.dwLowDateTime > e.creationTime.dwLowDateTime)
	{
		runningTime.dwHighDateTime = currentTime.dwHighDateTime - e.creationTime.dwHighDateTime;
		runningTime.dwLowDateTime = currentTime.dwLowDateTime - e.creationTime.dwLowDateTime;
	}
	else
	{
		runningTime.dwHighDateTime = currentTime.dwHighDateTime - e.creationTime.dwHighDateTime - 1;
		runningTime.dwLowDateTime = currentTime.dwLowDateTime - e.creationTime.dwLowDateTime;
	}

	SYSTEMTIME systemTime;
	FileTimeToSystemTime(&runningTime, &systemTime);
	_tprintf(TEXT("\t%02d-%02d:%02d:%02d"),
	                                      systemTime.wDay - 1, systemTime.wHour,
	                                      systemTime.wMinute, systemTime.wSecond);

	printf("\n\n");
	return OK ;
}

Status SaveProcess(DWORD processID, ElemType& e)
{
	HMODULE hMods[1];
	HANDLE hProcess;
	DWORD cbNeeded;
	//unsigned int i;

	//	printf( "\nProcess ID: %u\n", processID );
	e.processID = processID;

	e.memorySize = 0;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (NULL == hProcess) return OPEN_PROCESS_FAILED ;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		TCHAR szModName[MAX_PATH];
		if (GetModuleBaseName(hProcess, hMods[0], szModName, sizeof(szModName) / sizeof(TCHAR)))
		{
			//			_tprintf( TEXT("\t%s (0x%08X)\n"), szModName, hMods[0] );
			memcpy(e.name, szModName, sizeof(szModName));
		}
	}
	else memcpy(e.name, "<Unknown>", sizeof("<Unknown>"));

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
	e.memorySize = pmc.WorkingSetSize;

	FILETIME tempTime;
	GetProcessTimes(hProcess, &e.creationTime, &tempTime, &tempTime, &tempTime);

	CloseHandle(hProcess);
	return OK ;
}

void GenerateList(List &L)
{
	InitList(L);
	CreateList(L);
}
