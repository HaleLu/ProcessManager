// ProcessManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProcessManagerDlg �Ի���


CProcessManagerDlg::CProcessManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CProcessManagerDlg::~CProcessManagerDlg()
{
	DestroyList_Du(m_running_list);
	DestroyList_L(m_finished_list);
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

	// ����List Control����ʼ��
	m_running_list_crtl.InsertColumn(IDC_RUNNING_NAME, _T("��������"), LVCFMT_LEFT, 180, IDC_RUNNING_NAME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_STARTTIME, _T("����Ĵ�ʱ��"), LVCFMT_LEFT, 120, IDC_RUNNING_STARTTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_RUNNINGTIME, _T("�ܳ���ʱ��"), LVCFMT_LEFT, 80, IDC_RUNNING_RUNNINGTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_MEMORY, _T("ռ���ڴ�"), LVCFMT_LEFT, 60, IDC_RUNNING_MEMORY);

	m_finished_list_crtl.InsertColumn(IDC_FINISHED_NAME, _T("��������"), LVCFMT_LEFT, 180, IDC_FINISHED_NAME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_RUNNINGTIME, _T("�ܳ���ʱ��"), LVCFMT_LEFT, 80, IDC_FINISHED_RUNNINGTIME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_STARTTIME, _T("����Ĵ���ʱ��"), LVCFMT_LEFT, 120, IDC_RUNNING_STARTTIME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_FINISHEDTIME, _T("����Ľ���ʱ��"), LVCFMT_LEFT, 120, IDC_FINISHED_FINISHEDTIME);

	//��ȡList�����β���List Control��
	InitList_Du(m_running_list);
	InitList_L(m_finished_list);
	CreateRunningList_Du(m_running_list);
	m_list_helper.ListToView(m_running_list, m_running_list_crtl, m_list_helper.InsertRunningData);
	m_list_helper.ListToView(m_finished_list, m_finished_list_crtl, m_list_helper.InsertFinishedData);

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
	DuLinkList p;

	//��ʼ��flag���
	p = m_running_list->next;
	while (p)
	{
		p->flag = false;
		p = p->next;
	}

	//�ȶԵ�ǰ���еĽ��̣�����������finished_list�ȶԣ�������Insert��running_list����������º��ƶ���running_list
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return;
	cProcesses = cbNeeded / sizeof(DWORD);

	for (int i = 0; i < cProcesses; i++)
	{
		if (SaveProcess(aProcesses[i], tmp_e) == OK)
		{
			int pos_running;
			DuLinkList running_node = LocateElem_Du(m_running_list, tmp_e, cmpName, &pos_running);
			if (running_node == NULL)
			{	
				//�˽������½���

				int pos_finished;
				LinkList finished_node = LocateElem_L(m_finished_list, tmp_e, cmpName, &pos_finished);
				if (finished_node == NULL)
				{
					//�˽����ǵ�һ�α����У�����running_list�����½���

					m_list_helper.InsertRunningData(tmp_e, SortedListInsert_Du(m_running_list, tmp_e, cmpMemory), m_running_list_crtl);
				}
				else
				{
					//�˽��������й�����finished_list���Ƴ���running_list

					finished_node->next->data.memorySize = tmp_e.memorySize;
					finished_node->next->data.creationTime = tmp_e.creationTime;

					int pos;
					DuLinkList node = (DuLinkList)ListMoveNode_Du(m_running_list, finished_node, ID_RESTART, &pos);
					m_list_helper.InsertRunningData(node->data, pos, m_running_list_crtl);
					m_finished_list_crtl.DeleteItem(pos_finished);
				}
			}
			else
			{
				//�˽�����������

				if (running_node->flag)
				{
					//�˽����Ѹ��¹�

					running_node->data.memorySize += tmp_e.memorySize;
					if (CompareFileTime(&running_node->data.creationTime, &tmp_e.creationTime) < 0)
					{
						memcpy(&running_node->data.creationTime, &tmp_e.creationTime, sizeof(FILETIME));
					}
				}
				else
				{
					//�˽���δ������

					running_node->flag = true;
					running_node->data.memorySize = tmp_e.memorySize;
					running_node->data.processID = tmp_e.processID;
				}

				//ð�����򲢸��½���

				DuLinkList q = running_node->pre;
				int to = pos_running;
				while (q != m_running_list && running_node->data.memorySize > q->data.memorySize)
				{
					q = q->pre;
					to--;
				}

				if (q != running_node->pre)
				{
					running_node->pre->next = running_node->next;
					if (running_node->next) running_node->next->pre = running_node->pre;
					m_running_list_crtl.DeleteItem(pos_running);

					running_node->pre = q;
					running_node->next = q->next;
					q->next = running_node;
					running_node->next->pre = running_node;
					m_list_helper.InsertRunningData(running_node->data, to, m_running_list_crtl);
				}
				else
				{
					m_running_list_crtl.SetItemText(pos_running, IDC_RUNNING_STARTTIME, m_helper.FileTimeToWChar(running_node->data.creationTime));
					m_running_list_crtl.SetItemText(pos_running, IDC_RUNNING_RUNNINGTIME, m_helper.FileTimeAddToRunningTimeToWChar(running_node->data.creationTime, running_node->data.runningTime));
					m_running_list_crtl.SetItemText(pos_running, IDC_RUNNING_MEMORY, m_helper.DoubleToWchar_M((double)running_node->data.memorySize / 1024 / 1024));
				}
				
			}
		}
	}

	DuLinkList q;
	int tmp;
	int i = 0;
	p = m_running_list->next;
	while (p)
	{
		q = p->next;
		if (!p->flag)
		{
			//�����ѽ������ƶ���finished_list�����½���
			
			LinkList to = (LinkList)ListMoveNode_Du(p, m_finished_list, ID_FINISH, &tmp);
			m_running_list_crtl.DeleteItem(i);
			m_list_helper.InsertFinishedData(to->data, m_finished_list_crtl);
		}
		p = q;
		i++;
	}

}
