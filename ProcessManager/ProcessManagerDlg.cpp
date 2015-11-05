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
	m_list_running.InsertColumn(0, _T("PID"), LVCFMT_CENTER, rect.Width() / 5, IDC_PROCESSID);
	m_list_running.InsertColumn(1, _T("��������"), LVCFMT_CENTER, rect.Width()*2 / 5, IDC_NAME);
	m_list_running.InsertColumn(2, _T("����ʱ��"), LVCFMT_CENTER, rect.Width() / 5, IDC_TIME);
	m_list_running.InsertColumn(3, _T("�ڴ�"), LVCFMT_CENTER, rect.Width() / 5, IDC_MEMORY);
	
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
	ultoa(e.processID, buffer, 10);
	list_ctrl.InsertItem(0, helper.CharToWchar(buffer));
	return OK;
}