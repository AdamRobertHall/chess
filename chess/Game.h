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
	// ������ָ��
	CBoard* m_pBoard;
	// �洢���������߹��Ĳ�����ջ
	std::stack<CHESSSTEP> m_ChessStep;
	// �洢���˹��Ĳ�����ջ
	std::stack<CHESSSTEP> m_ChessNext;
	// ����ջ
	std::deque<CString> m_QiPuStep;
	// �߷�������
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

