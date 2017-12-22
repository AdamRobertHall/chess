// SearchEngine.h: interface for the CSearchEngine class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MoveGenerator.h"
#include "Eveluation.h"

//��������Ļ��� .����ӿڼ���������.
class CSearchEngine  
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();
	//���Ľӿ�,Ϊ��ǰ�����߳���һ����
	virtual BOOL SearchAGoodMove(BYTE position[10][9])=0;
	//�趨����������
	virtual void SetSearchDepth(int nDepth){m_nSearchDepth = nDepth;};
	//��ƹ�ֵ����
	virtual void SetEveluator(CEveluation *pEval){m_pEval = pEval;};
	//�趨�߷�������
	virtual void SetMoveGenerator(CMoveGenerator *pMG){m_pMG = pMG;};

protected:
	//����ĳһ�߷����������Ժ������
	virtual BYTE MakeMove(CHESSMOVE* move);
	//�ָ�ĳһ�߷�����������Ϊ�߹�֮ǰ��
	virtual void UnMakeMove(CHESSMOVE* move,BYTE type);
	//�����жϵ�ǰ�����Ƿ��Ѿ��ֳ�ʤ��
	virtual int IsGameOver(BYTE position[10][9], int nDepth);
	//�������ڵ�ǰ�ڵ�����״̬������
	BYTE CurPosition[10][9];
	//��¼����߷��ı���
	CHESSMOVE m_cmBestMove;
	//�߷�������ָ��
	CMoveGenerator *m_pMG;
	//��ֵ����ָ��
	CEveluation *m_pEval;
	//��ǰ����������������
	int m_nSearchDepth;
	//����������	
	int m_nMaxDepth;
};

