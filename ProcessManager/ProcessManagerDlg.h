// ProcessManagerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#ifndef LIST_H
#include "ProcessesToList.h"
#endif

#define IDC_RUNNING_PROCESSID 0
#define IDC_RUNNING_NAME 1
#define IDC_RUNNING_STARTTIME 2
#define IDC_RUNNING_RUNNINGTIME 3
#define IDC_RUNNING_MEMORY 4

#define IDC_FINISHED_PROCESSID 0
#define IDC_FINISHED_NAME 1
#define IDC_FINISHED_RUNNINGTIME 2
#define IDC_FINISHED_FINISHEDTIME 3

// CProcessManagerDlg 对话框
class CProcessManagerDlg : public CDialogEx
{
	// 构造
public:
	CProcessManagerDlg(CWnd* pParent = NULL); // 标准构造函数

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
	List m_running_list;
	List m_finished_list;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ReFresh();
};
