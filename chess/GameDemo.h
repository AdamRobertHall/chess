#pragma once
#include "game.h"
class CGameDemo :
	public CGame
{
public:
	CGameDemo(CBoard* pBoard);
	virtual ~CGameDemo(void);
	void init();
	void Prev();
	void Next();
	void Demoend();
};

