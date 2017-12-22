// SearchEngine.cpp: implementation of the CSearchEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "chessDlg.h"
#include "SearchEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSearchEngine::CSearchEngine()
{
	memset(CurPosition, 0, 90);
	m_nMaxDepth = 0;
}

CSearchEngine::~CSearchEngine()
{
	delete m_pMG;	//删除挂在搜索引擎上的走法产生器
	delete m_pEval;	//删除挂在搜索引擎上的估值核心
}

//根据传入的走法改变期盼
//move是要进行的走法
BYTE CSearchEngine::MakeMove(CHESSMOVE* move)
{
	move->ChessID = CurPosition[move->From.y][move->From.x];
	BYTE	nChessID;
	//取目标位置的棋子
	nChessID = CurPosition[move->To.y][move->To.x];
	//把棋子移动到目标位置上
	CurPosition[move->To.y][move->To.x] = 
	CurPosition[move->From.y][move->From.x];
	
	//将原位置清空
	CurPosition[move->From.y][move->From.x] = NOCHESS;

	return nChessID;	//返回被吃掉的棋子
}

//根据要传入的走法回复棋盘
//move是要恢复的走法
//nChessID是要原棋盘上move目标位置的棋子类型
void CSearchEngine::UnMakeMove(CHESSMOVE *move,BYTE nChessID)
{
	//将目标位置和棋子拷回原位
	CurPosition[move->From.y][move->From.x] = 
		CurPosition[move->To.y][move->To.x];
	//恢复目标位置的棋子
	CurPosition[move->To.y][move->To.x] = nChessID;
}

//用以检查给定局面游戏是否结束
//如果未结束,返回0,否则返回极大/极小值
int CSearchEngine::IsGameOver(BYTE position[10][9],int nDepth)
{
	int i,j;
	BOOL RedLive = FALSE,BlackLive = FALSE;
	//检查红方九宫是否有将帅
	for(i = 7;i < 10; i++)
	{
		for(j = 3; j < 6; j++)
		{
			if(position[i][j] == B_KING)
				BlackLive = TRUE;
			if(position[i][j] == R_KING)
				RedLive = TRUE;
		}
	}
	//检查黑方九宫是否有将帅
	for(i = 0; i < 3; i++)
	{
		for(j = 3; j < 6; j++)
		{
			if(position[i][j] == B_KING)
				BlackLive = TRUE;
			if(position[i][j] == R_KING)
				RedLive = TRUE;
		}
	}
	i = (m_nMaxDepth - nDepth + 1) % 2; //取当前是奇偶标志

	if(!RedLive)	//红将是否不在了
	{
		if(i)
			return 19990 + nDepth;	//奇数层返回极大值
		else
			return - 19990 - nDepth;	//偶数层返回极小值
	}
	if(!BlackLive)
	{
		if(i)
			return - 19990 - nDepth;	//奇数层返回极小值
		else
			return - 19990 + nDepth;	//偶数层返回极大值
	}

	return 0;//两个将都在,返回0
}
