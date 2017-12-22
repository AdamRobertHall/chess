
// chessDlg.h : 头文件
//

#pragma once
#include "board.h"
#include "afxwin.h"


// CchessDlg 对话框
class CchessDlg : public CDialogEx
{
// 构造
public:
	CchessDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 棋盘类
	CBoard m_Board;
	// 网络对战时服务端对话框指针
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
