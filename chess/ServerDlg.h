#pragma once


// CServerDlg �Ի���

class CServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerDlg)

public:
	CServerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServerDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedBtnOut();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
