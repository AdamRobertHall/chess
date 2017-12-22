
// chessDlg.h : ͷ�ļ�
//

#pragma once
#include "board.h"
#include "afxwin.h"


// CchessDlg �Ի���
class CchessDlg : public CDialogEx
{
// ����
public:
	CchessDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ������
	CBoard m_Board;
	// �����սʱ����˶Ի���ָ��
	CDialog* m_pDlg;
public:
	afx_msg void OnAboutgame();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedDraw();
	afx_msg void OnBnClickedGiveup();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedDemoend();
	afx_msg void OnPlayerfirst();
	afx_msg void OnComputerfirst();
	afx_msg void OnAgain();
	afx_msg void OnDisconnect();
	afx_msg void OnNetgameenter();
	afx_msg void OnNetgamesetup();
	afx_msg void OnOpendemo();
	afx_msg void OnQuit();
	afx_msg void OnSavedemo();
	CListBox m_OutputInfo;
	CMenu* pSysMenu;
	CListBox m_ChatList;
	virtual void OnOK();
	CEdit m_EditChat;
	afx_msg void OnBnClickedBtnSend();
};
