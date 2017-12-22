// ChessSocket.cpp : 实现文件
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


// CChessSocket 成员函数


void CChessSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加专用代码和/或调用基类
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->m_Board.m_bConnected = TRUE;
	pDlg->m_Board.SetGameMode(GAMETWO);
}


void CChessSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->m_Board.Receive();
}


void CChessSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	pDlg->MessageBox(_T("对方已经离开游戏，改日再较量不迟。"), _T("中国象棋"), MB_ICONINFORMATION);
	// 禁用所有项目，并使菜单生效
	pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_EDIT_CHAT)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
}
