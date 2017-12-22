#pragma once


// CServerDlg 对话框

class CServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerDlg)

public:
	CServerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServerDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedBtnOut();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
