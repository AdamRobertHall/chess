// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chess.h"
#include "ServerDlg.h"
#include "Board.h"
#include "chessDlg.h"
#include "afxdialogex.h"
#include "AtlConv.h"


// CServerDlg �Ի���

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


// CServerDlg ��Ϣ�������


void CServerDlg::OnBnClickedBtnListen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	SetDlgItemText(IDC_ST_STATUS, _T("״̬���ȴ�������Ҽ���..."));
	pDlg->m_Board.m_sock.Create(20000);
	pDlg->m_Board.m_sock.Listen();
	GetDlgItem(IDC_BTN_LISTEN)->EnableWindow(FALSE);
}


void CServerDlg::OnBnClickedBtnOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->m_Board.m_sock.Close();
	OnCancel();
}


BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetParent()->EnableWindow(FALSE);
	// ��ȡ��������IP��ַ
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CServerDlg::OnOK()
{
	// ���λس���Ϣ
}
