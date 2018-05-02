// DlgUserMange.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMV.h"
#include "DlgUserMange.h"
#include "afxdialogex.h"
#include "MyData.h"

// DlgUserMange �Ի���

IMPLEMENT_DYNAMIC(DlgUserManage, CDialogEx)

DlgUserManage::DlgUserManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_User_Manage, pParent)
{

}

DlgUserManage::~DlgUserManage()
{
}

void DlgUserManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UserList, CbUserList);
	DDX_Control(pDX, IDC_User_Name, CeUserName);
	DDX_Control(pDX, IDC_User_RegTime, CeRegTime);
	DDX_Control(pDX, IDC_User_LastLogTime, CeLogTime);
	DDX_Control(pDX, IDC_User_Note, CeUserNote);
	DDX_Control(pDX, IDC_CHECK_isAdmin, CbIsAdminstrator);
	DDX_Control(pDX, IDC_CHECK_isInpector, CbIsInspector);
	DDX_Control(pDX, IDC_Org_Password, CeOrgPassword);
	DDX_Control(pDX, IDC_New_Password, CeNewPass);
	DDX_Control(pDX, IDC_Repeat_Password, CeRepeatPass);
}


BEGIN_MESSAGE_MAP(DlgUserManage, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgUserManage::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_UserList, &DlgUserManage::OnCbnSelchangeComboUserlist)
	ON_BN_CLICKED(ID_ChangePass, &DlgUserManage::OnBnClickedChangepass)
	ON_BN_CLICKED(IDOK_ADD_User, &DlgUserManage::OnBnClickedAddUser)
	ON_BN_CLICKED(ID_Delete_User, &DlgUserManage::OnBnClickedDeleteUser)
END_MESSAGE_MAP()


// DlgUserMange ��Ϣ�������


void DlgUserManage::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������



	CDialogEx::OnOK();
}


BOOL DlgUserManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CList<CString, CString> User;

	UCI::sDB.MyFillUser(&User);    // �õ��û��б�


	POSITION pos = User.GetHeadPosition();
	while (pos != NULL) {
		CString stName = User.GetNext(pos);
		this->CbUserList.AddString(stName);
	}

	// 


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void DlgUserManage::OnCbnSelchangeComboUserlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UCI::ST_USER stUser;
	this->CbUserList.GetLBText(this->CbUserList.GetCurSel(), stUser.Name);	

	UCI::sDB.GetUserInfoByName(&stUser);

	this->CeUserName.SetWindowText(stUser.Name);

	this->CeLogTime.SetWindowText(stUser.stLastLogTime);
	this->CeRegTime.SetWindowText(stUser.stRegTime);
	this->CeUserNote.SetWindowText(stUser.Note);

	if (stUser.Permission > 1) {
		this->CbIsAdminstrator.SetCheck(TRUE);
	}
}


void DlgUserManage::OnBnClickedChangepass()
{
	UCI::ST_USER stUser;

	// �����û������ע
	CString stUserName;
	this->CeUserName.GetWindowText(stUser.Name);

	// ��һ���û����ڲ��� SQLITE_OK;
	

	if (SQLITE_OK != UCI::sDB.GetUserInfoByName(&stUser)) {
		MessageBox(L"�Բ����û������ڣ�");
		return;
	}

	CString Password;
	this->CeOrgPassword.GetWindowText(Password);

	if (Password != stUser.Password) {
		MessageBox(L"�Բ���ԭʼ���׼ȷ��");
		return;
	}

	CString NewPass;
	CString RepeatPass;
	this->CeNewPass.GetWindowText(NewPass);
	this->CeRepeatPass.GetWindowText(RepeatPass);

	if (NewPass.GetLength() > 0) {
		if (NewPass != RepeatPass) {
			MessageBox(L"�Բ����¿������ظ����һ�£�");
			return;
		}
	}

	stUser.Password = NewPass;
	this->CeUserNote.GetWindowText(stUser.Note);

	if (SQLITE_OK == UCI::sDB.UpdateUserInfoBy(&stUser)) {
		MessageBox(L"�����û���Ϣ�ɹ���");
	}
	else {
		MessageBox(L"�����û���Ϣʧ�ܣ�");
	}

}

// ����û�
void DlgUserManage::OnBnClickedAddUser()
{
	// �ж��ǲ�������ͬ�û�����
	UCI::ST_USER stUser;

	// �����û������ע
	this->CeUserName.GetWindowText(stUser.Name);

	if (SQLITE_OK == UCI::sDB.GetUserInfoByName(&stUser)) {
		MessageBox(L"��ͬ���û���ע�ᣬ������û�����");
		return;
	}

	CString NewPass;
	CString RepeatPass;
	this->CeNewPass.GetWindowText(NewPass);
	this->CeRepeatPass.GetWindowText(RepeatPass);

	if (NewPass.GetLength() == 0) {
		MessageBox(L"��������");
		return;
	}
	if (RepeatPass.GetLength() == 0) {
		MessageBox(L"���ظ����");
		return;
	}


	if (NewPass != RepeatPass) {
		MessageBox(L"�������һ�£�������������");
		return;
	}
	
	stUser.Password = NewPass;
	stUser.Permission = this->CbIsAdminstrator.GetCheck();
	this->CeUserNote.GetWindowText(stUser.Note);

	if (SQLITE_OK == UCI::sDB.AddUser(&stUser)) {
		MessageBox(L"�����û��ɹ���");
	}
	else {
		MessageBox(L"�����û�ʧ�ܣ�");
	}
}

// ɾ���û�
void DlgUserManage::OnBnClickedDeleteUser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UCI::ST_USER stUser;

	// �����û������ע
	this->CeUserName.GetWindowText(stUser.Name);

	if (SQLITE_OK == UCI::sDB.DeleteUserByName(&stUser)) {
		MessageBox(L"ɾ���û��ɹ���");
	}
	else {
		MessageBox(L"ɾ���û�ʧ�ܣ�");
	}
}
