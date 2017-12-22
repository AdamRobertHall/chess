
// chessDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CchessDlg �Ի���




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


// CchessDlg ��Ϣ�������

BOOL CchessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CchessDlg::OnPaint()
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
HCURSOR CchessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchessDlg::OnAboutgame()
{
	// TODO: �ڴ���������������
	CAboutDlg dlg;
	dlg.DoModal();
}


void CchessDlg::OnBnClickedBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Board.m_pGame->Back();
}


void CchessDlg::OnBnClickedDraw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Board.m_pGame->Draw();
}


void CchessDlg::OnBnClickedGiveup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Board.m_pGame->Giveup();
}


void CchessDlg::OnBnClickedPrev()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Board.m_pGame->Prev();
	m_Board.InvalidateRect(NULL, FALSE);
	m_Board.UpdateWindow();
}


void CchessDlg::OnBnClickedNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Board.m_pGame->Next();
	m_Board.InvalidateRect(NULL, FALSE);
	m_Board.UpdateWindow();
}


void CchessDlg::OnBnClickedDemoend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Board.m_pGame->Demoend();
}


void CchessDlg::OnPlayerfirst()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	m_Board.PlayAgain();
}


void CchessDlg::OnDisconnect()
{
	// TODO: �ڴ���������������
	m_Board.m_conn.Close();
	m_Board.m_sock.Close();
	GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
	GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	GetDlgItem(IDC_LIST_CHAT)->EnableWindow(FALSE);
	GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
	// ������������״̬
	m_Board.m_bConnected = FALSE;
}


void CchessDlg::OnNetgameenter()
{
	// TODO: �ڴ���������������
	CClientDlg client;
	client.DoModal();
}


void CchessDlg::OnNetgamesetup()
{
	// TODO: �ڴ���������������
	m_pDlg = new CServerDlg;
	m_pDlg->Create(IDD_DLG_SERVER, this);
	m_pDlg->ShowWindow(SW_SHOW);
}


void CchessDlg::OnOpendemo()
{
	// TODO: �ڴ���������������
	CString FilePathName;
	CString filter_str = _T("txt files (*.txt)|*.txt|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter_str, this);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
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
	// TODO: �ڴ���������������
	OnCancel();
}


void CchessDlg::OnSavedemo()
{
	// TODO: �ڴ���������������
	// TODO: Add your command handler code here
	CString filter_str = _T("txt files (*.txt)|*.txt|");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter_str, NULL);//��ʼ��
	dlg.m_ofn.lpstrInitialDir = _T("..\\qipu");
	if(dlg.DoModal()==IDOK)
	{
		CString FilePathName = dlg.GetPathName();
		m_Board.m_pGame->savedemo(FilePathName);
	}
}

void CchessDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
}


void CchessDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ������������
	CString cstr;
	GetDlgItemText(IDC_EDIT_CHAT, cstr);
	TCHAR* str = cstr.GetBuffer(cstr.GetLength());
	int len = cstr.GetLength() + 1;
	MSGSTRUCT msg;
	msg.uMsg = MSG_CHAT;
	_tcscpy_s(msg.szMsg, len, str);

	m_Board.m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
	// ���������¼
	CString strAdd;
	strAdd.Format(_T("�� ˵��%s\r\n"), str);
	m_ChatList.InsertString(-1, strAdd);
	m_ChatList.UpdateWindow();
	int iChatListLen = m_ChatList.GetCount();
	// ����������봰��
	cstr = _T("");
	SetDlgItemText(IDC_EDIT_CHAT, cstr);
}