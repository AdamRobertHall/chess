#include "stdafx.h"
#include "GameTwo.h"
#include "Board.h"
#include "chessDlg.h"

CGameTwo::CGameTwo(CBoard* pBoard):CGame(pBoard)
{
	//创建走法产生器
	m_pTempMG = new CMoveGenerator;
	m_pBoard->m_current_side = CURRENT_RED;
}


CGameTwo::~CGameTwo(void)
{
	delete m_pTempMG;
}

void CGameTwo::init()
{
	CDialog* pDlg = (CDialog*)AfxGetMainWnd();
	pDlg->GetDlgItem(IDC_PREV)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_NEXT)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_DEMOEND)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_BACK)->ShowWindow(SW_SHOW);
	pDlg->GetDlgItem(IDC_DRAW)->ShowWindow(SW_SHOW);
	pDlg->GetDlgItem(IDC_GIVEUP)->ShowWindow(SW_SHOW);
	pDlg->GetDlgItem(IDC_EDIT_CHAT)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(TRUE);
	pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_GRAYED);
	pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_GRAYED);
	pDlg->GetMenu()->EnableMenuItem(ID_SAVEDEMO, MF_ENABLED);
	pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_GRAYED);
}

void CGameTwo::SendStep(const CHESSSTEP &stepPut, TCHAR *szStep, UINT count)
{
	MSGSTRUCT msg;
	msg.uMsg = MSG_PUTSTEP;
	msg.oneStep = stepPut;
	_tcscpy_s(msg.szStep, 100, szStep);
	msg.nMethodCount = count;
	m_pBoard->m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
	AfxGetMainWnd()->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	m_pBoard->m_bWait = TRUE;
}

void CGameTwo::Draw()
{
	// 设置等待标志
	m_pBoard->m_bWait = TRUE;
	MSGSTRUCT msg;
	msg.uMsg = MSG_DRAW;
	m_pBoard->m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
}

void CGameTwo::Back()
{
	MSGSTRUCT msg;
	msg.uMsg = MSG_BACK;
	m_pBoard->m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
}

void CGameTwo::Giveup()
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	// 发送认输消息
	if (IDYES == pDlg->MessageBox(_T("你真的要认输吗？"), _T("认输"), MB_YESNO))
	{
		m_pBoard->m_isOver = TRUE;
		MSGSTRUCT msg;
		msg.uMsg = MSG_GIVEUP;
		m_pBoard->m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
		pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
		pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
		pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
		pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
	}
}
