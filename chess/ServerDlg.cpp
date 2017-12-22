// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chess.h"
#include "ServerDlg.h"
#include "Board.h"
#include "chessDlg.h"
#include "afxdialogex.h"
#include "AtlConv.h"


// CServerDlg 对话框

IMPLEMENT_DYNAMIC(CServerDlg, CDialogEx)

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
{

}

CServerDlg::~CServerDlg()
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LISTEN, &CServerDlg::OnBnClickedBtnListen)
	ON_BN_CLICKED(IDC_BTN_OUT, &CServerDlg::OnBnClickedBtnOut)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序


void CServerDlg::OnBnClickedBtnListen()
{
	// TODO: 在此添加控件通知处理程序代码
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	SetDlgItemText(IDC_ST_STATUS, _T("状态：等待其他玩家加入..."));
	pDlg->m_Board.m_sock.Create(20000);
	pDlg->m_Board.m_sock.Listen();
	GetDlgItem(IDC_BTN_LISTEN)->EnableWindow(FALSE);
}


void CServerDlg::OnBnClickedBtnOut()
{
	// TODO: 在此添加控件通知处理程序代码
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->m_Board.m_sock.Close();
	OnCancel();
}


BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetParent()->EnableWindow(FALSE);
	// 获取主机名及IP地址
	CHAR szHost[100];
	CHAR *szIP;
	hostent *host;

	gethostname(szHost,100);
	USES_CONVERSION;
	SetDlgItemText(IDC_EDIT_HOST, A2T(szHost));

	host = gethostbyname(szHost);
	for (int i = 0; host != NULL && host->h_addr_list[i] != NULL; i++)
	{
		szIP = inet_ntoa(*((in_addr *)host->h_addr_list[i]));
		break;
	}
	SetDlgItemText(IDC_EDIT_IP, A2T(szIP));
	GetDlgItem(IDC_BTN_LISTEN)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CServerDlg::OnOK()
{
	// 屏蔽回车消息
}
