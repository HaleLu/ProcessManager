// ProcessManagerDlg.h : ͷ�ļ�
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


// CProcessManagerDlg �Ի���
class CProcessManagerDlg : public CDialogEx
{
	// ����
public:
	CProcessManagerDlg(CWnd* pParent = NULL); // ��׼���캯��
	~CProcessManagerDlg();
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
	DuLinkList m_running_list;
	LinkList m_finished_list;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Refresh();

private:
	Helper m_helper;
	ListHelper m_list_helper;
};
