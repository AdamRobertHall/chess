#pragma once

#include "Game.h"
#include "GameDemo.h"
#include "GameOne.h"
#include "GameTwo.h"
#include "ChessSocket.h"
#include <mmsystem.h>
#include <string>

class CBoard : public CWnd
{
	DECLARE_DYNAMIC(CBoard)

private:
	// 棋盘位图
	CBitmap m_BoardBmp;
	// 棋子列表
	CImageList m_ChessList;
	// 棋盘高度
	int	m_nBoardWidth;
	// 棋盘宽度
	int	m_nBoardHeight;
	// 备份棋盘数组.用于出错恢复
	BYTE m_BackupChessBoard[10][9];
	// 用于保存当前被拖拽的棋子的结构
	MOVECHESS m_MoveChess;
	// 用与保存当前被拖拽的棋子的位置
	CHESSMANPOS m_ptMoveChess;	
public:
	// 游戏指针
	CGame* m_pGame;
	// 游戏模式为真时才可以下棋
	BOOL m_gameMode;
	// 游戏是否结束
	BOOL m_isOver;
	// 着法总数
	int m_QipuCount;
	// 标识正在下棋的一方的颜色
	int m_current_side;
	// 是否连接网络（客户端使用）
	BOOL m_bConnected;
	// 传输用套接字
	CChessSocket m_conn;
	CChessSocket m_sock;
	// 等待标志
	BOOL m_bWait; 
	// 电脑是否先走，为真电脑先走，为假玩家先走
	BOOL m_bComputerFirst;
	// 走法字符串
	CString m_ChessStepMethod;
	// 定义棋盘数组.用于显示棋盘
	BYTE m_ChessBoard[10][9];
	// 定义移动后棋子的坐标
	int m_px, m_py;

public:
	CBoard();
	virtual ~CBoard();

	BOOL IsJiangJun(BYTE position[10][9], int m_current_side);
	BOOL IsGameOver(BYTE position[][9]);
	BOOL IsGameOver(BYTE position[][9], int m_current_side);
	CString GenerateChessMethod(int side, int id, int Move_FromX, int Move_FromY, int Move_ToX, int Move_ToY);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void SetGameMode(int mode);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void Receive();
	void PlayAgain(void);
};


