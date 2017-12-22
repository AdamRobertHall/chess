// Eveluation.h: interface for the CEveluation class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

//定义一组宏定义每种棋子的基本价值
//兵100,士250,象250,车500,马350,炮350
//将帅 10000
#define BASEVALUE_PAWN 100
#define BASEVALUE_BISHOP 250
#define BASEVALUE_ELEPHANT 250
#define BASEVALUE_CAR 500
#define BASEVALUE_HORSE 350
#define BASEVALUE_CANON 350
#define BASEVALUE_KING 10000

//定义一组宏定义各种棋子的灵活性
//即每多一个可走位置上应加上的分值
//兵15,士1,象1,车6,马12,炮6,王0
#define FLEXIBILITY_PAWN		15
#define FLEXIBILITY_BISHOP		1
#define FLEXIBILITY_ELEPHANT	1
#define FLEXIBILITY_CAR			6
#define FLEXIBILITY_HORSE		12
#define FLEXIBILITY_CANON		6
#define FLEXIBILITY_KING		0

//估值核心
class CEveluation  
{
public:
	CEveluation();
	virtual ~CEveluation();
	//估值函数,对传入的棋盘打分,BIsRedTurn 标明轮到谁走棋
	virtual int Eveluate(BYTE position[10][9], BOOL bIsRedTurn);

protected:
	//列举与指定位置的棋子相关的棋子
	int GetRelatePiece(BYTE position[10][9],int j,int i);
	//判断位置From的棋子是否能走到位置To
	BOOL CanTouch(BYTE position[10][9],int nFromX,
		int nFromY,int nToX,int nToY);
	//将一个位置加入相关位置的队列
	void AddPoint(int x,int y);
	int m_BaseValue[15];//存放棋子基本价值的数组
	int m_FlexValue[15];//存放棋子灵活性分数的数组
	short m_AttackPos[10][9];//存放每一个位置被威胁的信息
	BYTE m_GuardPos[10][9];//存放每一位置被保护的信息
	BYTE m_FlexibilityPos[10][9];//存放每一个位置上的棋子灵活性分数
	int m_chessValue[10][9];//存放每一位置上的棋子的总价值
	int nPosCount;	//记录一棋子的相关位置个数
	POINT RelatePos[20];	//记录一个棋子相关位置的数组
};

