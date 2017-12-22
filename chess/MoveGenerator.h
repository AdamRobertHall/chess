// MoveGenerator.h: interface for the CMoveGenerator class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include <string>

//�߷�������
class CMoveGenerator  
{
public:
	CMoveGenerator();
	virtual ~CMoveGenerator();
	//���Լ��һ���߷��Ƿ�Ϸ��ľ�̬����
	static BOOL IsValidMove(BYTE position[10][9],int nFromX,
							int nFromY,int nToX,int nToY);
	//�������������ϵ����кϷ����߷�
	int CreatePossibleMove(BYTE position[10][9],int nPly,int nSide);
	//���CreatePossibleMove ���������߷��Ķ���
	CHESSMOVE m_MoveList[8][80];

protected:
	//��m_MoveList�в���һ���߷�
	int AddMove(int nFromX,int nToX,int nFromY,int nToY,int nPly);
	//�������������ϵĸ���λ���ϵĽ�/˧���߷�
	void Gen_KingMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵĺ��˵��߷�
	void Gen_RBishopMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵĺ�ʿ���߷�
	void Gen_BBishopMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵ���/����߷�
	void Gen_ElephantMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵ�����߷�
	void Gen_HorseMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵĳ����߷�
	void Gen_CarMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵĺ�����߷�
	void Gen_RPawnMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵĺڱ����߷�
	void Gen_BPawnMove(BYTE position[10][9],int i,int j,int nPly);
	//�������������ϵĸ���λ���ϵ��ڵ��߷�
	void Gen_CanonMove(BYTE position[10][9],int i,int j,int nPly);

	int m_nMoveCount;	//��¼m_MoveList���߷�������

};

