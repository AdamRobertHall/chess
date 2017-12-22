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
	// ����λͼ
	CBitmap m_BoardBmp;
	// �����б�
	CImageList m_ChessList;
	// ���̸߶�
	int	m_nBoardWidth;
	// ���̿��
	int	m_nBoardHeight;
	// ������������.���ڳ���ָ�
	BYTE m_BackupChessBoard[10][9];
	// ���ڱ��浱ǰ����ק�����ӵĽṹ
	MOVECHESS m_MoveChess;
	// ���뱣�浱ǰ����ק�����ӵ�λ��
	CHESSMANPOS m_ptMoveChess;	
public:
	// ��Ϸָ��
	CGame* m_pGame;
	// ��ϷģʽΪ��ʱ�ſ�������
	BOOL m_gameMode;
	// ��Ϸ�Ƿ����
	BOOL m_isOver;
	// �ŷ�����
	int m_QipuCount;
	// ��ʶ���������һ������ɫ
	int m_current_side;
	// �Ƿ��������磨�ͻ���ʹ�ã�
	BOOL m_bConnected;
	// �������׽���
	CChessSocket m_conn;
	CChessSocket m_sock;
	// �ȴ���־
	BOOL m_bWait; 
	// �����Ƿ����ߣ�Ϊ��������ߣ�Ϊ���������
	BOOL m_bComputerFirst;
	// �߷��ַ���
	CString m_ChessStepMethod;
	// ������������.������ʾ����
	BYTE m_ChessBoard[10][9];
	// �����ƶ������ӵ�����
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


