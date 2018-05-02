#include "stdafx.h"
#include "MainFrm.h"

#include "MyData.h"
#include "MyDispLay.h"

using namespace HalconCpp;

using namespace std;



//  ֱ�� Ԫ��
void ST_MYDISP::DisOnEleLineAdd()
{
	if (!this->TestCurrentAppState(APP_LogingOK)) return;
	this->AppStateChangeTo(App_ElementAdding);

	try {

		int curProject = UCI::OPTIONS["CurProjectID"];

		HalconCpp::HXLDCont RoiLineHXLD;
		double Row1, Column1, Row2, Column2;

		int elements = UCI::OPTIONS["Rake_Line_Elements"];
		double detect_height = UCI::OPTIONS["Rake_Line_Detect_Height"];
		double detect_width = UCI::OPTIONS["Rake_Line_Detect_Width"];

		HTuple hv_Length;
		draw_rake(&RoiLineHXLD,
			elements,
			detect_height,
			detect_width,
			&Row1,
			&Column1,
			&Row2,
			&Column2,
			&hv_Length);

		HTuple Line1ROIRow, Line1ROICol;

		Line1ROIRow.Clear();
		Line1ROIRow.Append(Row1);
		Line1ROIRow.Append(Row2);

		Line1ROICol.Clear();
		Line1ROICol.Append(Column1);
		Line1ROICol.Append(Column2);

		// �����������ӵ���ʾ��

		EnterCriticalSection(&m_csIPDisp);      // CriticalSect
		m_sIPResult.result_cont = m_sIPResult.result_cont.Union2ClosedContoursXld(RoiLineHXLD);
		LeaveCriticalSection(&m_csIPDisp);      // CriticalSect

		this->m_pMHWinMain->DispObj(RoiLineHXLD);

		// ������ROIֱ�ߵĳ���
		//HTuple hv_Length = RoiLineHXLD.LengthXld();

		// OptionsMap
		UCI::ELE.add_LINE(Line1ROIRow, Line1ROICol, hv_Length, curProject);
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnEleLineAdd()��");

	}

	this->AppStateChangeTo(APP_READY);
}

//  ��� Ԫ��
void ST_MYDISP::DisOnEleCameraAdd()
{
	if (!this->TestCurrentAppState(APP_LogingOK)) return;
	int curProject = UCI::OPTIONS["CurProjectID"];
	UCI::ELE.add_Camera_Element(curProject);
}

// ��ֱ�߽ǶȲ��� 
void ST_MYDISP::DisOnEleAngle()
{
	if (!this->TestCurrentAppState(APP_LogingOK)) return;
	int curProject = UCI::OPTIONS["CurProjectID"];
	UCI::ELE.add_2LineAngle_Element(curProject);
}

// ���� Ԫ��
void ST_MYDISP::OnEleDistance()
{
	if (!this->TestCurrentAppState(APP_LogingOK)) return;
	int curProject = UCI::OPTIONS["CurProjectID"];
	UCI::ELE.add_2Element_Distance(curProject);
}


// ���� Ԫ��
void ST_MYDISP::DisOnEleOutReport()
{
	if (!this->TestCurrentAppState(APP_LogingOK)) return;
	int curProject = UCI::OPTIONS["CurProjectID"];
	UCI::ELE.add_Report_Element(curProject);
}

void ST_MYDISP::DisOnMeaEleMillimeter()
{
	if (!this->TestCurrentAppState(APP_LogingOK)) return;
	int curProject = UCI::OPTIONS["CurProjectID"];
	UCI::ELE.add_mm_Element(curProject);
}

// Բ Ԫ��
void ST_MYDISP::DisOnEleCircleAdd()
{
	if (!this->TestCurrentAppState(APP_LogingOK)) return;
	this->AppStateChangeTo(App_ElementAdding);

	try {
		HalconCpp::HXLDCont RoiCircleHXLD;
		HTuple ROIRows, ROICols, Direct;


		int curProject = UCI::OPTIONS["CurProjectID"];
		//OnDisButMatchLoadPic();   // �����µ�ǰ�Ĳ�������

		int elements = UCI::OPTIONS["Spoke_Circle_Elements"];
		double detect_height = UCI::OPTIONS["Spoke_Circle_Detect_Height"];
		double detect_width = UCI::OPTIONS["Spoke_Circle_Detect_Width"];

		//HTuple ROIRadius, ROIStartPhi, ROIEndPhi, ROIPointOrder;


		HTuple hv_Length;

		draw_spoke(
			&RoiCircleHXLD,
			elements,
			detect_height,
			detect_width,
			&ROIRows,
			&ROICols,
			&Direct,
			&hv_Length
		);

		// �����������ӵ���ʾ��

		EnterCriticalSection(&m_csIPDisp);      // CriticalSect
		m_sIPResult.result_cont = m_sIPResult.result_cont.Union2ClosedContoursXld(RoiCircleHXLD);
		LeaveCriticalSection(&m_csIPDisp);      // CriticalSect

		this->m_pMHWinMain->DispObj(RoiCircleHXLD);

		// ������ROIԲ���ĳ���
		//HTuple hv_Length = RoiCircleHXLD.LengthXld();

		// OptionsMap
		UCI::ELE.add_CIRCLE(
			ROIRows,
			ROICols,
			Direct,
			hv_Length, curProject);
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"DisOnEleLineAdd()��");

	}

	this->AppStateChangeTo(APP_READY);
}






