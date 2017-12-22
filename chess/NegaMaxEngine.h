// NegaMaxEngine.h: interface for the CNegaMaxEngine class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SearchEngine.h"

//��������. ʹ�ø�����ֵ���� ��CSearchEngine������
class CNegaMaxEngine : public CSearchEngine
{
public:
	CNegaMaxEngine();
	virtual ~CNegaMaxEngine();
	//�����ҳ������������һ���߷�
	virtual BOOL SearchAGoodMove(BYTE position[10][9]);

protected:
	int NegaMax(int depth);	//������ֵ��������

};

