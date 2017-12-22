// Board.cpp : 实现文件
//

#include "stdafx.h"
#include "define.h"
#include "chess.h"
#include "chessDlg.h"
#include "Board.h"
#include <string>
#include <sstream>
#include <iomanip>

// CBoard

IMPLEMENT_DYNAMIC(CBoard, CWnd)

CBoard::CBoard()
{
	m_ChessList.Create(40, 40, ILC_COLOR32|ILC_MASK, 0, 2);
	CBitmap bmpBJIANG, bmpBCHE, bmpBMA, bmpBPAO, bmpBSHI, bmpBXIANG, bmpBING,
		bmpRJIANG, bmpRCHE, bmpRMA, bmpRPAO, bmpRSHI, bmpRXIANG, bmpRBING;
	bmpBJIANG.LoadBitmap(IDB_BJIANG);
	m_ChessList.Add(&bmpBJIANG, 0XFFFFFF);
	bmpBCHE.LoadBitmap(IDB_BCHE);
	m_ChessList.Add(&bmpBCHE, 0XFFFFFF);
	bmpBMA.LoadBitmap(IDB_BMA);
	m_ChessList.Add(&bmpBMA, 0XFFFFFF);
	bmpBPAO.LoadBitmap(IDB_BPAO);
	m_ChessList.Add(&bmpBPAO, 0XFFFFFF);
	bmpBSHI.LoadBitmap(IDB_BSHI);
	m_ChessList.Add(&bmpBSHI, 0XFFFFFF);
	bmpBXIANG.LoadBitmap(IDB_BXIANG);
	m_ChessList.Add(&bmpBXIANG, 0XFFFFFF);
	bmpBING.LoadBitmap(IDB_BBING);
	m_ChessList.Add(&bmpBING, 0XFFFFFF);
	bmpRJIANG.LoadBitmap(IDB_RJIANG);
	m_ChessList.Add(&bmpRJIANG, 0XFFFFFF);
	bmpRCHE.LoadBitmap(IDB_RCHE);
	m_ChessList.Add(&bmpRCHE, 0XFFFFFF);
	bmpRMA.LoadBitmap(IDB_RMA);
	m_ChessList.Add(&bmpRMA, 0XFFFFFF);
	bmpRPAO.LoadBitmap(IDB_RPAO);
	m_ChessList.Add(&bmpRPAO, 0XFFFFFF);
	bmpRSHI.LoadBitmap(IDB_RSHI);
	m_ChessList.Add(&bmpRSHI, 0XFFFFFF);
	bmpRXIANG.LoadBitmap(IDB_RXIANG);
	m_ChessList.Add(&bmpRXIANG, 0XFFFFFF);
	bmpRBING.LoadBitmap(IDB_RBING);
	m_ChessList.Add(&bmpRBING, 0XFFFFFF);

	//下面代码取棋盘的宽,高
	BITMAP	BitMap;
	m_BoardBmp.LoadBitmap(IDB_QIPAN);
	//取BitMap对象
	m_BoardBmp.GetBitmap(&BitMap);
	//棋盘宽度
	m_nBoardWidth = BitMap.bmWidth;
	//棋盘高度
	m_nBoardHeight = BitMap.bmHeight;
	m_BoardBmp.DeleteObject();

	memcpy(m_ChessBoard, InitChessBoard, 90);
	m_pGame = NULL;
	m_gameMode = FALSE;
	m_QipuCount = 1;
	m_current_side = CURRENT_RED;
	m_bConnected = FALSE;
	m_bWait = TRUE;
	m_bComputerFirst = FALSE;
	m_ChessStepMethod = "";
	m_px = m_py = 0;
	m_isOver = FALSE;
}

CBoard::~CBoard()
{
}


BEGIN_MESSAGE_MAP(CBoard, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CBoard 消息处理程序
void CBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CDC		 MemDC;
	int		 i,j;
	POINT	 pt;
	CBitmap  *pOldBmp;
	MemDC.CreateCompatibleDC(&dc);
	m_BoardBmp.LoadBitmap(IDB_QIPAN);
	pOldBmp = MemDC.SelectObject(&m_BoardBmp);

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (m_ChessBoard[i][j] == NOCHESS)
				continue;
			pt.x = j * GRILLELENGTH + 9;
			pt.y = i * GRILLELENGTH + 10;
			m_ChessList.Draw(&MemDC, m_ChessBoard[i][j] - 1, pt, ILD_TRANSPARENT);		
		}
	}
	//绘制用户正在拖动的棋子
	if (m_MoveChess.nChessID != NOCHESS)
		m_ChessList.Draw(&MemDC, m_MoveChess.nChessID - 1, 
		m_MoveChess.ptMovePoint, ILD_TRANSPARENT);
	//画个方框把移动的棋子框起来
	if (m_px && m_py)
	{
		MemDC.MoveTo(m_px, m_py);
		MemDC.LineTo(m_px + GRILLELENGTH, m_py);
		MemDC.LineTo(m_px + GRILLELENGTH, m_py + GRILLELENGTH);
		MemDC.LineTo(m_px, m_py + GRILLELENGTH);
		MemDC.LineTo(m_px, m_py);
	}	

	//将绘制内容刷新到屏幕
	dc.BitBlt(0,0,m_nBoardWidth,m_nBoardHeight,&MemDC,0,0,SRCCOPY);
	//恢复内存DC的原位图
	MemDC.SelectObject(&pOldBmp);
	//释放内存DC
	MemDC.DeleteDC();
	//删除棋盘位图对象
	m_BoardBmp.DeleteObject();
}

CString CBoard::GenerateChessMethod(int side, int id, int Move_FromX, int Move_FromY, int Move_ToX, int Move_ToY)
{
	std::string step;
	
	if (m_QipuCount %2 == 1)
	{
		int n = (m_QipuCount + 1)/2;
		std::ostringstream oss;
		oss << std::setw(3) << n;
		step = oss.str();
		step += ":  ";
	}
	else 
	{
		step = "    　";
	}

	char RedNum[9][3] = {"九","八","七","六","五","四","三","二","一"};
	char BlackN[9][3] = {"1","２","３","４","５","６","７","８","９"};
	char Name[15][3]  = {"", "将", "車", "马", "炮", "士", "象", "卒", 
		"帅", "車", "马", "炮", "仕", "相", "兵"};

	step += Name[id];     // 拷贝棋子名称

	// 拷贝棋子所在列的值
	if (CURRENT_RED == side) 
		step += RedNum[Move_FromX];  
	else 
		step += BlackN[Move_FromX];

	// 拷贝移动方式
	if (Move_FromY == Move_ToY) 
	{
		step += "平";
	}
	else if (CURRENT_RED == side) 
	{
		if (Move_FromY > Move_ToY) 
		{
			step += "进";
		}
		else 
		{
			step += "退";
		}
	}
	else
	{
		if (Move_FromY < Move_ToY)
		{
			step += "进";
		}
		else
		{
			step += "退";
		}
	}

	int target;
	// 拷贝移动的值
	switch(id)
	{
	case 1:
	case 2:		
	case 4:	
	case 7:
	case 8:
	case 9:		
	case 11:		
	case 14:                   // 将、卒、車、炮走直线的情况
		if (Move_FromY == Move_ToY)
		{
			target = Move_ToX; 
		}
		else 
		{
			if (CURRENT_RED == side) 
				target =9 - abs(Move_FromY - Move_ToY);
			else target = abs(Move_FromY - Move_ToY) -1;
		}
		break;			 
	case 3:
	case 5:
	case 6:
	case 10:
	case 12:
	case 13:                  // 士、象、马走斜线的情况
		target = Move_ToX; 
		break;
	}

	if (CURRENT_RED == side) 
	{
		step += RedNum[target]; 
	}
	else 
	{
		step += BlackN[target];
	}
	step += "　　　　　";
	m_QipuCount++;
	CString ChessMethod(step.c_str());
	return ChessMethod;
}

void CBoard::SetGameMode(int mode)
{
	if (GAMEDEMO == mode)
	{
		m_pGame = new CGameDemo(this);
	}
	else if(GAMEONE == mode)
	{
		m_pGame = new CGameOne(this);
	}
	else 
	{
		m_pGame = new CGameTwo(this);
	}
	m_pGame->init();
}


void CBoard::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x,y;

	// 游戏模式
	m_gameMode = FALSE;

	//将坐标换算成棋盘上的格子
	y = (point.y - 14) / GRILLELENGTH;
	x = (point.x - 15) / GRILLELENGTH;

	if (m_bWait | m_isOver)
	{
		MessageBeep(MB_OK);
	}
	else
	{
		// 如果红方走棋
		if (CURRENT_RED == m_current_side)
		{
			if (IsRed(m_ChessBoard[y][x]))
			{
				m_gameMode = TRUE;
				PlaySound(LPCTSTR(IDR_WAVE_SELECT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
			else m_gameMode = FALSE;
		}
		// 如果黑方走棋
		else 
		{
			if (IsBlack(m_ChessBoard[y][x]))
			{
				m_gameMode = TRUE;
				PlaySound(LPCTSTR(IDR_WAVE_SELECT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
			else m_gameMode = FALSE;
		}
	}

	//备份当前棋盘
	memcpy(m_BackupChessBoard,m_ChessBoard,90);

	//判断鼠标是否在棋盘内,并且点中了当前一方棋子
	if(point.x > 0 && point.y > 0                  
		&& point.x < m_nBoardWidth
		&& point.y < m_nBoardHeight    // 光标在棋盘内
		&& m_gameMode)                   // 选择正确颜色的棋子
	{
		//备份棋盘
		memcpy(m_BackupChessBoard,m_ChessBoard,90);

		m_ptMoveChess.x = x;
		m_ptMoveChess.y = y;

		m_MoveChess.nChessID = m_ChessBoard[y][x];

		//将该棋子原位置棋子去掉
		m_ChessBoard[m_ptMoveChess.y][m_ptMoveChess.x] = NOCHESS;

		//让棋子中点坐标位于鼠标所在点
		point.x -= 20;
		point.y -= 20;
		m_MoveChess.ptMovePoint = point;

		//重绘屏幕
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		SetCapture();
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CBoard::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_MoveChess.nChessID)
	{
		//防止将棋子拖出棋盘
		if(point.x < 15) //左面
			point.x = 15;
		if(point.y < 15) //上面
			point.y = 15;
		if(point.x > m_nBoardWidth -15) //右边
			point.x = m_nBoardWidth -15;
		if(point.y > m_nBoardHeight - 15) //下面
			point.y = m_nBoardHeight - 15;

		//让棋子的中心位于鼠标所在处
		point.x -= 20;
		point.y -= 20;

		//保存移动棋子的坐标
		m_MoveChess.ptMovePoint = point;

		//刷新窗口
		InvalidateRect(NULL,FALSE);

		//立即刷新
		UpdateWindow();
	}
	CWnd::OnMouseMove(nFlags, point);
}


void CBoard::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 棋子在棋盘数组中的坐标
	int x,y;

	//将坐标换算成棋盘上的格子
	y = (point.y - 9) / GRILLELENGTH;
	x = (point.x - 10) / GRILLELENGTH;

	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();

	//判断是否有移动棋子,并且该棋子的移动是一个合法走法
	if(m_MoveChess.nChessID && CMoveGenerator::IsValidMove(m_BackupChessBoard,
		m_ptMoveChess.x,m_ptMoveChess.y,x,y))
	{
		pDlg->GetDlgItem(IDC_BACK)->EnableWindow(m_pGame->m_ChessStep.size() > 1);
		pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(m_pGame->m_ChessStep.size() > 0);
		m_px = x * GRILLELENGTH + BORDERWIDTH;
		m_py = y * GRILLELENGTH + BORDERHEIGHT;
		CHESSSTEP OneStep = 
		{
			m_BackupChessBoard[m_ptMoveChess.y][m_ptMoveChess.x],
			m_ChessBoard[y][x],
			{m_ptMoveChess.x,m_ptMoveChess.y},
			{x,y}
		};

		CString qipu = GenerateChessMethod(m_current_side, 
			m_MoveChess.nChessID, m_ptMoveChess.x,m_ptMoveChess.y,x,y);	

		m_pGame->m_ChessStep.push(OneStep);
		// 画个方框把移动的棋子框起来

		if (m_ChessBoard[y][x] == NOCHESS)
		{
			PlaySound(LPCTSTR(IDR_WAVE_GO), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}
		else 
		{
			PlaySound(LPCTSTR(IDR_WAVE_EAT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}

		// 正在走的一步棋，起点棋A到目标棋B
		m_ChessBoard[y][x] = m_MoveChess.nChessID;

		// 输出棋谱列表
		pDlg->m_OutputInfo.InsertString(-1,qipu);
		int nSelect = pDlg->m_OutputInfo.GetCount() - 1;
		pDlg->m_OutputInfo.SetCurSel(nSelect);
		pDlg->m_OutputInfo.UpdateWindow();
	
		//将移动的棋子清空
		m_MoveChess.nChessID = NOCHESS;
		//重绘屏幕
		InvalidateRect(NULL, FALSE);
		UpdateWindow();
		//释放鼠标焦点
		ReleaseCapture();

		if (IsGameOver(m_ChessBoard, m_current_side))
		{
			MessageBox(_T("绝杀！"));
			if (CURRENT_RED == m_current_side)
			{
				MessageBox(_T("红方胜！"));
			}
			else
			{
				MessageBox(_T("黑方胜！"));
			}
			m_gameMode = FALSE;
			m_isOver = TRUE;
			pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
			pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
		}
		else if (IsGameOver(m_ChessBoard))
		{	
			m_gameMode = FALSE;
			m_isOver = TRUE;
			pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
			pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
		}	
		// 如果成功移动了红棋,那么下一步走黑棋,反之亦然
		if (CURRENT_RED == m_current_side)
		{
			m_current_side = CURRENT_BLACK;
		}
		else 
		{
			m_current_side = CURRENT_RED;
		}	
		m_pGame->SendStep(OneStep, qipu.GetBuffer(), m_QipuCount);
	}
	else
	{
		//恢复移动前的棋盘状态
		memcpy(m_ChessBoard,m_BackupChessBoard,90);
	}

	//将移动的棋子清空
	m_MoveChess.nChessID = NOCHESS;
	//重绘屏幕
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
	//释放鼠标焦点
	ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);
}

// 判断是否将军
BOOL CBoard::IsJiangJun(BYTE position[10][9], int m_current_side)
{
	int targetx1 = 100,targetx2 = 100,targety1 = 100,targety2 = 100;
	for (int i = 3; i <= 5; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			if (position[j][i] == B_KING || position[j][i] == R_KING)
			{
				targetx1 = i;
				targety1 = j;
				break;
			}
		}
	}
	for (int i = 3; i <= 5; i++)
	{
		for (int j = 7; j <= 9; j++)
		{
			if (position[j][i] == B_KING || position[j][i] == R_KING)
			{
				targetx2 = i;
				targety2 = j;
				break;
			}
		}
	}
	
	if (targety1 != 100 || targetx2 != 100)
	{
		int toX = 100;
		int toY = 100;
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (m_current_side && IsRed(position[j][i]))
				{
					toX = i;
					toY = j;
				}
				else if (!m_current_side && IsBlack(position[j][i]))
				{
					toX = i;
					toY = j;
				}
				if (toX != 100 && toY != 100)
				{
					if(CMoveGenerator::IsValidMove(position, toX, toY, targetx1, targety1)
						|| CMoveGenerator::IsValidMove(position, toX, toY, targetx2, targety2))
					{
						return TRUE;
					}
				}		
			}
		}
	}
	return FALSE;
}

BOOL CBoard::IsGameOver(BYTE position[][9])
{
	int i,j;
	BOOL RedLive = FALSE,BlackLive = FALSE;

	//检查红方九宫是否有将帅
	for(i = 7;i < 10; i++)
	{
		for(j = 3; j < 6; j++)
		{
			if(position[i][j] == B_KING)
				BlackLive = TRUE;
			if(position[i][j] == R_KING)
				RedLive = TRUE;
		}
	}

	//检查黑方九宫是否有将帅
	for(i = 0; i < 3; i++)
	{
		for(j = 3; j < 6; j++)
		{
			if(position[i][j] == B_KING)
				BlackLive = TRUE;
			if(position[i][j] == R_KING)
				RedLive = TRUE;
		}
	}

	if(!RedLive)	//红将是否不在了
	{
		PlaySound(LPCTSTR(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		MessageBox(_T("黑方胜！"));	
		return TRUE;
	}
	if(!BlackLive)
	{
		PlaySound(LPCTSTR(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		MessageBox(_T("红方胜！"));	 
		return TRUE;
	}
	return FALSE;//两个将都在,返回0
}

BOOL CBoard::IsGameOver(BYTE position[][9], int m_current_side)
{
	if (IsJiangJun(m_ChessBoard, m_current_side))
	{
		PlaySound(LPCTSTR(IDR_WAVE_TODIE), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		BOOL bJueSha = TRUE;
		int icount = m_pGame->m_pTempMG->CreatePossibleMove(m_ChessBoard, 1, !m_current_side);
		for (int i = 0; i < icount; ++i)
		{
			CHESSMOVE* move = &m_pGame->m_pTempMG->m_MoveList[1][i];
			BYTE aChessBoard[10][9];
			memcpy(aChessBoard, m_ChessBoard, 90);
			move->ChessID = aChessBoard[move->From.y][move->From.x];
			BYTE	nChessID;
			//取目标位置的棋子
			nChessID = aChessBoard[move->To.y][move->To.x];
			//把棋子移动到目标位置上
			aChessBoard[move->To.y][move->To.x] = 
				aChessBoard[move->From.y][move->From.x];
			//将原位置清空
			aChessBoard[move->From.y][move->From.x] = NOCHESS;
			if(!IsJiangJun(aChessBoard, m_current_side))
			{
				bJueSha = FALSE;
				break;
			}
		}
		if (bJueSha)
		{
			PlaySound(LPCTSTR(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			return TRUE;
		}
	}
	return FALSE;
}

void CBoard::Receive()
{
	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();
	MSGSTRUCT msgRecv;
	int nRet = m_conn.Receive(&msgRecv, sizeof(MSGSTRUCT));
	if (SOCKET_ERROR == nRet)
	{
		pDlg->MessageBox(_T("接收数据时发生错误，请检查您的网络连接。"), _T("错误"), MB_ICONSTOP);
	}	
	// 对各种消息分别进行处理
	switch (msgRecv.uMsg)
	{
	case MSG_PUTSTEP:
		{
			if (m_ChessBoard[msgRecv.oneStep.To.y][msgRecv.oneStep.To.x] == NOCHESS)
			{
				PlaySound(LPCTSTR(IDR_WAVE_GO), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
			else 
			{
				PlaySound(LPCTSTR(IDR_WAVE_EAT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}

			m_pGame->m_ChessStep.push(msgRecv.oneStep);
			// 绘制移动棋子的外边框
			m_px = msgRecv.oneStep.To.x * GRILLELENGTH + BORDERWIDTH;
			m_py = msgRecv.oneStep.To.y * GRILLELENGTH + BORDERHEIGHT;

			// 正在走的一步棋，起点棋A到目标棋B
			m_ChessBoard[msgRecv.oneStep.To.y][msgRecv.oneStep.To.x] = msgRecv.oneStep.FromID;
			//将移动的棋子清空
			m_ChessBoard[msgRecv.oneStep.From.y][msgRecv.oneStep.From.x] = NOCHESS;
			CString cstrStep = msgRecv.szStep;
			pDlg->m_OutputInfo.InsertString(-1, cstrStep);
			int nSelect = pDlg->m_OutputInfo.GetCount() - 1;
			pDlg->m_OutputInfo.SetCurSel(nSelect);
			m_QipuCount = msgRecv.nMethodCount;

			// 判断是否将军或者绝杀或者将帅被吃
			if (IsGameOver(m_ChessBoard, m_current_side))
			{
				MessageBox(_T("绝杀！"));
				if (CURRENT_RED == m_current_side)
				{
					MessageBox(_T("红方胜！"));
				}
				else
				{
					MessageBox(_T("黑方胜！"));
				}
				m_gameMode = FALSE;
				m_isOver = TRUE;
				pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
				pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
				pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
				pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
				break;
			}
			else if (IsGameOver(m_ChessBoard))
			{	
				m_gameMode = FALSE;
				m_isOver = TRUE;
				pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
				pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
				pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
				pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
				break;
			}	

			InvalidateRect(NULL,FALSE);
			UpdateWindow();

			if (CURRENT_RED == m_current_side)
			{
				m_current_side = CURRENT_BLACK;
			}
			else m_current_side = CURRENT_RED;
			m_bWait = FALSE;
			pDlg->GetDlgItem(IDC_BACK)->EnableWindow(TRUE);
		}
		break;
	case MSG_CHAT:
		{
			CString strAdd;
			strAdd.Format(_T("对方说：%s\r\n"), msgRecv.szMsg);
			pDlg->m_ChatList.InsertString(-1, strAdd);
			int iChatListLen = pDlg->m_ChatList.GetCount();
		}
		break;
	case MSG_BACK:
		{
			if (IDYES == MessageBox(_T("对方请求悔棋，接受这个请求吗？"),
				_T("悔棋"), MB_ICONQUESTION | MB_YESNO))
			{
				// 发送允许悔棋消息
				MSGSTRUCT msg;
				msg.uMsg = MSG_AGREEBACK;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
				// 给自己悔棋
				if (m_pGame->m_ChessStep.size() > 1)
				{
					for (int i = 0; i < 2; i++)
					{
						--m_QipuCount;
						CHESSSTEP PopStep = m_pGame->m_ChessStep.top();
						m_pGame->m_ChessStep.pop();
						m_ChessBoard[PopStep.From.y][PopStep.From.x] = PopStep.FromID;
						m_ChessBoard[PopStep.To.y][PopStep.To.x] = PopStep.ToID;

						// 绘制移动棋子的外边框
						m_px = PopStep.From.x * GRILLELENGTH + BORDERWIDTH;
						m_py = PopStep.From.y * GRILLELENGTH + BORDERHEIGHT;

						CString QIPUSTEP;
						pDlg->m_OutputInfo.GetText(pDlg->m_OutputInfo.GetCount() - 1,QIPUSTEP);
						m_pGame->m_QiPuStep.push_front(QIPUSTEP);
						pDlg->m_OutputInfo.DeleteString(pDlg->m_OutputInfo.GetCount() - 1);	
					}			
				}
				InvalidateRect(NULL,FALSE);
				UpdateWindow();
			}
			else
			{
				// 发送不允许悔棋消息
				MSGSTRUCT msg;
				msg.uMsg = MSG_REFUSEBACK;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
			}
		}
		break;
	case MSG_REFUSEBACK:
		{
			pDlg->MessageBox(_T("很抱歉，对方拒绝了您的悔棋请求。"), _T("悔棋"), MB_ICONINFORMATION);
		}
		break;
	case MSG_AGREEBACK:
		{
			if (m_pGame->m_ChessStep.size() > 1)
			{
				for (int i = 0; i < 2; i++)
				{
					--m_QipuCount;
					CHESSSTEP PopStep = m_pGame->m_ChessStep.top();
					m_pGame->m_ChessStep.pop();
					m_ChessBoard[PopStep.From.y][PopStep.From.x] = PopStep.FromID;
					m_ChessBoard[PopStep.To.y][PopStep.To.x] = PopStep.ToID;

					// 绘制移动棋子的外边框
					m_px = PopStep.From.x * GRILLELENGTH + BORDERWIDTH;
					m_py = PopStep.From.y * GRILLELENGTH + BORDERHEIGHT;

					CString QIPUSTEP;
					pDlg->m_OutputInfo.GetText(pDlg->m_OutputInfo.GetCount() - 1,QIPUSTEP);
					m_pGame->m_QiPuStep.push_front(QIPUSTEP);
					pDlg->m_OutputInfo.DeleteString(pDlg->m_OutputInfo.GetCount() - 1);
				}			
			}
			InvalidateRect(NULL,FALSE);
			UpdateWindow();
		}
		break;
	case MSG_DRAW:
		{
			if (IDYES == GetParent()->MessageBox(_T("对方请求和棋，接受这个请求吗？"),
				_T("和棋"), MB_ICONQUESTION | MB_YESNO))
			{
				// 发送允许和棋消息
				MSGSTRUCT msg;
				msg.uMsg = MSG_AGREEDRAW;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
				m_isOver = TRUE;
				pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
				pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
				pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
				pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
			}
			else
			{
				// 发送拒绝和棋消息
				MSGSTRUCT msg;
				msg.uMsg = MSG_REFUSEDRAW;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
			}
		}
		break;
	case MSG_AGREEDRAW:
		{
			pDlg->MessageBox(_T("看来真是棋逢对手，对方接受了您的和棋请求。"), _T("和棋"), MB_ICONINFORMATION);
			m_isOver = TRUE;
			pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
			pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
		}
		break;
	case MSG_REFUSEDRAW:
		{
			pDlg->MessageBox(_T("看来对方很有信心取得胜利，所以拒绝了您的和棋请求。"),
				_T("和棋"), MB_ICONINFORMATION);
			m_bWait = FALSE;
		}
		break;
	case MSG_GIVEUP:
		{        
			pDlg->MessageBox(_T("对方已经投子认输，恭喜您不战而屈人之兵！"), _T("胜利"), MB_ICONINFORMATION);
			m_isOver = TRUE;
			pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
			pDlg->GetMenu()->EnableMenuItem(ID_AGAIN, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_DISCONNECT, MF_ENABLED);
			pDlg->GetMenu()->EnableMenuItem(ID_OPENDEMO, MF_ENABLED);
		}
		break;
	case MSG_PLAYAGAIN:
		{
			if (IDYES == pDlg->MessageBox(_T("对方看来意犹未尽，请求与您再战一局，接受这个请求吗？\n\n选“否”将断开与他的连接。"),
				_T("再战"), MB_YESNO | MB_ICONQUESTION))
			{
				pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_DRAW)->EnableWindow();
				pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow();
				MSGSTRUCT msg;
				msg.uMsg = MSG_AGREEAGAIN;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
				SetGameMode(GAMETWO);
				m_bWait = FALSE;
				InvalidateRect(NULL, FALSE);
				UpdateWindow();
			}
			else
			{
				m_conn.Close();
				m_sock.Close();
				pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_DRAW)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
				pDlg->GetDlgItem(IDC_LIST_CHAT)->EnableWindow(FALSE);
				// 设置菜单状态
				m_bWait = TRUE;
				m_bConnected = FALSE;
			}
		}
		break;
	case MSG_AGREEAGAIN:
		{
			pDlg->GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
			pDlg->GetDlgItem(IDC_DRAW)->EnableWindow();
			pDlg->GetDlgItem(IDC_GIVEUP)->EnableWindow();
			SetGameMode(GAMETWO);
			m_bWait = TRUE;
			InvalidateRect(NULL, FALSE);
			UpdateWindow();
		}
		break;
	}
}

void CBoard::PlayAgain(void)
{
	MSGSTRUCT msg;
	msg.uMsg = MSG_PLAYAGAIN;
	m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
}
