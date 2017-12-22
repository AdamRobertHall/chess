// NegaMaxEngine.cpp: implementation of the CNegaMaxEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "ChessDlg.h"
#include "NegaMaxEngine.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegaMaxEngine::CNegaMaxEngine()
{

}

CNegaMaxEngine::~CNegaMaxEngine()
{

}
//�˺���ָ��Դ����position�ҳ�һ������㷨
//���޸���������Ϊ�߹���״̬
BOOL CNegaMaxEngine::SearchAGoodMove(BYTE position[10][9])
{
	BOOL flag = FALSE;
	//�趨��������Ϊ m_nSearchDepth
	m_nMaxDepth = m_nSearchDepth;
	//����������̸��Ƶ���Ա����
	memcpy(CurPosition,position,90);
	//���ø�����ֵ��������������߷�
	NegaMax(m_nMaxDepth);
	//�������޸�Ϊ�߹�
	BYTE nChessID = MakeMove(&m_cmBestMove);
	//�������߹���һ����ѹջ
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	CHESSSTEP OneStep;
	OneStep.From = m_cmBestMove.From;
	OneStep.To = m_cmBestMove.To;
	OneStep.FromID = m_cmBestMove.ChessID;
	OneStep.ToID = position[m_cmBestMove.To.y][m_cmBestMove.To.x];
	pDlg->m_Board.m_pGame->m_ChessStep.push(OneStep);
	pDlg->m_Board.m_px = m_cmBestMove.To.x * GRILLELENGTH + BORDERWIDTH;
	pDlg->m_Board.m_py = m_cmBestMove.To.y * GRILLELENGTH + BORDERHEIGHT;
	//���޸Ĺ������̸��Ƶ��������s��,����
	memcpy(position,CurPosition,90);
	if (nChessID == NOCHESS)
	{
		PlaySound(LPCTSTR(IDR_WAVE_GO), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	}
	else 
	{
		PlaySound(LPCTSTR(IDR_WAVE_EAT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	}
	
	//����һ������
	pDlg->m_Board.m_ChessStepMethod = pDlg->m_Board.GenerateChessMethod(
		((CchessDlg *)AfxGetMainWnd())->m_Board.m_current_side, m_cmBestMove.ChessID, 
		m_cmBestMove.From.x, m_cmBestMove.From.y, 
		m_cmBestMove.To.x, m_cmBestMove.To.y);
	return flag;
}

//������ֵ��������
//depth��ʾ�ڵ���Ҷ�ӽڵ�Ĳ���
int CNegaMaxEngine::NegaMax(int depth)
{
	int current = -20000;
	int score;
	int Count,i;
	BYTE type;
	i = IsGameOver(CurPosition,depth);	//�������Ƿ����
	if(i != 0)
		return i;	//��ֽ���,���ؼ���/��Сֵ
	if(depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
		//���ع�ֵ
		return m_pEval->Eveluate(CurPosition, (m_nMaxDepth - depth + ((CchessDlg *)AfxGetMainWnd())->m_Board.m_current_side) % 2);//(m_nMaxDepth - depth) % 2
	//�оٳ���ǰ������һ�����п��ܵ��߷�
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, (m_nMaxDepth - depth + ((CchessDlg *)AfxGetMainWnd())->m_Board.m_current_side) % 2);
	for(i = 0; i < Count; i++)
	{
		//�����߷������¾���
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);		
		//�ݹ���ø�����ֵ������һ��Ľڵ�
		score = -NegaMax(depth - 1);
		//�ָ���ǰ����
		UnMakeMove(&m_pMG->m_MoveList[depth][i],type);
		if(score > current)	//���score������֪�����ֵ
		{
			current = score;	//�޸ĵ�ǰ���ֵΪscore;
			if(depth == m_nMaxDepth)
			{
				//��������ʱ��������߷�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
		}
	}
	return current;	//���ؼ���ֵ
}
