#pragma once

#define NOCHESS			0 //û������

#define B_KING			1 //�ڽ�
#define B_CAR			2 //�ڳ�
#define B_HORSE			3 //����
#define B_CANON			4 //����
#define B_BISHOP		5 //��ʿ
#define B_ELEPHANT		6 //����
#define B_PAWN			7 //����
#define B_BEGIN			B_KING
#define B_END			B_PAWN

#define R_KING			8 //��˧
#define R_CAR			9 //�쳵
#define R_HORSE			10//����
#define R_CANON			11//����
#define R_BISHOP		12//��ʿ
#define R_ELEPHANT		13//����
#define R_PAWN			14//���
#define R_BEGIN			R_KING
#define R_END			R_PAWN

//���峣���������̵ĳ�ʼ״̬
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

#define BORDERWIDTH			10 //����(����)��Ե�Ŀ��
#define BORDERHEIGHT		8  //����(����)��Ե�ĸ߶�
#define GRILLELENGTH		40 //������ÿ�����ӵı߳�

#define IsBlack(x)	(x >= B_BEGIN && x <= B_END)	//�ж�һ�������ǲ��Ǻ�ɫ
#define IsRed(x)	(x >= R_BEGIN && x <= R_END)	//�ж�һ�������ǲ��Ǻ�ɫ

//�ж����������ǲ���ͬɫ
#define IsSameSide(x,y)  ((IsBlack(x)&&IsBlack(y))||(IsRed(x)&&IsRed(y)))

#define CURRENT_RED 1
#define CURRENT_BLACK 0

#define GAMEDEMO 0       //�������
#define GAMEONE  1       //������Ϸ
#define GAMETWO  2       //�����ս

//����һ������λ�õĽṹ
typedef struct _chessmanposition
{
	BYTE		x;
	BYTE		y;
}CHESSMANPOS;

//һ���߷��Ľṹ
typedef struct _chessmove
{
	BYTE		ChessID;	//������ʲô����
	CHESSMANPOS From;		//��ʼλ��
	CHESSMANPOS	To;			//�ߵ�λ��
}CHESSMOVE;

typedef struct _chessstep
{
	BYTE		FromID;	    //�ƶ�����ʼλ�õ�����
	BYTE		ToID;       //�ƶ���Ŀ��λ�õ�����
	CHESSMANPOS From;		//��ʼλ��
	CHESSMANPOS	To;			//�ߵ�λ��
}CHESSSTEP;

// ���ڱ��浱ǰ����ק�����ӵĽṹ
typedef struct _movechess
{
	BYTE	nChessID;
	POINT	ptMovePoint;
}MOVECHESS;

typedef struct _tagMsgStruct {
	// ��ϢID
	UINT uMsg;
	// һ����
	CHESSSTEP oneStep;
	// �з�
	TCHAR szStep[100];
	// ��Ϣ����
	TCHAR szMsg[256];
	// �ŷ�����
	UINT nMethodCount;
} MSGSTRUCT;

// ������Ϣ
#define MSG_PUTSTEP     0x00000001
// ������Ϣ
#define MSG_BACK        0x00000002
// ͬ�������Ϣ
#define MSG_AGREEBACK   0x00000003
// �ܾ�������Ϣ
#define MSG_REFUSEBACK  0x00000004
// ������Ϣ
#define MSG_DRAW        0x00000005
// ͬ�������Ϣ
#define MSG_AGREEDRAW   0x00000006
// �ܾ�������Ϣ
#define MSG_REFUSEDRAW  0x00000007
// ������Ϣ
#define MSG_GIVEUP      0x00000008
// ������Ϣ
#define MSG_CHAT        0x00000009
// �ٴο�����Ϣ
#define MSG_PLAYAGAIN   0x0000000a
// ͬ���ٴο�����Ϣ
#define MSG_AGREEAGAIN  0x0000000b
