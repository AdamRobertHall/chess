// ChessSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chess.h"
#include "chessDlg.h"
#include "ChessSocket.h"


// CChessSocket

CChessSocket::CChessSocket()
{
}

CChessSocket::~CChessSocket()
{
}


// CChessSocket ��Ա����


void CChessSocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->EnableWindow();
	delete [] pDlg->m_pDlg;
	pDlg->m_pDlg = NULL;
	pDlg->m_Board.m_sock.Accept(pDlg->m_Board.m_conn);
	pDlg->m_Board.m_bConnected = TRUE;
	pDlg->m_Board.m_bWait = FALSE;
	pDlg->m_Board.SetGameMode(GAMETWO);
}


void CChessSocket::OnConnect(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->m_Board.m_bConnected = TRUE;
	pDlg->m_Board.SetGameMode(GAMETWO);
}


void CChessSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->m_Board.Receive();
}


void CChessSocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->MessageBox(_T("�Է��Ѿ��뿪��Ϸ�������ٽ������١�"), _T("�й�����"), MB_ICONINFORMATION);
	// ����������Ŀ����ʹ�˵���Ч
	pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_EDIT_CHAT)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
}
