// NegaMaxEngine.h: interface for the CNegaMaxEngine class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SearchEngine.h"

//搜索引擎. 使用负极大值方法 由CSearchEngine派生来
class CNegaMaxEngine : public CSearchEngine
{
public:
	CNegaMaxEngine();
	virtual ~CNegaMaxEngine();
	//用以找出给定局面的下一步走法
	virtual BOOL SearchAGoodMove(BYTE position[10][9]);

protected:
	int NegaMax(int depth);	//负极大值搜索引擎

};

