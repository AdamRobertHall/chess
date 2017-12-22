// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chess.h"
#include "ClientDlg.h"
#include "chessDlg.h"
#include "Board.h"
#include "afxdialogex.h"


// CClientDlg �Ի���

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


// CClientDlg ��Ϣ�������


void CClientDlg::OnEnUpdateEditHost()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItemText(IDC_EDIT_HOST, str);
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(!str.IsEmpty());
}


void CClientDlg::OnBnClickedBtnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strHost;
	// ��ȡ��������
	GetDlgItemText(IDC_EDIT_HOST, strHost);
	// ��ʼ������״̬
	CBoard* pBoard = (CBoard*)AfxGetMainWnd()->GetDlgItem(IDD_BOARD);
	pBoard->m_bConnected = FALSE;
	// ���ÿؼ���Ч״̬
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HOST)->EnableWindow(FALSE);
	m_nTimer = 5;
	// �����׽��ֲ�����
	pBoard->m_conn.Create();
	pBoard->m_conn.Connect(strHost, 20000);
	SetTimer(1, 1000, NULL);
}


void CClientDlg::OnBnClickedBtnOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
	OnCancel();
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			MessageBox(_T("���ӶԷ�ʧ�ܣ�������������IP��ַ�Ƿ���ȷ���Լ����������Ƿ�������"),
				_T("����ʧ��"), MB_ICONERROR);
			SetDlgItemText(IDC_ST_TIMER, _T(""));
			GetDlgItem(IDC_EDIT_HOST)->EnableWindow();
			SetDlgItemText(IDC_EDIT_HOST, _T(""));
			GetDlgItem(IDC_EDIT_HOST)->SetFocus();
		}
		else
		{
			CString str;
			str.Format(_T("��������...(%d)"), m_nTimer--);
			SetDlgItemText(IDC_ST_TIMER, str);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CClientDlg::OnOK()
{
	// ���λس���Ϣ
}
