#pragma once
#include "ProcessesToList.h"
#include "ProcessManagerDlg.h"

class ListHelper
{
public:
	ListHelper();
	~ListHelper();

	static Status ListToView(List& L, CListCtrl& list_ctrl, Status translate(ElemType, CListCtrl&));
	static Status InsertRunningData(ElemType e, CListCtrl& list_ctrl);
	static Status InsertRunningData(ElemType e, int pos, CListCtrl& list_ctrl);
	static Status InsertFinishedData(ElemType e, CListCtrl& list_ctrl);
};

