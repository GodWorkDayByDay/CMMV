#include "stdafx.h"
#include "MainFrm.h"
#include "ChildView.h"

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;
using namespace std;


// �궨��δ����߳�
void ST_MYDISP::IPRunCamCaliPose(void *Param)
{
	HANDLE         lphEvents[2];
	HImage         Image;
	HXLDCont       Contours;
	Contours.GenEmptyObj();

	HTuple T1, T2;

	CChildView *pCV = (CChildView *)Param;
	const HWND PostWnd = pCV->GetSafeHwnd();

	//ST_MYDISP* pDis = pCV->pDis;

	lphEvents[0] = UCI::pDis->pCAM->GetNewImageEvent();
	lphEvents[1] = UCI::pDis->m_hFGStopEvent;

	UCI::pDis->pCAM->SetbOnePIcMaxErrorOK(false);  // ����ͼƬ��Ч  



	while (WAIT_OBJECT_0 == WaitForMultipleObjects(2, lphEvents, FALSE, INFINITE))
	{
		UCI::pDis->pCAM->GetBufferImage(&Image);

		// ���Ҫ����һ��ͼ����
		//UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_FUNCTION);
		//BOOL old = (*om)[L"USE_map_image"];
		//(*om)[L"USE_map_image"] = FALSE;
		//UCI::pDis->MeasureOneFUNCTION(om, Image, Contours);
		//(*om)[L"USE_map_image"] = old;

		CountSeconds(&T1);
		UCI::pDis->DisCamliPoseOnePicture(&Image, &Contours);
		CountSeconds(&T2);

		UCI::pDis->m_sIPResult.time_needed = 1000 * (T2[0].D() - T1[0].D());


		HTuple time = UCI::pDis->m_sIPResult.time_needed;
		//sVal.Format(_T("�����̬�궨 >��ǰ����ʱ��Ϊ: %4.2f ms"), time[0].D())

		//CString *sVal = new CString();
		//sVal->Format(_T("�����̬�궨 >��ǰ����ʱ��Ϊ: %4.2f ms"), time[0].D());

		CString str;
		str.Format(_T("�����̬�궨 >��ǰ����ʱ��Ϊ: %4.2f ms"), time[0].D());
		UCI::MyLog(str);

	}
	return;
}

// ��̬ȡͼ
void ST_MYDISP::DisCamliPoseOnePicture(HalconCpp::HImage * Image, HalconCpp::HXLDCont*  Contours)
{
	HTuple hv_TmpCtrl_ReferenceIndex;
	HTuple hv_StartParameters;
	HTuple hv_TmpCtrl_FindCalObjParNames;
	HTuple hv_TmpCtrl_FindCalObjParValues;
	//HTuple hv_CalibHandle;
	HTuple hv_CameraParameters;
	HTuple hv_CameraPose;
	HTuple hv_TmpCtrl_Errors;
	HTuple hv_Index;

	Contours->GenEmptyObj();
	int prID = UCI::OPTIONS[L"��Ŀ���"];
	int nPic = this->GetTotalCariPosePicNum(prID);
	int nTotal = 1;         // pose ֻҪһ��

	if (nPic >= nTotal) {                      // ͼƬ���������㣬���Լ��㲢����궨�ڲΡ�		
		this->DisGetCameraPoseParaFromPic();   // ȡ�õ�������ⲿ������

		this->StopContiouscap();

		Sleep(100);
		this->AppStateChangeTo(APP_READY);
		return;
	}

	this->pCAM->SetbOnePIcMaxErrorOK(false);   // ����ͼƬ��Ч  
	try
	{
		hv_TmpCtrl_ReferenceIndex = 0;

		// �궨�����ļ�	
		HTuple hv_TmpCtrl_PlateDescription = CT2A(this->pCAM->GetCaliDescribePathFile(prID)).m_psz;

		// �궨��ʼ������� 
		DisSetInnerAndPosStartPara(hv_StartParameters,
			hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// �����궨���
		HCalibData m_HCali;
		m_HCali.CreateCalibData("calibration_object", 1, 1);
		// ���ñ궨��ʼ����
		CString calimodel = UCI::OPTIONS[L"CamCalibrateModel"];
		m_HCali.SetCalibDataCamParam(0,
			CT2A(calimodel).m_psz,
			hv_StartParameters);
		// ���ñ궨�����ļ�
		m_HCali.SetCalibDataCalibObject(0, hv_TmpCtrl_PlateDescription);

		hv_Index = 0;
		// ����ͼ���еı궨ͼ��
		m_HCali.FindCalibObject(*Image, 0, 0, hv_Index, hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// ����һ��ͼ�����ɫ
		this->m_pMHWinMain->SetColor("magenta");
		HPose hv_pose = m_HCali.GetCalibDataObservPose(0, 0, hv_Index);

		// ��ʾͼ��
		this->m_pMHWinMain->DispObj(*Image);
		// ��ʾ�궨��Բ����
		this->m_pMHWinMain->DispCaltab(hv_TmpCtrl_PlateDescription,
			hv_StartParameters, hv_pose, 1.0);

		// ��ʾ�궨����������
		this->m_pMHWinMain->SetColor("red");
		this->m_pMHWinMain->SetLineWidth(2);

		*Contours = m_HCali.GetCalibDataObservContours("caltab", 0, 0, 0);

		this->m_pMHWinMain->DispObj(*Contours);

		// ����궨����
		hv_TmpCtrl_Errors = m_HCali.CalibrateCameras();

		write_message(8, 8, "���ڲɼ��궨ͼ�񣬵�ǰ���:" + hv_TmpCtrl_Errors
			+ "���أ��Ѿ��ɼ���" + HTuple(nPic)
			+ "��������" + HTuple(nTotal - nPic) + "��ͼ��", 1);

		double picError = UCI::OPTIONS[L"�궨�������"];
		if (hv_TmpCtrl_Errors <  picError) {
			this->pCAM->SetbOnePIcMaxErrorOK(true);
			// ������̬ͼ��
			//this->GrabImageERROK = *Image;
			write_message(100, 8, "���½�ͼ��ť����ɱ���ͼ���뾡��ѡȡ��ͬ��λ�ã�", 0);

			// ʹ�ܰ�ť UCI::RIBmenu.Rib_CALI_CAPTURE
			UCI::RIBmenu.Rib_CALI_CAPTURE = true;

			if (this->PushCaptureDown == 1) {
				this->PushCaptureDown = 0;
				this->SaveOneCaliPosePicture(prID, Image);   // ����һ���궨��ͼ��  
				this->pCAM->SetbOnePIcMaxErrorOK(false);
			}
			else {
				Sleep(50);
			}
		}
		else {
			this->pCAM->SetbOnePIcMaxErrorOK(false);
			write_message(100, 8, "��ǰͼ�����̫������һ�����û�ͷ��", 1);

			// ʹ�ܰ�ť
			UCI::RIBmenu.Rib_CALI_CAPTURE = false;
		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"���ڲɼ� ��̬ ͼ��, �����������⣺");

		// ʹ�ܰ�ť
		UCI::RIBmenu.Rib_CALI_CAPTURE = false;
	}

	PostMessage(UCI::pMain->m_hWnd, UM_UPDATE_RIBBON, (WPARAM)0, (LPARAM)0);
}

// ��ͼƬ��ȡ����̬��������
void ST_MYDISP::DisGetCameraPoseParaFromPic()
{
	HTuple hv_TmpCtrl_ReferenceIndex;
	HTuple hv_StartParameters;
	HTuple hv_TmpCtrl_FindCalObjParNames;
	HTuple hv_TmpCtrl_FindCalObjParValues;
	// HTuple hv_CalibHandle;
	HTuple hv_CameraParameters;
	//HTuple hv_CameraPose;
	HTuple hv_TmpCtrl_Errors;
	//HTuple hv_Index;
	int prID = UCI::OPTIONS[L"��Ŀ���"];
	int nPic = this->GetTotalCariPosePicNum(prID);
	int nTotal = 1;        // 

						   //this->DisDisplay();                        // ��ʾһ�µ�ǰ��ͼƬ
	if (nPic < nTotal) {                       // ͼƬ����������
		write_message(8, 8, "�궨��̬ͼ������̫С������" + HTuple(nTotal - nPic) + "��ͼ��", 1);
		return;
	}
	try {

		hv_TmpCtrl_ReferenceIndex = 0;

		// �궨�����ļ�
		HTuple hv_TmpCtrl_PlateDescription = CT2A(this->pCAM->GetCaliDescribePathFile(prID)).m_psz;

		// �궨��ʼ������� ��ɨ�裬����ʽ
		DisSetInnerAndPosStartPara(hv_StartParameters,
			hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// �����궨���
		HCalibData m_HCali;
		m_HCali.CreateCalibData("calibration_object", 1, 1);

		// ���ñ궨��ʼ����
		CString calimodel = UCI::OPTIONS[L"CamCalibrateModel"];
		m_HCali.SetCalibDataCamParam(0,
			CT2A(calimodel).m_psz,
			hv_StartParameters);

		// ���ñ궨�����ļ�
		m_HCali.SetCalibDataCalibObject(0, hv_TmpCtrl_PlateDescription);

		HalconCpp::HImage Image;

		CArray<CString, CString&> caFileName;     // ȡ�����е��ļ���
		UCI::FindSpecialFile(this->pCAM->GetCaliPosePicPath(prID), L"*.png", &caFileName);

		int nTpic = (int)caFileName.GetSize();
		for (int i = 0; i < nTpic; i++) {

			//this->Read_Image(Image, caFileName[i]);

			//HTuple h, w;
			//Image.GetImageSize(&w, &h);

			//int h1 = h.I();
			//int w1 = w.I();

			Image.ReadImage(CT2A(caFileName[i]).m_psz);

			//EnterCriticalSection(&this->m_csFGDisp);
			//this->GrabImageDisp = Image;
			//LeaveCriticalSection(&this->m_csFGDisp);
			this->m_pMHWinMain->DispObj(Image);
			//this->DisDisplay();
			write_message(12, 12, "���ڴ�" + HTuple(i) + "�ű궨���ռ����ݡ�����", 1);

			// ����ͼ���еı궨ͼ��
			m_HCali.FindCalibObject(Image, 0, 0, i, hv_TmpCtrl_FindCalObjParNames,
				hv_TmpCtrl_FindCalObjParValues);
		}

		// ����һ��ͼ�����ɫ
		this->m_pMHWinMain->SetColor("green");

		// ȡ�ñ궨��������Ϣ
		HPose hv_pose = m_HCali.GetCalibDataObservPose(0, 0, nTpic - 1);

		// ��ʾ�궨��Բ����
		this->m_pMHWinMain->DispCaltab(hv_TmpCtrl_PlateDescription,
			hv_StartParameters, hv_pose, 1.0);

		// ȡ�ñ궨����������
		HalconCpp::HXLDCont  Contours
			= m_HCali.GetCalibDataObservContours("caltab", 0, 0, nTpic - 1);

		// ��ʾ�궨����������
		this->m_pMHWinMain->SetColor("green");
		this->m_pMHWinMain->SetLineWidth(2);
		this->m_pMHWinMain->DispObj(Contours);

		// ����궨����
		hv_TmpCtrl_Errors = m_HCali.CalibrateCameras();

		double picError = UCI::OPTIONS[L"�궨�������"];
		if (hv_TmpCtrl_Errors < picError) {
			write_message(8, 8, "�ܹ��궨��" + HTuple(nTpic) + "��ͼ��,"
				+ "�궨�ɹ����궨���:" + hv_TmpCtrl_Errors, 1);

			// ȡ���������̬
			//m_HCali.GetCalibDataObservPose

			HPose hv_pose = m_HCali.GetCalibDataObservPose(0, 0, nTpic - 1);

			//hv_CameraPose = m_HCali.GetCalibData("calib_obj_pose", (Hlong)HTuple(0).TupleConcat(hv_TmpCtrl_ReferenceIndex), "pose");

			//
			////GetCalibData(hv_CalibHandle, "calib_obj_pose", HTuple(0).TupleConcat(hv_TmpCtrl_ReferenceIndex),
			////	"pose", &hv_CameraPose);

			//// write_message(600, 600, "�����̬pre��" + hv_CameraPose, 0);

			////Calibration 01: Adjust origin for plate thickness
			//SetOriginPose(hv_CameraPose, 0.0, 0.0, this->pCAM->GetCaliPlateThick(), &hv_CameraPose);

			if (this->pCAM->Write_CaliPose(hv_pose, prID)) {
				write_message(160, 8, "�����������ļ��ɹ���", 1);
			}

			write_message(600, 32, "�����̬��" + hv_pose, 0);

			Sleep(500);
		}
		else {
			write_message(8, 8, "�ܹ�" + HTuple(nTotal - nPic) + "��ͼ��,"
				+ "�궨��̬ʧ�ܣ��궨����:" + hv_TmpCtrl_Errors, 1);

			write_message(32, 8, "�����²ɼ�ͼ��궨��", 0);
		}

		//Calibration 01: Clear calibration model when done
		//ClearCalibData(hv_CalibHandle);
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"��ͼƬ��ȡ����̬, �����������⣺");
	}

}