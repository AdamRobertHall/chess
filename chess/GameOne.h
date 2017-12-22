#pragma once
#include "game.h"
#include "Board.h"
#include "NegaMaxEngine.h"
class CGameOne :
	public CGame
{
public:
	CGameOne(CBoard* pBoard);
	virtual ~CGameOne(void);
	
	void SendStep(const CHESSSTEP &stepPut, TCHAR *szStep, UINT count);
	void init();
	void Back();
	void Giveup();
	void CtrlComponentState();
private:
	// ËÑË÷ÒýÇæÖ¸Õë
	CSearchEngine* m_pSE;
};

