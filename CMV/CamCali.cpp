#include "stdafx.h"
#include "MainFrm.h"
#include "ChildView.h"

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;
using namespace std;



// �궨��ť����, ֻ�Ǳ궨�ڲ�
void ST_MYDISP::DisOnButCamCaliInner()
{
	this->PushCaptureDown = 0;

	if (this->mAppState == App_CamInner) {
		this->StopContiouscap();
		this->AppStateChangeTo(APP_READY);
		this->MyLog(L"��ֹͣ�궨����");
		return;
	}

	if (!TestCurrentAppState(APP_LOGGING_OK)) return;


	int prID = UCI::OPTIONS[L"��Ŀ���"];
	CString CaliPathFile = this->pCAM->GetCaliCalPathFile(prID);

	if (PathFileExists(CaliPathFile))
	{
		int res = ::MessageBox(NULL,
			L"��� ȷ��  ���±궨��\n"
			"��� ȡ��  ���ء�",
			L"����ڲα궨�ļ��Ѵ��ڣ�",
			MB_OKCANCEL | MB_ICONSTOP);
		if (res == IDCANCEL) return;
		if (res == IDOK) {
			CFile TempFile;
			TempFile.Remove(CaliPathFile);
		}
	}

	int total = UCI::OPTIONS[L"�궨ͼƬ����"];
	// ��ǰͼƬ������������
	if (this->GetTotalCariInnerPicNum(prID) >= total) {
		int res = ::MessageBox(NULL,
			L"��� ȷ��  ���²ɼ�ͼƬ��\n"
			"��� ȡ��  ���ء�",
			L"����궨�ڲ�ͼƬ�����ѴﵽҪ���ˡ���",
			MB_OKCANCEL | MB_ICONSTOP);
		if (res == IDCANCEL) return;
		if (res == IDOK) {
			this->DelTotalCariInnerPic(prID); // ��ɾ�����еĲɼ�ͼ���ļ�
		}
	}		

	this->DisOpenCamera();                                 // �����

	UCI::OptionsMap * camo = &UCI::ELE.EleList.GetHead();
	UCI::sDB.SetCamPara(*camo);
	

	// �趨һ�µ�ǰ������궨����	
	this->AppStateChangeTo(App_CamInner);
	this->StartContiouscap();
}

// ���水ť
void ST_MYDISP::DisOnSaveOnCaliPicture(int prID)
{
	// �����ǰ�Ǳ궨ģʽ����Ҫ����ͼƬ


	this->PushCaptureDown = 1; 
}


// �궨��ť����, ֻ�Ǳ궨��̬
void ST_MYDISP::DisOnButCamCaliPose()
{
	this->PushCaptureDown = 0;

	if (this->mAppState == App_CamPose) {
		this->StopContiouscap();
		this->AppStateChangeTo(APP_READY);
		this->MyLog(L"��ֹͣ�궨��̬��");
	}
	int prID = UCI::OPTIONS[L"��Ŀ���"];
	// �����ǲ����Ѵ����˱궨�ڲ��ļ��������̬�ļ�  PathFileExists
	if (PathFileExists(this->pCAM->GetCaliPoseDataPathFile(prID)))
	{
		int res = ::MessageBox(NULL,
			L"��� ȷ��  ���±궨��\n"
			"��� ȡ��  ���ء�",
			L"�����̬�궨�ļ��Ѵ��ڣ�",
			MB_OKCANCEL | MB_ICONSTOP);
		if (res == IDCANCEL) return;
		if (res == IDOK) {
			CFile TempFile;
			TempFile.Remove(this->pCAM->GetCaliPoseDataPathFile(prID));
		}
	}

	//int prID = UCI::OPTIONS[L"��Ŀ���"];

	// ��ǰͼƬ������������
	if (this->GetTotalCariPosePicNum(prID) >= 1) {
		int res = ::MessageBox(NULL,
			L"��� ȷ��  ���²ɼ�ͼƬ��\n"
			"��� ȡ��  ���ء�",
			L"����궨��̬ͼƬ�����ѴﵽҪ���ˡ���",
			MB_OKCANCEL | MB_ICONSTOP);
		if (res == IDCANCEL) return;
		if (res == IDOK) {
			this->DelTotalCariPosePic(prID); // ��ɾ�����еĲɼ�ͼ���ļ� 
		}
	}

	this->DisOpenCamera();   // �����

	UCI::OptionsMap * camo = &UCI::ELE.EleList.GetHead();
	UCI::sDB.SetCamPara(*camo);

	this->AppStateChangeTo(App_CamPose);
	this->StartContiouscap();

}

// ���ݱ궨��У��ͼ��
void ST_MYDISP::DisOnButCaliCorrect()
{
	
	BOOL isLoad = UCI::OPTIONS[L"�궨��Ϣ OK"];
	if (isLoad == FALSE) {
		if (this->LoadCaliPoseAndParamter() == FALSE) {
			write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");
			return;
		}
	}

	try{
		HImage ImageMapped;

		int width = UCI::OPTIONS[L"ͼ����"];
		int height = UCI::OPTIONS[L"ͼ��߶�"];

		this->m_pMHWinMain->SetPart(0, 0, height - 1, width - 1);

		MapImage(m_sIPResult.result_img, this->ho_RectMap, &ImageMapped);

		// this->m_sIPResult.result_img = ImageMapped;

		this->m_pMHWinMain->DispObj(ImageMapped);

	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"ST_MYDISP::DisOnButCaliCorrect()��");
	}

	// http://blog.csdn.net/u011058765/article/details/48469633

	int a = 0;
}

// ƥ��ͼ�񣬲ɼ�һ��ƥ���õ�ͼ���������趨��ӳ�䣬�� ���Ӵ���
void ST_MYDISP::DisOnMatchTakeOne()
{
	
	
	// 1 ��⵱ǰ��״̬
	if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;

	try {

		this->m_pMHWinMain->ClearWindow();

		// ���ֶ����������ǰ����
		HalconCpp::HXLDCont Contours;
		HalconCpp::HImage hImg;

		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_CAMERA);
		this->MeasureOneCamera(om, hImg, Contours);

	//	this->m_pMHWinMain->DispObj(hImg);

		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_ROI);
		this->MeasureOneROI(om, hImg, Contours);

	//	this->m_pMHWinMain->ClearWindow();
	//	this->m_pMHWinMain->DispObj(hImg);

		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_FUNCTION);
		this->MeasureOneFUNCTION(om, hImg, Contours);
		//om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
		//this->MeasureOneMATCH(om, hImg, Contours);
	//	this->m_pMHWinMain->ClearWindow();
	//	this->m_pMHWinMain->DispObj(hImg);

		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Contours;
		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp(NULL, NULL);              // ��������ʾһ��

	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnMatchTakeOne()��");
	}
}

// ��������ROI HXLD
void ST_MYDISP::DisDrawAllRoi(HalconCpp::HXLDCont& hv_Cont)
{
	POSITION pos = UCI::ELE.EleList.GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap *o = &UCI::ELE.EleList.GetNext(pos);
		this->DisDrawOneRoi(o, hv_Cont);
	}
}

void ST_MYDISP::DisDrawOneRoi(UCI::OptionsMap * o, HalconCpp::HXLDCont& hv_Cont)
{
	try {

		int type = (*o)[L"Ԫ������"];

		HalconCpp::HXLDCont hv_Out;

		switch (type) {
		case ELEMENT_LINE:
		{
			HTuple ht_Rows = (*o)[L"HTupleRows"];
			HTuple ht_Columns = (*o)[L"HTupleCols"];
			hv_Out.GenContourPolygonXld(ht_Rows, ht_Columns);

			//HTuple hv_Length = hv_Out.LengthXld();
			//this->write_message(300, 30, "hv_Length" + hv_Length);

			hv_Cont = hv_Cont.ConcatObj(hv_Out);
		}
		break;
		case ELEMENT_CIRCLE:
		{
			HTuple ht_Rows = (*o)[L"HTupleRows"];
			HTuple ht_Columns = (*o)[L"HTupleCols"];

			HTuple hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder;


			hv_Out.GenContourPolygonXld(ht_Rows, ht_Columns);
			//GenContourPolygonXld(&ho_Contour, (*hv_ROIRows), (*hv_ROICols));
			hv_Out.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
				&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);

			hv_Out.GenCircleContourXld(hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi,
				hv_EndPhi, hv_PointOrder, 3);
			//HTuple hv_Length = hv_Out.LengthXld();
			//this->write_message(300, 30, "hv_Length" + hv_Length);

			hv_Cont = hv_Cont.ConcatObj(hv_Out);

		}
		break;
		// ��ʾһ������Ԫ��
		case ELEMENT_DISTANCE:
		{
			// �õ�������������
			CString idDis1 = (*o)[L"����Ԫ������1"];
			CString idDis2 = (*o)[L"����Ԫ������1"];

			if (idDis1 == L"" || idDis2 == L"") return;

			if (idDis1 == idDis2) return;			

			UCI::OptionsMap * oDIS1 = UCI::ELE.GetOptionsMapByEleName(idDis1);
			UCI::OptionsMap * oDIS2 = UCI::ELE.GetOptionsMapByEleName(idDis2);

			CString eleParameter1 = (*o)[L"����Ԫ��1������"];
			CString eleParameter2 = (*o)[L"����Ԫ��2������"];

			int etype1 = (*oDIS1)[L"Ԫ������"];
			int etype2 = (*oDIS2)[L"Ԫ������"];
	

			double scalemm = UCI::OPTIONS[L"�������س���mm"];    // �������ص�mm�ı���

			HalconCpp::HXLDCont hx_tmp;

			// 1. �������ζ�û��ѡ��������
			if (eleParameter1 == "" && eleParameter2 == "")
			{
				if (etype1 == ELEMENT_CIRCLE && etype2 == ELEMENT_CIRCLE)
				{
					//// ����Բ, �������Բ��Բ�ľ�
					//double e1row = (*oDIS1)["RES_Circle_Center_Row"];
					//double e1col = (*oDIS1)["RES_Circle_Center_Column"];

					//double e2row = (*oDIS2)["RES_Circle_Center_Row"];
					//double e2col = (*oDIS2)["RES_Circle_Center_Column"];

					//this->gen_double_arrow_contour_xld(     // ����˫ͷ��ͷ
					//	&hx_tmp,
					//	e1row/scalemm,
					//	e1col / scalemm,
					//	e2row / scalemm,
					//	e2col / scalemm,
					//	alength,
					//	awidth);

					//hv_Cont = hv_Cont.ConcatObj(hx_tmp);
				}
				//else if ((etype1 == ELEMENT_LINE && etype2 == ELEMENT_CIRCLE)
				//	|| (etype2 == ELEMENT_LINE && etype1 == ELEMENT_CIRCLE))
				//{
				//	// Բ�ĵ�ֱ�ߵĴ�ֱ����
				//	double Rrow, Rcol;                     // Բ��
				//	double Lrow1, Lcol1, Lrow2, Lcol2;     // ֱ�߶����˵�

				//	UCI::OptionsMap * oCir = oDIS1;
				//	UCI::OptionsMap * oLine = oDIS2;

				//	if (etype1 == ELEMENT_LINE) {
				//		oCir = oDIS2;
				//		oLine = oDIS1;
				//	}


				//}
			}
		}
			break;
		case ELEMENT_ANGLE:
			break;
		default:
			break;
		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisDrawOneRoi �� ROI ����");
	}
}


// ɾ�����е�ƥ�����
void ST_MYDISP::DisOnMatchDelAllRoi()
{

	if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;


	this->m_sIPResult.result_cont.GenEmptyObj();
	this->ho_ModelRegion.GenEmptyRegion();
	this->hv_Model.Clear();

	// ��Ҫɾ������������ļ���ROI�ļ�
	int prID = UCI::OPTIONS[L"��Ŀ���"];

	CString stRegion = this->GetShapeModelRegionPathFile(prID);
	CString stModel = this->GetShapeModelPathFile(prID);

	CFile TempFile;
	if (GetFileAttributes(stRegion) != -1) { // �ļ�������
		TempFile.Remove(stRegion);
	}
	if (GetFileAttributes(stModel) != -1) { // �ļ�������
		TempFile.Remove(stModel);
	}

	this->OnMainDisp(NULL, NULL);
}
