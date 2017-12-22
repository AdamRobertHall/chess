// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chess.h"
#include "ClientDlg.h"
#include "chessDlg.h"
#include "Board.h"
#include "afxdialogex.h"


// CClientDlg 对话框

IMPLEMENT_DYNAMIC(CClientDlg, CDialogEx)

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{

}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_EN_UPDATE(IDC_EDIT_HOST, &CClientDlg::OnEnUpdateEditHost)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CClientDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_OUT, &CClientDlg::OnBnClickedBtnOut)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序


void CClientDlg::OnEnUpdateEditHost()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_EDIT_HOST, str);
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(!str.IsEmpty());
}


void CClientDlg::OnBnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strHost;
	// 获取主机名称
	GetDlgItemText(IDC_EDIT_HOST, strHost);
	// 初始化连接状态
	CBoard* pBoard = (CBoard*)AfxGetMainWnd()->GetDlgItem(IDD_BOARD);
	pBoard->m_bConnected = FALSE;
	// 设置控件生效状态
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HOST)->EnableWindow(FALSE);
	m_nTimer = 5;
	// 创建套接字并连接
	pBoard->m_conn.Create();
	pBoard->m_conn.Connect(strHost, 20000);
	SetTimer(1, 1000, NULL);
}


void CClientDlg::OnBnClickedBtnOut()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	OnCancel();
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBoard* pBoard = (CBoard*)AfxGetMainWnd()->GetDlgItem(IDD_BOARD);
	if (1 == nIDEvent)
	{
		if (pBoard->m_bConnected)
		{
			KillTimer(1);
			EndDialog(IDOK);
		}
		else if (0 == m_nTimer)
		{
			KillTimer(1);
			MessageBox(_T("连接对方失败，请检查主机名或IP地址是否正确，以及网络连接是否正常。"),
				_T("连接失败"), MB_ICONERROR);
			SetDlgItemText(IDC_ST_TIMER, _T(""));
			GetDlgItem(IDC_EDIT_HOST)->EnableWindow();
			SetDlgItemText(IDC_EDIT_HOST, _T(""));
			GetDlgItem(IDC_EDIT_HOST)->SetFocus();
		}
		else
		{
			CString str;
			str.Format(_T("正在连接...(%d)"), m_nTimer--);
			SetDlgItemText(IDC_ST_TIMER, str);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CClientDlg::OnOK()
{
	// 屏蔽回车消息
}
