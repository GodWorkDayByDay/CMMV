#include "stdafx.h"
#include "MainFrm.h"

#include "MyData.h"
#include "MyDispLay.h"

using namespace HalconCpp;
using namespace std;


void ST_MYDISP::DisProcessROIRegion(HalconCpp::HRegion &HXdis, HalconCpp::HRegion &HXsrc)
{
	
		switch (UCI::RIBmenu.iCOMBO_MATCH_ROI_TYPE)
		{
		case ROI_UNITE:
			HXdis = HXdis.Union2(HXsrc);
			break;
		case ROI_AND:
			HXdis = HXdis.Intersection(HXsrc);
			break;
		case ROI_SUB:
			HXdis = HXdis.Difference(HXsrc);
			break;
		case ROI_XOR:
			HXdis = HXdis.SymmDifference(HXsrc);
			break;
		default:
			break;
		}

}

void ST_MYDISP::DisProcessROImodel(HalconCpp::HImage &Img, HRegion &HXdis)
{
	
	try {
		HalconCpp::HImage ImageReduced;
		ImageReduced = Img.ReduceDomain(HXdis);

		UCI::OptionsMap* o = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);

		CString CreatNumLevels = (*o)[L"CreateShapeModleNumLevels"];
		double CreatAngleStart = (*o)[L"CreateShapeModleAngleStart"];
		double CreatAngleExtend = (*o)[L"CreateShapeModleAngleExtend"];
		CString CreateAngleStep = (*o)[L"CreateShapeModleAngleStep"];
		CString CreateOptimization = (*o)[L"CreateShapeModleOptimization"];
		CString CreateMetric = (*o)[L"CreateShapeModleMetric"];
		CString CreateContrast = (*o)[L"CreateShapeModleContrast"];
		CString CreateMinContrast = (*o)[L"CreateShapeModleMinContrast"];

		this->hv_Model.CreateShapeModel(
			ImageReduced,
			CT2A(CreatNumLevels).m_psz,
			CreatAngleStart,
			CreatAngleExtend,
			CT2A(CreateAngleStep).m_psz,
			CT2A(CreateOptimization).m_psz,
			CT2A(CreateMetric).m_psz,
			CT2A(CreateContrast).m_psz,
			CT2A(CreateMinContrast).m_psz);

		double FindAngleStart = (*o)[L"FindShapeModleAngleStart"];
		double FindAngleExtend = (*o)[L"FindShapeModleAngleExtend"];
		double FindMinScore = (*o)[L"FindShapeModleMinScore"];
		int FindNumMatches = (*o)[L"FindShapeModleNumMatches"];
		double FindOverlap = (*o)[L"FindShapeModleMaxOverlap"];
		CString FindSubPixel = (*o)[L"FindShapeModleSubPixel"];
		int FindNumLevels = (*o)[L"FindShapeModleNumLevels"];
		double FindGreediness = (*o)[L"FindShapeModleGreediness"];

		HTuple Row3, Column3, Angle1, Score1;
		this->hv_Model.FindShapeModel(
			Img,
			FindAngleStart,
			FindAngleExtend,
			FindMinScore,
			FindNumMatches,
			FindOverlap,
			CT2A(FindSubPixel).m_psz,
			FindNumLevels,
			FindGreediness,
			&Row3,
			&Column3,
			&Angle1,
			&Score1
		);


		EnterCriticalSection(&m_csIPDisp);
		m_sIPResult.result_cont = this->ho_ModelRegion;   // region ��ת���� xld
		LeaveCriticalSection(&m_csIPDisp);

		this->OnMainDisp(NULL, NULL);              // ��������ʾһ��

		CString DispShapeC = (*o)[L"DispShapeColor"];

		dev_display_shape_matching_results(
			this->hv_Model,
			CT2A(DispShapeC).m_psz,
			Row3,
			Column3,
			Angle1,
			1,
			1,
			0);


	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisProcessROImodel��");

	}
}

// ROI Rectangle2
void ST_MYDISP::DisOnRoiDrawRectangle2()
{
	
	if (UCI::RIBmenu.bIsInROIdrawing) {
		MyLog(L"��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row, Column, Phi, Length1, Length2;
		
		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

		EnterCriticalSection(&m_csIPDisp);  
		hImg = m_sIPResult.result_img;
		LeaveCriticalSection(&m_csIPDisp);    

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawRectangle2(&Row, &Column, &Phi, &Length1, &Length2);

		UCI::RIBmenu.bIsInROIdrawing = false;

		Hreg.GenRectangle2(Row, Column, Phi, Length1, Length2);

		// ��������
		DisProcessROIRegion(this->ho_ModelRegion, Hreg);	


		DisProcessROImodel(hImg, this->ho_ModelRegion);   // ����ģ��




	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnRoiDrawRectangle2()��");

	}
}

void ST_MYDISP::DisOnRoiDrawCircle()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		MyLog(L"��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row, Column, Radius;

		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

		EnterCriticalSection(&m_csIPDisp);	
		hImg = m_sIPResult.result_img;
		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawCircle(&Row, &Column, &Radius);	

		UCI::RIBmenu.bIsInROIdrawing = false;


		// ��������
		Hreg.GenCircle(Row, Column, Radius);

		DisProcessROIRegion(this->ho_ModelRegion, Hreg);

		DisProcessROImodel(hImg, this->ho_ModelRegion);   // ����ģ��
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnRoiDrawCircle()��");

	}
}

void ST_MYDISP::DisOnRoiDrawEllipse()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		MyLog(L"��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row, Column, Phi,Radius1, Radius2;

		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawEllipse(&Row, &Column, &Phi, &Radius1, &Radius2);

		UCI::RIBmenu.bIsInROIdrawing = false;

		// ��������
		Hreg.GenEllipse(Row, Column, Phi, Radius1, Radius2);

		// ��������
		DisProcessROIRegion(this->ho_ModelRegion, Hreg);


		DisProcessROImodel(hImg, this->ho_ModelRegion);   // ����ģ��
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnRoiDrawEllipse()");

	}
}

void ST_MYDISP::DisOnRoiDrawRectangle()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		MyLog(L"��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row1, Column1, Row2, Column2;

		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawRectangle1(&Row1, &Column1, &Row2, &Column2);
		UCI::RIBmenu.bIsInROIdrawing = false;

		// ��������
		Hreg.GenRectangle1(Row1, Column1, Row2, Column2);

		// ��ԭ���������������
		DisProcessROIRegion(this->ho_ModelRegion, Hreg);

		//this->m_pMHWinMain->ClearWindow();	   
		//this->m_pMHWinMain->DispObj(hImg);
		//this->m_pMHWinMain->DispObj(this->ho_ModelRegion);

		DisProcessROImodel(hImg, this->ho_ModelRegion);   // ����ģ��

	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnRoiDrawRectangle()");

	}
}

void ST_MYDISP::DisOnRoiDrawAnyLine()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		MyLog(L"��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		
		HalconCpp::HImage hImg;                      // ��ʾ��ͼ��
		HalconCpp::HRegion Hreg;                     // ����

		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		Hreg = this->m_pMHWinMain->DrawRegion();
		UCI::RIBmenu.bIsInROIdrawing = false;

		DisProcessROIRegion(this->ho_ModelRegion,Hreg);	

		DisProcessROImodel(hImg, this->ho_ModelRegion);   // ����ģ��

	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnRoiDrawAnyLine��");

	}
}


// ��ʾ��ǰ��ģ����Ϣ\nģ����Ϣ
void ST_MYDISP::DisOnRoiInfo()
{
	//try {

	//	// �򿪵�ǰ�����ͼƬ

	//	int prID = UCI::OPTIONS[L"��Ŀ���"];

	//	this->OnDisButMatchLoadPic();

	//	CString stRegion = this->GetShapeModelRegionPathFile(prID);
	//	CString stModel = this->GetShapeModelPathFile(prID);

	//	if (GetFileAttributes(stRegion) == -1) { // �ļ�������
	//		MyLog(stRegion);
	//		MyLog("DisOnRoiInfo() ROI�������ļ������ڣ� ");
	//		return;
	//	}

	//	//
	//	HXLDCont HXdis;
	//	HalconCpp::HImage hImg;
	//	HXdis.ReadObject(CT2A(stRegion).m_psz);

	//	if (GetFileAttributes(stModel) == -1) { // �ļ�������
	//		MyLog(stModel);
	//		MyLog("DisOnRoiInfo()ROI��ģ���ļ������ڣ� ");
	//		return;
	//	}

	//	this->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);

	//	EnterCriticalSection(&m_csIPDisp);
	//	m_sIPResult.result_cont = HXdis;
	//	hImg = m_sIPResult.result_img;
	//	LeaveCriticalSection(&m_csIPDisp);

	//	this->OnMainDisp(NULL, NULL);

	//	double AngleStart = UCI::OPTIONS[L"FindShapeModleAngleStart"];
	//	double AngleExtend = UCI::OPTIONS[L"FindShapeModleAngleExtend"];

	//	HTuple Row3, Column3, Angle1, Score1;
	//	this->hv_Model.FindShapeModel(
	//		hImg,
	//		AngleStart,
	//		AngleExtend,
	//		0.5,
	//		1,
	//		0.5,
	//		"least_squares",
	//		0,
	//		0.9,
	//		&Row3,
	//		&Column3,
	//		&Angle1,
	//		&Score1
	//	);


	//	dev_display_shape_matching_results(
	//		this->hv_Model,
	//		"red",
	//		Row3,
	//		Column3,
	//		Angle1,
	//		1,
	//		1,
	//		0);

	//}
	//catch (HalconCpp::HException& except) {
	//	UCI::sDB.Log(except, L"DisOnRoiInfo()��");

	//}
}

void ST_MYDISP::DisOnRoiFindModel()
{
	try {		

		this->m_pMHWinMain->ClearWindow();

		HalconCpp::HImage hImg;
		HalconCpp::HXLDCont Cont;


		UCI::OPTIONS[L"ƥ��ģ�� OK"] = FALSE;

		UCI::OptionsMap *om;
		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_CAMERA);
		this->MeasureOneCamera(om, hImg, Cont);

		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_ROI);
		this->MeasureOneROI(om, hImg, Cont);

		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_FUNCTION);
		this->MeasureOneFUNCTION(om, hImg, Cont);
		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
		if (this->MeasureOneMATCH(om, hImg, Cont) == FALSE) {
			write_message(10, 10, "û���ҵ���Ӧ��ģ��");
			return;
		}

		//Cont = Cont.ConcatObj(this->ho_ModelRegion);

		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Cont;
		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp(NULL, NULL);              // ��������ʾһ��


												   // ��ʾһ��ģ����
		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
		HTuple Row = (*om)[L"RES_FindShapeRow"];
		HTuple Column = (*om)[L"RES_FindShapeColumn"];
		HTuple Angle = (*om)[L"RES_FindShapeAngle"];
		HTuple Score = (*om)[L"RES_FindShapeScore"];

		if (Row.Length() > 0) {

			write_message(100, 20, "�ҵ��˶��󣬵÷֣�" + Score, true);

			CString mcolor = (*om)[L"DispShapeColor"];

			dev_display_shape_matching_results(
				this->hv_Model,
				CT2A(mcolor).m_psz,
				Row,
				Column,
				Angle,
				1,
				1,
				0);
		}




		//int prID = UCI::OPTIONS[L"��Ŀ���"];
		//// �ɼ�һ��ͼƬ
		//CString stModel = this->GetShapeModelPathFile(prID);		//

		//HalconCpp::HImage hImg;
		//if (GetFileAttributes(stModel) == -1) { // �ļ�������
		//	MyLog(stModel);
		//	MyLog("DisOnRoiInfo() �ļ������ڣ� ");
		//	return;
		//}
		//this->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);

		//EnterCriticalSection(&m_csIPDisp);
		////m_sIPResult.result_cont = HXdis;
		//hImg = m_sIPResult.result_img;
		//LeaveCriticalSection(&m_csIPDisp);

		//this->OnMainDisp(NULL, NULL);

		//double AngleStart = UCI::OPTIONS[L"FindShapeModleAngleStart"];
		//double AngleExtend = UCI::OPTIONS[L"FindShapeModleAngleExtend"];

		//HTuple Row3, Column3, Angle1, Score1;
		//this->hv_Model.FindShapeModel(
		//	hImg,
		//	AngleStart,
		//	AngleExtend,
		//	0.5,
		//	1,
		//	0.5,
		//	"least_squares",
		//	0,
		//	0.9,
		//	&Row3,
		//	&Column3,
		//	&Angle1,
		//	&Score1
		//);

		//if (Row3.Length() < 1) {
		//	MyLog("DisOnRoiFindModel() û���ҵ���Ӧ�Ĳ�Ʒ��״");
		//	return;
		//}
		//

		//double dMin = UCI::OPTIONS[L"FindShapeModleMinScore"];

		//this->write_message(20, 20, "��ǰ���ҵ÷֣�" + Score1 + " ����ֵ��" + dMin);
		//if (Score1 >= dMin) {
		//	dev_display_shape_matching_results(
		//		this->hv_Model,
		//		"red",
		//		Row3,
		//		Column3,
		//		Angle1,
		//		1,
		//		1,
		//		0);
		//}
		//else {
		//	MyLog("DisOnRoiFindModel() û���ҵ���Ӧ�Ĳ�Ʒ��״");
		//}

	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnRoiFindModel()��");

	}
}

// ����ROI 
void ST_MYDISP::DisOnButMatchSaveModel()
{
	if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;

	try {
		int prID = UCI::OPTIONS[L"��Ŀ���"];
		// ���ж�һ���ļ��Ƿ����
		CString str = this->GetMatchPathFile(prID);
		if (GetFileAttributes(str) != -1) { // �ļ�����
			int res = ::MessageBox(NULL,
				L"��� ȷ��  ����ʵ�ֲ���һ����\n"
				"��� ȡ��  ���ء�",
				L"ƥ���ļ��Ѵ��ڣ�",
				MB_OKCANCEL | MB_ICONSTOP);
			if (res == IDCANCEL) return;
		}

		// 1. ���浱ǰ��ͼ��
		this->SaveImgToROI();

		// ����һ��������ҵ����� ROI
		this->ho_ModelRegion.WriteRegion(CT2A(this->GetShapeModelRegionPathFile(prID)).m_psz);

		// ����һ��������ҵ�ģ�� 
		this->hv_Model.WriteShapeModel(CT2A(this->GetShapeModelPathFile(prID)).m_psz);

		this->write_message(12, 12, "����ƥ����Ϣ�ɹ��ˣ�");
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnButMatchSaveModel������ƥ��");

		this->write_message(12, 12, "����ƥ����Ϣʧ�ܣ� ������鿴��־");
	}
}

// �����ϴα����ƥ���ͼ
void ST_MYDISP::OnDisButMatchLoadPic()
{	
	if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;

	try {

		this->m_pMHWinMain->ClearWindow();

		int prID = UCI::OPTIONS[L"��Ŀ���"];

		// �ȿ���û�м��ر궨����
		BOOL isLoad = UCI::OPTIONS[L"�궨��Ϣ OK"];
		if (isLoad == FALSE) {
			if (this->LoadCaliPoseAndParamter() == FALSE) {
				write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");
				return;
			}
		}

		HalconCpp::HImage hImg;
		HalconCpp::HXLDCont Contours;

		this->ho_ModelRegion.GenEmptyObj();            // ���һ��ģ������
		UCI::OPTIONS[L"ƥ��ģ�� OK"] = FALSE;
		Contours.GenEmptyObj();

		HTuple hv_ImageWidth, hv_ImageHeight;

		CString str = this->GetMatchPathFile(prID);    // �õ�ƥ���ļ���		
		hImg.ReadImage(CT2A(str).m_psz);
		hImg.GetImageSize(&hv_ImageWidth, &hv_ImageHeight);		

		int iHeight = hv_ImageHeight.I();
		int iWidth = hv_ImageWidth.I();

		int oldH = UCI::OPTIONS[L"ͼ��߶�"];
		int oldW = UCI::OPTIONS[L"ͼ����"];
		if (oldH != iHeight || oldW != iWidth) {

			UCI::OPTIONS[L"ͼ����"] = iWidth;
			UCI::OPTIONS[L"ͼ��߶�"] = iHeight;
			UCI::OPTIONS[L"ORGwhByCamera"] = 0;
		}

		this->m_pMHWinMain->SetPart(0, 0, iHeight - 1, iWidth - 1);


		// ͬʱҲ���浽ץȡͼ�������ȥ
		this->GrabImageDisp = hImg;
		this->m_pHWinFG->SetPart(0, 0, iHeight - 1, iWidth - 1);
		this->m_pHWinFG->DispObj(hImg);     // ���Դ�����ʾ


		// �����ߺ������
		UCI::OptionsMap * om;
		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_ROI);
		this->MeasureOneROI(om, hImg, Contours);

		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_FUNCTION);
		this->MeasureOneFUNCTION(om, hImg, Contours);

		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
		this->MeasureOneMATCH(om, hImg, Contours);


		Contours = Contours.ConcatObj(this->ho_ModelRegion);

		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Contours;
		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp(NULL, NULL);              // ��������ʾһ��


		// ��ʾһ��ģ����
		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
		HTuple Row = (*om)[L"RES_FindShapeRow"];
		HTuple Column = (*om)[L"RES_FindShapeColumn"];
		HTuple Angle = (*om)[L"RES_FindShapeAngle"];
		HTuple Score = (*om)[L"RES_FindShapeScore"];

		if (Row.Length() > 0) {

			write_message(100, 20, "�ҵ��˶��󣬵÷֣�" + Score, true);

			CString mcolor = (*om)[L"DispShapeColor"];

			dev_display_shape_matching_results(
				this->hv_Model,
				CT2A(mcolor).m_psz,
				Row,
				Column,
				Angle,
				1,
				1,
				0);
		}

		



		//HXLDCont ho_RoiHxld;
		//ho_RoiHxld.GenEmptyObj();
		//this->DisDrawAllRoi(ho_RoiHxld);            // ��ʾ���е�Ԫ�ص� ROI
		//this->m_pMHWinMain->DispObj(ho_RoiHxld);		

		return;
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"OnDisButMatchLoadPic��");

		return;
	}
}

// ֻ�Ǽ���ԭͼ��
void ST_MYDISP::OnDisButLoadPic()
{
	if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;

	try {

		this->m_pMHWinMain->ClearWindow();

		// ���ֶ����������ǰ����
		HalconCpp::HXLDCont Contours;
		HalconCpp::HImage hImg;

		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_CAMERA);
		this->MeasureOneCamera(om, hImg, Contours);

		this->m_pMHWinMain->DispObj(hImg);

		om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_ROI);
		this->MeasureOneROI(om, hImg, Contours);
		
		return;
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"OnDisButLoadPic()");

		return;
	}
}

void ST_MYDISP::DisOnElementDeleteAll()
{
	if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;
	
	int res = ::MessageBox(NULL,
		L"��� ȷ��  ɾ�����в���Ԫ�أ�\n"
		"��� ȡ��  ���ء�",
		L"���棡�ף������Ҫɾ�����в���Ԫ����",
		MB_OKCANCEL | MB_ICONSTOP);
	if (res == IDCANCEL) return;

	int prID = UCI::OPTIONS[L"��Ŀ���"];

	// �ӵ�������ʼһ��һ��ɾ������
	int size = (int)UCI::ELE.EleList.GetCount();

	int i = size;
	POSITION pos = UCI::ELE.EleList.GetTailPosition();
	while (pos != NULL) {
		UCI::OptionsMap *ot = &UCI::ELE.EleList.GetPrev(pos);
		if (i > 4) {
			UCI::ELE.EleList.RemoveTail();
		}
		i--;
	}

	UCI::pMain->MProjectTreeFillByPrID();

}






