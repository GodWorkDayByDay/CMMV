#pragma once
#include "afxwin.h"


// CDialogUserLog �Ի���

class CDialogUserLog : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogUserLog)

public:
	CDialogUserLog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogUserLog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit ID_UserName;
	CEdit ID_Password;
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	//afx_msg void OnBnSetfocusOk();
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedUserManage();
};
