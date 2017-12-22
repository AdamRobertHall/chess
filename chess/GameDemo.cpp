#include "StdAfx.h"
#include "GameDemo.h"
#include "chessDlg.h"


CGameDemo::CGameDemo(CBoard* pBoard):CGame(pBoard)
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	pDlg->GetDlgItem(IDC_PREV)->ShowWindow(SW_SHOW);
	pDlg->GetDlgItem(IDC_PREV)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_NEXT)->ShowWindow(SW_SHOW);
	pDlg->GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_DEMOEND)->ShowWindow(SW_SHOW);
	pDlg->GetDlgItem(IDC_DEMOEND)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BACK)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_DRAW)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_GIVEUP)->ShowWindow(SW_HIDE);
	pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
}


CGameDemo::~CGameDemo(void)
{
}

void CGameDemo::init()
{
}

void CGameDemo::Next()
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	if (m_ChessStep.size() <= 1)
	{
		pDlg->GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);
	}
	if(!m_ChessStep.empty())
	{
		CHESSSTEP PopStep = m_ChessStep.top();
		m_ChessStep.pop();
		m_ChessNext.push(PopStep);		
		m_pBoard->m_ChessBoard[PopStep.From.y][PopStep.From.x] = PopStep.FromID;
		m_pBoard->m_ChessBoard[PopStep.To.y][PopStep.To.x] = PopStep.ToID;
		
		CString aStep;
		int nSelect = pDlg->m_OutputInfo.GetCount() - 1;
		pDlg->m_OutputInfo.GetText(nSelect,aStep);
		pDlg->m_OutputInfo.DeleteString(nSelect);
		nSelect = pDlg->m_OutputInfo.GetCount() - 1;
		pDlg->m_OutputInfo.SetCurSel(nSelect);	
		m_QiPuStep.push_front(aStep);
		pDlg->GetDlgItem(IDC_PREV)->EnableWindow(TRUE);

	}
}

void CGameDemo::Prev()
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	if (m_ChessNext.size() <= 1)
	{
		pDlg->GetDlgItem(IDC_PREV)->EnableWindow(FALSE);
	}
	if (!m_ChessNext.empty())
	{
		CHESSSTEP PopStep = m_ChessNext.top();
		m_ChessNext.pop();
		m_ChessStep.push(PopStep);
		m_pBoard->m_ChessBoard[PopStep.From.y][PopStep.From.x] = NOCHESS;
		m_pBoard->m_ChessBoard[PopStep.To.y][PopStep.To.x] = PopStep.FromID;

		CString aStep = m_QiPuStep.front();
		m_QiPuStep.pop_front();
		pDlg->m_OutputInfo.InsertString(-1,aStep);
		int nSelect = pDlg->m_OutputInfo.GetCount() - 1;
		pDlg->m_OutputInfo.SetCurSel(nSelect);
		pDlg->GetDlgItem(IDC_NEXT)->EnableWindow(TRUE);
	}
}

void CGameDemo::Demoend()
{
	CDialog* pDlg = (CDialog*)AfxGetMainWnd();
	pDlg->GetDlgItem(IDC_PREV)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_DEMOEND)->EnableWindow(FALSE);
}
