#pragma once


// CClientDlg 对话框

class CClientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClientDlg)

public:
	CClientDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnUpdateEditHost();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnOut();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	int m_nTimer;
	virtual void OnOK();
};
