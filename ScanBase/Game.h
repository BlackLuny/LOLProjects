#pragma once


// CGame �Ի���

class CGame : public CDialogEx
{
	DECLARE_DYNAMIC(CGame)

public:
	CGame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGame();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
