#include "stdafx.h"
#include "CMV.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ChildView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ѡ����һ�����
void CMainFrame::OnComboCameraName()
{
	// TODO: �ڴ���������������
	//// 2. �õ� device 	
	//CMFCRibbonComboBox* pCameraComboBox = DYNAMIC_DOWNCAST(
	//	CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_CAMERA_NAME));

	//std::string str = CW2A(pCameraComboBox->GetItem(pCameraComboBox->GetCurSel()));
	//OPTIONS[L"�������"] = str;
}

// ����������ͣ� DirectShow, File ....
void CMainFrame::OnComboCameraConnectType()
{
	// TODO: �ڴ���������������

	//CMFCRibbonComboBox* pCameraComboBox = DYNAMIC_DOWNCAST(
	//	CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_CAMERA_CONNECT_TYPE));
	//
	//std::string st = CW2A(pCameraComboBox->GetItem(pCameraComboBox->GetCurSel()));
	//OPTIONS[L"���Э��"] = st;

}

// ������ʱ
void CMainFrame::OnEditCameraTrigTime()
{
	// TODO: �ڴ���������������

	// 1. �õ��������� Directshow File...
	//CMFCRibbonEdit* pE = DYNAMIC_DOWNCAST(
	//	CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_CAMERA_TRIG_TIME));

	//CString text = pE->GetEditText();
	//int v = _wtoi(text);
	//OPTIONS[L"������ʱ"] = v;	

	//int k = UCI::OPTIONS[L"������ʱ"];
	//int a = 0;
}

// �������ʾģʽ ģ�� RGB
void CMainFrame::OnCheckCameraIsRgb()
{
	// TODO: �ڴ���������������
	//BOOL bRGB = UCI::OPTIONS[L"bRGBmode"];
	//OPTIONS[L"bRGBmode"] = !bRGB;
	
}

void CMainFrame::OnUpdateCheckCameraIsRgb(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//BOOL bRGB = UCI::OPTIONS[L"bRGBmode"];
	//pCmdUI->SetCheck(bRGB);
}

void CMainFrame::OnUpdateCheckCameraSoftTrig(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//BOOL isH = UCI::OPTIONS[L"bIsHardTrigger"];
	//pCmdUI->SetCheck(!isH);
}


// ����Ĵ�����ʽ����
void CMainFrame::OnCheckCameraSoftTrig()
{
	// TODO: �ڴ���������������
	//BOOL isH = UCI::OPTIONS[L"bIsHardTrigger"];
	//OPTIONS[L"bIsHardTrigger"] = !isH;
}

// ������عⷽʽ����
void CMainFrame::OnCheckCameraExposureMode()
{
	// TODO: �ڴ���������������
	//CCamera* pCam = this->GetpDis()->GetCam();
	//UCI::pDis->pCAM->SetExposureMode(!UCI::pDis->pCAM->GetExposureMode());
}

void CMainFrame::OnUpdateCheckCameraExposureMode(CCmdUI *pCmdUI)
{
	// pCmdUI->Enable(this->GetpDis()->pACQU->IsOpened());
	
	// TODO: �ڴ������������û����洦��������
	//CCamera* pCam = this->GetpDis()->GetCam();
	//Sleep(100);
	//pCmdUI->SetCheck(UCI::pDis->pCAM->GetExposureMode());

	
}




void CMainFrame::OnEditCameraGain()
{
	// TODO: �ڴ���������������
	//CMFCRibbonEdit* pE = DYNAMIC_DOWNCAST(
	//	CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_CAMERA_GAIN));

	//CString text = pE->GetEditText();
	//double v = _wtof(text);

	//UCI::pDis->pCAM->SetGain(v);

	
}

