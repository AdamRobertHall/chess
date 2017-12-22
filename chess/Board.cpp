// Board.cpp : ʵ���ļ�
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

	//�������ȡ���̵Ŀ�,��
	BITMAP	BitMap;
	m_BoardBmp.LoadBitmap(IDB_QIPAN);
	//ȡBitMap����
	m_BoardBmp.GetBitmap(&BitMap);
	//���̿��
	m_nBoardWidth = BitMap.bmWidth;
	//���̸߶�
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



// CBoard ��Ϣ�������
void CBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
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
	//�����û������϶�������
	if (m_MoveChess.nChessID != NOCHESS)
		m_ChessList.Draw(&MemDC, m_MoveChess.nChessID - 1, 
		m_MoveChess.ptMovePoint, ILD_TRANSPARENT);
	//����������ƶ������ӿ�����
	if (m_px && m_py)
	{
		MemDC.MoveTo(m_px, m_py);
		MemDC.LineTo(m_px + GRILLELENGTH, m_py);
		MemDC.LineTo(m_px + GRILLELENGTH, m_py + GRILLELENGTH);
		MemDC.LineTo(m_px, m_py + GRILLELENGTH);
		MemDC.LineTo(m_px, m_py);
	}	

	//����������ˢ�µ���Ļ
	dc.BitBlt(0,0,m_nBoardWidth,m_nBoardHeight,&MemDC,0,0,SRCCOPY);
	//�ָ��ڴ�DC��ԭλͼ
	MemDC.SelectObject(&pOldBmp);
	//�ͷ��ڴ�DC
	MemDC.DeleteDC();
	//ɾ������λͼ����
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
		step = "    ��";
	}

	char RedNum[9][3] = {"��","��","��","��","��","��","��","��","һ"};
	char BlackN[9][3] = {"1","��","��","��","��","��","��","��","��"};
	char Name[15][3]  = {"", "��", "܇", "��", "��", "ʿ", "��", "��", 
		"˧", "܇", "��", "��", "��", "��", "��"};

	step += Name[id];     // ������������

	// �������������е�ֵ
	if (CURRENT_RED == side) 
		step += RedNum[Move_FromX];  
	else 
		step += BlackN[Move_FromX];

	// �����ƶ���ʽ
	if (Move_FromY == Move_ToY) 
	{
		step += "ƽ";
	}
	else if (CURRENT_RED == side) 
	{
		if (Move_FromY > Move_ToY) 
		{
			step += "��";
		}
		else 
		{
			step += "��";
		}
	}
	else
	{
		if (Move_FromY < Move_ToY)
		{
			step += "��";
		}
		else
		{
			step += "��";
		}
	}

	int target;
	// �����ƶ���ֵ
	switch(id)
	{
	case 1:
	case 2:		
	case 4:	
	case 7:
	case 8:
	case 9:		
	case 11:		
	case 14:                   // �����䡢܇������ֱ�ߵ����
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
	case 13:                  // ʿ��������б�ߵ����
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
	step += "����������";
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int x,y;

	// ��Ϸģʽ
	m_gameMode = FALSE;

	//�����껻��������ϵĸ���
	y = (point.y - 14) / GRILLELENGTH;
	x = (point.x - 15) / GRILLELENGTH;

	if (m_bWait | m_isOver)
	{
		MessageBeep(MB_OK);
	}
	else
	{
		// ����췽����
		if (CURRENT_RED == m_current_side)
		{
			if (IsRed(m_ChessBoard[y][x]))
			{
				m_gameMode = TRUE;
				PlaySound(LPCTSTR(IDR_WAVE_SELECT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
			else m_gameMode = FALSE;
		}
		// ����ڷ�����
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

	//���ݵ�ǰ����
	memcpy(m_BackupChessBoard,m_ChessBoard,90);

	//�ж�����Ƿ���������,���ҵ����˵�ǰһ������
	if(point.x > 0 && point.y > 0                  
		&& point.x < m_nBoardWidth
		&& point.y < m_nBoardHeight    // �����������
		&& m_gameMode)                   // ѡ����ȷ��ɫ������
	{
		//��������
		memcpy(m_BackupChessBoard,m_ChessBoard,90);

		m_ptMoveChess.x = x;
		m_ptMoveChess.y = y;

		m_MoveChess.nChessID = m_ChessBoard[y][x];

		//��������ԭλ������ȥ��
		m_ChessBoard[m_ptMoveChess.y][m_ptMoveChess.x] = NOCHESS;

		//�������е�����λ��������ڵ�
		point.x -= 20;
		point.y -= 20;
		m_MoveChess.ptMovePoint = point;

		//�ػ���Ļ
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		SetCapture();
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CBoard::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_MoveChess.nChessID)
	{
		//��ֹ�������ϳ�����
		if(point.x < 15) //����
			point.x = 15;
		if(point.y < 15) //����
			point.y = 15;
		if(point.x > m_nBoardWidth -15) //�ұ�
			point.x = m_nBoardWidth -15;
		if(point.y > m_nBoardHeight - 15) //����
			point.y = m_nBoardHeight - 15;

		//�����ӵ�����λ��������ڴ�
		point.x -= 20;
		point.y -= 20;

		//�����ƶ����ӵ�����
		m_MoveChess.ptMovePoint = point;

		//ˢ�´���
		InvalidateRect(NULL,FALSE);

		//����ˢ��
		UpdateWindow();
	}
	CWnd::OnMouseMove(nFlags, point);
}


void CBoard::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ���������������е�����
	int x,y;

	//�����껻��������ϵĸ���
	y = (point.y - 9) / GRILLELENGTH;
	x = (point.x - 10) / GRILLELENGTH;

	CchessDlg* pDlg = (CchessDlg*)AfxGetMainWnd();

	//�ж��Ƿ����ƶ�����,���Ҹ����ӵ��ƶ���һ���Ϸ��߷�
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
		// ����������ƶ������ӿ�����

		if (m_ChessBoard[y][x] == NOCHESS)
		{
			PlaySound(LPCTSTR(IDR_WAVE_GO), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}
		else 
		{
			PlaySound(LPCTSTR(IDR_WAVE_EAT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}

		// �����ߵ�һ���壬�����A��Ŀ����B
		m_ChessBoard[y][x] = m_MoveChess.nChessID;

		// ��������б�
		pDlg->m_OutputInfo.InsertString(-1,qipu);
		int nSelect = pDlg->m_OutputInfo.GetCount() - 1;
		pDlg->m_OutputInfo.SetCurSel(nSelect);
		pDlg->m_OutputInfo.UpdateWindow();
	
		//���ƶ����������
		m_MoveChess.nChessID = NOCHESS;
		//�ػ���Ļ
		InvalidateRect(NULL, FALSE);
		UpdateWindow();
		//�ͷ���꽹��
		ReleaseCapture();

		if (IsGameOver(m_ChessBoard, m_current_side))
		{
			MessageBox(_T("��ɱ��"));
			if (CURRENT_RED == m_current_side)
			{
				MessageBox(_T("�췽ʤ��"));
			}
			else
			{
				MessageBox(_T("�ڷ�ʤ��"));
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
		// ����ɹ��ƶ��˺���,��ô��һ���ߺ���,��֮��Ȼ
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
		//�ָ��ƶ�ǰ������״̬
		memcpy(m_ChessBoard,m_BackupChessBoard,90);
	}

	//���ƶ����������
	m_MoveChess.nChessID = NOCHESS;
	//�ػ���Ļ
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
	//�ͷ���꽹��
	ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);
}

// �ж��Ƿ񽫾�
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

	//���췽�Ź��Ƿ��н�˧
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

	//���ڷ��Ź��Ƿ��н�˧
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

	if(!RedLive)	//�콫�Ƿ�����
	{
		PlaySound(LPCTSTR(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		MessageBox(_T("�ڷ�ʤ��"));	
		return TRUE;
	}
	if(!BlackLive)
	{
		PlaySound(LPCTSTR(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		MessageBox(_T("�췽ʤ��"));	 
		return TRUE;
	}
	return FALSE;//����������,����0
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
			//ȡĿ��λ�õ�����
			nChessID = aChessBoard[move->To.y][move->To.x];
			//�������ƶ���Ŀ��λ����
			aChessBoard[move->To.y][move->To.x] = 
				aChessBoard[move->From.y][move->From.x];
			//��ԭλ�����
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
		pDlg->MessageBox(_T("��������ʱ�����������������������ӡ�"), _T("����"), MB_ICONSTOP);
	}	
	// �Ը�����Ϣ�ֱ���д���
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
			// �����ƶ����ӵ���߿�
			m_px = msgRecv.oneStep.To.x * GRILLELENGTH + BORDERWIDTH;
			m_py = msgRecv.oneStep.To.y * GRILLELENGTH + BORDERHEIGHT;

			// �����ߵ�һ���壬�����A��Ŀ����B
			m_ChessBoard[msgRecv.oneStep.To.y][msgRecv.oneStep.To.x] = msgRecv.oneStep.FromID;
			//���ƶ����������
			m_ChessBoard[msgRecv.oneStep.From.y][msgRecv.oneStep.From.x] = NOCHESS;
			CString cstrStep = msgRecv.szStep;
			pDlg->m_OutputInfo.InsertString(-1, cstrStep);
			int nSelect = pDlg->m_OutputInfo.GetCount() - 1;
			pDlg->m_OutputInfo.SetCurSel(nSelect);
			m_QipuCount = msgRecv.nMethodCount;

			// �ж��Ƿ񽫾����߾�ɱ���߽�˧����
			if (IsGameOver(m_ChessBoard, m_current_side))
			{
				MessageBox(_T("��ɱ��"));
				if (CURRENT_RED == m_current_side)
				{
					MessageBox(_T("�췽ʤ��"));
				}
				else
				{
					MessageBox(_T("�ڷ�ʤ��"));
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
			strAdd.Format(_T("�Է�˵��%s\r\n"), msgRecv.szMsg);
			pDlg->m_ChatList.InsertString(-1, strAdd);
			int iChatListLen = pDlg->m_ChatList.GetCount();
		}
		break;
	case MSG_BACK:
		{
			if (IDYES == MessageBox(_T("�Է�������壬�������������"),
				_T("����"), MB_ICONQUESTION | MB_YESNO))
			{
				// �������������Ϣ
				MSGSTRUCT msg;
				msg.uMsg = MSG_AGREEBACK;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
				// ���Լ�����
				if (m_pGame->m_ChessStep.size() > 1)
				{
					for (int i = 0; i < 2; i++)
					{
						--m_QipuCount;
						CHESSSTEP PopStep = m_pGame->m_ChessStep.top();
						m_pGame->m_ChessStep.pop();
						m_ChessBoard[PopStep.From.y][PopStep.From.x] = PopStep.FromID;
						m_ChessBoard[PopStep.To.y][PopStep.To.x] = PopStep.ToID;

						// �����ƶ����ӵ���߿�
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
				// ���Ͳ����������Ϣ
				MSGSTRUCT msg;
				msg.uMsg = MSG_REFUSEBACK;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
			}
		}
		break;
	case MSG_REFUSEBACK:
		{
			pDlg->MessageBox(_T("�ܱ�Ǹ���Է��ܾ������Ļ�������"), _T("����"), MB_ICONINFORMATION);
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

					// �����ƶ����ӵ���߿�
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
			if (IDYES == GetParent()->MessageBox(_T("�Է�������壬�������������"),
				_T("����"), MB_ICONQUESTION | MB_YESNO))
			{
				// �������������Ϣ
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
				// ���;ܾ�������Ϣ
				MSGSTRUCT msg;
				msg.uMsg = MSG_REFUSEDRAW;
				m_conn.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
			}
		}
		break;
	case MSG_AGREEDRAW:
		{
			pDlg->MessageBox(_T("�������������֣��Է����������ĺ�������"), _T("����"), MB_ICONINFORMATION);
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
			pDlg->MessageBox(_T("�����Է���������ȡ��ʤ�������Ծܾ������ĺ�������"),
				_T("����"), MB_ICONINFORMATION);
			m_bWait = FALSE;
		}
		break;
	case MSG_GIVEUP:
		{        
			pDlg->MessageBox(_T("�Է��Ѿ�Ͷ�����䣬��ϲ����ս������֮����"), _T("ʤ��"), MB_ICONINFORMATION);
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
			if (IDYES == pDlg->MessageBox(_T("�Է���������δ��������������սһ�֣��������������\n\nѡ���񡱽��Ͽ����������ӡ�"),
				_T("��ս"), MB_YESNO | MB_ICONQUESTION))
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
				// ���ò˵�״̬
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
