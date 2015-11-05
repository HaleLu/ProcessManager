// ProcessManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessManagerDlg.h"
#include "ProcessesToList.h"
#include "afxdialogex.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProcessManagerDlg �Ի���


CProcessManagerDlg::CProcessManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RUNNING, m_list_running);
}

BEGIN_MESSAGE_MAP(CProcessManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	END_MESSAGE_MAP()


// CProcessManagerDlg ��Ϣ�������

BOOL CProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE); // ���ô�ͼ��
	SetIcon(m_hIcon, FALSE); // ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// ����List_Running����ʼ��
	CRect rect;
	m_list_running.GetClientRect(&rect);
	m_list_running.InsertColumn(IDC_PROCESSID,	_T("PID"),		LVCFMT_LEFT, 40, IDC_PROCESSID);
	m_list_running.InsertColumn(IDC_NAME,		_T("��������"), LVCFMT_LEFT, 180, IDC_NAME);
	m_list_running.InsertColumn(IDC_STARTTIME,	_T("����ʱ��"), LVCFMT_LEFT, 120, IDC_STARTTIME);
	m_list_running.InsertColumn(IDC_RUNNINGTIME,_T("����ʱ��"), LVCFMT_LEFT, 80, IDC_RUNNINGTIME);
	m_list_running.InsertColumn(IDC_MEMORY,		_T("�ڴ�"),		LVCFMT_LEFT, 60, IDC_MEMORY);
	
	//��ȡList�����β���List Control��
	List L;
	GenerateList(L);
	ListToView(L, m_list_running, InsertData);


	return TRUE; // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CProcessManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CProcessManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

Status InsertData(ElemType e, CListCtrl &list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];
	int count = list_ctrl.GetItemCount();
	list_ctrl.InsertItem(count, NULL);
	list_ctrl.SetItemText(count, IDC_PROCESSID, helper.CharToWchar(ultoa(e.processID, buffer, 10)));
	list_ctrl.SetItemText(count, IDC_NAME, e.name);
	list_ctrl.SetItemText(count, IDC_STARTTIME, helper.FileTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(count, IDC_RUNNINGTIME, helper.FileTimeToRunningTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(count, IDC_MEMORY, helper.DoubleToWchar_M((double)e.memorySize/1024/1024));
	return OK;
}