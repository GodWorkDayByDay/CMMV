#include "stdafx.h"
#include "MainFrm.h"

#include "MyData.h"
#include "MyDispLay.h"
#include "ChildFrm.h"

using namespace std;
using namespace HalconCpp;
using namespace Gdiplus;


// �Զ���⿪ʼ ��ť
void ST_MYDISP::DisOnAutoStart()
{

	try {
		// 1 ��⵱ǰ��״̬
		if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;

		DisOpenCamera();

		int oldW = UCI::OPTIONS[L"ͼ����"];
		int oldH = UCI::OPTIONS[L"ͼ��߶�"];

		this->m_pMHWinMain->SetPart(0, 0, oldH - 1, oldW - 1);

		if (this->LoadCaliPoseAndParamter() == FALSE) {
			write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");
			return;
		}

		// 2 ��ԭ������Ĳ���ģ��
		int prID = UCI::OPTIONS[L"��Ŀ���"];

		CString stRegion = this->GetShapeModelRegionPathFile(prID);
		CString stModel = this->GetShapeModelPathFile(prID);

		if (GetFileAttributes(stRegion) == -1) { // �ļ�������
			MyLog(stRegion);

			string str = "DisOnAutoStart() ROI �����ļ������ڣ�";
			MyLog(str);
			write_message(200, 20, str.c_str());
			return;
		}

		if (GetFileAttributes(stModel) == -1) { // �ļ�������
			MyLog(stModel);

			string str = "DisOnAutoStart() Model �ļ������ڣ�";
			MyLog(str);
			write_message(300, 20, str.c_str());
			return;
		}

		this->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);       // ����ģ���ļ�
		this->ho_ModelRegion.ReadRegion(CT2A(stRegion));          // ���������ļ�
		

		// 3 ��ʼ���������߳� 
		UCI::OptionsMap * camo = &UCI::ELE.EleList.GetHead();
		//UCI::pDis->OnDisMeasureOnePic(camo);

		this->AppStateChangeTo(App_AutoMeasureing);
		this->StartAutoMeasureCap();

	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnAutoStart():");

	}
}

// �Զ������߳̿�ʼ

void ST_MYDISP::StartAutoMeasureCap()
{

	this->IsSpaceKeyDown = FALSE;

	if (this->pCAM->GetIsFGRuning()) {
		MyLog(L"��ǰ�Ѿ����Զ�������!");  //BEEP_ERROR;
		return;
	}
	else {

		if (this->m_hFGThrHandle)
		{
			WaitForSingleObject(m_hFGThrHandle, INFINITE);
			CloseHandle(this->m_hFGThrHandle);
			this->m_hFGThrHandle = NULL;
		}

		// start a new thread which openes a frame grabber and grabs continuously
		this->m_hFGThrHandle = (HANDLE)_beginthreadex(NULL,  // do not suspend thread  
			0,      // use same stack size as with main thread 
			(unsigned(__stdcall *)(void*))&FGCapContinues,  //unsigned ( __stdcall *start_address )( void * ) 
			this->pCView, //this->,   // parameters          
			0,      // start thread; don't suspend it       
			&this->m_uiFGThreadId);/* Win32 thread id.; note that this is  */

		MyLog(L"��ʼ�����Զ�������...");
	}
}

// �����ɼ�ͼ�񣬸��ݳ���״̬ ����
void ST_MYDISP::FGCapContinues(void *Param)
{

	// open a framegrabber and grab in a loop
	HImage      GrabImage;

	CChildView *pCV = (CChildView *)Param;
	const HWND PostWnd = pCV->GetSafeHwnd();

	//ST_MYDISP* pDis = pCV->pDis;

	UCI::pDis->pCAM->SetFGRuning(TRUE);


	switch (UCI::pDis->mAppState) {
	case App_CamInner:             // �ڲα궨
		UCI::pDis->m_hIPThrHandle = (HANDLE)_beginthreadex(NULL,
			0,
			(unsigned(__stdcall *)(void*))&IPRunCamCaliInner,  //unsigned ( __stdcall *start_address )( void * ) 
			UCI::pDis->pCView, //this->,   // parameters          
			0,      // start thread; don't suspend it       
			&UCI::pDis->m_uiIPThreadId);/* Win32 thread id.; note that this is  */
		break;
	case App_CamPose:            // ��α궨
		UCI::pDis->m_hIPThrHandle = (HANDLE)_beginthreadex(NULL,
			0,
			(unsigned(__stdcall *)(void*))&IPRunCamCaliPose,  //unsigned ( __stdcall *start_address )( void * ) 
			UCI::pDis->pCView, //this->,   // parameters          
			0,      // start thread; don't suspend it       
			&UCI::pDis->m_uiIPThreadId);/* Win32 thread id.; note that this is  */
		break;
	case App_ContinueingCap:
		UCI::pDis->m_hIPThrHandle = (HANDLE)_beginthreadex(NULL,
			0,
			(unsigned(__stdcall *)(void*))&IPContinous,  //unsigned ( __stdcall *start_address )( void * ) 
			UCI::pDis->pCView, //this->,   // parameters          
			0,      // start thread; don't suspend it       
			&UCI::pDis->m_uiIPThreadId);/* Win32 thread id.; note that this is  */
		break;
	case App_AutoMeasureing:

		UCI::pDis->m_hIPThrHandle = (HANDLE)_beginthreadex(NULL,
			0,
			(unsigned(__stdcall *)(void*))&IPAutoMeasure,  //unsigned ( __stdcall *start_address )( void * ) 
			UCI::pDis->pCView, //this->,   // parameters          
			0,      // start thread; don't suspend it       
			&UCI::pDis->m_uiIPThreadId);/* Win32 thread id.; note that this is  */

		break;
	default:
		break;
	}


	// IPContinous

	while (WAIT_OBJECT_0 != WaitForSingleObject(UCI::pDis->m_hFGStopEvent, 0))
	{
		UCI::pDis->pCAM->GrabOneImageAsync(&GrabImage);

		// Display the grabbed image
		EnterCriticalSection(&UCI::pDis->m_csFGDisp);
		UCI::pDis->GrabImageDisp = GrabImage;
		LeaveCriticalSection(&UCI::pDis->m_csFGDisp);

		PostMessage(PostWnd, WM_FGDISP, NULL, NULL);    // ��ʾ���ͼ��
		Sleep(20);
	}

	UCI::pDis->pCAM->ResetIPNewImageEvent();

	// wait for the IP Thread (also stopped by m_hFGStopEvent)
	WaitForSingleObject(UCI::pDis->m_hIPThrHandle, INFINITE);

	ResetEvent(UCI::pDis->m_hFGStopEvent);

	UCI::pDis->pCAM->SetFGRuning(FALSE);

	return;

}

void ST_MYDISP::IPContinous(void *Param)
{
	// perform some image processing on the latest image
	HXLDCont       Contours;
	HImage         Image;
	//unsigned       idx;
	HANDLE         lphEvents[2];
	double         dAlpha;
	const double   cdAlphaMin = 0.4, cdAlphaMax = 15.0;
	int            iSliderMin, iSliderMax, iSliderPos;

	CChildView *pCV = (CChildView *)Param;
	const HWND PostWnd = pCV->GetSafeHwnd();

	//ST_MYDISP* pDis = UCI::pDis;

	lphEvents[0] = UCI::pDis->pCAM->GetNewImageEvent();
	lphEvents[1] = UCI::pDis->m_hFGStopEvent;

	iSliderMin = 0;
	iSliderMax = 100;

	BOOL isUseFunction = UCI::OPTIONS[L"�����ɼ�Ч��ʹ��"];

	while (WAIT_OBJECT_0 == WaitForMultipleObjects(2, lphEvents, FALSE, INFINITE))
	{
		iSliderPos = 50;
		dAlpha = (cdAlphaMax - cdAlphaMin)*double(iSliderPos - 0) / double(iSliderMax - iSliderMin) + cdAlphaMin;

		// get the new image buffer
		UCI::pDis->pCAM->GetBufferImage(&Image);

		if (isUseFunction) {

			Contours.GenEmptyObj();

			UCI::OptionsMap * om;
			om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_ROI);
			UCI::pDis->MeasureOneROI(om, Image, Contours);

			om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_FUNCTION);
			UCI::pDis->MeasureOneFUNCTION(om, Image, Contours);

			//om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
			//UCI::pDis->MeasureOneMATCH(om, hImg, Contours);
		}
		
		

		// perform the image processing
		//Contours = Image.EdgesSubPix("canny_junctions", dAlpha, 20, 40);

		// Display the results: contours on original image
		EnterCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect		
		UCI::pDis->m_sIPResult.result_img = Image;
		//UCI::pDis->m_sIPResult.result_cont = Contours;
		LeaveCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect
		PostMessage(PostWnd, WM_IPDISP, NULL, NULL);                   // ����ú� ��ʾһ��
	}
	return;
}

// �Զ������߳� 
void ST_MYDISP::IPAutoMeasure(void *Param)
{
	// perform some image processing on the latest image
	HXLDCont       Contours;
	HImage         Image;
	HANDLE         lphEvents[2];

	CChildView *pCV = (CChildView *)Param;
	const HWND PostWnd = pCV->GetSafeHwnd();

	lphEvents[0] = UCI::pDis->pCAM->GetNewImageEvent();
	lphEvents[1] = UCI::pDis->m_hFGStopEvent;

	int delay = UCI::OPTIONS[L"AutoMeasureDelayTime"];

	BOOL bIsDispOrgPic = UCI::OPTIONS[L"��ʾԭͼ"];


	while (WAIT_OBJECT_0 == WaitForMultipleObjects(2, lphEvents, FALSE, INFINITE))
	{
		// get the new image buffer

		HTuple T1, T2;
		CountSeconds(&T1);

		UCI::pDis->pCAM->GetBufferImage(&Image);
		Contours.GenEmptyObj();	

		EnterCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect	

		// ==========================================================
		// ����һ��ͼ��  
		UCI::pDis->AutoMeasureOnePicture(Image, Contours);
		if (bIsDispOrgPic) {
		}
		else {
			UCI::pDis->m_sIPResult.result_img = Image;
		}

		UCI::pDis->m_sIPResult.result_cont = Contours;
		LeaveCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect	

		PostMessage(PostWnd, WM_IPDISP, NULL, NULL);                   // ����ú� ��ʾһ��		

		CountSeconds(&T2);

		HTuple time = 1000 * (T2[0].D() - T1[0].D());
		CString str;
		str.Format(_T("�Զ�����ʱ��: %4.2f ms"), time[0].D());
		UCI::MyLog(str);

		Sleep(delay);
	}
	return;
}


// �Զ�����һ��ͼ��
void ST_MYDISP::AutoMeasureOnePicture(HalconCpp::HImage & hImg, HXLDCont& Cont)
{
	
	
	UCI::OPTIONS[L"GLO_Measure_OK"] = TRUE;                // �Ƿ�ϸ�

	POSITION pos = UCI::ELE.EleList.GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap *o = &UCI::ELE.EleList.GetNext(pos);
		//////////////////////////////////////////////////
		// ��ÿһ��Ԫ�ؽ��в���		
		if (this->MeasureOneElement(o, hImg, Cont) == FALSE) {
			
			UCI::OPTIONS[L"GLO_Measure_OK"] = FALSE;
			UCI::OPTIONS[L"GLO_Measure_Always_OK"] = FALSE;

			break;
		}
	}
}

void ST_MYDISP::DisplayOtherInfo()
{
	
	if (this->mAppState == App_AutoMeasureing) {

		BOOL Result = UCI::OPTIONS[L"GLO_Measure_OK"];

		if (Result == TRUE) {
			BOOL always = UCI::OPTIONS[L"GLO_Measure_Always_OK"];
			if (always == FALSE){
				
				UCI::OPTIONS[L"GLO_Measure_Always_OK"] = TRUE;				

				UCI::Speech(L"�ϸ�", true);

				// ������Զ����棬�ͱ���һ��
				BOOL autoSave = UCI::OPTIONS[L"ProdutctOKAutoSave"];

				if (autoSave) {
					UCI::pMain->ResDeleAllItems();
					int productID;
					UCI::sDB.RecordOneProdcut(productID);
				}
			}


			SolidBrush blackBrush(Color(255, 0, 0, 255));  //��͸��+����RGB��ɫ
			this->DisFontLarge(L"PASS", 0, 0, 60.0f, blackBrush);
		}
		else {
			this->m_pMHWinMain->SetColor("red");
		//	this->set_display_font(*this->m_pMHWinMain, 16, "mono", "true", "false");
			this->m_pMHWinMain->SetTposition(10, 10);
			this->m_pMHWinMain->WriteString("�Զ������С����� ��ǰû�м�⵽��Ʒ");		
		}

		if (this->IsSpaceKeyDown) {

			// ����һ����Ʒ
			UCI::pMain->ResDeleAllItems();

			int productID;
			UCI::sDB.RecordOneProdcut(productID);

			this->IsSpaceKeyDown = FALSE;
		}
	}
}
