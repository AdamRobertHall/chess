// Eveluation.cpp: implementation of the CEveluation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "Eveluation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//定义2个常量数组存放兵在不同位置的附加值
//基本上是过河之后越靠近老将越高

//红卒的附加值矩阵
const int BA0[10][9] = 
{
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{90, 90, 110,120,120,120,110,90, 90},
	{90, 90, 110,120,120,120,110,90, 90},
	{70, 90, 110,110,110,110,110,90, 70},
	{70, 70, 70, 70, 70, 70, 70, 70, 70},
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
};

//黑色的附加值矩阵
const int BA1[10][9] = 
{
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{70, 70, 70, 70, 70, 70, 70, 70, 70},
	{70, 90, 110,110,110,110,110,90, 70},
	{90, 90, 110,120,120,120,110,90, 90},
	{90, 90, 110,120,120,120,110,90, 90},
	{0,  0,  0,  0,  0,  0,  0,  0,  0 }
};

//为每一个兵返回附加值
//x是横坐标,y是纵坐标,CurSituation是棋盘
//不是兵值返回零
int GetBingValue(int x,int y,BYTE CurSituation[][9])
{
	//如果红卒返回其位置附加值
	if(CurSituation[y][x] == R_PAWN)
		return BA0[y][x];
	//如果黑兵返回其位置附加值
	if(CurSituation[y][x] == B_PAWN)
		return BA1[y][x];

	return 0;
}

CEveluation::CEveluation()
{
	//初始化每个棋子的基本价值数组
	m_BaseValue[B_KING] = BASEVALUE_KING; 
	m_BaseValue[B_CAR] = BASEVALUE_CAR; 
	m_BaseValue[B_HORSE] = BASEVALUE_HORSE; 
	m_BaseValue[B_BISHOP] = BASEVALUE_BISHOP; 
	m_BaseValue[B_ELEPHANT] = BASEVALUE_ELEPHANT; 
	m_BaseValue[B_CANON] = BASEVALUE_CANON; 
	m_BaseValue[B_PAWN] = BASEVALUE_PAWN; 

	m_BaseValue[R_KING] = BASEVALUE_KING; 
	m_BaseValue[R_CAR] = BASEVALUE_CAR; 
	m_BaseValue[R_HORSE] = BASEVALUE_HORSE; 
	m_BaseValue[R_BISHOP] = BASEVALUE_BISHOP; 
	m_BaseValue[R_ELEPHANT] = BASEVALUE_ELEPHANT; 
	m_BaseValue[R_CANON] = BASEVALUE_CANON; 
	m_BaseValue[R_PAWN] = BASEVALUE_PAWN; 

	m_FlexValue[B_KING] = FLEXIBILITY_KING; 
	m_FlexValue[B_CAR] = FLEXIBILITY_CAR; 
	m_FlexValue[B_HORSE] = FLEXIBILITY_HORSE; 
	m_FlexValue[B_BISHOP] = FLEXIBILITY_BISHOP; 
	m_FlexValue[B_ELEPHANT] = FLEXIBILITY_ELEPHANT; 
	m_FlexValue[B_CANON] = FLEXIBILITY_CANON; 
	m_FlexValue[B_PAWN] = FLEXIBILITY_PAWN; 

	m_FlexValue[R_KING] = FLEXIBILITY_KING; 
	m_FlexValue[R_CAR] = FLEXIBILITY_CAR; 
	m_FlexValue[R_HORSE] = FLEXIBILITY_HORSE; 
	m_FlexValue[R_BISHOP] = FLEXIBILITY_BISHOP; 
	m_FlexValue[R_ELEPHANT] = FLEXIBILITY_ELEPHANT; 
	m_FlexValue[R_CANON] = FLEXIBILITY_CANON; 
	m_FlexValue[R_PAWN] = FLEXIBILITY_PAWN; 
}

CEveluation::~CEveluation()
{

}
int count = 0;	//定义一个全局变量.用以统计调用了估值的子节点次数

//估值函数
//position是要估值的棋盘
//bIsRedTurn是轮到谁走棋的标志,TRUE是红,FALSE是黑
int CEveluation::Eveluate(BYTE position[10][9],BOOL bIsRedTurn)
{
	int i,j,k;
	int nChessType,nTargetType;
	count++;	//每调用一次估值函数就增加一次,用以统计调用了
	//估值函数的子节点的次数

	//初始化临时变量
	memset(m_chessValue,0, 360);
	memset(m_AttackPos,0, 180); 
	memset(m_GuardPos,0, 90); 
	memset(m_FlexibilityPos, 0, 90);

	//扫描棋盘,找出每一个棋子,及其威胁/保护的棋子,还有其灵活性
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 9; j++)
		{
			if(position[i][j] != NOCHESS) //如果不是空白
			{
				nChessType = position[i][j];//取棋子类型
				GetRelatePiece(position,j,i);//找出该棋子所有相关位置
				for(k = 0; k < nPosCount; k++)//对每一个目标位置
				{
					//取目标位置棋子类型
					nTargetType = 
						position[RelatePos[k].y][RelatePos[k].x];
					if(nTargetType == NOCHESS)//如果是空白
						m_FlexibilityPos[i][j]++;//灵活性增加
					else
					{
						//是棋子
						if(IsSameSide(nChessType,nTargetType))
						{
							//如果是我方棋子,目标受保护
							m_GuardPos[RelatePos[k].y][RelatePos[k].x]++;
						}
						else
						{
							//如果是对方棋子,目标受威胁
							m_AttackPos[RelatePos[k].y][RelatePos[k].x]++;
							m_FlexibilityPos[i][j]++; //灵活性增加
							switch(nTargetType)
							{
							case R_KING:	//如果红将
								if(!bIsRedTurn) //如果轮到黑棋走
									return 18888; //返回失败极值
								break;

							case B_KING:	//如果是黑帅
								if(bIsRedTurn)//如果轮到红棋走
									return 18888; //返回失败
								break;
							
							//不是将的其他棋子
							default:
								//根据威胁的棋子加上威胁分值
								m_AttackPos[RelatePos[k].y][RelatePos[k].x]
									+= (30 + (m_BaseValue[nTargetType]
									- m_BaseValue[nChessType]) / 10) /10;
								break;

							}
						}
					}

				}
			}
		}
	}

	//以上是扫描棋盘的部分
	//下面的循环统计扫描到的部分
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 9; j++)
		{
			if(position[i][j] != NOCHESS)
			{
				nChessType = position[i][j];//棋子类型
				m_chessValue[i][j]++;	//如果棋子存在其价值不为0
				//把每一个棋子的灵活性价值加进棋子价值
				m_chessValue[i][j] += 
					m_FlexValue[nChessType] * m_FlexibilityPos[i][j];

				//加上兵的位置附加值
				m_chessValue[i][j] += GetBingValue(j,i,position);
			}
		}
	}
	
	//下面的循环继续统计扫描到的数据
	int nHalfvalue;
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 9; j++)
		{
			if(position[i][j] != NOCHESS) //如果不是空白
			{
				nChessType = position[i][j];//取棋子类型
				//棋子基本价值的1/16作为威胁/保护增量
				nHalfvalue = m_BaseValue[nChessType] / 16;
				//把每个棋子的基本价值加入总价值
				m_chessValue[i][j] += m_BaseValue[nChessType];
				if(IsRed(nChessType)) //如果是红棋
				{
					if(m_AttackPos[i][j])
					{
						if(bIsRedTurn)
						{
							//如果轮到红棋走
							if(nChessType == R_KING)
							{
								//如果是红将
								m_chessValue[i][j] -= 20;
								//价值减低20
							}
							else
							{
								//价值减去2倍nHalfvalue
								m_chessValue[i][j] -= nHalfvalue * 2;
								if(m_GuardPos[i][j]) //是否被我方棋子保护
									m_chessValue[i][j] += nHalfvalue;
								//被保护再加上nHalfvalue
							}
						}
						else
						{
							//当前红旗被威胁,轮到黑棋走
							if(nChessType == R_KING)	//是否是红将
								return 18888;	//返回失败的极值
							//减去10倍的nHalfvalue.表示威胁程度高
							m_chessValue[i][j] -= nHalfvalue * 10;
							if(m_GuardPos[i][j]) //如果被保护
								m_chessValue[i][j] += nHalfvalue * 9;
							//被保护再加上9倍nHalfvalue
						}
						//被威胁的棋子加上威胁差,防止一个兵威胁
						//一个被保护的车,而估值函数没有反应之类的问题.
						m_chessValue[i][j] -= m_AttackPos[i][j];
					}
					else
					{
						//没收到威胁
						if(m_GuardPos[i][j])
							m_chessValue[i][j] += 5;//受保护.加一点分
					}
				}
				else
				{
					//如果是黑棋
					if(m_AttackPos[i][j])
					{
						//受威胁
						if(!bIsRedTurn)
						{
							//轮到黒棋走
							if(nChessType == B_KING)	//如果是黑将
								m_chessValue[i][j] -= 20;	//棋子价值降低20
							else
							{
								//棋子价值降低2倍nHalfvalue
								m_chessValue[i][j] -= nHalfvalue * 2;
								if(m_GuardPos[i][j]) //如果受到保护
									m_chessValue[i][j] += nHalfvalue;
									//棋子价值增加nHalfvalue
							}
						}
						else
						{
							//轮到红棋走
							if(nChessType == B_KING)	//如果是黑将
								return 18888;	//返回失败极值
							//棋子价值减少10倍nHalfvalue
							m_chessValue[i][j] -= nHalfvalue * 10;
							if(m_GuardPos[i][j]) //受到保护
								m_chessValue[i][j] += nHalfvalue * 9;
								//被保护再加上9倍nHalfvalue
						}
						//被威胁的棋子加上威胁差,防止一个兵威胁
						//一个被保护的车,二估值函数没有反映之类的问题
						m_chessValue[i][j] -= m_AttackPos[i][j];
					}
					else
					{
						//没受到威胁
						if(m_GuardPos[i][j])
							m_chessValue[i][j] += 5; //受保护,加点分数
					}

				}
			}
		}
	}
	//以上生成统计了每一个棋子的总价值

	//下面是统计黑红两方的总分
	int nRedValue = 0;
	int nBlackValue = 0;
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 9; j++)
		{
			nChessType = position[i][j];	//取棋子类型
			if(nChessType != NOCHESS)	//如果不是空白
			{
				if(IsRed(nChessType))
					nRedValue += m_chessValue[i][j];	//把宏棋的值加总
				else
					nBlackValue += m_chessValue[i][j];	//把黑棋的值加总
			}
		}
	}
	if(bIsRedTurn)
		return nRedValue - nBlackValue;	//如果轮到红旗走,返回估值
	else
		return nBlackValue - nRedValue;	//轮到黑棋,返回负的估值
}

//将一个位置加入数组RelatePos
void CEveluation::AddPoint(int x,int y)
{
	RelatePos[nPosCount].x = x;
	RelatePos[nPosCount].y = y;
	nPosCount++;
}

//枚举给定位置上棋子的所有相关位置
//包括可走到的位置和可以保护的位置
//position是当前棋盘
//x是棋子的横坐标,y是棋子的纵坐标
int CEveluation::GetRelatePiece(BYTE position[10][9],int j, int i)
{
	nPosCount = 0;
	BYTE nChessID;
	BYTE flag;
	int x,y;
	
	nChessID = position[i][j];
	switch(nChessID)
	{
	case R_KING:	//红将
	case B_KING:	//黑帅
		//循环检查九宫之内的那些位置壳到达/保护
		//循环两边九宫包含了照像的情况
		for(y = 0; y < 3; y++)
		{
			for(x = 3; x < 6; x++)
			{
				if(CanTouch(position,j,i,x,y)) //能否走到
					AddPoint(x,y);	//加入可到达/保护的数组
			}
		}
		//循环检查九宫之内的那些位置壳到达/保护
		//循环两边九宫包含了照像的情况
		for(y = 7; y < 10; y++)
		{
			for(x = 3; x < 6; x++)
			{
				if(CanTouch(position,j,i,x,y)) //能否走到
					AddPoint(x,y);	//加入可到达/保护的数组
			}
		}
		break;

	case B_BISHOP:	//黑士
		//循环检查九宫之内的那些位置壳到达/保护
		for(y = 0; y < 3; y++)
		{
			for(x = 3; x < 6; x++)
			{
				if(CanTouch(position,j,i,x,y)) //能否走到
					AddPoint(x,y);	//加入可到达/保护的数组
			}
		}
		break;

	case R_BISHOP:	//红仕
		//循环检查九宫之内的那些位置壳到达/保护
		for(y = 7; y < 10; y++)
		{
			for(x = 3; x < 6; x++)
			{
				if(CanTouch(position,j,i,x,y)) //能否走到
					AddPoint(x,y);	//加入可到达/保护的数组
			}
		}
		break;

	case R_ELEPHANT:	//红相
	case B_ELEPHANT:	//黑象
		//右下
		x = j + 2;
		y = i + 2;
		if(x < 9 && y < 10 && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//右上
		x = j + 2;
		y = i - 2;
		if(x < 9 && y >= 0 && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//左下
		x = j - 2;
		y = i + 2;
		if(x >= 0 && y < 10 && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//左上
		x = j - 2;
		y = i - 2;
		if(x >= 0 && y >= 0 && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		break;

	case R_HORSE:	//红马
	case B_HORSE:	//黑马
		//检查右下方是否能走/保护
		x = j + 2;
		y = i + 1;
		if((x < 9 && y < 10) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//检查右上方是否能走/保护
		x = j + 2;
		y = i - 1;
		if((x < 9 && y >= 0) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//检查左下方是否能走/保护
		x = j - 2;
		y = i + 1;
		if((x >= 0 && y < 10) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//检查左上方是否能走/保护
		x = j - 2;
		y = i - 1;
		if((x >= 0 && y >= 0) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);

		//检查右下方是否能走/保护
		x = j + 1;
		y = i + 2;
		if((x < 9 && y < 10) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//检查右上方是否能走/保护
		x = j + 1;
		y = i - 2;
		if((x < 9 && y >= 0) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//检查左下方是否能走/保护
		x = j - 1;
		y = i + 2;
		if((x >= 0 && y < 10) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		//检查左上方是否能走/保护
		x = j - 1;
		y = i - 2;
		if((x >= 0 && y >= 0) && CanTouch(position,j,i,x,y))
			AddPoint(x,y);
		break;

	case R_CAR:		//红车
	case B_CAR:		//黑車
		//检查向右的位置是否能走/保护
		x = j + 1;
		y = i;
		while(x < 9)
		{
			if(NOCHESS == position[y][x]) //空白
				AddPoint(x,y);
			else
			{
				//碰到第一个棋子
				AddPoint(x,y);
				break;	//后面位置不可以走
			}
			x++;
		}
		//检查向左的位置是否能走/保护
		x = j - 1;
		y = i;
		while(x >= 0)
		{
			if(NOCHESS == position[y][x]) //空白
				AddPoint(x,y);
			else
			{
				//碰到第一个棋子
				AddPoint(x,y);
				break;	//后面位置不可以走
			}
			x--;
		}
		//检查向下的位置是否能走/保护
		x = j;
		y = i + 1;
		while(y < 10)
		{
			if(NOCHESS == position[y][x]) //空白
				AddPoint(x,y);
			else
			{
				//碰到第一个棋子
				AddPoint(x,y);
				break;	//后面位置不可以走
			}
			y++;
		}
		//检查向下的位置是否能走/保护
		x = j;
		y = i - 1;
		while(y > 0)
		{
			if(NOCHESS == position[y][x]) //空白
				AddPoint(x,y);
			else
			{
				//碰到第一个棋子
				AddPoint(x,y);
				break;	//后面位置不可以走
			}
			y--;
		}	
		break;
	
	case R_PAWN:	//红卒
		//查看向前是否到底
		y = i - 1;
		x = j;
		if(y >= 0)
			AddPoint(x,y);	//没到底.可走
		if(i < 5)
		{
			//如果已经过河
			y = i;
			x = j + 1;//向右
			if(x < 9)
				AddPoint(x,y);//未到右边,可走
			x = j - 1;//向左
			if(x >= 0)
				AddPoint(x,y);//未到左边,可走
		}
		break;

	case B_PAWN:	//黑兵
		//查看向前是否到底
		y = i + 1;
		x = j;
		if(y < 10) //是否已经沉底
			AddPoint(x,y);//没到底
		if(i < 4)
		{
			//如已过河
			y = i;
			x = j + 1;//向右
			if(x < 9)
				AddPoint(x,y);//未到右边,可走
			x = j - 1;//向左
			if(x >= 0)
				AddPoint(x,y);//未到左边,可走
		}
		break;

	case B_CANON:	//黑炮
	case R_CANON:	//红炮
		//查看向右方向上的可走/保护的位置
		x = j + 1;
		y = i;
		flag = FALSE;
		while(x < 9)
		{
			if(NOCHESS == position[y][x])
			{
				//空白位置
				if(!flag)
					AddPoint(x,y);
			}
			else
			{
				//有棋子
				if(!flag)
					flag = TRUE;	//是第一个棋子
				else
				{
					//是第二个棋子
					AddPoint(x,y);
					break;
				}
			}
			x++; //继续向右
		}
		//查看向左方向上的可走/保护的位置
		x = j - 1;
		y = i;
		flag = FALSE;
		while(x >= 9)
		{
			if(NOCHESS == position[y][x])
			{
				//空白位置
				if(!flag)
					AddPoint(x,y);
			}
			else
			{
				//有棋子
				if(!flag)
					flag = TRUE;	//是第一个棋子
				else
				{
					//是第二个棋子
					AddPoint(x,y);
					break;
				}
			}
			x--;//继续向左
		}
		//查看向下方向上的可走/保护的位置
		x = j;
		y = i + 1;
		flag = FALSE;
		while(y < 10)
		{
			if(NOCHESS == position[y][x])
			{
				//空白位置
				if(!flag)
					AddPoint(x,y);
			}
			else
			{
				//有棋子
				if(!flag)
					flag = TRUE;	//是第一个棋子
				else
				{
					//是第二个棋子
					AddPoint(x,y);
					break;
				}
			}
			y++;	//继续向下
		}
		//查看向下方向上的可走/保护的位置
		x = j;
		y = i - 1;
		flag = FALSE;
		while(y >= 0)
		{
			if(NOCHESS == position[y][x])
			{
				//空白位置
				if(!flag)
					AddPoint(x,y);
			}
			else
			{
				//有棋子
				if(!flag)
					flag = TRUE;	//是第一个棋子
				else
				{
					//是第二个棋子
					AddPoint(x,y);
					break;
				}
			}
			y--;	//继续向上
		}
		break;

		default:
			break;

	}
	return nPosCount;
}

//判断棋盘position上位置from的棋子是否能走到位置to
//如果能返回TRUE.否则返回FALSE
BOOL CEveluation::CanTouch(BYTE position[10][9],int nFromX,
						   int nFromY,int nToX,int nToY)
{
	int i,j;
	int nMoveChessID,nTargetID;
	if(nFromY == nToY && nFromX == nToX)
		return FALSE;	//目的与原来位置相同
	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];
	switch(nMoveChessID)
	{
	case B_KING:
		if(nTargetID == R_KING)	//是否是老将见面
		{
			if(nFromX != nToX)
				return FALSE;
			for(i = nFromX + 1; i < nToY; i++)
				if(position[i][nFromX] != NOCHESS)
					return FALSE;
		}
		else
		{
			if(nToY > 2 || nToX > 5 || nToX < 3)
				return FALSE;	//目标点在九宫之外
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1)
				return FALSE;	//将帅只能走一步
		}
		break;

	case R_BISHOP:	//红仕
		if(nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;	//仕出九宫
		if(abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//仕走斜线
		break;

	case B_BISHOP:	//黑仕
		if(nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;	//仕出九宫
		if(abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//仕走斜线
		break;

	case R_ELEPHANT://红相
		if(nToY < 5)
			return FALSE;	//相不能过河
		if(abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;	//相走田字
		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;	//相眼被堵塞
		break;

	case B_ELEPHANT://黑象
		if(nToY > 4)
			return FALSE;	//象不能过河
		if(abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;	//象走田字
		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;	//象眼被堵塞
		break;

	case B_PAWN:	//黑兵
		if(nToY < nFromY)
			return FALSE;	//兵不能后退
		if(nFromY < 5 && nFromY == nToY)
			return FALSE;	//兵过河只能直走
		if(nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;	//兵只走一步直线
		break;
	
	case R_PAWN:	//红卒
		if(nToY > nFromY)
			return FALSE;	//兵不能后退
		if(nFromY > 4 && nFromY == nToY)
			return FALSE;	//兵过河只能直走
		if(nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;	//兵只走一步直线
		break;

	case R_KING:
		if(nTargetID == B_KING)	//是否是老将见面
		{
			if(nFromX != nToX)
				return FALSE;	//将帅不在同一列
			for(i = nFromY - 1; i > nToY; i--)
				if(position[i][nFromX] != NOCHESS)
					return FALSE;	//中间有棋子
		}
		else
		{
			if(nToY < 7 || nToX > 5 || nToX < 3)
				return FALSE;	//目标点在九宫之外
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1)
				return FALSE;	//将帅只能走一步
		}
		break;

	case R_CAR:	//红車
	case B_CAR:		//黑車
		if(nFromY != nToY && nFromX != nToX)
			return FALSE;	//车走直线
		if(nFromY == nToY)
		{
			//横向
			if(nFromX < nToX)
			{
				//向右
				for(i = nFromX + 1; i < nToX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
			else
			{
				//向左
				for(i = nToX + 1; i < nFromX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
		}
		else
		{
			//纵向
			if(nFromY < nToY)
			{
				//向下
				for(j = nFromY + 1; j < nToY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
			else
			{
				//向上
				for(j = nToY + 1; j < nFromY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
		}
		break;

	case B_HORSE:	//黑马
	case R_HORSE:	//红马
		if(!((abs(nToX - nFromX) == 1 && abs(nToY - nFromY) == 2)
			||(abs(nToX -nFromX) == 2 && abs(nToY - nFromY) == 1)))
			return FALSE;	//马走日字
		if(nToX - nFromX == 2)
		{
			//横向右走
			i = nFromX + 1;
			j = nFromY;
		}
		else if(nFromX - nToX == 2)
		{
			//横向左走
			i = nFromX - 1;
			j = nFromY;
		}
		else if(nToY - nFromY == 2)
		{
			//纵向下走
			i = nFromX;
			j = nFromY + 1;
		}
		else if(nFromY - nToY == 2)
		{
			//纵向上走
			i = nFromX;
			j = nFromY - 1;
		}
		if(position[j][i] != (BYTE)NOCHESS)
			return FALSE;	//绊马腿
		break;

	case B_CANON:	//黑炮
	case R_CANON:	//红炮
		if(nFromY != nToY && nFromX != nToX)
			return FALSE;	//炮走直线
		//炮不吃字时经过的路线不能有棋子
		if(position[nToY][nToX] == NOCHESS)
		{
			//不吃子
			if(nFromY == nToY)
			{
				//横向
				if(nFromX < nToX)
				{
					//向右
					for(i = nFromX + 1; i < nToX; i++)
						if(position[nFromY][i] != NOCHESS)
							return FALSE;
				}
				else
				{
					//向左
					for(i = nToX + 1; i < nFromX; i++)
						if(position[nFromY][i] != NOCHESS)
							return FALSE;
				}
			}
			else
			{
				//纵向
				if(nFromY < nToY)
				{
					//向下
					for(j = nFromY + 1; j < nToY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
				else
				{
					//向上
					for(j = nToY + 1; j < nFromY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
			}
		}
		else
		{
			//吃子时
			int count = 0;
			if(nFromY == nToY)
			{
				//横向
				if(nFromX < nToX)
				{
					//向右
					for(i = nFromX + 1; i < nToX; i++)
						if(position[nFromY][i] != NOCHESS)
							count++;
					if(count != 1)	//不是隔一个子不能吃
						return FALSE;
				}
				else
				{
					//向左
					for(i = nToX + 1; i < nFromX; i++)
						if(position[nFromY][i] != NOCHESS)
							count++;
					if(count != 1)	//不是隔一个子不能吃
						return FALSE;
				}
			}
			else
			{
				//纵向
				if(nFromY < nToY)
				{
					//向下
					for(j = nFromY + 1; j < nToY; j ++)
						if(position[j][nFromX] != NOCHESS)
							count++;
					if(count != 1)	//不是隔一个子不能吃
						return FALSE;
				}
				else
				{
					//向上
					for(j = nToY + 1; j < nFromY; j ++)
						if(position[j][nFromX] != NOCHESS)
							count++;
					if(count != 1)	//不是隔一个子不能吃
						return FALSE;
				}
			}

		}
		break;

	default:
		return FALSE;
	}
	return TRUE;

}