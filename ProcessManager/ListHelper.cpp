#include "stdafx.h"
#include "ListHelper.h"

ListHelper::ListHelper()
{
}

Status ListHelper::ListToView(DuLinkList& L, CListCtrl& list_ctrl, Status translate(ElemType, CListCtrl&))
{
	if (L == NULL) exit(HEAD_IS_NULL);
	DuLinkList p = L->next;
	while (p != NULL)
	{
		if (translate(p->data, list_ctrl) != OK) return VISIT_FAILED;
		p = p->next;
	}
	return OK;
}//ListToView

Status ListHelper::ListToView(LinkList& L, CListCtrl& list_ctrl, Status translate(ElemType, CListCtrl&))
{
	if (L == NULL) exit(HEAD_IS_NULL);
	LinkList p = L->next;
	while (p != NULL)
	{
		if (translate(p->data, list_ctrl) != OK) return VISIT_FAILED;
		p = p->next;
	}
	return OK;
}//ListToView

Status ListHelper::InsertRunningData(ElemType e, CListCtrl& list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];
	int count = list_ctrl.GetItemCount();
	list_ctrl.InsertItem(count, NULL);
	list_ctrl.SetItemText(count, IDC_RUNNING_NAME, e.name);
	list_ctrl.SetItemText(count, IDC_RUNNING_STARTTIME, helper.FileTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(count, IDC_RUNNING_RUNNINGTIME, helper.FileTimeAddToRunningTimeToWChar(e.creationTime, e.runningTime));
	list_ctrl.SetItemText(count, IDC_RUNNING_MEMORY, helper.DoubleToWchar_M((double)e.memorySize / 1024 / 1024));
	return OK;
}

Status ListHelper::InsertRunningData(ElemType e, int pos, CListCtrl& list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];
	list_ctrl.InsertItem(pos, NULL);
	list_ctrl.SetItemText(pos, IDC_RUNNING_NAME, e.name);
	list_ctrl.SetItemText(pos, IDC_RUNNING_STARTTIME, helper.FileTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(pos, IDC_RUNNING_RUNNINGTIME, helper.FileTimeAddToRunningTimeToWChar(e.creationTime, e.runningTime));
	list_ctrl.SetItemText(pos, IDC_RUNNING_MEMORY, helper.DoubleToWchar_M((double)e.memorySize / 1024 / 1024));
	return OK;
}

Status ListHelper::InsertFinishedData(ElemType e, CListCtrl& list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];

	SYSTEMTIME cLocalTime;
	GetLocalTime(&cLocalTime);

	list_ctrl.InsertItem(0, NULL);
	list_ctrl.SetItemText(0, IDC_FINISHED_NAME, e.name);
	list_ctrl.SetItemText(0, IDC_FINISHED_STARTTIME, helper.FileTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(0, IDC_FINISHED_RUNNINGTIME, helper.FileTimeAddToRunningTimeToWChar(e.creationTime, e.runningTime));
	list_ctrl.SetItemText(0, IDC_FINISHED_FINISHEDTIME, helper.SystemTimeToWChar(cLocalTime));
	return OK;
}