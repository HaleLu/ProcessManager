// ProcessManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessManagerDlg.h"
#include "afxdialogex.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProcessManagerDlg 对话框


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


// CProcessManagerDlg 消息处理程序

BOOL CProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE); // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 创建List Control并初始化
	m_running_list_crtl.InsertColumn(IDC_RUNNING_PROCESSID, _T("PID"), LVCFMT_LEFT, 40, IDC_RUNNING_PROCESSID);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_NAME, _T("进程名称"), LVCFMT_LEFT, 180, IDC_RUNNING_NAME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_STARTTIME, _T("创建时间"), LVCFMT_LEFT, 120, IDC_RUNNING_STARTTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_RUNNINGTIME, _T("持续时间"), LVCFMT_LEFT, 80, IDC_RUNNING_RUNNINGTIME);
	m_running_list_crtl.InsertColumn(IDC_RUNNING_MEMORY, _T("内存"), LVCFMT_LEFT, 60, IDC_RUNNING_MEMORY);

	m_finished_list_crtl.InsertColumn(IDC_FINISHED_PROCESSID, _T("PID"), LVCFMT_LEFT, 40, IDC_FINISHED_PROCESSID);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_NAME, _T("进程名称"), LVCFMT_LEFT, 180, IDC_FINISHED_NAME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_RUNNINGTIME, _T("持续时间"), LVCFMT_LEFT, 80, IDC_FINISHED_RUNNINGTIME);
	m_finished_list_crtl.InsertColumn(IDC_FINISHED_FINISHEDTIME, _T("结束时间"), LVCFMT_LEFT, 120, IDC_FINISHED_FINISHEDTIME);

	//获取List，依次插入List Control中
	InitList(m_running_list);
	InitList(m_finished_list);
	CreateRunningList(m_running_list);
	ListToView(m_running_list, m_running_list_crtl, InsertRunningData);
	ListToView(m_finished_list, m_finished_list_crtl, InsertFinishedData);

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

Status InsertFinishedData(ElemType e, CListCtrl& list_ctrl)
{
	Helper helper;
	char buffer[MAX_PATH];
	int count = list_ctrl.GetItemCount();

	SYSTEMTIME cLocalTime;
	GetLocalTime(&cLocalTime);

	list_ctrl.InsertItem(count, NULL);
	list_ctrl.SetItemText(count, IDC_FINISHED_PROCESSID, helper.CharToWchar(ultoa(e.processID, buffer, 10)));
	list_ctrl.SetItemText(count, IDC_FINISHED_NAME, e.name);
	list_ctrl.SetItemText(count, IDC_FINISHED_RUNNINGTIME, helper.FileTimeToRunningTimeToWChar(e.creationTime));
	list_ctrl.SetItemText(count, IDC_FINISHED_FINISHEDTIME, helper.SystemTimeToWChar(cLocalTime));
	return OK ;
}

void CProcessManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	ReFresh();
	CDialogEx::OnTimer(nIDEvent);
}

void CProcessManagerDlg::ReFresh()
{
	RefreshList(m_running_list, m_finished_list);

	m_running_list_crtl.DeleteAllItems();
	m_finished_list_crtl.DeleteAllItems();
	ListToView(m_running_list, m_running_list_crtl, InsertRunningData);
	ListToView(m_finished_list, m_finished_list_crtl, InsertFinishedData);
	
}