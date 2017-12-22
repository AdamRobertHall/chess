#include "StdAfx.h"
#include "GameOne.h"
#include "chessDlg.h"
#include <windows.h>


CGameOne::CGameOne(CBoard* pBoard):CGame(pBoard)
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	pDlg->GetDlgItem(IDC_PREV)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_NEXT)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_DEMOEND)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_BACK)->ShowWindow(SW_SHOW);
	pDlg->GetDlgItem(IDC_DRAW)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_GIVEUP)->ShowWindow(SW_SHOW);
	pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
}


CGameOne::~CGameOne(void)
{
	delete m_pSE;
}

void CGameOne::init()
{
	CMoveGenerator  *pMG;
	CEveluation	    *pEvel;
	m_pSE = new CNegaMaxEngine;
	m_pSE->SetSearchDepth(3);
	pEvel = new CEveluation;
	//创建走法产生器
	pMG = new CMoveGenerator;
	//将走法传声器传给搜索引擎
	m_pSE->SetMoveGenerator(pMG);
	//将估值核心传给搜索引擎
	m_pSE->SetEveluator(pEvel);
	m_pTempMG = pMG;
	AfxGetMainWnd()->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	AfxGetMainWnd()->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	AfxGetMainWnd()->GetMenu()->EnableMenuItem(ID_SAVEDEMO, MF_ENABLED);
	if (m_pBoard->m_bComputerFirst)
	{
		Sleep(500);
		//m_pSE->SearchAGoodMove(m_pBoard->m_ChessBoard);
		CHESSSTEP OneStep = 
		{
			R_CANON,
			NOCHESS,
			{1,7},
			{4,7}
		};
		CString qipu = ((CchessDlg*)AfxGetMainWnd())->m_Board.GenerateChessMethod(m_pBoard->m_current_side,R_CANON,1,7,4,7);	
		m_ChessStep.push(OneStep);
		m_pBoard->m_ChessBoard[7][1] = NOCHESS;
		m_pBoard->m_ChessBoard[7][4] = R_CANON;
		m_pBoard->m_px = 4 * GRILLELENGTH + BORDERWIDTH;
		m_pBoard->m_py = 7 * GRILLELENGTH + BORDERHEIGHT;
		PlaySound(LPCTSTR(IDR_WAVE_GO), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		((CchessDlg*)AfxGetMainWnd())->m_OutputInfo.InsertString(-1, qipu);
		((CchessDlg*)AfxGetMainWnd())->GetDlgItem(IDC_LIST_STEP)->UpdateWindow();
		if (CURRENT_RED == m_pBoard->m_current_side)
		{
			m_pBoard->m_current_side = CURRENT_BLACK;
		}
		else 
		{
			m_pBoard->m_current_side = CURRENT_RED;
		}
	}
}

void CGameOne::SendStep(const CHESSSTEP &stepPut, TCHAR *szStep, UINT count)
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	if (pDlg->m_Board.m_gameMode)
	{
		Sleep(500);
		m_pSE->SearchAGoodMove(m_pBoard->m_ChessBoard);
		pDlg->m_OutputInfo.InsertString(-1, m_pBoard->m_ChessStepMethod);
		int nSelect = pDlg->m_OutputInfo.GetCount() - 1;
		pDlg->m_OutputInfo.SetCurSel(nSelect);
		pDlg->m_OutputInfo.UpdateWindow();
		if (m_pBoard->IsGameOver(m_pBoard->m_ChessBoard, m_pBoard->m_current_side))
		{
			pDlg->MessageBox(_T("绝杀！"));
			if (CURRENT_RED == m_pBoard->m_current_side)
			{
				pDlg->MessageBox(_T("红方胜！"));
			}
			else
			{
				pDlg->MessageBox(_T("黑方胜！"));
			}
		}
		if (m_pBoard->IsGameOver(m_pBoard->m_ChessBoard))
		{
			m_pBoard->m_isOver = TRUE;
		}
		if (CURRENT_RED == m_pBoard->m_current_side)
		{
			m_pBoard->m_current_side = CURRENT_BLACK;
		}
		else 
		{
			m_pBoard->m_current_side = CURRENT_RED;
		}
	}
	pDlg->GetDlgItem(IDC_BACK)->EnableWindow(m_ChessStep.size() > 1);
	pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(m_ChessStep.size() > 0);
}

void CGameOne::Back()
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	if (m_ChessStep.size() <= 3)
	{
		AfxGetMainWnd()->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	}
	if (m_ChessStep.size() > 1)
	{
		for (int i = 0; i < 2; i++)
		{
			CHESSSTEP PopStep = m_ChessStep.top();
			m_ChessStep.pop();
			m_ChessNext.push(PopStep);
			m_pBoard->m_ChessBoard[PopStep.From.y][PopStep.From.x] = PopStep.FromID;
			m_pBoard->m_ChessBoard[PopStep.To.y][PopStep.To.x] = PopStep.ToID;					

			CString aStep;
			pDlg->m_OutputInfo.GetText(pDlg->m_OutputInfo.GetCount() - 1,aStep);
			pDlg->m_OutputInfo.DeleteString(pDlg->m_OutputInfo.GetCount() - 1);
			--m_pBoard->m_QipuCount;
		}
		m_pBoard->m_px = 0;
		m_pBoard->m_py = 0;
	}

	((CchessDlg*)AfxGetMainWnd())->m_Board.InvalidateRect(NULL,FALSE);
	((CchessDlg*)AfxGetMainWnd())->m_Board.UpdateWindow();
}

void CGameOne::Giveup()
{
	m_pBoard->m_isOver = TRUE;
	if(CURRENT_RED == m_pBoard->m_current_side)	//红将是否不在了
	{
		PlaySound(LPCTSTR(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		AfxGetMainWnd()->MessageBox(_T("黑方胜！"));	
	}
	else
	{
		PlaySound(LPCTSTR(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		AfxGetMainWnd()->MessageBox(_T("红方胜！"));	 
	}
}

void CGameOne::CtrlComponentState()
{
	if (m_ChessStep.size() < 2)
	{
		AfxGetMainWnd()->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
		AfxGetMainWnd()->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	}
	else
	{
		AfxGetMainWnd()->GetDlgItem(IDC_BACK)->EnableWindow(TRUE);
		AfxGetMainWnd()->GetDlgItem(IDC_GIVEUP)->EnableWindow(TRUE);
	}
}