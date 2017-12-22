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
//此函数指针对传入的position找出一步最佳算法
//并修改棋盘数据为走过的状态
BOOL CNegaMaxEngine::SearchAGoodMove(BYTE position[10][9])
{
	BOOL flag = FALSE;
	//设定搜索层数为 m_nSearchDepth
	m_nMaxDepth = m_nSearchDepth;
	//将传入的棋盘复制到成员变量
	memcpy(CurPosition,position,90);
	//调用负极大值搜索函数找最佳走法
	NegaMax(m_nMaxDepth);
	//将棋盘修改为走过
	BYTE nChessID = MakeMove(&m_cmBestMove);
	//将电脑走过的一步棋压栈
	CchessDlg *pDlg = (CchessDlg *)AfxGetMainWnd();
	CHESSSTEP OneStep;
	OneStep.From = m_cmBestMove.From;
	OneStep.To = m_cmBestMove.To;
	OneStep.FromID = m_cmBestMove.ChessID;
	OneStep.ToID = position[m_cmBestMove.To.y][m_cmBestMove.To.x];
	pDlg->m_Board.m_pGame->m_ChessStep.push(OneStep);
	pDlg->m_Board.m_px = m_cmBestMove.To.x * GRILLELENGTH + BORDERWIDTH;
	pDlg->m_Board.m_py = m_cmBestMove.To.y * GRILLELENGTH + BORDERHEIGHT;
	//将修改过的棋盘复制到传入的棋s盘,传出
	memcpy(position,CurPosition,90);
	if (nChessID == NOCHESS)
	{
		PlaySound(LPCTSTR(IDR_WAVE_GO), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	}
	else 
	{
		PlaySound(LPCTSTR(IDR_WAVE_EAT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	}
	
	//生成一招棋谱
	pDlg->m_Board.m_ChessStepMethod = pDlg->m_Board.GenerateChessMethod(
		((CchessDlg *)AfxGetMainWnd())->m_Board.m_current_side, m_cmBestMove.ChessID, 
		m_cmBestMove.From.x, m_cmBestMove.From.y, 
		m_cmBestMove.To.x, m_cmBestMove.To.y);
	return flag;
}

//负极大值搜索函数
//depth表示节点里叶子节点的层数
int CNegaMaxEngine::NegaMax(int depth)
{
	int current = -20000;
	int score;
	int Count,i;
	BYTE type;
	i = IsGameOver(CurPosition,depth);	//检查棋局是否结束
	if(i != 0)
		return i;	//棋局结束,返回极大/极小值
	if(depth <= 0)	//叶子节点取估值
		//返回估值
		return m_pEval->Eveluate(CurPosition, (m_nMaxDepth - depth + ((CchessDlg *)AfxGetMainWnd())->m_Board.m_current_side) % 2);//(m_nMaxDepth - depth) % 2
	//列举出当前局面下一步所有可能的走法
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, (m_nMaxDepth - depth + ((CchessDlg *)AfxGetMainWnd())->m_Board.m_current_side) % 2);
	for(i = 0; i < Count; i++)
	{
		//根据走法产生新局面
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);		
		//递归调用付极大值搜索下一层的节点
		score = -NegaMax(depth - 1);
		//恢复当前局面
		UnMakeMove(&m_pMG->m_MoveList[depth][i],type);
		if(score > current)	//如果score大于已知的最大值
		{
			current = score;	//修改当前最大值为score;
			if(depth == m_nMaxDepth)
			{
				//靠近根部时保存最佳走法
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
		}
	}
	return current;	//返回极大值
}
