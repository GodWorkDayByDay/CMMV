#include "stdafx.h"
#include "MainFrm.h"
#include "ChildView.h"

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;
using namespace std;

// �궨�ڲδ����߳�
void ST_MYDISP::IPRunCamCaliInner(void *Param)
{
	HANDLE         lphEvents[2];
	HImage         Image;
	HXLDCont       Contours;
	Contours.GenEmptyObj();

	HTuple T1, T2;

	CChildView *pCV = (CChildView *)Param;
	const HWND PostWnd = pCV->GetSafeHwnd();

	ST_MYDISP* pDis = UCI::pDis;

	lphEvents[0] = pDis->pCAM->GetNewImageEvent();
	lphEvents[1] = pDis->m_hFGStopEvent;

	pDis->pCAM->SetbOnePIcMaxErrorOK(false);  // ����ͼƬ��Ч  



	while (WAIT_OBJECT_0 == WaitForMultipleObjects(2, lphEvents, FALSE, INFINITE))
	{
		pDis->pCAM->GetBufferImage(&Image);

		// ���Ҫ����һ��ͼ����, ������ô���
		//UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_FUNCTION);

		//BOOL old = (*om)[L"USE_map_image"];
		//(*om)[L"USE_map_image"] = FALSE;
		//UCI::pDis->MeasureOneFUNCTION(om, Image, Contours);
		//(*om)[L"USE_map_image"] = old;

		CountSeconds(&T1);
		pDis->DisCamliInnerOnePicture(&Image, &Contours);
		CountSeconds(&T2);

		pDis->m_sIPResult.time_needed = 1000 * (T2[0].D() - T1[0].D());


		HTuple time = pDis->m_sIPResult.time_needed;

		CString str;
		str.Format(_T("�ڲα궨 >��ǰ����ʱ��Ϊ: %4.2f ms"), time[0].D());
		UCI::MyLog(str);

		int delay = UCI::OPTIONS[L"����궨��ʱ"];
		Sleep(delay);
	}
	return;
}

// �ڲ�ȡͼ
void ST_MYDISP::DisCamliInnerOnePicture(HalconCpp::HImage * Image, HalconCpp::HXLDCont*  Contours)
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
	int nPic = this->GetTotalCariInnerPicNum(prID);
	int nTotal = UCI::OPTIONS[L"�궨ͼƬ����"];


	if (nPic >= nTotal) {                      // ͼƬ���������㣬���Լ��㲢����궨�ڲΡ�		
		this->DisGetCameraInnerParaFromPic();  // ȡ�õ�������ڲ�������

		this->StopContiouscap();
		this->AppStateChangeTo(APP_READY);
		return;
	}

	try
	{
		hv_TmpCtrl_ReferenceIndex = 0;

		// �궨�����ļ�	
		CString descFile = this->pCAM->GetCaliDescribePathFile(prID);
		HTuple hv_TmpCtrl_PlateDescription = CT2A(descFile).m_psz;

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
		// ����ͼ���еı궨��
		m_HCali.FindCalibObject(*Image, 0, 0, hv_Index, hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// ����һ��ͼ�����ɫ
		this->m_pMHWinMain->SetColor("magenta");

		// �õ��궨�����̬
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
			// ����ͼ��
			//this->GrabImageERROK = *Image;
			write_message(100, 8, "���½�ͼ��ť����ɱ���ͼ���뾡��ѡȡ��ͬ��λ�ã�", 0);

			if (this->PushCaptureDown == 1) {
				this->PushCaptureDown = 0;
				this->SaveOneCaliInnerPicture(prID, Image);   // ����һ���궨��ͼ��  
				this->pCAM->SetbOnePIcMaxErrorOK(false);
			}
			else {
				Sleep(50);
			}

			// ʹ�ܰ�ť
			UCI::RIBmenu.Rib_CALI_CAPTURE = true;

		}
		else {
			this->pCAM->SetbOnePIcMaxErrorOK(false);

			// ʹ�ܰ�ť
			UCI::RIBmenu.Rib_CALI_CAPTURE = false;

			write_message(100, 8, "��ǰͼ�����̫������һ�����û�ͷ��", 0);
		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"���ڲɼ� �ڲ� ͼ��, �����������⣺");

		// ʹ�ܰ�ť
		UCI::RIBmenu.Rib_CALI_CAPTURE = false;
	}

	PostMessage(UCI::pMain->m_hWnd, UM_UPDATE_RIBBON, (WPARAM)0, (LPARAM)0);
}

void ST_MYDISP::DisSetInnerAndPosStartPara(
	HalconCpp::HTuple & hv_StartParameters,
	HalconCpp::HTuple & hv_TmpCtrl_FindCalObjParNames, 
	HalconCpp::HTuple & hv_TmpCtrl_FindCalObjParValues)
{

	// �궨��ʼ������� ��ɨ�裬����ʽ
	hv_StartParameters.Clear();     // 
	double focus = UCI::OPTIONS[L"��ͷ����"];
	hv_StartParameters[0] = focus / 1000.0;
	hv_StartParameters[1] = 0;
	hv_StartParameters[2] = 0;
	hv_StartParameters[3] = 0;
	hv_StartParameters[4] = 0;
	hv_StartParameters[5] = 0;

	double fsx = UCI::OPTIONS[L"���ؿ��"];
	double fsy = UCI::OPTIONS[L"���ظ߶�"];
	hv_StartParameters[6] = fsx / 1e+006;
	hv_StartParameters[7] = fsy / 1e+006;

	hv_StartParameters[8] = this->GetWidth() / 2;
	hv_StartParameters[9] = this->GetHeight() / 2;
	hv_StartParameters[10] = this->GetWidth();
	hv_StartParameters[11] = this->GetHeight();

	// FindCalObjParNames;

	hv_TmpCtrl_FindCalObjParNames.Clear();
	hv_TmpCtrl_FindCalObjParNames[0] = "gap_tolerance";
	hv_TmpCtrl_FindCalObjParNames[1] = "alpha";
	hv_TmpCtrl_FindCalObjParNames[2] = "max_diam_marks";
	hv_TmpCtrl_FindCalObjParNames[3] = "skip_find_caltab";

	// FindCalObjParValues

	double gap_tolerance = UCI::OPTIONS[L"�궨�����������"];
	double alpha = UCI::OPTIONS[L"�궨ƽ��alpha"];
	double max_diam_marks = UCI::OPTIONS[L"�궨����־��ֱ��"];

	hv_TmpCtrl_FindCalObjParValues.Clear();
	hv_TmpCtrl_FindCalObjParValues[0] = gap_tolerance;
	hv_TmpCtrl_FindCalObjParValues[1] = alpha;
	hv_TmpCtrl_FindCalObjParValues[2] = max_diam_marks;
	hv_TmpCtrl_FindCalObjParValues[3] = "false";
}



// ��ͼƬ��ȡ���ڲΣ�������
void ST_MYDISP::DisGetCameraInnerParaFromPic()
{
	HTuple hv_TmpCtrl_ReferenceIndex;
	HTuple hv_StartParameters;
	HTuple hv_TmpCtrl_FindCalObjParNames;
	HTuple hv_TmpCtrl_FindCalObjParValues;
	HTuple hv_CameraParameters;
	HTuple hv_CameraPose;
	HTuple hv_TmpCtrl_Errors;

	int prID = UCI::OPTIONS[L"��Ŀ���"];
	int nPic = this->GetTotalCariInnerPicNum(prID);
	int nTotal = UCI::OPTIONS[L"�궨ͼƬ����"];        // 

	if (nPic < nTotal) {                       // ͼƬ����������
		write_message(8, 8, "�궨ͼ������̫С������" + HTuple(nTotal - nPic) + "��ͼ��", 0);
		return;
	}
	try {

		hv_TmpCtrl_ReferenceIndex = 0;

		// �궨�����ļ�
		HTuple hv_TmpCtrl_PlateDescription = CT2A(this->pCAM->GetCaliDescribePathFile(prID));

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
		UCI::FindSpecialFile(this->pCAM->GetCaliInnerPicPath(prID), L"*.png", &caFileName);

		int nTpic = (int)caFileName.GetSize();
		for (int i = 0; i < nTpic; i++) {

			//this->Read_Image(Image, caFileName[i]);

			Image.ReadImage(CT2A(caFileName[i]).m_psz);

			//EnterCriticalSection(&this->m_csFGDisp);
			//this->GrabImageDisp = Image;
			//LeaveCriticalSection(&this->m_csFGDisp);		
			this->m_pMHWinMain->DispObj(Image);
			//this->DisDisplay();
			write_message(12, 12, "���ڴ�" + HTuple(i) + "�ű궨���ռ����ݡ�����", 0);

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

			// ȡ��������ڲ�
			hv_CameraParameters = m_HCali.GetCalibData("camera", 0, "params");

			if (this->pCAM->Write_CaliCal(hv_CameraParameters, prID)) {
				write_message(60, 8, "��������ڲ�������ļ��ɹ���", 0);
			}

			write_message(300, 8, "����ڲΣ�" + hv_CameraParameters, 0);

			double  curErr = hv_TmpCtrl_Errors.D();
			UCI::OPTIONS[L"�궨��ǰ���"] = curErr;

		}
		else {
			write_message(60, 8, "�ܹ�" + HTuple(nTotal - nPic) + "��ͼ��,"
				+ "�궨ʧ�ܣ��궨����:" + hv_TmpCtrl_Errors, 1);

			write_message(120, 8, "�����²ɼ�ͼ��궨��", 0);
		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"��ͼƬ��ȡ���ڲ�, �����������⣺");
	}
}