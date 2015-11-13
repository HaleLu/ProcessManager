// ProcessManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProcessManagerDlg 对话框


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


// CProcessManagerDlg 消息处理程序

BOOL CProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE); // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// 创建List Control并初始化
	m_running_list_crtl.InsertColumn(IDC_RUNNING_NAME, _T("进程名称"), LVCFMT_LEFT, 180, IDC_RUNNING_NAME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_STARTTIME, _T("最近的打开时间"), LVCFMT_LEFT, 120, IDC_RUNNING_STARTTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_RUNNINGTIME, _T("总持续时间"), LVCFMT_LEFT, 80, IDC_RUNNING_RUNNINGTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_MEMORY, _T("占用内存"), LVCFMT_LEFT, 60, IDC_RUNNING_MEMORY);

	m_finished_list_crtl.InsertColumn(IDC_FINISHED_NAME, _T("进程名称"), LVCFMT_LEFT, 180, IDC_FINISHED_NAME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_RUNNINGTIME, _T("总持续时间"), LVCFMT_LEFT, 80, IDC_FINISHED_RUNNINGTIME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_STARTTIME, _T("最近的创建时间"), LVCFMT_LEFT, 120, IDC_RUNNING_STARTTIME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_FINISHEDTIME, _T("最近的结束时间"), LVCFMT_LEFT, 120, IDC_FINISHED_FINISHEDTIME);

	//获取List，依次插入List Control中
	InitList_Du(m_running_list);
	InitList_L(m_finished_list);
	CreateRunningList_Du(m_running_list);
	m_list_helper.ListToView(m_running_list, m_running_list_crtl, m_list_helper.InsertRunningData);
	m_list_helper.ListToView(m_finished_list, m_finished_list_crtl, m_list_helper.InsertFinishedData);

	SetTimer(1, 1000, NULL);

	return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProcessManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

	//初始化flag标记
	p = m_running_list->next;
	while (p)
	{
		p->flag = false;
		p = p->next;
	}

	//比对当前运行的进程，新增进程与finished_list比对，若无则Insert进running_list，若有则更新后移动进running_list
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
				//此进程是新建的

				int pos_finished;
				LinkList finished_node = LocateElem_L(m_finished_list, tmp_e, cmpName, &pos_finished);
				if (finished_node == NULL)
				{
					//此进程是第一次被运行，加入running_list并更新界面

					m_list_helper.InsertRunningData(tmp_e, SortedListInsert_Du(m_running_list, tmp_e, cmpMemory), m_running_list_crtl);
				}
				else
				{
					//此进程已运行过，从finished_list中移出至running_list

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
				//此进程正在运行

				if (running_node->flag)
				{
					//此进程已更新过

					running_node->data.memorySize += tmp_e.memorySize;
					if (CompareFileTime(&running_node->data.creationTime, &tmp_e.creationTime) < 0)
					{
						memcpy(&running_node->data.creationTime, &tmp_e.creationTime, sizeof(FILETIME));
					}
				}
				else
				{
					//此进程未被更新

					running_node->flag = true;
					running_node->data.memorySize = tmp_e.memorySize;
					running_node->data.processID = tmp_e.processID;
				}

				//冒泡排序并更新界面

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
			//进程已结束，移动至finished_list并更新界面
			
			LinkList to = (LinkList)ListMoveNode_Du(p, m_finished_list, ID_FINISH, &tmp);
			m_running_list_crtl.DeleteItem(i);
			m_list_helper.InsertFinishedData(to->data, m_finished_list_crtl);
		}
		p = q;
		i++;
	}

}
