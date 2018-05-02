#include "stdafx.h"
#include "CMV.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ChildView.h"
#include "CExcelOperation.h"
#include "DialogUserLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


// ��һ��ͼƬ
void CMainFrame::OnFileOpen()
{
	// TODO: �ڴ���������������	

	//CChildFrame *pChild = (CChildFrame *)(this->GetActiveFrame());
	//CChildView *pView = &pChild->m_wndView;	

	// ����Ӵ����Ѵ�������ͷ
	if (UCI::pDis->IsCanLoadPicture() == false) {
		LogOut(L"��ǰ����ͷ���ڲ�ͼ, ���ܴ�ͼƬ!");
		return;
	}
	UCI::pDis->DisOnOpenImage();

	this->m_wndPictureView.AdjustLayout();


}


void CMainFrame::OnFileSave()
{
	// TODO: �ڴ���������������
	UCI::pDis->DisOnSaveCurrentDISPtoFile();
}


void CMainFrame::OnCamOneShot()
{
	UCI::pDis->DisOnOneShot();
	UCI::pDis->DisOnSaveCurrentDISPtoFile();  // �ٱ���һ��	
}


void CMainFrame::OnCamCapMulti()
{
	// TODO: �ڴ���������������
	UCI::pDis->DisOnCamCapMulti();
}




void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CMDIFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::OnWinZoomOut()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnWinZoomIn()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnWinRestore()
{
	// TODO: �ڴ���������������
}



void CMainFrame::OnUserLoging()
{
	if (UCI::User.islogin) {
		MessageBox(L"��ǰ�ѵ�¼�ɹ��ˣ�");
		return;
	}
	
	CDialogUserLog dlg(this);

	if (dlg.DoModal() == IDOK) {

		//CString userName;
		//CString passWord;
		//dlg.ID_UserName.GetWindowText(userName);
		//dlg.ID_Password.GetWindowText(passWord);


	}
}

void CMainFrame::OnExportEXCEL()
{
	if (!UCI::bIsCanOperate(L"�������")) return;

	CExcelOperation oApp;
	oApp.exportProductExcel(0);
}


void CMainFrame::OnReportManage()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnSysSetting()
{
	// TODO: �ڴ���������������
}


//void CMainFrame::OnFileNew()
//{
//	// TODO: �ڴ���������������
//	int a = 0;
//}


void CMainFrame::OnCalibrate()
{
	// TODO: �ڴ���������������
	UCI::pDis->DisOnButCamCaliInner();
}


void CMainFrame::OnCamPoseSet()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnRectityPicture()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnModelSet()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMatchModel()
{
	// TODO: �ڴ���������������
}



void CMainFrame::OnMeasureAuto()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMeasureStop()
{
	// TODO: �ڴ���������������
}




void CMainFrame::OnComboCameraResolutiion()
{
	// TODO: �ڴ���������������
}








void CMainFrame::OnBCrossColor1()
{
	// TODO: �ڴ���������������
}

//// �궨����ڲ�
//void CMainFrame::OnCalibrateInner()
//{
//	// TODO: �ڴ���������������
//	this->GetpDis()->DisOnButCamCaliInner();
//}

// �궨����ڲ�
void CMainFrame::OnButCaliInner()
{
	// TODO: �ڴ���������������
	UCI::pDis->DisOnButCamCaliInner();
}

void CMainFrame::OnButCaliPose()
{
	// TODO: �ڴ���������������
	UCI::pDis->DisOnButCamCaliPose();
}

// �궨����ڲ��ǽ�ͼ����
void CMainFrame::OnButCaliCapture()
{
	// TODO: �ڴ���������������
	int prID = UCI::OPTIONS[L"��Ŀ���"];
	UCI::pDis->DisOnSaveOnCaliPicture(prID);
}

void CMainFrame::OnComboCaliMapType()
{
	// TODO: �ڴ���������������
}

// ��������У��ӳ��ͼ��
void CMainFrame::OnButtonGenRectifyMap()
{
	UCI::pDis->DisOnButtonGenRectifyMap();
}


void CMainFrame::OnButtonTakeOne()
{
	UCI::pDis->DisOnOneShot();
}


// ��ʾ��Ŀ��ͼ����
void CMainFrame::OnViewProjectView()
{
	// TODO: �ڴ���������������   m_wndFileView;

	if (this->m_wndProjectView.IsVisible() == FALSE) {
		this->m_wndProjectView.ShowPane(TRUE, FALSE, TRUE);
	}
	else {
		this->m_wndProjectView.ShowPane(FALSE, FALSE, TRUE);
	}
}


void CMainFrame::OnUpdateViewProjectView(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(this->m_wndProjectView.IsVisible());
}


void CMainFrame::OnViewOutputwnd()
{
	// TODO: �ڴ���������������
	if (this->m_wndOutput.IsVisible() == FALSE) {
		this->m_wndOutput.ShowPane(TRUE, FALSE, TRUE);
	}
	else {
		this->m_wndOutput.ShowPane(FALSE, FALSE, TRUE);
	}
}


void CMainFrame::OnUpdateViewOutputwnd(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(this->m_wndOutput.IsVisible());
}


void CMainFrame::OnViewPropertieswnd()
{
	if (this->m_wndProperties.IsVisible() == FALSE) {
		this->m_wndProperties.ShowPane(TRUE, FALSE, TRUE);
	}
	else {
		this->m_wndProperties.ShowPane(FALSE, FALSE, TRUE);
	}
}


void CMainFrame::OnUpdateViewPropertieswnd(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(this->m_wndProperties.IsVisible());
}


void CMainFrame::OnViewClassview()
{
	// TODO: �ڴ��������������� 
	if (this->m_wndClassView.IsVisible() == FALSE) {
		this->m_wndClassView.ShowPane(TRUE, FALSE, TRUE);
	}
	else {
		this->m_wndClassView.ShowPane(FALSE, FALSE, TRUE);
	}
}


void CMainFrame::OnUpdateViewClassview(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(this->m_wndClassView.IsVisible());
}


void CMainFrame::OnViewPictureview()
{
	// TODO: �ڴ��������������� 
	if (this->m_wndPictureView.IsVisible() == FALSE) {
		this->m_wndPictureView.ShowPane(TRUE, FALSE, TRUE);
	}
	else {
		this->m_wndPictureView.ShowPane(FALSE, FALSE, TRUE);
	}
}


void CMainFrame::OnUpdateViewPictureview(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(this->m_wndPictureView.IsVisible());
}


void CMainFrame::OnDebugStarp()
{
	// TODO: �ڴ���������������
	//CChildFrame *pChild = (CChildFrame *)(this->GetActiveFrame());
	//CChildView *pView = &pChild->m_wndView;
	UCI::pDis->OnButtonFGStart();
}


void CMainFrame::OnDebugStop()
{
	// TODO: �ڴ���������������
	UCI::pDis->OnButtonFGStop();
}


void CMainFrame::OnUpdateButCaliCapture(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(UCI::RIBmenu.Rib_CALI_CAPTURE);
}


void CMainFrame::OnComboMatchRoiType()
{
	// TODO: �ڴ���������������
	static int imagIndex[] = { 4, 3,2,1 };

	CMFCRibbonComboBox* pComboBox = DYNAMIC_DOWNCAST(
		CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_MATCH_ROI_TYPE));

	//std::string st = CW2A(pCameraComboBox->GetItem(pCameraComboBox->GetCurSel()));

	int sel = pComboBox->GetCurSel();

	UCI::RIBmenu.iCOMBO_MATCH_ROI_TYPE = (eCOMBO_MATCH_ROI_TYPE)sel;

	pComboBox->SetImageIndex(imagIndex[sel], false);

	pComboBox->Redraw();

	//m_wndRibbonBar.RedrawWindow();

}


void CMainFrame::OnUpdateComboMatchRoiType(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

// ���ص�ǰ�����ƥ��ͼƬ
void CMainFrame::OnButMatchLoadPic()
{
	UCI::pDis->OnDisButMatchLoadPic();
}

// �ɼ�һ��ƥ���õ�ͼ���������趨��ӳ�䣬�� ���Ӵ���
void CMainFrame::OnMatchTakeOne()
{
	UCI::pDis->DisOnMatchTakeOne();
}


void CMainFrame::OnMatchDelAllRoi()
{
	UCI::pDis->DisOnMatchDelAllRoi();
}

// б����ģ�����
void CMainFrame::OnRoiDrawRectangle2()
{
	UCI::pDis->DisOnRoiDrawRectangle2();
}

void CMainFrame::OnRoiDrawCircle()
{
	UCI::pDis->DisOnRoiDrawCircle();
}


void CMainFrame::OnRoiDrawEllipse()
{
	UCI::pDis->DisOnRoiDrawEllipse();
}


void CMainFrame::OnRoiDrawRectangle()
{
	UCI::pDis->DisOnRoiDrawRectangle();
}


void CMainFrame::OnRoiDrawAnyLine()
{
	UCI::pDis->DisOnRoiDrawAnyLine();
}

// ��ʾ��ǰ��ģ����Ϣ\nģ����Ϣ
void CMainFrame::OnRoiInfo()
{
	UCI::pDis->DisOnRoiInfo();
}

// ��λ����
void CMainFrame::OnRoiFindModel()
{
	UCI::pDis->DisOnRoiFindModel();
}


void CMainFrame::OnMeaEleArc()
{
	// TODO: �ڴ���������������
}

// ��Բ����
void CMainFrame::OnMeaEleEllispe()
{
	// TODO: �ڴ���������������
	UCI::pDis->DisOnMeaEleEllispe();

}


void CMainFrame::OnMeaEleEllispeArc()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMeaEleWidth()
{
	// һά��������
	UCI::pDis->DisOnAddMEASURE_RECTANGLE2();
}


void CMainFrame::OnMeaEleFindEdge()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMeaEleFindTop()
{
	UCI::pDis->DisOnAddFindPeekElement();    // �Ҷ���Ԫ��
}


void CMainFrame::OnMeaEleArea()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMeaEleCounter()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMeaEleMeasure()
{	
	UCI::pDis->DisOnElementMeasureManual();   // Ԫ���ֶ�����
}





void CMainFrame::OnButMatchSaveModel()
{
	UCI::pDis->DisOnButMatchSaveModel();
	// ������ȫ�ֲ���
	UCI::sDB.MySaveSettingToSqlite(UCI::OPTIONS);        // ����ȫ�ֲ���
}

// ɾ�����в���Ԫ��
void CMainFrame::OnMeaEleDelAll()
{
	UCI::pDis->DisOnElementDeleteAll();
}


void CMainFrame::OnButCaliCorrect()
{
	if (!UCI::pDis->TestCurrentAppState(APP_LOGGING_OK)) return;
	UCI::pDis->DisOnButCaliCorrect();     // ���ݱ궨��У��ͼ��
}


void CMainFrame::OnMeaEleOutput()
{
	if (!UCI::pDis->TestCurrentAppState(APP_LOGGING_OK)) return;
	UCI::pDis->DisOnEleOutReport();        // ����Ԫ�����
}

// ����ת��Ϊ����Ԫ��
void CMainFrame::OnMeaEleMillimeter()
{
	UCI::pDis->DisOnMeaEleMillimeter();        // ����Ԫ�����
}

void CMainFrame::OnMeaEleCamera()
{
	// TODO: �ڴ���������������
	UCI::pDis->DisOnEleCameraAdd();
}

// ����ֱ��ROI
void CMainFrame::OnMeaEleLine()
{
	UCI::pDis->DisOnEleLineAdd();
}

// ����Բ��ROI
void CMainFrame::OnMeaEleCircle()
{
	UCI::pDis->DisOnEleCircleAdd();
}

// ��������ֱ�ߵĽǶȼ�����
void CMainFrame::OnEleAngle()
{
	UCI::pDis->DisOnEleAngle();
}

// ��������Ԫ�صľ���
void CMainFrame::OnEleDistance()
{
	UCI::pDis->DisOnEleDistance();
}

void CMainFrame::OnMeaEleFunction()
{
	UCI::pDis->DisMeaEleFunction();
}


void CMainFrame::OnMeaEleMatching()
{
	UCI::pDis->DisMeaEleMatching();
}

// ͨ���ĸ��������������ҵĳ���
void CMainFrame::OnMeaEleBalloonLength()
{
	UCI::pDis->DisOnMeaEleBalloonLength();
}


void CMainFrame::OnMeaEleStep()
{
	// TODO: �ڴ���������������
}

// �Զ���⿪ʼ
void CMainFrame::OnAutoStart()
{
	UCI::pDis->DisOnAutoStart();
}

// �Զ�������
void CMainFrame::OnAutoStop()
{
	UCI::pDis->DisOnAutoStop();
}

// �����ɼ�ͼ��
void CMainFrame::OnCamContinueCap()
{
	UCI::pDis->DisOnCamContinueCap();
}

void CMainFrame::OnMeaBalloonEccent()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMeaEleBalloonFlaw()
{
	UCI::pDis->DisOnMeaEleBalloonFlaw();  // ���ұ���ȱ�ݼ��
}

// ��ʾ��ǰ�ı궨��Ϣ
void CMainFrame::OnButCaliShowInfo()
{
	UCI::pDis->DisOnButCaliShowInfo(); // ��ʾ��ǰ�ı궨��Ϣ
}


void CMainFrame::OnMeaEleRoi()
{
	UCI::pDis->DisOnMeaEleAddRoi(); // ��ʾ��ǰ�ı궨��Ϣ
}




