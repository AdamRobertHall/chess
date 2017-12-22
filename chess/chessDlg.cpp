
// chessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chess.h"
#include "chessDlg.h"
#include "ServerDlg.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CchessDlg 对话框




CchessDlg::CchessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CchessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STEP, m_OutputInfo);
	DDX_Control(pDX, IDC_LIST_CHAT, m_ChatList);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_EditChat);
}

BEGIN_MESSAGE_MAP(CchessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUTGAME, &CchessDlg::OnAboutgame)
	ON_BN_CLICKED(IDC_BACK, &CchessDlg::OnBnClickedBack)
	ON_BN_CLICKED(IDC_DRAW, &CchessDlg::OnBnClickedDraw)
	ON_BN_CLICKED(IDC_GIVEUP, &CchessDlg::OnBnClickedGiveup)
	ON_BN_CLICKED(IDC_PREV, &CchessDlg::OnBnClickedPrev)
	ON_BN_CLICKED(IDC_NEXT, &CchessDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_DEMOEND, &CchessDlg::OnBnClickedDemoend)
	ON_COMMAND(ID_PLAYERFIRST, &CchessDlg::OnPlayerfirst)
	ON_COMMAND(ID_COMPUTERFIRST, &CchessDlg::OnComputerfirst)
	ON_COMMAND(ID_AGAIN, &CchessDlg::OnAgain)
	ON_COMMAND(ID_DISCONNECT, &CchessDlg::OnDisconnect)
	ON_COMMAND(ID_NETGAMEENTER, &CchessDlg::OnNetgameenter)
	ON_COMMAND(ID_NETGAMESETUP, &CchessDlg::OnNetgamesetup)
	ON_COMMAND(ID_OPENDEMO, &CchessDlg::OnOpendemo)
	ON_COMMAND(ID_QUIT, &CchessDlg::OnQuit)
	ON_COMMAND(ID_SAVEDEMO, &CchessDlg::OnSavedemo)
	ON_BN_CLICKED(IDC_BTN_SEND, &CchessDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CchessDlg 消息处理程序

BOOL CchessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Board.CreateEx(WS_EX_CLIENTEDGE, _T("ChessBoard"), NULL, WS_VISIBLE | WS_BORDER | WS_CHILD,
		CRect(0, 0, 376, 420), this, IDD_BOARD);
	m_pDlg = NULL;
	GetDlgItem(IDC_PREV)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DEMOEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BACK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DRAW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_GIVEUP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_CHAT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	GetMenu()->EnableMenuItem(ID_AGAIN, MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_SAVEDEMO, MF_GRAYED);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CchessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CchessDlg::OnPaint()
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
HCURSOR CchessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchessDlg::OnAboutgame()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}


void CchessDlg::OnBnClickedBack()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Board.m_pGame->Back();
}


void CchessDlg::OnBnClickedDraw()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Board.m_pGame->Draw();
}


void CchessDlg::OnBnClickedGiveup()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Board.m_pGame->Giveup();
}


void CchessDlg::OnBnClickedPrev()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Board.m_pGame->Prev();
	m_Board.InvalidateRect(NULL, FALSE);
	m_Board.UpdateWindow();
}


void CchessDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Board.m_pGame->Next();
	m_Board.InvalidateRect(NULL, FALSE);
	m_Board.UpdateWindow();
}


void CchessDlg::OnBnClickedDemoend()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Board.m_pGame->Demoend();
}


void CchessDlg::OnPlayerfirst()
{
	// TODO: 在此添加命令处理程序代码
	m_Board.m_bWait = FALSE;
	m_Board.m_current_side = CURRENT_RED;
	m_Board.m_bComputerFirst = FALSE;
	m_OutputInfo.ResetContent();
	m_Board.SetGameMode(GAMEONE);
	m_Board.InvalidateRect(NULL, FALSE);
	m_Board.UpdateWindow();
}

void CchessDlg::OnComputerfirst()
{
	// TODO: 在此添加命令处理程序代码
	m_Board.m_bWait = FALSE;
	m_Board.m_current_side = CURRENT_RED;
	m_Board.m_bComputerFirst = TRUE;
	m_OutputInfo.ResetContent();
	m_Board.SetGameMode(GAMEONE);
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
}

void CchessDlg::OnAgain()
{
	// TODO: 在此添加命令处理程序代码
	m_Board.PlayAgain();
}


void CchessDlg::OnDisconnect()
{
	// TODO: 在此添加命令处理程序代码
	m_Board.m_conn.Close();
	m_Board.m_sock.Close();
	GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
	GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	GetDlgItem(IDC_LIST_CHAT)->EnableWindow(FALSE);
	GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
	// 设置网络连接状态
	m_Board.m_bConnected = FALSE;
}


void CchessDlg::OnNetgameenter()
{
	// TODO: 在此添加命令处理程序代码
	CClientDlg client;
	client.DoModal();
}


void CchessDlg::OnNetgamesetup()
{
	// TODO: 在此添加命令处理程序代码
	m_pDlg = new CServerDlg;
	m_pDlg->Create(IDD_DLG_SERVER, this);
	m_pDlg->ShowWindow(SW_SHOW);
}


void CchessDlg::OnOpendemo()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;
	CString filter_str = _T("txt files (*.txt)|*.txt|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter_str, this);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	dlg.m_ofn.lpstrInitialDir = _T("..\\qipu");
	if(dlg.DoModal()==IDOK)
	{
		m_OutputInfo.ResetContent();
		m_Board.SetGameMode(GAMEDEMO);	
		FilePathName = dlg.GetPathName();
		m_Board.m_pGame->opendemo(FilePathName);
		m_Board.InvalidateRect(NULL, FALSE);
		m_Board.UpdateWindow();
		m_Board.SetActiveWindow();
	}
}


void CchessDlg::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}


void CchessDlg::OnSavedemo()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: Add your command handler code here
	CString filter_str = _T("txt files (*.txt)|*.txt|");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter_str, NULL);//初始化
	dlg.m_ofn.lpstrInitialDir = _T("..\\qipu");
	if(dlg.DoModal()==IDOK)
	{
		CString FilePathName = dlg.GetPathName();
		m_Board.m_pGame->savedemo(FilePathName);
	}
}

void CchessDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
}


void CchessDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	// 发送聊天内容
	CString cstr;
	GetDlgItemText(IDC_EDIT_CHAT, cstr);
	TCHAR* str = cstr.GetBuffer(cstr.GetLength());
	int len = cstr.GetLength() + 1;
	MSGSTRUCT msg;
	msg.uMsg = MSG_CHAT;
	_tcscpy_s(msg.szMsg, len, str);

	m_Board.m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
	// 加入聊天记录
	CString strAdd;
	strAdd.Format(_T("你 说：%s\r\n"), str);
	m_ChatList.InsertString(-1, strAdd);
	m_ChatList.UpdateWindow();
	int iChatListLen = m_ChatList.GetCount();
	// 清空聊天输入窗口
	cstr = _T("");
	SetDlgItemText(IDC_EDIT_CHAT, cstr);
}