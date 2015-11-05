// ProcessManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#define IDC_PROCESSID 0
#define IDC_NAME 1
#define IDC_TIME 2
#define IDC_MEMORY 3


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
	CListCtrl m_list_running;
};
