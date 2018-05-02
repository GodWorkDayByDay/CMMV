#include "stdafx.h"
#include "MainFrm.h"

#include "MyData.h"
#include "MyDispLay.h"

using namespace HalconCpp;
using namespace std;
using namespace Gdiplus;

// ����Ԫ��
BOOL ST_MYDISP::MeasureOneElement(UCI::OptionsMap * o, HImage &himg, HXLDCont& Cont)
{
	int itype = (*o)[L"Ԫ������"];
	int etype = eELEMENT_TYPE(itype);

	switch (etype) {
	case ELEMENT_CAMERA:
		return this->MeasureOneCamera(o, himg, Cont);
		break;
	case ELEMENT_LINE:
		return this->MeasureOneLine(o, himg, Cont);
		break;
	case ELEMENT_CIRCLE:
		return this->MeasureOneCircle(o, himg, Cont);
		break;
	case ELEMENT_RECT:
		break;
	case ELEMENT_ELLIPSE:
		break;
	case ELEMENT_DISTANCE:
		return this->MeasureOneDistance(o, himg, Cont);
		break;
	case ELEMENT_ANGLE:
		return this->MeasureOneAngle(o, himg, Cont);
		break;
	case ELEMENT_RESULT:      // �ɼ�����������
		return MeasureOneResult(o, Cont);
		break;
	case ELEMENT_MM:          // 
		return MeasureOneMM(o, Cont);
		break;
	case ELEMENT_PEAK:
		return MeasureOnePeak(o, himg, Cont);
		break;
	case ELEMENT_BALLOONLENGTH:
		return MeasureOneBalloonLenght(o, Cont);
		break;
	case ELEMENT_FUNCTION:
		return MeasureOneFUNCTION(o, himg, Cont);
		break;
	case ELEMENT_MATCH:
		return MeasureOneMATCH(o, himg, Cont);
		break;
	case ELEMENT_MEASURE_RECTANGLE2:
		return MeasureOneELEMENT_MEASURE_RECTANGLE2(o, himg, Cont);
		break;
	case ELEMENT_BALLOON_FLAW:
		return MeasureOneELEMENT_MEASURE_BalloonFlaw(o, himg, Cont);
		break;
	case ELEMENT_ROI:
		return MeasureOneROI(o, himg, Cont);
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}

// ����һ��ֱ��
BOOL ST_MYDISP::MeasureOneLine(UCI::OptionsMap* o, HImage &himg, HXLDCont& Cont)
{
	
	try {
		BOOL isHomMat2DOK = UCI::OPTIONS[L"ת������ɹ�"];
		if (isHomMat2DOK == FALSE) return FALSE;

		// ȡ�� ROI
		double Row1 = (*o)[L"ROI_Row1"];
		double Column1 = (*o)[L"ROI_Column1"];
		double Row2 = (*o)[L"ROI_Row2"];
		double Column2 = (*o)[L"ROI_Column2"];

		HTuple Rows, Cols;
		Rows.Append(Row1);
		Rows.Append(Row2);
		Cols.Append(Column1);
		Cols.Append(Column2);

		// �Բ���ֱ��ROI���б任
		HTuple NewLine1Row, NewLine1Col;
		AffineTransPoint2d(this->HomMat2D,
			Rows, Cols,
			&NewLine1Row, &NewLine1Col);

		// ���ֱ�� 1
		// �ұ�Ե
		int hv_Elements = (*o)[L"Rakeֱ�߼�����"];
		double hv_DetectHeight = (*o)[L"Rakeֱ�߼��߶�"];
		double hv_DetectWidth = (*o)[L"Rakeֱ�߼����"];
		double hv_Sigma = (*o)[L"Rakeֱ�߼��Sigma"];
		double hv_Threshold = (*o)[L"Rakeֱ�߼����ֵ"];
		CString hv_Transition = (*o)[L"Rake_Line_Detect_Transition"];
		CString hv_Select = (*o)[L"Rake_Line_Detect_Select"];

		HXLDCont RegionsLine;
		HTuple ResultRow1, ResultColumn1;
		rake(
			himg,
			&RegionsLine,                                   // ��� rake ��������
			hv_Elements,                                    // elements
			hv_DetectHeight,
			hv_DetectWidth,
			hv_Sigma,
			hv_Threshold,
			CT2A(hv_Transition).m_psz,
			CT2A(hv_Select).m_psz,
			NewLine1Row[0],
			NewLine1Col[0],
			NewLine1Row[1],
			NewLine1Col[1],
			&ResultRow1,
			&ResultColumn1);

		//this->m_pMHWinMain->DispObj(RegionsLine);

		int hv_ActiveNum = (*o)[L"ֱ�������С����"];

		// ���
		HXLDCont Line1;
		HTuple 	Line1Row11, Line1Column11, Line1Row21, Line1Column21;
		pts_to_best_line(
			&Line1,
			ResultRow1,
			ResultColumn1,
			hv_ActiveNum,
			&Line1Row11,
			&Line1Column11,
			&Line1Row21,
			&Line1Column21);

		double scalemm = UCI::OPTIONS[L"�������س���mm"];    // �������ص�mm�ı���

		double L1row = Line1Row11[0].D() * scalemm;
		double L2row = Line1Row21[0].D() * scalemm;

		double L1col = Line1Column11[0].D() * scalemm;
		double L2col = Line1Column21[0].D() * scalemm;

															 // ����������������Ԫ����ȥ
		(*o)[L"_RES_Line_Row1"] = L1row;
		(*o)[L"_RES_Line_Row2"] = L2row;

		(*o)[L"_RES_Line_Col1"] = L1col;
		(*o)[L"_RES_Line_Col2"] = L2col;

		Cont = Cont.ConcatObj(Line1);
	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneLine(): ");
		return FALSE;
	}

	(*o)[L"Ԫ����ƥ��"] = 1;
	return TRUE;
}

// ����һ��Բ
BOOL ST_MYDISP::MeasureOneCircle(UCI::OptionsMap* o, HImage &himg, HXLDCont& Cont)
{
	try {

		BOOL isHomMat2DOK = UCI::OPTIONS[L"ת������ɹ�"];
		if (isHomMat2DOK == FALSE) return FALSE;

		// ȡ�������ROI
		double ROI_RowC = (*o)[L"ROI_RowC"];
		double ROI_ColumnC = (*o)[L"ROI_ColumnC"];
		double ROI_Radius = (*o)[L"ROI_Radius"];
		double ROI_StartPhi = (*o)[L"ROI_StartPhi"];
		double ROI_EndPhi = (*o)[L"ROI_EndPhi"];
		CString ROI_PointOrder = (*o)[L"ROI_PointOrder"];

		HXLDCont ho_ContCircle;
		ho_ContCircle.GenCircleContourXld(ROI_RowC, 
			ROI_ColumnC, 
			ROI_Radius, 
			ROI_StartPhi,
			ROI_EndPhi, CT2A(ROI_PointOrder).m_psz, 3.0f);

		HTuple Rows, Cols;
		ho_ContCircle.GetContourXld(&Rows, &Cols);

		if (Rows.Length() < 4) {
			MyLog("ԲROI����ȷ!, Rows �е�������");
			return FALSE;
		}

		// �Բ���ֱ��ROI���б任
		HTuple NewCircleRow, NewCircleCol;

		AffineTransPoint2d(this->HomMat2D,
			Rows, Cols,
			&NewCircleRow, &NewCircleCol);
		
		int hv_Elements = (*o)[L"Spoke_Circle_Elements"];
		double hv_DetectHeight = (*o)[L"Spoke_Circle_Detect_Height"];
		double hv_DetectWidth = (*o)[L"Spoke_Circle_Detect_Width"];
		double hv_Sigma = (*o)[L"Spoke_Circle_Detect_Sigma"];
		double hv_Threshold = (*o)[L"Spoke_Circle_Detect_Threshold"];
		CString hv_Transition = (*o)[L"Spoke_Circle_Detect_Transition"];
		CString hv_Select = (*o)[L"Spoke_Circle_Detect_Select"];
		CString hv_Direction = (*o)[L"Spoke_Circle_Direction"];

		HTuple hv_ResultRow, hv_ResultColumn, hv_ArcType;

		HXLDCont RegionsSpoke;
		spoke(himg,
			&RegionsSpoke,
			hv_Elements,
			hv_DetectHeight,
			hv_DetectWidth,
			hv_Sigma,
			hv_Threshold,
			CT2A(hv_Transition).m_psz,
			CT2A(hv_Select).m_psz,
			NewCircleRow,
			NewCircleCol,
			CT2A(hv_Direction).m_psz,
			&hv_ResultRow,
			&hv_ResultColumn,
			&hv_ArcType);

		// 
		if (hv_ResultRow.Length() == 0) {
			MyLog(L"û���ҵ�Բ spoke");
			return FALSE;
		}

		int CrossSize = (*o)[L"Spoke_ʮ�ֳߴ�"];

		HXLDCont Cross; // Բ����ɵĵ�
		Cross.GenCrossContourXld(hv_ResultRow, hv_ResultColumn, HTuple(CrossSize), 0.785398);
		Cont = Cont.ConcatObj(Cross);

		int Min_Points_Num = (*o)[L"Circle_Fit_Min_Points_Num"];
		// �жϱ�Ե�����Ƿ���ڵ�����С��Ч����
		if (hv_ResultRow.Length() < Min_Points_Num) {
			MyLog(L"û���ҵ�Բ spoke");
			return FALSE;
		}

		HTuple hv_Result_Center_Row, hv_Result_Center_Column, hv_Result_Radius;
		HTuple hv_Result_StartPhi, hv_Result_EndPhi, hv_PointOrders;

		HXLDCont Result_xld;

		// ���Բ 
		pts_to_best_circle(
			&Result_xld,
			hv_ResultRow,
			hv_ResultColumn,
			Min_Points_Num,
			hv_ArcType,
			&hv_Result_Center_Row,
			&hv_Result_Center_Column,
			&hv_Result_Radius,
			&hv_Result_StartPhi,
			&hv_Result_EndPhi,
			&hv_PointOrders);

		Cont = Cont.ConcatObj(Result_xld);

		double scalemm = UCI::OPTIONS[L"�������س���mm"];    // �������ص�mm�ı���

															 // �����������
		(*o)[L"RES_Circle_Center_Row"] = hv_Result_Center_Row * scalemm;
		(*o)[L"RES_Circle_Center_Column"] = hv_Result_Center_Column * scalemm;

		//double cRow = hv_Result_Center_Row * scalemm;
		//double cCol = hv_Result_Center_Column * scalemm;

		(*o)[L"RES_Բ�뾶"] = hv_Result_Radius* scalemm;
		(*o)[L"RES_Բֱ��"] = hv_Result_Radius* scalemm*2.0f;
		(*o)[L"RES_Circle_StartPhi"] = hv_Result_StartPhi;                  // ��ʼ��
		(*o)[L"RES_Circle_EndPhi"] = hv_Result_EndPhi;                      // ������

		(*o)[L"RES_Circle_PointOrders"] = (CString)hv_PointOrders[0].S();
		(*o)[L"RES_Circle_Type"] = (CString)hv_ArcType[0].S();
	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneCircle(): ");
		return FALSE;
	}

	(*o)[L"Ԫ����ƥ��"] = 1;
	return TRUE;
}

// ����һ������
BOOL ST_MYDISP::MeasureOnePeak(UCI::OptionsMap * o, HImage &himg, HXLDCont &Cont)
{
	
	try {
		BOOL isHomMat2DOK = UCI::OPTIONS[L"ת������ɹ�"];
		if (isHomMat2DOK == FALSE) return FALSE;

		double hv_Row = (*o)[L"Peak_Rectangle2_Row"];
		double hv_Column = (*o)[L"Peak_Rectangle2_Colum"];
		double hv_Phi = (*o)[L"Peak_Rectangle2_Phi"];

		double hv_Length1 = (*o)[L"Peak_Rectangle2_Length1"];
		double hv_Length2 = (*o)[L"Peak_Rectangle2_Length2"];

		double hv_DetectWidth = (*o)[L"Peak_DetectWidth"];
		double hv_Sigma = (*o)[L"Peak_Sigma"];
		double hv_Threshold = (*o)[L"Peak_Threshold"];

		CString hv_Transition = (*o)[L"Peak_Transition"];
		CString hv_Select = (*o)[L"Peak_Select"];

		HTuple new_hv_Row, new_hv_Column, new_hv_Phi;
		HTuple hv_EdgesY, hv_EdgesX, hv_ResultRow, hv_ResultColumn;

		// Ҫ�任һ�°�
		AffineTransPoint2d(this->HomMat2D,
			hv_Row, hv_Column,
			&new_hv_Row, &new_hv_Column);

		new_hv_Phi = hv_Phi + this->HomMatAngle;


		this->peak(himg, new_hv_Row, new_hv_Column, new_hv_Phi,
			hv_Length1, hv_Length2, hv_DetectWidth, hv_Sigma, hv_Threshold,
			CT2A(hv_Transition).m_psz, CT2A(hv_Select).m_psz, &hv_EdgesY, &hv_EdgesX, &hv_ResultRow, &hv_ResultColumn);


		double scalemm = UCI::OPTIONS[L"�������س���mm"];                // �������ص�mm�ı���
		 
		(*o)[L"RES_hv_ResultRow"] = hv_ResultRow * scalemm;              // ����
		(*o)[L"RES_hv_ResultColumn"] = hv_ResultColumn * scalemm;

		double cross_size = (*o)[L"Peak_cross_size"];
		double cross_angle = (*o)[L"Peak_cross_angle"];

		HObject  ho_Cross;
		GenCrossContourXld(&ho_Cross, hv_ResultRow, hv_ResultColumn, cross_size, cross_angle);

		Cont = Cont.ConcatObj(ho_Cross);

	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOnePeak(): ");
		return FALSE;
	}

	(*o)[L"Ԫ����ƥ��"] = 1;
	return TRUE;	

}

// ���ҳ���
BOOL ST_MYDISP::MeasureOneBalloonLenght(UCI::OptionsMap * o, HXLDCont &Cont)
{
	try {
		// ȡ���ĸ���������
		CString iPeek1 = (*o)[L"���ö���1"];
		CString iPeek2 = (*o)[L"���ö���2"];

		CString iPeek3 = (*o)[L"���ö���3"];
		CString iPeek4 = (*o)[L"���ö���4"];

		if (iPeek1 == iPeek2) {
			MyLog("MeasureOneBalloonLenght, 1��2 ����������ͬһ����");
			return FALSE;
		}
		if (iPeek3 == iPeek4) {
			MyLog("MeasureOneBalloonLenght, 3��4 ����������ͬһ����");
			return FALSE;
		}

		UCI::OptionsMap * oPeak1 = UCI::ELE.GetOptionsMapByEleName(iPeek1);
		UCI::OptionsMap * oPeak2 = UCI::ELE.GetOptionsMapByEleName(iPeek2);

		UCI::OptionsMap * oPeak3 = UCI::ELE.GetOptionsMapByEleName(iPeek3);
		UCI::OptionsMap * oPeak4 = UCI::ELE.GetOptionsMapByEleName(iPeek4);

		double scalemm = UCI::OPTIONS[L"�������س���mm"];      // �������ص�mm�ı���

		double arrSize = UCI::OPTIONS[L"ElementArrowSize"];   // ��ʾ��ͷ�Ĵ�С��

		CString stColor = UCI::OPTIONS[L"ElementDimColor"];
		int width = UCI::OPTIONS[L"ElementDimWidth"];

		
		HTuple hv_p1_row = (*oPeak1)[L"RES_hv_ResultRow"];
		HTuple hv_p1_col = (*oPeak1)[L"RES_hv_ResultColumn"];

		double p1row = hv_p1_row[0].D();
		double p1col = hv_p1_col[0].D();

		HTuple hv_p2_row = (*oPeak2)[L"RES_hv_ResultRow"];
		HTuple hv_p2_col = (*oPeak2)[L"RES_hv_ResultColumn"];

		double p2row = hv_p2_row[0].D();
		double p2col = hv_p2_col[0].D();

		HTuple hv_p3_row = (*oPeak3)[L"RES_hv_ResultRow"];
		HTuple hv_p3_col = (*oPeak3)[L"RES_hv_ResultColumn"];

		double p3row = hv_p3_row[0].D();
		double p3col = hv_p3_col[0].D();

		HTuple hv_p4_row = (*oPeak4)[L"RES_hv_ResultRow"];
		HTuple hv_p4_col = (*oPeak4)[L"RES_hv_ResultColumn"];

		double p4row = hv_p4_row[0].D();
		double p4col = hv_p4_col[0].D();


		// 1. �����߶���������е�

		double mid12row = (p1row + p2row) / 2;
		double mid12col = (p1col + p2col) / 2;

		double mid34row = (p3row + p4row) / 2;
		double mid34col = (p3col + p4col) / 2;

		HTuple Distance;

		// 2. ��������е�ľ���
		DistancePp(mid12row, mid12col, mid34row, mid34col, &Distance);

		double dDis = Distance[0].D();

		(*o)[L"RES_���ҳ���"] = dDis;

		(*o)[L"Balloon����Row1"] = mid12row / scalemm;
		(*o)[L"Balloon����Col1"] = mid12col / scalemm;
		(*o)[L"Balloon����Row2"] = mid34row / scalemm;
		(*o)[L"Balloon����Col2"] = mid34col / scalemm;

		// ����һ����ͷ����ʾһ�����ҵĳ���
		HXLDCont Arrow;
		double Arrow_HeadLength = (*o)[L"�ߴ��ͷ����"];
		double Arrow_HeadWidth = (*o)[L"�ߴ��ͷ���"];
		gen_double_arrow_contour_xld(
			&Arrow,
			mid12row / scalemm,
			mid12col / scalemm,
			mid34row / scalemm,
			mid34col / scalemm,
			Arrow_HeadLength,
			Arrow_HeadWidth);

		Cont = Cont.ConcatObj(Arrow);    // �����ͷ

	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneBalloonLenght��");
		return FALSE;
	}

	(*o)[L"Ԫ����ƥ��"] = 1;
	return TRUE;
}

// ����һ���Ƕ�
BOOL ST_MYDISP::MeasureOneAngle(UCI::OptionsMap *o, HImage & hImg, HXLDCont& Cont)
{
	try {
		// �õ�����ֱ��
		CString iLine1 = (*o)[L"USE_LINE1_Parament_Name"];
		CString iLine2 = (*o)[L"USE_LINE2_Parament_Name"];

		if (iLine1 == iLine2)
		{
			MyLog(L"MeasureOneAngle, �ǶȲ�������������ͬһ���ߣ�");
			return FALSE;
		}

		UCI::OptionsMap * oLine1 = UCI::ELE.GetOptionsMapByEleName(iLine1);
		UCI::OptionsMap * oLine2 = UCI::ELE.GetOptionsMapByEleName(iLine2);

		double L1Row1 = (*oLine1)[L"_RES_Line_Row1"];
		double L1Column1 = (*oLine1)[L"_RES_Line_Col1"];
		double L1Row2 = (*oLine1)[L"_RES_Line_Row2"];
		double L1Column2 = (*oLine1)[L"_RES_Line_Col2"];

		double L2Row1 = (*oLine2)[L"_RES_Line_Row1"];
		double L2Column1 = (*oLine2)[L"_RES_Line_Col1"];
		double L2Row2 = (*oLine2)[L"_RES_Line_Row2"];
		double L2Column2 = (*oLine2)[L"_RES_Line_Col2"];

		HTuple HV_Angle;

		// ������ߵļн�
		AngleLl(
			L1Row1,
			L1Column1,
			L1Row2,
			L1Column2,
			L2Row1,
			L2Column1,
			L2Row2,
			L2Column2,
			&HV_Angle);

		double angleRad = HV_Angle[0].D();
		(*o)[L"RES_2LINE_Angle_Rad"] = angleRad;

		HTuple angleDeg;
		TupleDeg(angleRad, &angleDeg);
		(*o)[L"RES_�н�"] = angleDeg;

	
		HTuple InterRow, InterCol, IsOver;

		// ������ߵĽ���
		IntersectionLines(
			L1Row1,
			L1Column1,
			L1Row2,
			L1Column2,
			L2Row1,
			L2Column1,
			L2Row2,
			L2Column2,
			&InterRow,
			&InterCol,
			&IsOver);  // �ǲ���ͬһ����

		double dInterRow = InterRow[0].D();
		double dInterCol = InterCol[0].D();

		HTuple hv_points;
		hv_points.Clear();
		hv_points[0] = dInterRow;
		hv_points[1] = dInterCol;
		
		(*o)[L"RES_Intersection_Row"] = dInterRow;
		(*o)[L"RES_Intersection_Column"] = dInterCol;
		(*o)[L"RES_Intersection_Points"] = hv_points;		
	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneAngle��");
		return FALSE;
	}
	(*o)[L"Ԫ����ƥ��"] = 1;
	return TRUE;
	
}

// ����һ������
BOOL ST_MYDISP::MeasureOneDistance(UCI::OptionsMap * o, HImage & hImg, HXLDCont& Cont)
{
	try
	{
		// �õ�������������
		CString idDis1 = (*o)[L"����Ԫ������1"];
		CString idDis2 = (*o)[L"����Ԫ������2"];

		if (idDis1 == idDis2)
		{
			MyLog("MeasureOneAngle, �������������������ͬһ����");
			return FALSE;
		}

		UCI::OptionsMap * oDIS1 = UCI::ELE.GetOptionsMapByEleName(idDis1);
		UCI::OptionsMap * oDIS2 = UCI::ELE.GetOptionsMapByEleName(idDis2);

		CString eleParameter1 = (*o)[L"����Ԫ��1������"];
		CString eleParameter2 = (*o)[L"����Ԫ��2������"];

		int etype1 = (*oDIS1)[L"Ԫ������"];
		int etype2 = (*oDIS2)[L"Ԫ������"];

		double scalemm = UCI::OPTIONS[L"�������س���mm"];      // �������ص�mm�ı���

		double arrSize = UCI::OPTIONS[L"ElementArrowSize"];   // ��ʾ��ͷ�Ĵ�С��

		CString stColor = UCI::OPTIONS[L"ElementDimColor"];
		int width = UCI::OPTIONS[L"ElementDimWidth"];

		//  �п����ǳ�ͻ
		//EnterCriticalSection(&m_csIPDisp);
		//UCI::pDis->m_pMHWinMain->SetColor(stColor.c_str());
		//UCI::pDis->m_pMHWinMain->SetLineWidth(width);
		//LeaveCriticalSection(&this->m_csFGDisp);

		//BOOL isDisDim = (*o)[L"IsDisplayDimension"];


		// 1. �������ζ�û��ѡ��������
		if (eleParameter1 == "" && eleParameter2 == "")
		{
			if (etype1 == ELEMENT_CIRCLE && etype2 == ELEMENT_CIRCLE)
			{
				// ����Բ, �������Բ��Բ�ľ�
				double e1row = (*oDIS1)[L"RES_Circle_Center_Row"];
				double e1col = (*oDIS1)[L"RES_Circle_Center_Column"];

				double e2row = (*oDIS2)[L"RES_Circle_Center_Row"];
				double e2col = (*oDIS2)[L"RES_Circle_Center_Column"];

				HTuple hv_Dis;
				DistancePp(e1row, e1col, e2row, e2col, &hv_Dis);

				(*o)[L"RES_����"] = hv_Dis;    // ����ѳ��˱�����	

				(*o)[L"����Row1"] = e1row / scalemm;
				(*o)[L"����Col1"] = e1col / scalemm;
				(*o)[L"����Row2"] = e2row / scalemm;
				(*o)[L"����Col2"] = e2col / scalemm;	

			}
			else if ((etype1 == ELEMENT_LINE && etype2 == ELEMENT_CIRCLE)
				|| (etype2 == ELEMENT_LINE && etype1 == ELEMENT_CIRCLE))
			{
				// Բ�ĵ�ֱ�ߵĴ�ֱ����
				double Rrow, Rcol;                     // Բ��
				double Lrow1, Lcol1, Lrow2, Lcol2;     // ֱ�߶����˵�

				UCI::OptionsMap * oCir = oDIS1;
				UCI::OptionsMap * oLine = oDIS2;

				if (etype1 == ELEMENT_LINE) {
					oCir = oDIS2;
					oLine = oDIS1;
				}

				Rrow = (*oCir)[L"RES_Circle_Center_Row"];
				Rcol = (*oCir)[L"RES_Circle_Center_Column"];

				Lrow1 = (*oLine)[L"_RES_Line_Row1"];
				Lcol1 = (*oLine)[L"_RES_Line_Col1"];

				Lrow2 = (*oLine)[L"_RES_Line_Row2"];
				Lcol2 = (*oLine)[L"_RES_Line_Col2"];
				HTuple hv_Dis;

				DistancePl(Rrow, Rcol, Lrow1, Lcol1, Lrow2, Lcol2, &hv_Dis);
				(*o)[L"RES_����"] = hv_Dis;

				// ���Բ�ĵ�ֱ�ߴ���
				HTuple hv_foot_row, hv_foot_col;
				ProjectionPl(
					Rrow, Rcol, Lrow1, Lcol1, Lrow2, Lcol2,
					&hv_foot_row, &hv_foot_col);


				(*o)[L"����Row1"] = Rrow / scalemm;
				(*o)[L"����Col1"] = Rcol / scalemm;
				(*o)[L"����Row2"] = hv_foot_row[0].D() / scalemm;
				(*o)[L"����Col2"] = hv_foot_col[0].D() / scalemm;				

			}
			else if (etype1 == ELEMENT_LINE && etype2 == ELEMENT_LINE) {
				// ֱ�ߵ�ֱ�ߵľ��룬 ����Ҫ��һ��ֱ�ߵ��е㣬����һ��ֱ�ߵľ���
				double L1row1, L1col1, L1row2, L1col2;     // ֱ��1 �����˵�
				double L2row1, L2col1, L2row2, L2col2;     // ֱ��2 �����˵�

				L1row1 = (*oDIS1)[L"_RES_Line_Row1"];
				L1col1 = (*oDIS1)[L"_RES_Line_Col1"];

				L1row2 = (*oDIS1)[L"_RES_Line_Row2"];
				L1col2 = (*oDIS1)[L"_RES_Line_Col2"];

				L2row1 = (*oDIS2)[L"_RES_Line_Row1"];
				L2col1 = (*oDIS2)[L"_RES_Line_Col1"];

				L2row2 = (*oDIS2)[L"_RES_Line_Row2"];
				L2col2 = (*oDIS2)[L"_RES_Line_Col2"];

				// 1. ���һֱ�ߵ��е�
				double L1MidRow = (L1row1 + L1row2) / 2;
				double L1MidCol = (L1col1 + L1col2) / 2;

				// 2. ���е㵽ֱ�ߵľ���
				HTuple hv_Dis;

				DistancePl(L1MidRow, L1MidCol, L2row1, L2col1, L2row2, L2col2, &hv_Dis);
				(*o)[L"RES_����"] = hv_Dis;

				// ���Բ�ĵ�ֱ�ߴ���
				HTuple hv_foot_row, hv_foot_col;
				ProjectionPl(
					L1MidRow, L1MidCol, L2row1, L2col1, L2row2, L2col2,
					&hv_foot_row, &hv_foot_col);

				(*o)[L"����Row1"] = L1MidRow / scalemm;
				(*o)[L"����Col1"] = L1MidCol / scalemm;
				(*o)[L"����Row2"] = hv_foot_row[0].D() / scalemm;
				(*o)[L"����Col2"] = hv_foot_col[0].D() / scalemm;		

			}
			else if (etype1 == ELEMENT_ANGLE && etype2 == ELEMENT_ANGLE) {
				// ���������ĳ���

				// ����Բ, �������Բ��Բ�ľ�
				double e1row = (*oDIS1)[L"RES_Intersection_Row"];
				double e1col = (*oDIS1)[L"RES_Intersection_Column"];

				double e2row = (*oDIS2)[L"RES_Intersection_Row"];
				double e2col = (*oDIS2)[L"RES_Intersection_Column"];

				HTuple hv_Dis;
				DistancePp(e1row, e1col, e2row, e2col, &hv_Dis);

				(*o)[L"RES_����"] = hv_Dis;    // ����ѳ��˱�����	

				(*o)[L"����Row1"] = e1row / scalemm;
				(*o)[L"����Col1"] = e1col / scalemm;
				(*o)[L"����Row2"] = e2row / scalemm;
				(*o)[L"����Col2"] = e2col / scalemm;

			}
		}
		

		// ����һ����ͷ����ʾһ�¾���

		double row1 = (*o)[L"����Row1"];
		double col1 = (*o)[L"����Col1"];
		double row2 = (*o)[L"����Row2"];
		double col2 = (*o)[L"����Col2"];

		HXLDCont Arrow;
		double Arrow_HeadLength = (*o)[L"�ߴ��ͷ����"];
		double Arrow_HeadWidth = (*o)[L"�ߴ��ͷ���"];
		gen_double_arrow_contour_xld(
			&Arrow,
			row1,
			col1,
			row2,
			col2,
			Arrow_HeadLength,
			Arrow_HeadWidth);
		Cont = Cont.ConcatObj(Arrow);    // �����ͷ

	}
	catch (HException& except){		
		UCI::sDB.Log(except, L"MeasureOneDistance��");
		return FALSE;
	}


	(*o)[L"Ԫ����ƥ��"] = 1;
	return TRUE;

}

// ����һ�����Ԫ��
BOOL ST_MYDISP::MeasureOneCamera(UCI::OptionsMap * o, HImage & hImg, HXLDCont& Cont)
{
	
	try {

		if (this->mAppState == APP_MANUAL_MEASURING) {   // ��ǰ���ֹ����� ������Ҫ�򿪲�������ģ��	
			UCI::OPTIONS[L"ת������ɹ�"] = FALSE;
			UCI::OPTIONS[L"ƥ��ģ�� OK"] = FALSE;
		}

		// 2. �ɼ�һ��ͼ��,
		this->OnDisMeasureOnePic(o, &hImg);                              // ʵʱ�ɼ�һ��ͼ������ͼ��У��	
		Cont.GenEmptyObj();
	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneCamera��");
		return FALSE;
	}

	return TRUE;
}

BOOL ST_MYDISP::MeasureOneROI(UCI::OptionsMap * o, HalconCpp::HImage & hImg, HalconCpp::HXLDCont &)
{
	try {
		//HImage ImageTmp;

		//EnterCriticalSection(&m_csIPDisp);
		//ImageTmp = this->m_sIPResult.result_img;    // ȡ��У�����ͼ��
		//LeaveCriticalSection(&m_csIPDisp);

		//this->m_pMHWinMain->ClearWindow();


		double Row = (*o)[L"ROI_Rectangle2_Row"];
		double Column = (*o)[L"ROI_Rectangle2_Column"];
		double Phi = (*o)[L"ROI_Rectangle2_Phi"];
		double Length1 = (*o)[L"ROI_Rectangle2_Length1"];
		double Length2 = (*o)[L"ROI_Rectangle2_Length2"];

		//=============================================================================
		// ��У��ͼ��
		BOOL bIsMap = (*o)[L"USE_map_image"];
		if (bIsMap) {
			BOOL bLoadCalibrateOK = UCI::OPTIONS[L"�궨��Ϣ OK"];
			if (bLoadCalibrateOK == FALSE) {

				this->LoadCaliPoseAndParamter();
				bLoadCalibrateOK = UCI::OPTIONS[L"�궨��Ϣ OK"];
				if (bLoadCalibrateOK == FALSE)
				{
					write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");
					UCI::MyLog(L"MeasureOneFUNCTION()�� ����ӳ��ͼ��ʧ�ܣ�");
					return FALSE;
				}
			}
			// У��ͼ��
			hImg = hImg.MapImage(this->ho_RectMap);				
		}

		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		LeaveCriticalSection(&m_csIPDisp);

		HalconCpp::HRegion Rectangle;
		Rectangle.GenRectangle2(Row, Column, Phi, Length1, Length2);

		hImg = hImg.ReduceDomain(Rectangle);

	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneROI()��");
		return FALSE;
	}
	return TRUE;
}

// ����
BOOL ST_MYDISP::MeasureOneFUNCTION(UCI::OptionsMap * o, HImage & hImg, HXLDCont &Cont)
{
	try {

		HImage ImageTmp = hImg;
		HRegion RegionTmp;

		//this->m_pMHWinMain->ClearWindow();
		//this->m_pMHWinMain->DispObj(ImageTmp);



		//this->m_pMHWinMain->ClearWindow();
		//this->m_pMHWinMain->DispObj(ImageTmp);

		//this->m_pMHWinMain->ClearWindow();
		//this->m_pMHWinMain->DispObj(hImg);

		//=============================================================================
		// ֱ��ͼ����
		BOOL bIsequ_histo_image = (*o)[L"USE_equ_histo_image"];
		if (bIsequ_histo_image) {
			ImageTmp = ImageTmp.EquHistoImage();
		}
		
		//=============================================================================
		// ǿ��ͼ��

		BOOL bIsUSE_emphasize = (*o)[L"USE_emphasize"];
		if (bIsUSE_emphasize) {

			int emphasize_MaskWidth = (*o)[L"emphasize_MaskWidth"];
			int emphasize_MaskHeight = (*o)[L"emphasize_MaskHeight"];
			double emphasize_Factor = (*o)[L"emphasize_Factor"];

			ImageTmp = ImageTmp.Emphasize(emphasize_MaskWidth, emphasize_MaskHeight, emphasize_Factor);
		}

		//=============================================================================
		// mean_image
		BOOL bIsUSE_mean_image = (*o)[L"USE_mean_image"];
		if (bIsUSE_mean_image) {

			int mean_image_MaskWidth = (*o)[L"mean_image_MaskWidth"];
			int mean_image_MaskHeight = (*o)[L"mean_image_MaskHeight"];			

			ImageTmp = ImageTmp.MeanImage(mean_image_MaskWidth, mean_image_MaskHeight);
		}

	//	this->m_pMHWinMain->ClearWindow();
	//	this->m_pMHWinMain->DispObj(ImageTmp);

		BOOL bIsUSE_threshold = (*o)[L"USE_threshold"];
		if (bIsUSE_threshold) {

			double threshold_MinGray = (*o)[L"threshold_MinGray"];
			double threshold_MaxGray = (*o)[L"threshold_MaxGray"];

			RegionTmp = ImageTmp.Threshold(threshold_MinGray, threshold_MaxGray);
		}

		//this->m_pMHWinMain->ClearWindow();
		//this->m_pMHWinMain->DispObj(RegionTmp);

		// ����
		BOOL isUSE_dilation_circle = (*o)[L"USE_dilation_circle"];
		if (isUSE_dilation_circle) {
			double dilation_circle_radius = (*o)[L"dilation_circle_radius"];

			RegionTmp = RegionTmp.DilationCircle(dilation_circle_radius);
		}

		//this->m_pMHWinMain->ClearWindow();
		//this->m_pMHWinMain->DispObj(RegionTmp);

		// fill_up
		BOOL USE_fill_up = (*o)[L"USE_fill_up"];
		if (USE_fill_up) {
			RegionTmp = RegionTmp.FillUp();
		}

		//this->m_pMHWinMain->ClearWindow();
		//this->m_pMHWinMain->DispObj(RegionTmp);
		
		// ����
		BOOL USE_erosion_circle = (*o)[L"USE_erosion_circle"];
		if (USE_erosion_circle) {
			double erosion_circle_radius = (*o)[L"erosion_circle_radius"];

			RegionTmp = RegionTmp.ErosionCircle(erosion_circle_radius);			
		}

		//this->m_pMHWinMain->ClearWindow();
		//this->m_pMHWinMain->DispObj(RegionTmp);

		// �Ѳ�����Ҫ�Ĳ��ֲ��ָ�ɰ�ɫ�ġ�	
		
		//= hImg.Difference(hImg);

	//	this->m_pMHWinMain->ClearWindow();
	//	this->m_pMHWinMain->DispObj(RegionBack);
	//	HImage reTmp;

		BOOL bIsUSE_reduce_domain = (*o)[L"USE_reduce_domain"];
		if (bIsUSE_reduce_domain) {

			HRegion RegionBack;
			Difference(hImg, RegionTmp, &RegionBack);
			RegionBack.OverpaintRegion(ImageTmp, 0, "fill");

			BOOL USE_reduce_domain_Gray = (*o)[L"USE_reduce_domain_Gray"];
			if (USE_reduce_domain_Gray) {
				RegionTmp.OverpaintRegion(ImageTmp, 255, "fill");
			}
		}

		hImg = ImageTmp;
	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneFUNCTION()��");
		return FALSE;
	}
	return TRUE;
}

// ƥ��
BOOL ST_MYDISP::MeasureOneMATCH(UCI::OptionsMap * o, HImage & hImg, HXLDCont &Cont)
{
	try {

		BOOL bLoadROI = UCI::OPTIONS[L"ƥ��ģ�� OK"];

		if (bLoadROI == FALSE) {

			int prID = UCI::OPTIONS[L"��Ŀ���"];
			
			CString stRegion = this->GetShapeModelRegionPathFile(prID); // 1. ��ԭ������Ĳ���ģ��
			CString stModel = this->GetShapeModelPathFile(prID);

			if (GetFileAttributes(stRegion) == -1) { // �ļ�������
				MyLog(stRegion);
				MyLog("MeasureOneMATCH() ROI�������ļ������ڣ� ");
				return FALSE;
			}

			if (GetFileAttributes(stModel) == -1) { // �ļ�������
				MyLog(stModel);
				MyLog("MeasureOneMATCH() ROI��ģ���ļ������ڣ� ");
				return FALSE;
			}

			this->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);       // ����ģ���ļ�
			this->ho_ModelRegion.ReadRegion(CT2A(stRegion));          // ����ƥ�������ļ�

			UCI::OPTIONS[L"ƥ��ģ�� OK"] = TRUE;
		}

		// 1. ����ͼ���еĲ�Ʒ
		double AngleStart = (*o)[L"FindShapeModleAngleStart"];
		double AngleExtend = (*o)[L"FindShapeModleAngleExtend"];
		double minScroe = (*o)[L"FindShapeModleMinScore"];
		int NumMatches = (*o)[L"FindShapeModleNumMatches"];
		double MaxOverlap = (*o)[L"FindShapeModleMaxOverlap"];
		CString SubPixel = (*o)[L"FindShapeModleSubPixel"];
		int NumLevels = (*o)[L"FindShapeModleNumLevels"];
		double Greediness = (*o)[L"FindShapeModleGreediness"];

		HTuple Row, Column, Angle, Score;
		this->hv_Model.FindShapeModel(
			hImg,
			AngleStart,
			AngleExtend,
			minScroe,
			NumMatches,
			MaxOverlap,
			CT2A(SubPixel).m_psz,
			NumLevels,
			Greediness,
			&Row,
			&Column,
			&Angle,
			&Score
		);

		(*o)[L"RES_FindShapeRow"] = Row;
		(*o)[L"RES_FindShapeColumn"] = Column;
		(*o)[L"RES_FindShapeAngle"] = Angle;
		(*o)[L"RES_FindShapeScore"] = Score;



		if (Row.Length() < 1 ) {

			//write_message(420, 20, "û�з��ֶ���, �����²ɼ�����", true);

			this->IsSpaceKeyDown = FALSE;   // ��ǰ�İ�ť��Ч
			return FALSE;
		}



		//write_message(100, 20, "�ҵ��˶��󣬵÷֣�" + Score, true);


		//dev_display_shape_matching_results(
		//	this->hv_Model,
		//	"red",
		//	Row,
		//	Column,
		//	Angle,
		//	1,
		//	1,
		//	0);

		//

		HXLDCont Contour, Cross;
		HTuple Row0, Col0;

		//HHomMat2D HomMat2D_T;
		// 4. ��λת�� �ҵ�����, ��������, �� ROI ��������з���任	
		// ����ģ����������任����
		//HomMat2D_T.VectorAngleToRigid(HTuple(0), HTuple(0), HTuple(0), Row, Column, Angle);
		// ��ȡģ������		
		//Contour = this->hv_Model.GetShapeModelContours(1);
		// ��ģ���������з���任
		//Contour = HomMat2D_T.AffineTransContourXld(Contour);
		// ��ģ��������ӵ���ʾͼ��
		//objDisp = objDisp.ConcatObj(Contour);
		//this->m_pMHWinMain->DispObj(objDisp);

		// ��ģ�����Ĳ���һ��x,��ʾģ������
		// Cross.GenCrossContourXld(Row, Column, HTuple(20), HTuple(45.).TupleRad());		


		// ��ģ������x��ӵ���ʾͼ��
		//Cont = Cont.ConcatObj(Cross);

		// ����ģ����������

		//this->m_pMHWinMain->DispObj(this->ho_ModelRegion);

		this->ho_ModelRegion.AreaCenter(&Row0, &Col0);

		//this->write_message(300, 50, "Row0 " +  Row0 + " Col0 " + Col0);
		//this->write_message(600, 50, "Row  " + Row + " Column " + Column);

		// ����ROI����任����  ��������Ԫ����Ҫ����任		
		this->HomMat2D.VectorAngleToRigid(Row0, Col0, HTuple(0), Row, Column, Angle);
	
		this->HomMatAngle = Angle;  // ����һ�±任�Ƕ�

		UCI::OPTIONS[L"ת������ɹ�"] = TRUE;
	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneFUNCTION()��");
		return FALSE;
	}
	return TRUE;
}

// 1ά�ߴ����
BOOL ST_MYDISP::MeasureOneELEMENT_MEASURE_RECTANGLE2(UCI::OptionsMap * o, HImage & hImg, HXLDCont &Cont)
{
	try {
		// ׼������
		double hv_Row = (*o)[L"Measure_Rectangle2_Row"];
		double hv_Column = (*o)[L"Measure_Rectangle2_Column"];
		double hv_Phi = (*o)[L"Measure_Rectangle2_Phi"];
		double hv_Length1 = (*o)[L"Measure_Rectangle2_Length1"];
		double hv_Length2 = (*o)[L"Measure_Rectangle2_Length2"];

		CString Interpolation = (*o)[L"Measure_Rectangle2_Interpolation"];

		int width = UCI::OPTIONS[L"ͼ����"];
		int height = UCI::OPTIONS[L"ͼ��߶�"];

		HTuple new_hv_Row, new_hv_Column, new_hv_Phi;
		// Ҫ�任һ�°�
		AffineTransPoint2d(this->HomMat2D,
			hv_Row, hv_Column,
			&new_hv_Row, &new_hv_Column);

		new_hv_Phi = hv_Phi + this->HomMatAngle;

		// ��ʾһ�²�������
		HXLDCont Rect;
		Rect.GenRectangle2ContourXld(new_hv_Row[0].D(),
			new_hv_Column[0].D(),
			new_hv_Phi[0].D(),
			hv_Length1,
			hv_Length2);

		Cont = Cont.ConcatObj(Rect);

		HMeasure ho_Measure;
		ho_Measure.GenMeasureRectangle2(
			new_hv_Row[0].D(),
			new_hv_Column[0].D(),
			new_hv_Phi[0].D(),
			hv_Length1,
			hv_Length2,
			width,
			height,
			CT2A(Interpolation).m_psz);

		// ���ò������Ӳ���
		double Sigma = (*o)[L"measure_pairs_Sigma"];
		double Threshold = (*o)[L"measure_pairs_Threshold"];
		CString Transition = (*o)[L"measure_pairs_Transition"];
		CString Select = (*o)[L"measure_pairs_Select"];

		HTuple RowEdgeFirst, ColumnEdgeFirst, AmplitudeFirst, RowEdgeSecond, ColumnEdgeSecond, AmplitudeSecond, IntraDistance, InterDistance;
	
		ho_Measure.MeasurePairs(
			hImg,
			Sigma,
			Threshold,
			CT2A(Transition).m_psz,
			CT2A(Select).m_psz,
			&RowEdgeFirst,
			&ColumnEdgeFirst,
			&AmplitudeFirst,
			&RowEdgeSecond,
			&ColumnEdgeSecond,
			&AmplitudeSecond,
			&IntraDistance,
			&InterDistance);

		int len = (int)RowEdgeFirst.Length();

		// ����������
		(*o)[L"measure_pairs_RowEdgeFirst"] = RowEdgeFirst;
		(*o)[L"measure_pairs_ColumnEdgeFirst"] = ColumnEdgeFirst;
		(*o)[L"measure_pairs_AmplitudeFirst"] = AmplitudeFirst;
		(*o)[L"measure_pairs_RowEdgeSecond"] = RowEdgeSecond;
		(*o)[L"measure_pairs_ColumnEdgeSecond"] = ColumnEdgeSecond;
		(*o)[L"measure_pairs_AmplitudeSecond"] = AmplitudeSecond;
		(*o)[L"RES_measure_pairs_IntraDistance"] = IntraDistance;
		(*o)[L"RES_measure_pairs_InterDistance"] = InterDistance;

		double scalemm = UCI::OPTIONS[L"�������س���mm"];                // �������ص�mm�ı���

		double dis = 0.0f;

		for (int i = 0; i < IntraDistance.Length(); i++) {
			dis += IntraDistance[i].D();
		}

		for (int i = 0; i < InterDistance.Length(); i++) {
			dis += InterDistance[i].D();
		}

		(*o)[L"RES_һά�ܳ�"] = dis * scalemm;              // �����о��������


		// ��ʾ����ߴ�
		//hv_Length2 

		p_disp_dimensions(RowEdgeFirst, ColumnEdgeFirst, RowEdgeSecond, ColumnEdgeSecond,
			IntraDistance, InterDistance, new_hv_Phi, hv_Length2, scalemm,Cont);

	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MeasureOneELEMENT_MEASURE_RECTANGLE2()��");
		return FALSE;
	}
	return TRUE;

	//return 0;
}

// ������ҵ�ȱ��
BOOL ST_MYDISP::MeasureOneELEMENT_MEASURE_BalloonFlaw(UCI::OptionsMap * o, HalconCpp::HImage & hImg, HalconCpp::HXLDCont &Cont)
{
	
	try {
		HImage ImageTmp;

		EnterCriticalSection(&m_csIPDisp);
		ImageTmp = this->m_sIPResult.result_img;    // ȡ��У�����ͼ��
		LeaveCriticalSection(&m_csIPDisp);

		//this->m_pMHWinMain->ClearWindow();

		// reduce_domain
		double Row = (*o)[L"Flaw_Rectangle2_Row"];
		double Column = (*o)[L"Flaw_Rectangle2_Column"];
		double Phi = (*o)[L"Flaw_Rectangle2_Phi"];
		double Length1 = (*o)[L"Flaw_Rectangle2_Length1"];
		double Length2 = (*o)[L"Flaw_Rectangle2_Length2"];

		// Ҫת��һ�°�

		HTuple new_Row, new_Column, new_Phi;
		// Ҫ�任һ�°�
		AffineTransPoint2d(this->HomMat2D,
			Row, Column,
			&new_Row, &new_Column);

		new_Phi = Phi + this->HomMatAngle;


		HalconCpp::HRegion Rectangle;
		Rectangle.GenRectangle2(new_Row, new_Column, new_Phi, (HTuple)Length1, (HTuple)Length2);

		ImageTmp = ImageTmp.ReduceDomain(Rectangle);

		//this->m_pMHWinMain->DispObj(ImageTmp);

		int mean_image_MaskWidth = (*o)[L"Flaw_mean_image_MaskWidth"];
		int mean_image_MaskHeight = (*o)[L"Flaw_mean_image_MaskHeight"];

		HImage ImageMean;
		ImageMean = ImageTmp.MeanImage(mean_image_MaskWidth, mean_image_MaskHeight);

		HalconCpp::HRegion RegionDynThresh;

		double dyn_threshold_Offset = (*o)[L"Flaw_dyn_threshold_Offset"];
		CString dyn_threshold_LightDark = (*o)[L"Flaw_dyn_threshold_LightDark"];

		RegionDynThresh = ImageTmp.DynThreshold(ImageMean, dyn_threshold_Offset, CT2A(dyn_threshold_LightDark).m_psz);

		//this->m_pMHWinMain->DispObj(RegionDynThresh);

		HalconCpp::HRegion ConnectedRegions;
		ConnectedRegions = RegionDynThresh.Connection();

		CString select_shape_Features = (*o)[L"Flaw_select_shape_Features"];
		CString select_shape_Operation = (*o)[L"Flaw_select_shape_Operation"];
		double select_shape_Min = (*o)[L"Flaw_select_shape_Min"];
		double select_shape_Max = (*o)[L"Flaw_select_shape_Max"];		

		HalconCpp::HRegion SelectedRegions;

		SelectedRegions = ConnectedRegions.SelectShape(CT2A(select_shape_Features).m_psz,
			CT2A(select_shape_Operation).m_psz,
			select_shape_Min,
			select_shape_Max);

		double closing_circle_Radius = (*o)[L"Flaw_closing_circle_Radius"];

		HalconCpp::HRegion RegionClosing;

		RegionClosing = SelectedRegions.ClosingCircle(closing_circle_Radius);

		HTuple Areas, Rows, Columns;
		Areas = RegionClosing.AreaCenter(&Rows, &Columns);

		int len = (int)Areas.Length();
		
		(*o)[L"RES_ȱ������"] = (double)len;
		double min_area = select_shape_Max;
		double max_area = 0.0;

		for (int i = 0; i < len; i++) {
			// ��ͼ���ȱ������
			HalconCpp::HXLDCont Circle;

			double area = Areas[i].D();

			if (area > max_area) {
				max_area = area;
			}
			if (area < min_area) {
				min_area = area;
			}

			double radius = sqrt(area / 3.14f) * 3.0f;
			Circle.GenCircleContourXld(Rows[i].D(),
				Columns[i].D(), radius,0.0, 6.28318f,"positive",1.0f);
			Cont = Cont.ConcatObj(Circle);
		}

		(*o)[L"RES_ȱ��������"] = max_area;
		(*o)[L"RES_ȱ����С���"] = min_area;

		//int num = RegionClosing.nu
		//this->m_pMHWinMain->DispObj(RegionClosing);
		//int a = 0;

	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"MEASURE_BalloonFlaw()��");
		return FALSE;
	}
	return TRUE;
}

// ����һ����� ���������
BOOL ST_MYDISP::MeasureOneResult(UCI::OptionsMap * o, HXLDCont& Cont)
{
	CString ResNormal = (*o)[L"����������"];
	CString eleName = (*o)[L"���Ԫ������"];
	CString eleParameter = (*o)[L"���Ԫ�صĲ���"];

	// ���ҵ����Ԫ��
	UCI::OptionsMap * ores = UCI::ELE.GetOptionsMapByEleName(eleName);

	if (ores == nullptr) {
		MyLog(L"Ҫ������Ԫ�ز����ڣ�");
		MyLog(eleName + eleParameter);
		return FALSE;
	}

	if (eleParameter == "") {
		return FALSE;
	}

	double dRes = (*ores)[eleParameter];

	(*o)[L"����Ĳ���ֵ"] = dRes;

	double lower_limit = (*o)[L"������¹���"];
	double upper_limit = (*o)[L"������Ϲ���"];
	double standard = (*o)[L"����ı�׼ֵ"];

	if (dRes < (standard + lower_limit)) {
		(*o)[L"�����������"] = CString(L"TS");
		UCI::OPTIONS[L"GLO_Measure_OK"] = FALSE;
	}
	else if (dRes >(standard + upper_limit)) {
		(*o)[L"�����������"] = CString(L"TL");
		UCI::OPTIONS[L"GLO_Measure_OK"] = FALSE;
	}
	else {
		(*o)[L"�����������"] = CString(L"OK");
	}


	(*o)[L"Ԫ����ƥ��"] = 1;

	return TRUE;
}

// �Զ�У�������� mm 
BOOL ST_MYDISP::MeasureOneMM(UCI::OptionsMap * o, HXLDCont& Cont)
{
	//string ResNormal = (*o)[L"����������"];

	CString eleNameX = (*o)[L"У��Ԫ�� X"];	

	CString eleNameY = (*o)[L"У��Ԫ�� Y"];

	// ���ҵ����Ԫ��
	UCI::OptionsMap * oresX = UCI::ELE.GetOptionsMapByEleName(eleNameX);
	UCI::OptionsMap * oresY = UCI::ELE.GetOptionsMapByEleName(eleNameY);

	if (oresX == nullptr) {
		MyLog(L"Ҫ������Ԫ�� X �����ڣ�");
		MyLog(eleNameX);
		return FALSE;
	}
	if (oresY == nullptr) {
		MyLog(L"Ҫ������Ԫ�� Y �����ڣ�");
		MyLog(eleNameY);
		return FALSE;
	}


	double dResX = (*oresX)[L"����Ĳ���ֵ"];
	double dResY = (*oresY)[L"����Ĳ���ֵ"];


	double standardX = (*oresX)[L"����ı�׼ֵ"];
	double standardY = (*oresY)[L"����ı�׼ֵ"];

	if (standardX == 0) standardX = 1.0f;
	if (standardY == 0) standardY = 1.0f;

	double scalemm = UCI::OPTIONS[L"�������س���mm"];      // �������ص�mm�ı���

	dResX = dResX / scalemm;    // �������
	dResY = dResY / scalemm;

	double scalemmX = standardX / dResX;
	double scalemmY = standardY / dResY;

	double scalem = (scalemmX + scalemmY) / 2.0f;
	
	UCI::OPTIONS[L"�������س���mm"] = scalem;

	MyLog(L"У�� mm �ɹ���");
	CString st = to_wstring(scalem).c_str();
	MyLog(L"OnePixelScalemm = " + st);

	// ����һ�²���
	UCI::sDB.MySaveSettingToSqlite(UCI::OPTIONS);     // ����ȫ�ֲ���
	
	return TRUE;
}

// �ֶ�����һ��
void ST_MYDISP::DisOnElementMeasureManual()
{
	
	// 1 ��⵱ǰ��״̬
	if (!this->TestCurrentAppState(APP_LOGGING_OK)) return;
	this->AppStateChangeTo(APP_MANUAL_MEASURING);
	
	this->bManualMeasureReady = FALSE;	

	try {

		this->m_pMHWinMain->ClearWindow(); 

		HXLDCont Contours;
		HImage hImg;

		BOOL isLoad = UCI::OPTIONS[L"�궨��Ϣ OK"];
		if (isLoad == FALSE) {
			if (this->LoadCaliPoseAndParamter() == FALSE) {

				UCI::OptionsMap * camo = &UCI::ELE.EleList.GetHead();
				//UCI::pDis->OnDisMeasureOnePic(camo);

				if (this->LoadCaliPoseAndParamter() == FALSE) {
					write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");

					this->AppStateChangeTo(APP_READY);
					return;
				}
			}
		}

		HTuple T1, T2;
		CountSeconds(&T1);
		
		// 5. ���β�����������������Ԫ����
		// �������и���Ŀ��Ԫ��
		UCI::OPTIONS[L"GLO_Measure_OK"] = TRUE;                // �Ƿ�ϸ�
		
		//this->elementResultIndex = 1;
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *o = &UCI::ELE.EleList.GetNext(pos);
			//////////////////////////////////////////////////
			// ��ÿһ��Ԫ�ؽ��в���		
			if (!this->MeasureOneElement(o, hImg, Contours)) {				

				CString eleName = (*o)[L"Ԫ������"];
				CString str = L"�ֶ���������Ԫ�����ƣ�" + eleName;			
				MyLog(str);

				this->AppStateChangeTo(APP_READY);
				return;   // ��һ��Ԫ��û�в����ɹ�
			}
		}


		// ��ʾ���յĲ������ 
		CString stColor = UCI::OPTIONS[L"ElementHDLXColor"];
		int width = UCI::OPTIONS[L"ElementHDLXWidth"];

		UCI::pDis->m_pMHWinMain->SetColor(CT2A(stColor).m_psz);
		UCI::pDis->m_pMHWinMain->SetLineWidth(width);

		// ��ʾһ�¼�������
		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_cont = Contours;

		// �ж��ǲ�����ʾԭͼ
		BOOL isDispOrgPic = UCI::OPTIONS[L"��ʾԭͼ"];
		if (!isDispOrgPic) {
			this->m_sIPResult.result_img = hImg;	        // �����ʾԭ����ͼ���һЩ
		}
		LeaveCriticalSection(&m_csIPDisp);		

		
		this->OnMainDisp(NULL, NULL);	

		// ������������LIST��
		UCI::pMain->ResDeleAllItems();
		pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *o = &UCI::ELE.EleList.GetNext(pos);

			int itype = (*o)[L"Ԫ������"];
			if (itype == ELEMENT_RESULT) {
				UCI::pMain->insertOneResult(o);	

				// ��ʾһ�³ߴ�
				CString outID = (*o)[L"���Ԫ������"];
				UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(outID);				
				this->DisElementDimension(om);				
			}
		}
		CountSeconds(&T2);

		HTuple time = 1000 * (T2[0].D() - T1[0].D());
		CString str;
		str.Format(_T("�ֶ�����ʱ��: %4.2f ms"), time[0].D());
		UCI::MyLog(str);
		
		BOOL MOK = UCI::OPTIONS[L"GLO_Measure_OK"];
		if (MOK) {
			UCI::Speech(L"�ϸ�", true);

			SolidBrush blackBrush(Color(255, 0, 0, 255));  //��͸��+����RGB��ɫ
			this->DisFontLarge(L"PASS", 0, 0, 60.0f, blackBrush);
		}
		else {
			UCI::Speech(L"���ϸ�", true);

			SolidBrush blackBrush(Color(255, 255, 0, 0));  //��͸��+����RGB��ɫ
			this->DisFontLarge(L"NG", 0, 0, 60.0f, blackBrush);
		}
	}
	catch (HException& except) {
		UCI::sDB.Log(except, L"DisOnElementMeasureManual��");

	}

	this->bManualMeasureReady = TRUE;
	this->AppStateChangeTo(APP_READY);
}

// ��ʾԪ�صĳߴ� 
void ST_MYDISP::DisElementDimension(UCI::OptionsMap * o)
{
	if (o == nullptr) return;
	
	int etype = (*o)[L"Ԫ������"];
	switch (etype) 
	{
	case ELEMENT_MEASURE_RECTANGLE2:  // һά����
	{
		double midrow = (*o)[L"Measure_Rectangle2_Row"];     // �������ε�����
		double midcol = (*o)[L"Measure_Rectangle2_Column"];

		double hv_Dis = (*o)[L"RES_һά�ܳ�"];

		DisElemnetDimension(o, hv_Dis, midrow, midcol);

	}
		break;
	case ELEMENT_ANGLE:      // ����ֱ����ɵĳ���
		break;
	case ELEMENT_DISTANCE:   // �����ߵľ���
	{
		double row1 = (*o)[L"����Row1"];
		double col1 = (*o)[L"����Col1"];
		double row2 = (*o)[L"����Row2"];
		double col2 = (*o)[L"����Col2"];
		double hv_Dis = (*o)[L"RES_����"];

		double midrow = (row1 + row2) / 2;
		double midcol = (col1 + col2) / 2;	

		DisElemnetDimension(o, hv_Dis, midrow, midcol);

	}
		break;
	case ELEMENT_BALLOONLENGTH: // �ĸ�������ɵ����ҳ���
	{
		double row1 = (*o)[L"Balloon����Row1"];
		double col1 = (*o)[L"Balloon����Col1"];
		double row2 = (*o)[L"Balloon����Row2"];
		double col2 = (*o)[L"Balloon����Col2"];
		double hv_Dis = (*o)[L"RES_���ҳ���"];

		double midrow = (row1 + row2) / 2;
		double midcol = (col1 + col2) / 2;

		DisElemnetDimension(o, hv_Dis, midrow, midcol);
	}
		break;
	default:
		break;
	}
}

void ST_MYDISP::DisElemnetDimension(UCI::OptionsMap * o, double hv_Dis, double midrow, double midcol)
{
	double dimOffsetCol = (*o)[L"�ߴ���ʾƫ��X"];
	double dimOffsetRow = (*o)[L"�ߴ���ʾƫ��Y"];

	// ��ʾ����
	// this->m_pMHWinMain->DispLine(midrow, midcol, midrow + dimOffsetRow, midcol + dimOffsetCol);
	this->m_pMHWinMain->SetTposition(int(midrow + dimOffsetRow), int(midcol + dimOffsetCol));

	// ��ʾ
	HTuple hv_string;
	TupleString(hv_Dis, ".3f", &hv_string);
	this->m_pMHWinMain->WriteString(hv_string);
}



