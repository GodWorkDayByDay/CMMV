#pragma once
#include "afxwin.h"




// DlgUserMange �Ի���

class DlgUserManage : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUserManage)

public:
	DlgUserManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgUserManage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_User_Manage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboUserlist();
	CComboBox CbUserList;
	CEdit CeUserName;
	CEdit CeRegTime;
	CEdit CeLogTime;
	CEdit CeUserNote;
	CButton CbIsAdminstrator;
	CButton CbIsInspector;
	CEdit CeOrgPassword;
	CEdit CeNewPass;
	CEdit CeRepeatPass;
	afx_msg void OnBnClickedChangepass();
	afx_msg void OnBnClickedAddUser();
	afx_msg void OnBnClickedDeleteUser();
};
