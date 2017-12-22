#pragma once

#include "resource.h"
#include "define.h"
#include "MoveGenerator.h"
#include <stack>
#include <deque>

class CBoard;
class CGame
{
public:
	CGame(CBoard* pBoard);
	virtual ~CGame(void);
	void savedemo(CString filepath);
	void opendemo(CString filepath);
	virtual void SendStep(const CHESSSTEP &stepPut, TCHAR *szStep, UINT count) {}
public:
	// 棋盘类指针
	CBoard* m_pBoard;
	// 存储棋盘现有走过的步法堆栈
	std::stack<CHESSSTEP> m_ChessStep;
	// 存储回退过的步法堆栈
	std::stack<CHESSSTEP> m_ChessNext;
	// 棋谱栈
	std::deque<CString> m_QiPuStep;
	// 走法产生器
	CMoveGenerator* m_pTempMG;
public:
	virtual void init() {}
	virtual void Prev() {}
	virtual void Next() {}
	virtual void Demoend() {}
	virtual void Back() {}
	virtual void Giveup() {}
	virtual void Draw() {}
	virtual void CtrlComponentState() {}
};

