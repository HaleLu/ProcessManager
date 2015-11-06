// ProcessManagerDlg.h : ͷ�ļ�
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

// CProcessManagerDlg �Ի���
class CProcessManagerDlg : public CDialogEx
{
	// ����
public:
	CProcessManagerDlg(CWnd* pParent = NULL); // ��׼���캯��

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSMANAGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
