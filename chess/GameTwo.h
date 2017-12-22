#pragma once
#include "game.h"
class CGameTwo :
	public CGame
{
public:
	CGameTwo(CBoard* pBoard);
	virtual ~CGameTwo(void);
	void init();
	void SendStep(const CHESSSTEP &stepPut, TCHAR *szStep, UINT count);
	void Draw();
	void Back();
	void Giveup();
};