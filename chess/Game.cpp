#include "StdAfx.h"
#include "Game.h"
#include "chessDlg.h"
#include <mmsystem.h>
#include <fstream>
#include <vector>
#include <deque>

CGame::CGame(CBoard* pBoard):m_pBoard(pBoard)
{
	if(NULL != m_pBoard->m_pGame)
	{
		delete m_pBoard->m_pGame;
	}
	PlaySound(LPCTSTR(IDR_WAVE_BEGIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	memcpy(m_pBoard->m_ChessBoard, InitChessBoard, 90);
	//清空着法列表
	((CchessDlg*)AfxGetMainWnd())->m_OutputInfo.ResetContent();
	((CchessDlg*)AfxGetMainWnd())->m_ChatList.ResetContent();
	m_pBoard->m_QipuCount = 1;
	m_pBoard->m_px = 0;
	m_pBoard->m_py = 0;
	m_pBoard->m_isOver = FALSE;
	while (!m_ChessNext.empty())
	{
		m_ChessNext.pop();
	}
	while (!m_ChessStep.empty())
	{
		m_ChessStep.pop();
	}
	m_QiPuStep.clear();
	m_pBoard->m_pGame = this;
}

CGame::~CGame(void)
{
}

void CGame::savedemo(CString filepath)
{
	std::ofstream ofs;
	ofs.open(filepath.GetBuffer(filepath.GetLength()));
	std::stack<CHESSSTEP> tempSteps(m_ChessStep);
	while (!tempSteps.empty())
	{
		CHESSSTEP aStep = tempSteps.top();
		tempSteps.pop();
		ofs << (int)aStep.FromID << " " << (int)aStep.From.x << " " << (int)aStep.From.y << " "
			<< (int)aStep.ToID << " " << (int)aStep.To.x << " " << (int)aStep.To.y << std::endl;
	}
	ofs.close();
}

void CGame::opendemo(CString filepath)
{
	std::ifstream ifs;
	ifs.open(filepath.GetBuffer(filepath.GetLength()));
	std::vector<CHESSSTEP> vec;
	CHESSSTEP aStep;
	int m_current_side;
	while (TRUE)
	{
		int FromID, Fromx, Fromy, ToID, Tox, Toy;
		ifs >> FromID >> Fromx >> Fromy
			>> ToID >> Tox >> Toy;
		if (ifs.eof())
		{
			break;
		}
		aStep.FromID = FromID;
		aStep.From.x = Fromx;
		aStep.From.y = Fromy;
		aStep.ToID = ToID;
		aStep.To.x = Tox;
		aStep.To.y = Toy;
		vec.push_back(aStep);
	}
	ifs.close();

	for (size_t sz = 0; sz < vec.size(); ++sz)
	{	
		aStep = vec[sz];	
		m_ChessNext.push(aStep);
	}
	for (int sz = vec.size() - 1; sz >= 0; --sz)
	{
		aStep = vec[sz];
		if (IsRed(aStep.FromID))
		{
			m_current_side = CURRENT_RED;
		}
		else
		{
			m_current_side = CURRENT_BLACK;
		}
		CString aQipu = m_pBoard->GenerateChessMethod(
			m_current_side, aStep.FromID, 
			aStep.From.x, aStep.From.y, 
			aStep.To.x, aStep.To.y);
		m_QiPuStep.push_back(aQipu);
	}
}