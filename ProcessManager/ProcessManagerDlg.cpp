// ProcessManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessManagerDlg.h"
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
	DDX_Control(pDX, IDC_LIST_RUNNING, m_running_list_crtl);
	DDX_Control(pDX, IDC_LIST_FINISHED, m_finished_list_crtl);
}

BEGIN_MESSAGE_MAP(CProcessManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
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
	// ����List Control����ʼ��
	m_running_list_crtl.InsertColumn(IDC_RUNNING_PROCESSID, _T("PID"), LVCFMT_LEFT, 40, IDC_RUNNING_PROCESSID);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_NAME, _T("��������"), LVCFMT_LEFT, 180, IDC_RUNNING_NAME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_STARTTIME, _T("����ʱ��"), LVCFMT_LEFT, 120, IDC_RUNNING_STARTTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_RUNNINGTIME, _T("����ʱ��"), LVCFMT_LEFT, 80, IDC_RUNNING_RUNNINGTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_MEMORY, _T("�ڴ�"), LVCFMT_LEFT, 60, IDC_RUNNING_MEMORY);

	m_finished_list_crtl.InsertColumn(IDC_FINISHED_PROCESSID, _T("PID"), LVCFMT_LEFT, 40, IDC_FINISHED_PROCESSID);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_NAME, _T("��������"), LVCFMT_LEFT, 180, IDC_FINISHED_NAME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_RUNNINGTIME, _T("����ʱ��"), LVCFMT_LEFT, 80, IDC_FINISHED_RUNNINGTIME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_STARTTIME, _T("����ʱ��"), LVCFMT_LEFT, 120, IDC_RUNNING_STARTTIME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_FINISHEDTIME, _T("����ʱ��"), LVCFMT_LEFT, 120, IDC_FINISHED_FINISHEDTIME);

	//��ȡList�����β���List Control��
	InitList(m_running_list);
	InitList(m_finished_list);
	CreateRunningList(m_running_list);
	ListToView(m_running_list, m_running_list_crtl, InsertRunningData);
	ListToView(m_finished_list, m_finished_list_crtl, InsertFinishedData);

	SetTimer(1, 1000, NULL);

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

Status InsertRunningData(ElemType e, CListCtrl& list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];
	int count = list_ctrl.GetItemCount();
	list_ctrl.InsertItem(count, NULL);
	list_ctrl.SetItemText(count, IDC_RUNNING_PROCESSID, helper.CharToWchar(ultoa(e.processID, buffer, 10)));
	list_ctrl.SetItemText(count, IDC_RUNNING_NAME, e.name);
	list_ctrl.SetItemText(count, IDC_RUNNING_STARTTIME, helper.FileTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(count, IDC_RUNNING_RUNNINGTIME, helper.FileTimeToRunningTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(count, IDC_RUNNING_MEMORY, helper.DoubleToWchar_M((double)e.memorySize / 1024 / 1024));
	return OK ;
}

int InsertRunningData(ElemType e, int pos, CListCtrl& list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];
	list_ctrl.InsertItem(pos, NULL);
	list_ctrl.SetItemText(pos, IDC_RUNNING_PROCESSID, helper.CharToWchar(ultoa(e.processID, buffer, 10)));
	list_ctrl.SetItemText(pos, IDC_RUNNING_NAME, e.name);
	list_ctrl.SetItemText(pos, IDC_RUNNING_STARTTIME, helper.FileTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(pos, IDC_RUNNING_RUNNINGTIME, helper.FileTimeToRunningTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(pos, IDC_RUNNING_MEMORY, helper.DoubleToWchar_M((double)e.memorySize / 1024 / 1024));
	return OK ;
}

Status InsertFinishedData(ElemType e, CListCtrl& list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];

	SYSTEMTIME cLocalTime;
	GetLocalTime(&cLocalTime);

	list_ctrl.InsertItem(0, NULL);
	list_ctrl.SetItemText(0, IDC_FINISHED_PROCESSID, helper.CharToWchar(ultoa(e.processID, buffer, 10)));
	list_ctrl.SetItemText(0, IDC_FINISHED_NAME, e.name);
	list_ctrl.SetItemText(0, IDC_FINISHED_STARTTIME, helper.FileTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(0, IDC_FINISHED_RUNNINGTIME, helper.FileTimeToRunningTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(0, IDC_FINISHED_FINISHEDTIME, helper.SystemTimeToWChar(cLocalTime));
	return OK ;
}

void CProcessManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	Refresh();
	CDialogEx::OnTimer(nIDEvent);
}

void CProcessManagerDlg::Refresh()
{
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;
	ElemType tmp_e;

	//�ƶ�running_list������Ľ�����finished_list����running_list���ڴ���������
	List p = m_running_list->next;
	List q, tmp;
	unsigned int i = 0, j;
	while (p != NULL)
	{
		Status res = SaveProcess(p->data.processID, tmp_e);
		if (res == OPEN_PROCESS_FAILED || res == PROCESS_HAS_FINISHED || LocateElem(m_running_list, tmp_e, cmpIfSame) == 0)
		{
			q = p;
			p = p->next;
			ListMoveNode(m_running_list, m_finished_list, q, ID_FINISH);
			m_running_list_crtl.DeleteItem(i);//??
			InsertFinishedData(q->data, m_finished_list_crtl);
		}
		else
		{
			tmp = p->next;
			q = p->pre;
			j = i;
			p->data.memorySize = tmp_e.memorySize;
			while (q != m_running_list && p->data.memorySize > q->data.memorySize)
			{
				q = q->pre;
				j--;
			}
			
			if (q != p->pre)
			{
				p->pre->next = p->next;
				if (p->next) p->next->pre = p->pre;
				m_running_list_crtl.DeleteItem(i);//??

				p->pre = q;
				p->next = q->next;
				q->next = p;
				p->next->pre = p;
				InsertRunningData(p->data, j, m_running_list_crtl);
			} 
			else
			{
				m_running_list_crtl.SetItemText(i, IDC_RUNNING_RUNNINGTIME, helper.FileTimeToRunningTimeToWChar(p->data.creationTime));
				m_running_list_crtl.SetItemText(i, IDC_RUNNING_MEMORY, helper.DoubleToWchar_M((double)p->data.memorySize / 1024 / 1024));
			}
			
			p = tmp;
			i++;
		}
		CString x = m_running_list_crtl.GetItemText(i, 0);
	}

	//�ȶԵ�ǰ���еĽ��̣���������Insert��running_list
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return;
	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < cProcesses; i++)
	{
		if (SaveProcess(aProcesses[i], tmp_e) == OK)
		{
			if (LocateElem(m_running_list, tmp_e, cmpIfSame) == 0)
			{
				InsertRunningData(tmp_e, ListInsert(m_running_list, tmp_e, cmpMemory), m_running_list_crtl);
			}
		}
	}
}
