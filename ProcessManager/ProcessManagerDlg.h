// ProcessManagerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "Helper.h"
#include "ListHelper.h"


#define IDC_RUNNING_NAME 0
#define IDC_RUNNING_STARTTIME 1
#define IDC_RUNNING_RUNNINGTIME 2
#define IDC_RUNNING_MEMORY 3

#define IDC_FINISHED_NAME 0
#define IDC_FINISHED_RUNNINGTIME 1
#define IDC_FINISHED_STARTTIME 2
#define IDC_FINISHED_FINISHEDTIME 3


// CProcessManagerDlg 对话框
class CProcessManagerDlg : public CDialogEx
{
	// 构造
public:
	CProcessManagerDlg(CWnd* pParent = NULL); // 标准构造函数
	~CProcessManagerDlg();
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSMANAGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_running_list_crtl;
	CListCtrl m_finished_list_crtl;
	DuLinkList m_running_list;
	LinkList m_finished_list;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Refresh();

private:
	Helper m_helper;
	ListHelper m_list_helper;
};
