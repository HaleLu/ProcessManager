#pragma once
#include "ProcessesToList.h"
#include "ProcessManagerDlg.h"

class ListHelper
{
public:
	ListHelper();
	//���ܣ�ͨ��ָ����translate����
//TO-DO
	static Status ListToView(DuLinkList& L, CListCtrl& list_ctrl, Status translate(ElemType, CListCtrl&));
	static Status ListToView(LinkList& L, CListCtrl& list_ctrl, Status translate(ElemType, CListCtrl&));
	static Status InsertRunningData(ElemType e, CListCtrl& list_ctrl);
	static Status InsertRunningData(ElemType e, int pos, CListCtrl& list_ctrl);
	static Status InsertFinishedData(ElemType e, CListCtrl& list_ctrl);
};

