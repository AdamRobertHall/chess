#pragma once

#define NOCHESS			0 //没有棋子

#define B_KING			1 //黑将
#define B_CAR			2 //黑车
#define B_HORSE			3 //黑马
#define B_CANON			4 //黑炮
#define B_BISHOP		5 //黑士
#define B_ELEPHANT		6 //黑象
#define B_PAWN			7 //黑卒
#define B_BEGIN			B_KING
#define B_END			B_PAWN

#define R_KING			8 //红帅
#define R_CAR			9 //红车
#define R_HORSE			10//红马
#define R_CANON			11//红炮
#define R_BISHOP		12//红士
#define R_ELEPHANT		13//红相
#define R_PAWN			14//红兵
#define R_BEGIN			R_KING
#define R_END			R_PAWN

//定义常量保存棋盘的初始状态
const BYTE InitChessBoard[10][9]=
{
	{B_CAR,   B_HORSE, B_ELEPHANT, B_BISHOP, B_KING,  B_BISHOP, B_ELEPHANT, B_HORSE, B_CAR},
	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},
	{NOCHESS, B_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    B_CANON, NOCHESS},
	{B_PAWN,  NOCHESS, B_PAWN,     NOCHESS,  B_PAWN,  NOCHESS,  B_PAWN,     NOCHESS, B_PAWN},
	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},

	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},
	{R_PAWN,  NOCHESS, R_PAWN,     NOCHESS,  R_PAWN,  NOCHESS,  R_PAWN,     NOCHESS, R_PAWN},
	{NOCHESS, R_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    R_CANON, NOCHESS},
	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},
	{R_CAR,   R_HORSE, R_ELEPHANT, R_BISHOP, R_KING,  R_BISHOP, R_ELEPHANT, R_HORSE, R_CAR},
};

#define BORDERWIDTH			10 //棋盘(左右)边缘的宽度
#define BORDERHEIGHT		8  //棋盘(上下)边缘的高度
#define GRILLELENGTH		40 //棋盘上每个格子的边长

#define IsBlack(x)	(x >= B_BEGIN && x <= B_END)	//判断一个棋子是不是黑色
#define IsRed(x)	(x >= R_BEGIN && x <= R_END)	//判断一个棋子是不是红色

//判断两个棋子是不是同色
#define IsSameSide(x,y)  ((IsBlack(x)&&IsBlack(y))||(IsRed(x)&&IsRed(y)))

#define CURRENT_RED 1
#define CURRENT_BLACK 0

#define GAMEDEMO 0       //播放棋局
#define GAMEONE  1       //单人游戏
#define GAMETWO  2       //网络对战

//定义一个棋子位置的结构
typedef struct _chessmanposition
{
	BYTE		x;
	BYTE		y;
}CHESSMANPOS;

//一个走法的结构
typedef struct _chessmove
{
	BYTE		ChessID;	//表明是什么棋子
	CHESSMANPOS From;		//起始位置
	CHESSMANPOS	To;			//走到位置
}CHESSMOVE;

typedef struct _chessstep
{
	BYTE		FromID;	    //移动的起始位置的棋子
	BYTE		ToID;       //移动的目标位置的棋子
	CHESSMANPOS From;		//起始位置
	CHESSMANPOS	To;			//走到位置
}CHESSSTEP;

// 用于保存当前被拖拽的棋子的结构
typedef struct _movechess
{
	BYTE	nChessID;
	POINT	ptMovePoint;
}MOVECHESS;

typedef struct _tagMsgStruct {
	// 消息ID
	UINT uMsg;
	// 一步棋
	CHESSSTEP oneStep;
	// 招法
	TCHAR szStep[100];
	// 消息内容
	TCHAR szMsg[256];
	// 着法总数
	UINT nMethodCount;
} MSGSTRUCT;

// 落子消息
#define MSG_PUTSTEP     0x00000001
// 悔棋消息
#define MSG_BACK        0x00000002
// 同意悔棋消息
#define MSG_AGREEBACK   0x00000003
// 拒绝悔棋消息
#define MSG_REFUSEBACK  0x00000004
// 和棋消息
#define MSG_DRAW        0x00000005
// 同意和棋消息
#define MSG_AGREEDRAW   0x00000006
// 拒绝和棋消息
#define MSG_REFUSEDRAW  0x00000007
// 认输消息
#define MSG_GIVEUP      0x00000008
// 聊天消息
#define MSG_CHAT        0x00000009
// 再次开局消息
#define MSG_PLAYAGAIN   0x0000000a
// 同意再次开局消息
#define MSG_AGREEAGAIN  0x0000000b
