#include "stdafx.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "CExcelOperation.h"

#include "MyData.h"
#include "MyDispLay.h"
#include "DialogUserLog.h"



using namespace HalconCpp;
using namespace std;

//void ST_MYDISP::OnDisKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//
//}

BOOL ST_MYDISP::TestCurrentAppState(eAppState s)
{
	if (this->mAppState == s) return TRUE;

	if (this->mAppState != APP_READY) {
		::MessageBox(NULL, L"���ܵ�¼", L"��ǰ״̬���� APP_READY", MB_OK);
		return FALSE;
	}

	CString str = L"��ǰ״̬Ϊ: " + STATE_NAME[this->mAppState]
		+ " ӦΪ��" + STATE_NAME[s];

	UCI::pMain->StatusCommon(str);

	this->write_message(100, 20, CT2A(str).m_psz);

	CDialogUserLog dlg(UCI::pMain);

	dlg.DoModal();

	return FALSE;
}


void ST_MYDISP::DisDisplay(CRect rect)
{
	this->ChangeImageCRect(rect);
	this->SetPartHvWnd();

	// this->DisDisplay();

	/*WaitForSingleObject(this->hMutex_disp, 500);
	ClearWindow(this->hv_HWD);
	this->DisDisplay();
	ReleaseMutex(this->hMutex_disp);*/
}



// ����ͼ�����ʾ����Ϊ��ǰ�� Rect
void ST_MYDISP::SetPartHvWnd()
{
	CRect* rc = &rcImage;
	this->m_pMHWinMain->SetPart(rc->top, rc->left, rc->bottom, rc->right);
}






void ST_MYDISP::write_message(HalconCpp::HTuple hv_Row, HalconCpp::HTuple hv_Column,
	HalconCpp::HTuple hv_String, bool isclear)
{

	// Local control variables
	HTuple  hv_Red, hv_Green, hv_Blue;
	Hlong hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part;
	Hlong   hv_RowWin, hv_ColumnWin, hv_WidthWin, hv_HeightWin;
	HTuple  hv_Width;
	HTuple  hv_Index;
	Hlong hv_ExpDefaultCtrlDummyVar, hv_W;
	HTuple hv_Function;
	HTuple  hv_Sequence, hv_Chr, hv_Ascent;
	Hlong hv_Descent, hv_Height;
	HTuple  hv_FrameHeight, hv_FrameWidth, hv_FactorRow, hv_FactorColumn;
	HTuple  hv_DrawMode, hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S;
	HTuple  hv_R1, hv_C1, hv_R2, hv_C2;

	//
	//Estimate extentions of text depending on font size, 
	//draw rectangle at the specified position
	//and write text in this rectangle
	//
	//HDevWindowStack::SetActive(hv_HWD);

	this->m_pMHWinMain->GetRgb(&hv_Red, &hv_Green, &hv_Blue);
	this->m_pMHWinMain->GetPart(&hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
	this->m_pMHWinMain->GetWindowExtents(&hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);

	if (0 != (hv_Row == -1))
	{
		hv_Row = 12;
	}
	if (0 != (hv_Column == -1))
	{
		hv_Column = 12;
	}
	hv_String = (" " + ((("" + hv_String) + "").TupleSplit("\n"))) + " ";
	hv_Width = HTuple();
	{
		HTuple end_val17 = (hv_String.TupleLength()) - 1;
		HTuple step_val17 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val17, step_val17); hv_Index += step_val17)
		{
			this->m_pMHWinMain->GetStringExtents(HTuple(hv_String[hv_Index]), &hv_ExpDefaultCtrlDummyVar,
				&hv_W, &hv_ExpDefaultCtrlDummyVar);
			hv_Width = hv_Width.TupleConcat(hv_W);
		}
	}
	CreateFunct1dArray(HTuple(256, 0), &hv_Function);
	Funct1dToPairs(hv_Function, &hv_Sequence, &HTuple(hv_ExpDefaultCtrlDummyVar));
	hv_Chr = hv_Sequence.TupleChr();

	this->m_pMHWinMain->GetStringExtents(hv_Chr, &hv_Descent, &hv_ExpDefaultCtrlDummyVar,
		&hv_Height);
	hv_Height = (HTuple(hv_Height).TupleConcat(hv_Descent)).TupleMax();
	//
	hv_FrameHeight = hv_Height*(hv_String.TupleLength());
	hv_FrameWidth = hv_Width.TupleMax();
	//
	//Adapt text position and extents because of image zooming
	hv_FactorRow = (1.*((hv_Row2Part - hv_Row1Part) + 1)) / hv_HeightWin;

	//double f = hv_FactorRow.D();

	hv_FactorColumn = (1.*((hv_Column2Part - hv_Column1Part) + 1)) / hv_WidthWin;

	hv_DrawMode = this->m_pMHWinMain->GetDraw();
	//GetDraw(hv_HWD, &hv_DrawMode);

	this->m_pMHWinMain->SetDraw("fill");

	HTuple hv_WindowCoordinates = 0;

	if (0 != hv_WindowCoordinates)
	{
		this->m_pMHWinMain->SetColor("light gray");
		//SetColor(hv_HWD, "light gray");
		hv_R1_S = (((hv_Row + 3)*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C1_S = (((hv_Column + 3)*hv_FactorColumn) + 0.5) + hv_Column1Part;
		hv_R2_S = (((((hv_Row + 3) + hv_FrameHeight) - 1)*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C2_S = (((((hv_Column + 3) + hv_FrameWidth) - 1)*hv_FactorColumn) + 0.5) + hv_Column1Part;

		this->m_pMHWinMain->DispRectangle1(hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		//DispRectangle1(hv_HWD, hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		this->m_pMHWinMain->SetColor("white");
		//SetColor(hv_HWD, "white");
		hv_R1 = ((hv_Row*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C1 = ((hv_Column*hv_FactorColumn) + 0.5) + hv_Column1Part;
		hv_R2 = ((((hv_Row + hv_FrameHeight) - 1)*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C2 = ((((hv_Column + hv_FrameWidth) - 1)*hv_FactorColumn) + 0.5) + hv_Column1Part;

		this->m_pMHWinMain->DispRectangle1(hv_R1, hv_C1, hv_R2, hv_C2);
		//DispRectangle1(hv_HWD, hv_R1, hv_C1, hv_R2, hv_C2);
		this->m_pMHWinMain->SetColor("black");
		//SetColor(hv_HWD, "black");
	}
	else
	{
		this->m_pMHWinMain->SetColor("light gray");
		//SetColor(hv_HWD, "light gray");
		hv_R1_S = hv_Row + 3;
		hv_C1_S = hv_Column + 3;
		hv_R2_S = ((hv_Row + 3) + ((hv_FrameHeight - 1)*hv_FactorRow)) + 0.5;
		hv_C2_S = ((hv_Column + 3) + ((hv_FrameWidth - 1)*hv_FactorColumn)) + 0.5;

		this->m_pMHWinMain->DispRectangle1(hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		//DispRectangle1(hv_HWD, hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		this->m_pMHWinMain->SetColor("white");
		//SetColor(hv_HWD, "white");
		hv_R1 = hv_Row;
		hv_C1 = hv_Column;
		hv_R2 = (hv_Row + ((hv_FrameHeight - 1)*hv_FactorRow)) + 0.5;
		hv_C2 = (hv_Column + ((hv_FrameWidth - 1)*hv_FactorColumn)) + 0.5;

		this->m_pMHWinMain->DispRectangle1(hv_R1, hv_C1, hv_R2, hv_C2);
		//DispRectangle1(hv_HWD, hv_R1, hv_C1, hv_R2, hv_C2);
		this->m_pMHWinMain->SetColor("black");
	}
	//Write text into rectangle
	{
		HTuple end_val66 = (hv_String.TupleLength()) - 1;
		HTuple step_val66 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val66, step_val66); hv_Index += step_val66)
		{
			this->m_pMHWinMain->SetTposition(hv_R1.I() + int((int(hv_Height)*hv_FactorRow.D())*hv_Index.I()), hv_C1);
			//this->m_pMHWinMain->SetTposition(hv_R1 + ((hv_Height*hv_FactorRow)*hv_Index), hv_C1);
			//SetTposition(hv_HWD, hv_R1 + ((hv_Height*hv_FactorRow)*hv_Index), hv_C1);
			this->m_pMHWinMain->WriteString(HTuple(hv_String[hv_Index]));
			//WriteString(hv_HWD, HTuple(hv_String[hv_Index]));
		}
	}
	this->m_pMHWinMain->SetDraw(hv_DrawMode);
	//if (HDevWindowStack::IsOpen())
	//	SetDraw(HDevWindowStack::GetActive(), hv_DrawMode);

	this->m_pMHWinMain->SetRgb(hv_Red, hv_Green, hv_Blue);
	//SetRgb(hv_HWD, hv_Red, hv_Green, hv_Blue);
	return;
}





// ͼ�����߳�

void ST_MYDISP::IPRun(void *Param)
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

	while (WAIT_OBJECT_0 == WaitForMultipleObjects(2, lphEvents, FALSE, INFINITE))
	{
		iSliderPos = 50;
		dAlpha = (cdAlphaMax - cdAlphaMin)*double(iSliderPos - 0) / double(iSliderMax - iSliderMin) + cdAlphaMin;

		// get the new image buffer
		UCI::pDis->pCAM->GetBufferImage(&Image);

		// perform the image processing
		Contours = Image.EdgesSubPix("canny_junctions", dAlpha, 20, 40);

		// Display the results: contours on original image
		EnterCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect		
		UCI::pDis->m_sIPResult.result_img = Image;
		UCI::pDis->m_sIPResult.result_cont = Contours;
		LeaveCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect
		PostMessage(PostWnd, WM_IPDISP, NULL, NULL);                   // ����ú� ��ʾһ��
	}
	return;
}


//EnterCriticalSection(&m_csIPDisp);      // CriticalSect
//Image = m_sIPResult.result_img;         // CriticalSect
//Cont = m_sIPResult.result_cont;         // CriticalSect
//LeaveCriticalSection(&m_csIPDisp);      // CriticalSect

// ͼ��ɼ��߳�
// The frame grabber thread
void ST_MYDISP::FGRun(void* Param)
{
	// open a framegrabber and grab in a loop
	HImage      GrabImage;
	HANDLE      hIPThrHandle;
	unsigned    uiIPThreadId;
	//unsigned    idx;

	CChildView *pCV = (CChildView *)Param;
	const HWND PostWnd = pCV->GetSafeHwnd();

	//ST_MYDISP* pDis = pCV->pDis;


	UCI::pDis->pCAM->SetFGRuning(TRUE);

	// ��ʼͼ�����߳�
	// start a new thread which performs the image processing
	hIPThrHandle = (HANDLE)_beginthreadex(NULL,        // do not suspend thread  
		0,             // use same stack size as with main thread 
		(unsigned(__stdcall *)(void*))&IPRun,  //unsigned ( __stdcall *start_address )( void * ) 
		pCV,           // parameters          
		0,             // start thread; don't suspend it       
		&uiIPThreadId);// Win32 thread id.



	while (WAIT_OBJECT_0 != WaitForSingleObject(UCI::pDis->m_hFGStopEvent, 0))
	{

		UCI::pDis->pCAM->GrabOneImageAsync(&GrabImage);  // �첽�ɼ�һ��ͼ��

														 // Display the grabbed image
		EnterCriticalSection(&UCI::pDis->m_csFGDisp);
		UCI::pDis->GrabImageDisp = GrabImage;
		LeaveCriticalSection(&UCI::pDis->m_csFGDisp);


		Sleep(20);

		PostMessage(PostWnd, WM_FGDISP, NULL, NULL);    // ��ʾ���ͼ��
	}

	UCI::pDis->pCAM->ResetIPNewImageEvent();

	// wait for the IP Thread (also stopped by m_hFGStopEvent)
	WaitForSingleObject(hIPThrHandle, INFINITE);

	ResetEvent(UCI::pDis->m_hFGStopEvent);

	UCI::pDis->pCAM->SetFGRuning(FALSE);

	return;
}




void ST_MYDISP::OnDestroy()
{

	BOOL b_FGIsRunning;

	b_FGIsRunning = this->pCAM->GetIsFGRuning();


	if (b_FGIsRunning)
	{
		//stop threads
		SetEvent(m_hFGStopEvent);
		WaitForSingleObject(m_hFGThrHandle, INFINITE);
	}


	DeleteCriticalSection(&m_csIPDisp);
	DeleteCriticalSection(&m_csFGDisp);
	CloseHandle(this->m_hFGStopEvent);

	if (NULL != this->m_hFGThrHandle)
		CloseHandle(this->m_hFGThrHandle);

	delete m_pHWinFG;
	delete m_pMHWinMain;

	//delete pIPWinStatic;
	delete pFGWinStatic;
	delete PMainStatic;
}

void ST_MYDISP::InitThreadCommunication()
{
	// initialize thread communication
	m_hFGThrHandle = NULL;

	InitializeCriticalSection(&m_csFGDisp);
	InitializeCriticalSection(&m_csIPDisp);

	m_hFGStopEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

	ResetEvent(m_hFGStopEvent);


	CRect rect;
	this->pFGWinStatic->GetClientRect(&rect);
	//SetCheck("~father");
	m_pHWinFG = new HWindow(0, 0, rect.Width(), rect.Height(), this->pFGWinStatic->m_hWnd, "visible", "");
	//SetCheck("father");


	this->PMainStatic->GetClientRect(&rect);  // m_pMHWinMain
											  //SetCheck("~father");
	m_pMHWinMain = new HWindow(0, 0, rect.Width(), rect.Height(), this->PMainStatic->m_hWnd, "visible", "");
	//SetCheck("father");

	m_pMHWinMain->SetDraw("fill");
	m_pMHWinMain->SetColor("green");
	m_pMHWinMain->SetLineWidth(1);


	this->pCAM->SetFGRuning(FALSE);

}

void ST_MYDISP::ResetAllCStaticPart()
{

	int h = UCI::OPTIONS[L"ͼ��߶�"];
	int w = UCI::OPTIONS[L"ͼ����"];

	if (h < 50) return;
	if (w < 50) return;

	this->m_pHWinFG->SetPart(0, 0, h - 1, w - 1);
	this->m_pMHWinMain->SetPart(0, 0, h - 1, w - 1);
}

void ST_MYDISP::ResetAllCStaticWinExtends()
{
	CRect rect;
	//this->pIPWinStatic->GetClientRect(&rect);
	//this->m_pHWinIP->SetWindowExtents(0, 0, rect.Width(), rect.Height());

	this->pFGWinStatic->GetClientRect(&rect);
	this->m_pHWinFG->SetWindowExtents(0, 0, rect.Width(), rect.Height());

	this->PMainStatic->GetClientRect(&rect);

	CRect cr = { 0,0,rect.Width() - 1, rect.Height() - 1 };
	this->ChangeImageCRect(cr);

	this->m_pMHWinMain->SetWindowExtents(0, 0, rect.Width(), rect.Height());
}
void ST_MYDISP::AppStateChangeTo(eAppState e)
{
	this->mAppState = e;
	if (e == APP_READY) {
		if (UCI::User.islogin) {
			this->mAppState = APP_LOGGING_OK;
		}
	}
	PostMessage(UCI::pMain->m_hWnd, UM_UPDATESTATUS_APPSTATE, (WPARAM)0, (LPARAM)(LPCTSTR)0);
}

// ����ͼ��ʾ
LRESULT ST_MYDISP::OnFGDisp(WPARAM wParam, LPARAM lParam)
{
	try {
		HImage   Image;

		EnterCriticalSection(&m_csFGDisp);
		Image = this->GrabImageDisp;
		LeaveCriticalSection(&m_csFGDisp);

		this->m_pHWinFG->DispObj(Image);     // ���Դ�����ʾ
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"OnFGDisp()��");

	}
	return 0L;
}

LRESULT ST_MYDISP::OnMainDisp(WPARAM wParam, LPARAM lParam)
{
	try
	{
		HImage   Image;
		HXLDCont Cont;

		EnterCriticalSection(&m_csIPDisp);      // CriticalSect
		Image = m_sIPResult.result_img;         // CriticalSect
		Cont = m_sIPResult.result_cont;         // CriticalSect
		LeaveCriticalSection(&m_csIPDisp);      // CriticalSect

												// ����һ��ROI����ɫ���߿�
		CString HDLXcolor = UCI::OPTIONS[L"ElementHDLXColor"];
		int HDLXWidth = UCI::OPTIONS[L"ElementHDLXWidth"];
		this->m_pMHWinMain->SetColor(CT2A(HDLXcolor).m_psz);
		this->m_pMHWinMain->SetLineWidth(HDLXWidth);

		this->m_pMHWinMain->DispObj(Image);
		this->m_pMHWinMain->DispObj(Cont);

		this->DisplayOtherInfo();

	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"OnMainDisp()��");
	}

	return 0L;
}


// ���Կ�ʼ��ť
void ST_MYDISP::OnButtonFGStart()
{

	BOOL b_FGIsRunning;

	//EnterCriticalSection(&newImageMutex);
	b_FGIsRunning = this->pCAM->GetIsFGRuning();
	//LeaveCriticalSection(&newImageMutex);

	// Call back function to start the framegrabber
	if (b_FGIsRunning == TRUE)
	{
		// already started		
		MyLog(L"��ǰ�Ѿ��ڲɼ�ͼ����!");  //BEEP_ERROR;
		return;
	}
	else
	{
		if (!this->pCAM->GetIsOpen()) {
			this->pCAM->Connect(L"pendulum/pendulum");
		}
		this->ResetAllCStaticPart();
		this->ResetAllCStaticWinExtends();
		//m_cFGStartButton.EnableWindow(FALSE);

		if (this->m_hFGThrHandle)
		{
			WaitForSingleObject(m_hFGThrHandle, INFINITE);
			CloseHandle(this->m_hFGThrHandle);
			this->m_hFGThrHandle = NULL;
		}

		// start a new thread which openes a frame grabber and grabs continuously
		m_hFGThrHandle = (HANDLE)_beginthreadex(NULL,  // do not suspend thread  
			0,      // use same stack size as with main thread 
			(unsigned(__stdcall *)(void*))&FGRun,  //unsigned ( __stdcall *start_address )( void * ) 
			pCView, //this->,   // parameters          
			0,      // start thread; don't suspend it       
			&m_uiFGThreadId);/* Win32 thread id.; note that this is  */

							 //m_cFGStopButton.EnableWindow(TRUE);

							 //BEEP_OK;
		MyLog(L"��ʼ�ɼ�ͼ����...");  //BEEP_ERROR;
	}
}
// ����ֹͣ��ť
void ST_MYDISP::OnButtonFGStop()
{
	BOOL b_FGIsRunning;

	//EnterCriticalSection(&newImageMutex);
	b_FGIsRunning = this->pCAM->GetIsFGRuning();
	//LeaveCriticalSection(&newImageMutex);

	if (b_FGIsRunning)
	{
		// send stop signal to running framegrabber
		SetEvent(m_hFGStopEvent);

		//m_cFGStopButton.EnableWindow(FALSE);
		//m_cFGStartButton.EnableWindow(TRUE);
		//BEEP_OK;
		MyLog(L"stop ok.");  //BEEP_ERROR;

		this->DisCloseCamera();   // ������� 
	}
	else
	{
		// nothing to stop
		//BEEP_ERROR;
		MyLog(L"nothing to stop...");  //BEEP_ERROR;
	}
}

BOOL ST_MYDISP::CopyCaliPoseAndParamterFile(int oldPr, int newPr)
{



	if (!this->pCAM->Read_CaliCal(this->hv_CamParameters, newPr)) {
		MyLog(L"��ȡ����ڲ��ļ�ʧ��!");

		// ����һ��
		if (this->pCAM->Read_CaliCal(this->hv_CamParameters, oldPr)) {

			CString  of = this->pCAM->GetCaliCalPathFile(oldPr);
			CString  nf = this->pCAM->GetCaliCalPathFile(newPr);

			CopyFile(of, nf, false);

			MyLog(L"�����ڲ��ļ��ɹ���");
		}
	}

	if (!this->pCAM->Read_CaliPose(this->hv_CamPose, newPr)) {
		MyLog(L"��ȡ�����̬�ļ�ʧ��!");

		// ����һ��
		if (this->pCAM->Read_CaliPose(this->hv_CamPose, oldPr)) {

			CString of = this->pCAM->GetCaliPoseDataPathFile(oldPr);
			CString nf = this->pCAM->GetCaliPoseDataPathFile(newPr);

			CopyFile(of, nf, false);

			MyLog(L"���������̬�ļ��ɹ���");
		}
	}

	return 0;
}

// ��������궨����
BOOL ST_MYDISP::LoadCaliPoseAndParamter()
{
	try {
		this->DisOpenCamera();
		int isWHbyCamera = UCI::OPTIONS[L"ORGwhByCamera"];
		if (!isWHbyCamera) {
			write_message(100, 20, "������򿪳���");
			return FALSE;
		}


		int prID = UCI::OPTIONS[L"��Ŀ���"];
		// ��������ڲ�
		if (!this->pCAM->Read_CaliCal(this->hv_CamParameters, prID)) {
			MyLog(L"��ȡ����ڲ��ļ�ʧ��!");
			return FALSE;
		}

		if (!this->pCAM->Read_CaliPose(this->hv_CamPose, prID)) {
			MyLog(L"��ȡ�����̬�ļ�ʧ��!");
			return FALSE;
		}
		// ��������ԭ�� 
		double orgX = UCI::OPTIONS[L"�궨ԭ�� X ����"];
		double orgY = UCI::OPTIONS[L"�궨ԭ�� Y ����"];
		double orgZ = UCI::OPTIONS[L"�궨ԭ�� Z ����"];

		double angleX = UCI::OPTIONS[L"�궨 X �� �Ƕ�"];
		double angleY = UCI::OPTIONS[L"�궨 Y �� �Ƕ�"];
		double angleZ = UCI::OPTIONS[L"�궨 Z �� �Ƕ�"];

		HTuple htPos = this->hv_CamPose.ConvertToTuple();

		htPos[3] = htPos[3] - angleX;  // X ת  ��
		htPos[4] = htPos[4] - angleY;  // X ת  ��
		htPos[5] = htPos[5] - angleZ;  // X ת  ��

		this->hv_CamPose = HPose(htPos[0], htPos[1], htPos[2], htPos[3], htPos[4], htPos[5], "Rp+T", "gba", "point");

		double TmpCtrl_RectificationWidth = UCI::OPTIONS[L"ӳ����"];
		TmpCtrl_RectificationWidth = TmpCtrl_RectificationWidth / 1000;

		hv_CamPose = hv_CamPose.SetOriginPose(orgX*TmpCtrl_RectificationWidth, orgY*TmpCtrl_RectificationWidth, orgZ);

		int width = UCI::OPTIONS[L"ͼ����"];
		int height = UCI::OPTIONS[L"ͼ��߶�"];

		// ���� ImagMap
		this->ho_RectMap.GenImageToWorldPlaneMap(this->hv_CamParameters,
			this->hv_CamPose,
			width,
			height,
			width,
			height,
			TmpCtrl_RectificationWidth / width,
			"bilinear");

		UCI::OPTIONS[L"�궨��Ϣ OK"] = 1;      // �ѽ��������

		MyLog(L"���������̬�����ڲα궨��Ϣ�ɹ��ˣ�");
		return TRUE;
	}
	catch (HalconCpp::HException& except) {

		UCI::OPTIONS[L"�궨��Ϣ OK"] = 0;      // �ѽ��������
		UCI::sDB.Log(except, L"LoadCaliPoseAndParamter()��");
		return FALSE;
	}
}



// ƥ�����һ��ͼ
BOOL ST_MYDISP::OnDisMeasureOnePic(UCI::OptionsMap * camo, HImage * img)
{
	try {
		BOOL isUseStorePic = UCI::OPTIONS[L"TestUseStorePicture"];
		if (isUseStorePic) {   // ����һ������ͼ			
			HImage ImageAlreadyMapped;
			CString name = UCI::OPTIONS[L"TestUseStorePicName"];

			string fnmae = CT2A(UCI::stPictureSavePath).m_psz;
			fnmae = fnmae + "\\" + CT2A(name).m_psz;

			ImageAlreadyMapped.ReadImage(fnmae.c_str());

			HTuple hv_width, hv_height;

			ImageAlreadyMapped.GetImageSize(&hv_width, &hv_height);

			//MapImage(ImageMapped, this->ho_RectMap, &ImageMapped);
			m_sIPResult.result_img = ImageAlreadyMapped;

			int oldW = UCI::OPTIONS[L"ͼ����"];
			int oldH = UCI::OPTIONS[L"ͼ��߶�"];
			int newW = hv_width.I();
			int newH = hv_height.I();

			if (oldW != newW || oldH != newH) {
				// �õ�ͼƬ�Ŀ�ȣ��볤��
				UCI::OPTIONS[L"ͼ����"] = hv_width.I();
				UCI::OPTIONS[L"ͼ��߶�"] = hv_height.I();
				UCI::OPTIONS[L"ORGwhByCamera"] = 0;
			}

			this->m_pMHWinMain->SetPart(0, 0, hv_height.I() - 1, hv_width.I() - 1);
			this->m_pMHWinMain->DispObj(m_sIPResult.result_img);
			// ����һ������ͼ

			BOOL isLoad = UCI::OPTIONS[L"�궨��Ϣ OK"];
			if (isLoad == FALSE) {
				if (this->LoadCaliPoseAndParamter() == FALSE) {
					write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");
					return FALSE;
				}
			}
		}
		else {

			DisOpenCamera();

			int oldW = UCI::OPTIONS[L"ͼ����"];
			int oldH = UCI::OPTIONS[L"ͼ��߶�"];

			this->m_pMHWinMain->SetPart(0, 0, oldH - 1, oldW - 1);


			//BOOL isLoad = UCI::OPTIONS[L"�궨��Ϣ OK"];
			//if (isLoad == FALSE) {
			//	if (this->LoadCaliPoseAndParamter() == FALSE) {
			//		write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");
			//		return;
			//	}
			//}

			UCI::sDB.SetCamPara(*camo);

			this->pCAM->OneShot();
			EnterCriticalSection(&this->m_csFGDisp);
			this->pCAM->GetBufferImage(&this->GrabImageDisp);
			*img = this->GrabImageDisp;                 // ȡ��ͼ��
			LeaveCriticalSection(&this->m_csFGDisp);
			this->OnFGDisp(NULL, NULL);                 // ���Դ�����ʾ	

		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"OnDisMeasureOnePic()��");
		return FALSE;
	}

	return TRUE;
}


CString ST_MYDISP::GetMatchPathFile(int prID)
{


	CString format = UCI::OPTIONS[L"���񱣴��ʽ"];
	CString stDeviceName = UCI::OPTIONS[L"�������"];

	CString str;
	str.Format(L"%d", prID);


	CString res = UCI::strMoudlePath + L"\\project\\"
		+ str + L"\\ROI\\"
		+ stDeviceName + L"\\roimatch."
		+ format;
	return res;
}

CString ST_MYDISP::GetShapeModelPathFile(int prID)
{

	CString stDeviceName = UCI::OPTIONS[L"�������"];

	CString str;
	str.Format(L"%d", prID);

	CString res = UCI::strMoudlePath + L"\\project\\"
		+ str + L"\\ROI\\" +
		stDeviceName + L"\\roishapemodel.shm";

	return res;
}

CString ST_MYDISP::GetShapeModelRegionPathFile(int prID)
{

	CString stDeviceName = UCI::OPTIONS[L"�������"];

	CString str;
	str.Format(L"%d", prID);

	CString res = UCI::strMoudlePath + L"\\project\\"
		+ str + L"\\ROI\\" +
		stDeviceName + L"\\roishmRegion.hobj";

	return res;
}


// ��ʼ��
ST_MYDISP::ST_MYDISP(CWnd* cwnd)
{
	pCView = cwnd;

	rcImage = { 0,0,600,800 };
	m_bRectMapOK = false;

	pFGWinStatic = NULL;
	PMainStatic = NULL;

	m_hIPThrHandle = NULL;

	pCAM = new CCameraCPP();

	this->m_sIPResult.result_img.GenEmptyObj();
	this->m_sIPResult.result_cont.GenEmptyObj();
	GrabImageDisp.GenEmptyObj();

	this->ho_ModelRegion.GenEmptyObj();

	this->mAppState = APP_READY;            // ��һ�γ�ʼ��

	this->CrossLine[0] = { 100,100,RGB(255,0,0),true };
	this->CrossLine[1] = { 1900,1400,RGB(0,255,0),true };
	this->CrossLine[2] = { 1000,700,RGB(0,0,255),true };

	this->hv_Model.Clear();

}

// 
ST_MYDISP::~ST_MYDISP()
{
	this->OnDestroy();
	delete this->pCAM;
}


// �ǲ����ܼ���ͼ�������������ˣ��Ͳ��ܼ���ͼƬ��
bool ST_MYDISP::IsCanLoadPicture()
{

	BOOL b_FGIsRunning;
	b_FGIsRunning = this->pCAM->GetIsFGRuning();

	if (b_FGIsRunning) {
		return false;
	}
	return true;
}

void ST_MYDISP::DisOpenCamera()
{

	if (!this->pCAM->GetIsOpen()) {
		this->pCAM->Connect();

		this->ResetAllCStaticPart();
		this->ResetAllCStaticWinExtends();
	}
	else {
		MyLog(L"����Ѵ�");
	}
}

void ST_MYDISP::DisCloseCamera()
{
	if (this->pCAM->close_camera()) {
		UCI::MyLog(L"��ǰ������ѹر��ˣ�");
	}
}



// ������̬ͼƬ
bool ST_MYDISP::SaveOneCaliPosePicture(int prid, HImage* img) {

	try {
		int prid = UCI::OPTIONS[L"��Ŀ���"];

		CString  Path = this->pCAM->GetCaliPosePicPath(prid);

		CString  PathName = Path + L"\\" + this->GetTimeMsToString() + L".png";


		if (Write_Image(*img, PathName)) {
			CString str;
			str.Format(L"�ɹ�������һ���궨��̬ͼ��. ��ǰ���� %d �궨OK��ͼ��",
				this->GetTotalCariPosePicNum(prid));
			UCI::MyLog(str);
			return true;
		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"SaveOneCaliPosePicture��");
	}

	return false;
}

bool ST_MYDISP::SaveOneCaliInnerPicture(int prid, HImage* img)
{
	try {
		CString  Path = this->pCAM->GetCaliInnerPicPath(prid);

		CString  PathName = Path + L"\\"
			+ this->GetTimeMsToString() + L".png";


		if (Write_Image(*img, PathName))
		{
			CString str;
			str.Format(L"�ɹ�������һ���궨�ڲ�ͼ��. ��ǰ���� %d �궨OK��ͼ��",
				this->GetTotalCariInnerPicNum(prid));
			UCI::MyLog(str);
			return true;
		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"SaveOneCaliInnerPicture��");
	}
	return false;
}

CString ST_MYDISP::GetTimeMsToString()
{
	CTime t = CTime::GetCurrentTime();
	CString s = t.Format(L"%Y%m%d%H%M%S");

	return s;
}

int ST_MYDISP::GetTotalCariInnerPicNum(int prID)
{
	return UCI::FindSpecialFile(this->pCAM->GetCaliInnerPicPath(prID), L"*.png", NULL);
}

// ɾ�����еı궨�ڲ�ͼ�� 
void ST_MYDISP::DelTotalCariInnerPic(int prID)
{
	UCI::FindSpecialFile(this->pCAM->GetCaliInnerPicPath(prID), L"*.png", NULL, true);
}

int ST_MYDISP::GetTotalCariPosePicNum(int prID) {
	return UCI::FindSpecialFile(this->pCAM->GetCaliPosePicPath(prID), L"*.png", NULL);
}
// ɾ�����еı궨��̬ͼ�� 
void ST_MYDISP::DelTotalCariPosePic(int prID)
{
	UCI::FindSpecialFile(this->pCAM->GetCaliPosePicPath(prID), L"*.png", NULL, true);
}











