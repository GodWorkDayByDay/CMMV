#include "stdafx.h"
#include "Myhead.h"
#include "MainFrm.h"

#include "sapi.h"


using namespace HalconCpp;
using namespace std;




void ST_MYDISP::DisBalloonOffileMold()   // �������߽�ģ
{
	
	UCI::OPTIONS["CAMCurGroupID"] = 1;

	try {
		// ��һ��ͼƬ
		HalconCpp::HImage   Image;

		this->DisOnOneShot();
		EnterCriticalSection(&this->m_csFGDisp);
		Image = this->GrabImageDisp;
		LeaveCriticalSection(&this->m_csFGDisp);

		SetSystem("border_shape_models", "true");
		// ���ԭ����ģ��
		this->hv_Model.Clear();

		this->write_message(20, 20, "��ģ������,������λ����,��ɺ�������Ҽ�ȷ��", true);

		double Row, Column, Phi, Length1, Length2;
		HalconCpp::HImage ho_ImageReduced;
		// ������
		this->m_pMHWinMain->DrawRectangle2(&Row, &Column, &Phi, &Length1, &Length2);
		//DrawRectangle2(this->hv_HWD, &Row, &Column, &Phi, &Length1, &Length2);		

		// ��������
		GenRectangle2(&this->ho_ModelRegion, Row, Column, Phi, Length1, Length2);
		// �ü�ģ����ͼ��
		ho_ImageReduced = Image.ReduceDomain(this->ho_ModelRegion);

		this->hv_Model.CreateShapeModel(
			ho_ImageReduced,
			"auto",
			HTuple(-180).TupleRad(),
			HTuple(360).TupleRad(),
			"auto",
			"auto",
			"use_polarity",
			"auto",
			"auto"
		);

		// ����ԭͼ�ϲ���ȷ��һ��.
		HTuple Row3, Column3, Angle1, Score1;

		this->hv_Model.FindShapeModel(
			Image,
			HTuple(-180).TupleRad(),
			HTuple(360).TupleRad(),
			0.5,
			1,
			0.5,
			"least_squares",
			0,
			0.9,
			&Row3,
			&Column3,
			&Angle1,
			&Score1
		);

		// ��ʾһ�²��ҵ���ģ�� 
		dev_display_shape_matching_results(
			this->hv_Model,
			"red",
			Row3,
			Column3,
			Angle1,
			1,
			1,
			0);

		if (Score1 < 0.9) {
			write_message(20, 20, "ģ�嶨�岻׼ȷ! �����»���ģ������ ", true);
			return;
		}

		// ����ֱ��1, �����⾶��һ����
		// 3.  ����ֱ�� 1 ��� ROI
		// DispObj(this->ho_pImage, this->hv_HWD);

		this->m_pMHWinMain->SetColor("green");

		HXLDCont Regions;
		double Row1, Column1, Row2, Column2;

		draw_rake(&Regions, 
			10,
			30,
			15,
			&Row1,
			&Column1,
			&Row2,
			&Column2);
		
		this->m_pMHWinMain->DispObj(Regions);

		HTuple Line1ROIRow, Line1ROICol;

		Line1ROIRow.Clear();
		Line1ROIRow.Append(Row1);
		Line1ROIRow.Append(Row2);

		Line1ROICol.Clear();
		Line1ROICol.Append(Column1);
		Line1ROICol.Append(Column2);

		// 4. 
		this->m_pMHWinMain->SetColor("red");
		draw_rake(&Regions,
			10,
			30,
			15,
			&Row1,
			&Column1,
			&Row2,
			&Column2);

		this->m_pMHWinMain->DispObj(Regions);

		//this->m_pMHWinMain->DrawEllipse

		HTuple Line2ROIRow, Line2ROICol;

		Line2ROIRow.Clear();
		Line2ROIRow.Append(Row1);
		Line2ROIRow.Append(Row2);

		Line2ROICol.Clear();
		Line2ROICol.Append(Column1);
		Line2ROICol.Append(Column2);

		// ������� 
		WriteRegion(this->ho_ModelRegion, "ModelROI.reg");

		//if (this->hv_ModelID > -1) {
		//	WriteShapeModel(this->hv_ModelID, "Model.shm");
		//	ClearShapeModel(this->hv_ModelID);
		//	this->hv_ModelID = -1;
		//}
		this->hv_Model.WriteShapeModel("Model.shm");
		this->hv_Model.Clear();

		// ����ֱ�� 1 ROI ����
		WriteTuple(Line1ROIRow, "Line1ROIRow.tup");
		WriteTuple(Line1ROICol, "Line1ROICol.tup");

		// ����ֱ�� 2 ROI ����
		WriteTuple(Line2ROIRow, "Line2ROIRow.tup");
		WriteTuple(Line2ROICol, "Line2ROICol.tup");

		write_message(100, 20, "���Ҷ�λģ���ѽ����ɹ�. ��! �����Կ�ʼ���������!");
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
	}
}

void ST_MYDISP::DisOnBalloonMeasure()
{
	try {
		ReadRegion(&this->ho_ModelRegion, "ModelROI.reg");
		this->hv_Model.ReadShapeModel("Model.shm");
		//ReadShapeModel("Model.shm", &this->hv_ModelID);

		// ��ȡֱ��1 ROI ����
		HTuple Line1ROIRow, Line1ROICol;
		ReadTuple("Line1ROIRow.tup", &Line1ROIRow);
		ReadTuple("Line1ROICol.tup", &Line1ROICol);

		// ��ȡֱ��2 ROI ����
		HTuple Line2ROIRow, Line2ROICol;
		ReadTuple("Line2ROIRow.tup", &Line2ROIRow);
		ReadTuple("Line2ROICol.tup", &Line2ROICol);

		// ����ģ����������
		HTuple Area, Row0, Column0;
		AreaCenter(this->ho_ModelRegion, &Area, &Row0, &Column0);

		this->DisOnOneShot();   // ��һ��ͼƬ
		HalconCpp::HImage result_img;
		EnterCriticalSection(&this->m_csFGDisp);
		result_img = this->GrabImageDisp;
		LeaveCriticalSection(&this->m_csFGDisp);

		HTuple Row, Column, Angle, Score;

		this->hv_Model.FindShapeModel(
			result_img,
			HTuple(-180).TupleRad(),
			HTuple(360).TupleRad(),
			0.5,
			1,
			0.5,
			"least_squares",
			0,
			0.9,
			&Row,
			&Column,
			&Angle,
			&Score
		);	

		if (Row.TupleLength() < 1 || Score<0.8) {
			write_message(20, 20, "��, û�з�������, ����, ������Ҳ����Ķ���?", true);
			return;
		}

		// 4. �ҵ�����, ��������, �� ROI ��������з���任

		HTuple HomMat2D;
		VectorAngleToRigid(Row0, Column0, 0, Row, Column, Angle, &HomMat2D);

		// ��ֱ�ߺ�Բ ROI ������������з���任

		// ֱ�� 1 ROI ����
		HTuple NewLine1ROIRow, NewLine1ROICol;
		AffineTransPoint2d(HomMat2D, Line1ROIRow, Line1ROICol,
			&NewLine1ROIRow, &NewLine1ROICol);

		// ֱ�� 2 ROI ����
	    HTuple NewLine2ROIRow, NewLine2ROICol;
		AffineTransPoint2d(HomMat2D, Line2ROIRow, Line2ROICol,
		&NewLine2ROIRow, &NewLine2ROICol);


		// 5. ����ֱ��1, ֱ��2�ı�Ե�����

		// ���ֱ�� 1
		// �ұ�Ե
		HXLDCont Regions1;
		HTuple ResultRow1, ResultColumn1;
		rake(
			result_img,
			&Regions1,
			10,
			30,
			15,
			1,
			20,
			"all",
			"max",
			NewLine1ROIRow[0],
			NewLine1ROICol[0],
			NewLine1ROIRow[1],
			NewLine1ROICol[1],
			&ResultRow1,
			&ResultColumn1);


		// ���
		HXLDCont Line1;
		HTuple 	Line1Row11, Line1Column11, Line1Row21, Line1Column21;
		pts_to_best_line(
			&Line1,
			ResultRow1,
			ResultColumn1,
			2,
			&Line1Row11,
			&Line1Column11,
			&Line1Row21,
			&Line1Column21);

		// ���ֱ�� 2
		// �ұ�Ե
		HObject Regions2;
		HTuple ResultRow2, ResultColumn2;
		rake(
			result_img,
			&Regions1,
			10,
			30,
			15,
			1,
			20,
			"all",
			"max",
			NewLine2ROIRow[0],
			NewLine2ROICol[0],
			NewLine2ROIRow[1],
			NewLine2ROICol[1],
			&ResultRow2,
			&ResultColumn2);

		// ���
		HXLDCont Line2;
		HTuple 	Line2Row11, Line2Column11, Line2Row21, Line2Column21;
		pts_to_best_line(
			&Line2,
			ResultRow2,
			ResultColumn2,
			2,
			&Line2Row11,
			&Line2Column11,
			&Line2Row21,
			&Line2Column21);

		// ��ֱ��1��ֱ��2�ľ���
		// ��ֱ��2���е�, ���е㵽ֱ��1�ľ�����Ϊ2��ֱ�ߵľ���
		HTuple Line2CenterY = (Line2Row11 + Line2Row21)*0.5;
		HTuple Line2CenterX = (Line2Column11 + Line2Column21)*0.5;
		HTuple Line2ToLine1Distance;

		DistancePl(Line2CenterY, Line2CenterX,
			Line1Row11, Line1Column11,
			Line1Row21, Line1Column21,
			&Line2ToLine1Distance);

		// ��ʾͼ��ͽ��
		this->m_pMHWinMain->SetLineWidth(2);
		this->m_pMHWinMain->DispObj(result_img);

		// ��ʾֱ��1
		this->m_pMHWinMain->SetColor("red");
		this->m_pMHWinMain->DispObj(Line1);

		// ��ʾֱ��2
		this->m_pMHWinMain->SetColor("green");
		this->m_pMHWinMain->DispObj(Line2);

		write_message(300, 20, "��, ����ֱ����:" + Line2ToLine1Distance, true);

	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
	}
}

void ST_MYDISP::DisOnButOffLineMold()
{

	//CLSID CLSID_SpVoice;    // class ID for the SAPI SpVoice object
	//LPDISPATCH pDisp;       // dispatch interface for the class
	//ISpeechVoice voice;     // use the MFC Class Wizard generated wrapper

	//CLSIDFromProgID(L"SAPI.SpVoice", &CLSID; _SpVoice);
	//voice.CreateDispatch(CLSID_SpVoice);
	//pDisp = voice.m_lpDispatch;

	UCI::OPTIONS["CAMCurGroupID"] = 1;
	
	// ��һ��ͼƬ
	this->DisOnOneShot(); 
	HalconCpp::HImage result_img;
	EnterCriticalSection(&this->m_csFGDisp);
	result_img = this->GrabImageDisp;
	LeaveCriticalSection(&this->m_csFGDisp);

	//// Display the results: contours on original image
	//EnterCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect		
	//UCI::pDis->m_sIPResult.result_img = result_img;
	////UCI::pDis->m_sIPResult.result_cont = Contours;
	//LeaveCriticalSection(&UCI::pDis->m_csIPDisp);      // CriticalSect
	//PostMessage(this->pCView->GetSafeHwnd(), WM_IPDISP, NULL, NULL);

	this->m_pMHWinMain->DispObj(result_img);

	//
	SetSystem("border_shape_models", "true");

	// ���ԭ����ģ��
	this->hv_Model.Clear();

	this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��", true);

	double Row, Column, Phi, Length1, Length2;
	HalconCpp::HImage ho_ImageReduced;
	// ������
	this->m_pMHWinMain->DrawRectangle2(&Row, &Column, &Phi, &Length1, &Length2);
	// ��������
	GenRectangle2(&this->ho_ModelRegion, Row, Column, Phi, Length1, Length2);
	// �ü�ģ����ͼ��
	ho_ImageReduced = result_img.ReduceDomain(this->ho_ModelRegion);
	//ReduceDomain(this->ho_Image, this->ho_ModelRegion, &ho_ImageReduced);

	HShapeModel hsModel;

	hsModel.CreateShapeModel(
		ho_ImageReduced,
		"auto",
		HTuple(-180).TupleRad(),
		HTuple(360).TupleRad(),
		"auto",
		"auto",
		"use_polarity",
		"auto",
		"auto"
	);

	// ����ԭͼ�ϲ���ȷ��һ��.
	HTuple Row3, Column3, Angle1, Score1;

	hsModel.FindShapeModel(
		result_img,
		HTuple(-180).TupleRad(),
		HTuple(360).TupleRad(),
		0.5,
		1,
		0.5,
		"least_squares",
		0,
		0.9,
		&Row3,
		&Column3,
		&Angle1,
		&Score1
	);

	// ��ʾһ�²��ҵ���ģ�� 
	dev_display_shape_matching_results(
		hsModel,
		"red",
		Row3,
		Column3,
		Angle1,
		1,
		1,
		0);

	if (Score1 < 0.9) {
		write_message(20, 20, "ģ�嶨�岻׼ȷ! ",true);
		return;
	}

	// ����ֱ�� 1 ��� ROI
	this->m_pMHWinMain->SetColor("green");
	
	HXLDCont Regions;
	double Row1, Column1, Row2, Column2;
	draw_rake(&Regions, 
		10,
		30,
		15,
		&Row1,
		&Column1,
		&Row2,
		&Column2);

	this->m_pMHWinMain->DispObj(Regions);

	HTuple Line1ROIRow, Line1ROICol;

	Line1ROIRow.Clear();
	Line1ROIRow.Append(Row1);
	Line1ROIRow.Append(Row2);

	Line1ROICol.Clear();
	Line1ROICol.Append(Column1);
	Line1ROICol.Append(Column2);

	// ����Բ��� ROI
	HXLDCont Regions1;
	HTuple ROIRows, ROICols, Direct;
	draw_spoke(
		//result_img,
		&Regions1,
		30,
		30,
		15,
		&ROIRows,
		&ROICols,
		&Direct
	);

	HTuple CircleROIRow, CircleROICol;
	CircleROIRow = ROIRows;
	CircleROICol = ROICols;

	this->m_pMHWinMain->DispObj(Regions1);

	// �������
	WriteRegion(this->ho_ModelRegion, "ModelROI.reg");

	//if (ModelID > -1) {
	//	WriteShapeModel(ModelID, "Model.shm");
	//	ClearShapeModel(ModelID);
	//	ModelID = -1;
	//}
	hsModel.WriteShapeModel("Model.shm");
	hsModel.Clear();

	// ����ֱ�� 1 ROI ����
	WriteTuple(Line1ROIRow, "Line1ROIRow.tup");
	WriteTuple(Line1ROICol, "Line1ROICol.tup");


	// ����Բ ROI ����
	WriteTuple(CircleROIRow, "CircleROIRow.tup");
	WriteTuple(CircleROICol, "CircleROICol.tup");       
	WriteTuple(Direct, "CircleDirect.tup");             // Բ�ķ���

	//
	write_message(100, 20, "���߽�ģ�ɹ���!", true);
}



void ST_MYDISP::DisOnButOnlineInspect() {
	// ��ȡģ������
	HObject ModelROI;
	HTuple ModelID;

	try {
		ReadRegion(&ModelROI, "ModelROI.reg");
		//ReadShapeModel("Model.shm", &ModelID);
		this->hv_Model.ReadShapeModel("Model.shm");
		
		// ��ȡֱ��1 ROI ����
		HTuple Line1ROIRow, Line1ROICol;
		ReadTuple("Line1ROIRow.tup", &Line1ROIRow);
		ReadTuple("Line1ROICol.tup", &Line1ROICol);

		// ��ȡԲ ROI ����
		HTuple CircleROIRow, CircleROICol;
		ReadTuple("CircleROIRow.tup", &CircleROIRow);
		ReadTuple("CircleROICol.tup", &CircleROICol);

		SetSystem("border_shape_models", "true");

		// ����ģ����������
		HTuple Area, Row0, Column0;
		AreaCenter(ModelROI, &Area, &Row0, &Column0);

		this->DisOnOneShot();   // ��һ��ͼƬ
		HalconCpp::HImage result_img;
		EnterCriticalSection(&this->m_csFGDisp);
		result_img = this->GrabImageDisp;
		LeaveCriticalSection(&this->m_csFGDisp);

		this->m_pMHWinMain->DispObj(result_img);

		HTuple Row, Column, Angle, Score;
		this->hv_Model.FindShapeModel(
			result_img,
			HTuple(-180).TupleRad(),
			HTuple(360).TupleRad(),
			0.5,
			1,
			0.5,
			"least_squares",
			0,
			0.9,
			&Row,
			&Column,
			&Angle,
			&Score);		

		if (Row.TupleLength() < 1) {
			write_message(20, 20, "û�з��ֶ���, �����´򿪶���", true);
			return;
		}

		// ���濪ʼ����
		

		// 4. �ҵ�����, ��������, �� ROI ��������з���任

		HTuple HomMat2D;
		VectorAngleToRigid(Row0, Column0, 0, Row, Column, Angle, &HomMat2D);

		// ��ֱ�ߺ�Բ ROI ������������з���任

		// ֱ�� 1 ROI ����
		HTuple NewLine1ROIRow, NewLine1ROICol;
		AffineTransPoint2d(HomMat2D, Line1ROIRow, Line1ROICol,
			&NewLine1ROIRow, &NewLine1ROICol);

		// ֱ�� 2 ROI ����
		//HTuple NewLine2ROIRow, NewLine2ROICol;
		//AffineTransPoint2d(HomMat2D, Line2ROIRow, Line2ROICol,
		//	&NewLine2ROIRow, &NewLine2ROICol);

		// Բ ROI ����
		HTuple NewCircleROIRow, NewCircleROICol;
		AffineTransPoint2d(HomMat2D, CircleROIRow, CircleROICol,
			&NewCircleROIRow, &NewCircleROICol);

		// 5. ����ֱ��1, ֱ��2�ı�Ե�����

		// ���ֱ�� 1
		// �ұ�Ե
		HXLDCont Regions1;
		HTuple ResultRow1, ResultColumn1;
		rake(
			result_img,
			&Regions1,
			10,
			30,
			15,
			1,
			20,
			"all",
			"max",
			NewLine1ROIRow[0],
			NewLine1ROICol[0],
			NewLine1ROIRow[1],
			NewLine1ROICol[1],
			&ResultRow1,
			&ResultColumn1);


		// ���
		HXLDCont Line1;
		HTuple 	Line1Row11, Line1Column11, Line1Row21, Line1Column21;
		pts_to_best_line(
			&Line1,
			ResultRow1,
			ResultColumn1,
			2,
			&Line1Row11,
			&Line1Column11,
			&Line1Row21,
			&Line1Column21);

		// ���Բ
		// �ұ�Ե
		HXLDCont Regions3;
		HTuple ResultRow3, ResultColumn3, ArcType;
		spoke(
			result_img,
			&Regions3,
			30,
			30,
			15,
			1,
			20,
			"all",
			"max",
			NewCircleROIRow,
			NewCircleROICol,
			"inner",
			&ResultRow3,
			&ResultColumn3,
			&ArcType);

		// ���
		HXLDCont Circle1;
		HTuple RowCenter, ColCenter, Radius, StartPhi, EndPhi, PointOrder;
		pts_to_best_circle(
			&Circle1,
			ResultRow3,
			ResultColumn3,
			3,
			"circle",
			&RowCenter,
			&ColCenter,
			&Radius,
			&StartPhi,
			&EndPhi,
			&PointOrder);

		// ����Բ��
		HObject Cross;
		GenCrossContourXld(&Cross, RowCenter, ColCenter, 16, 0);

		// 7. ����, ���������ص���, �Ѳ��������ͼ������ת����������ϵ�ռ�

		// ��ֱ��1��ֱ��2�ľ���
		// ��ֱ��2���е�, ���е㵽ֱ��1�ľ�����Ϊ2��ֱ�ߵľ���
		//HTuple Line2CenterY = (Line2Row11 + Line2Row21)*0.5;
		//HTuple Line2CenterX = (Line2Column11 + Line2Column21)*0.5;
		HTuple Line2ToLine1Distance;

		// ��Բ�ĵ�ֱ�ߵľ���
		DistancePl(RowCenter, ColCenter,
			Line1Row11, Line1Column11,
			Line1Row21, Line1Column21,
			&Line2ToLine1Distance);

		// �Ѿ����ͼ��ռ�ת��������ռ�,��λ mm
		HTuple Res = 1.0;
		Line2ToLine1Distance = Line2ToLine1Distance * Res;

		//// ��ֱ��2��ֱ��1�ļн�,��λ: ����
		//HTuple Line2ToLine1Angle;
		//AngleLl(
		//	Line2Row11,
		//	Line2Column11,
		//	Line2Row21,
		//	Line2Column21,
		//	Line1Row11,
		//	Line1Column11,
		//	Line1Row21,
		//	Line1Column21,
		//	&Line2ToLine1Angle);

		//// ��ֱ��1��ֱ��2�ļн�,��λ: ����
		//HTuple Line1ToLine2Angle;
		//AngleLl(
		//	Line1Row11,
		//	Line1Column11,
		//	Line1Row21,
		//	Line1Column21,
		//	Line2Row11,
		//	Line2Column11,
		//	Line2Row21,
		//	Line2Column21,
		//	&Line1ToLine2Angle);

		// Բ�İ뾶, ��ͼ��ռ�ת��������ռ� ��λ:mm
		HTuple RadiusWord = Radius*Res;

		// ��ʾͼ��ͽ��
		this->m_pMHWinMain->SetLineWidth(3);
		this->m_pMHWinMain->DispObj(result_img);

		// ��ʾֱ��1
		this->m_pMHWinMain->SetColor("red");
		this->m_pMHWinMain->DispObj(Line1);

		// ��ʾֱ��2
		//SetColor(this->hv_HWD, "green");
		//DispObj(Line2, this->hv_HWD);

		// ��ʾԲ��Բ��
		this->m_pMHWinMain->SetColor("blue");
		this->m_pMHWinMain->DispObj(Circle1);
		this->m_pMHWinMain->DispObj(Cross);

		write_message(80, 20, "�ҵ�����,�÷�:" + Score, true);
		// ��ʾ�������
		write_message(200, 20, "Բ�ĵ�ֱ��1�ľ���Ϊ(mm)" + Line2ToLine1Distance, false);
		//write_message(240, 20, "ֱ��2��ֱ��1�ļн� ����:" + Line2ToLine1Angle, true);
		//write_message(280, 20, "ֱ��1��ֱ��2�ļн� ����:" + Line1ToLine2Angle, true);
		write_message(320, 20, "Բ�뾶 mm:" + RadiusWord,false);

		if (ModelID > -1) {
			ClearShapeModel(ModelID);
		}


	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
	}
}

void ST_MYDISP::DisOnButOnlineInspect_3_86()
{
	// ��ȡģ������
	HObject ModelROI;
	//HTuple ModelID;

	try {

		ReadRegion(&ModelROI, "ModelROI.reg");

		this->hv_Model.ReadShapeModel("Model.shm");
		//ReadShapeModel("Model.shm", &ModelID);

		// ��ȡֱ��1 ROI ����
		HTuple Line1ROIRow, Line1ROICol;
		ReadTuple("Line1ROIRow.tup", &Line1ROIRow);
		ReadTuple("Line1ROICol.tup", &Line1ROICol);

		// ��ȡֱ��2 ROI ����
		HTuple Line2ROIRow, Line2ROICol;
		ReadTuple("Line2ROIRow.tup", &Line2ROIRow);
		ReadTuple("Line2ROICol.tup", &Line2ROICol);

		// ��ȡԲ ROI ����
		HTuple CircleROIRow, CircleROICol;
		ReadTuple("CircleROIRow.tup", &CircleROIRow);
		ReadTuple("CircleROICol.tup", &CircleROICol);

		SetSystem("border_shape_models", "true");

		// ����ģ����������
		HTuple Area, Row0, Column0;
		AreaCenter(ModelROI, &Area, &Row0, &Column0);

		DisOnOpenImage();
		HalconCpp::HImage result_img;
		EnterCriticalSection(&this->m_csFGDisp);
		result_img = this->GrabImageDisp;
		LeaveCriticalSection(&this->m_csFGDisp);

		HTuple Row, Column, Angle, Score;
		this->hv_Model.FindShapeModel(
			result_img,
			HTuple(-180).TupleRad(),
			HTuple(360).TupleRad(),
			0.5,
			1,
			0.5,
			"least_squares",
			0,
			0.9,
			&Row,
			&Column,
			&Angle,
			&Score);		

		if (Row.TupleLength() < 1) {
			write_message(20, 20, "û�з��ֶ���, �����´򿪶���", true);
			return;
		}

		write_message(80, 20, "�ҵ�����,�÷�:" + Score, true);

		// 4. �ҵ�����, ��������, �� ROI ��������з���任

		HTuple HomMat2D;
		VectorAngleToRigid(Row0, Column0, 0, Row, Column, Angle, &HomMat2D);

		// ��ֱ�ߺ�Բ ROI ������������з���任
		
		// ֱ�� 1 ROI ����
		HTuple NewLine1ROIRow, NewLine1ROICol;
		AffineTransPoint2d(HomMat2D, Line1ROIRow, Line1ROICol,
			&NewLine1ROIRow, &NewLine1ROICol);

		// ֱ�� 2 ROI ����
		HTuple NewLine2ROIRow, NewLine2ROICol;
		AffineTransPoint2d(HomMat2D, Line2ROIRow, Line2ROICol,
			&NewLine2ROIRow, &NewLine2ROICol);

		// Բ ROI ����
		HTuple NewCircleROIRow, NewCircleROICol;
		AffineTransPoint2d(HomMat2D, CircleROIRow, CircleROICol,
			&NewCircleROIRow, &NewCircleROICol);

		// 5. ����ֱ��1, ֱ��2�ı�Ե�����

		// ���ֱ�� 1
		// �ұ�Ե
		HXLDCont Regions1;
		HTuple ResultRow1, ResultColumn1;
		rake(
			result_img,
			&Regions1,
			10,
			30,
			15,
			1,
			20,
			"all",
			"max",
			NewLine1ROIRow[0],
			NewLine1ROICol[0],
			NewLine1ROIRow[1],
			NewLine1ROICol[1],
			&ResultRow1,
			&ResultColumn1);


		// ���
		HXLDCont Line1;
		HTuple 	Line1Row11, Line1Column11, Line1Row21, Line1Column21;
		pts_to_best_line(
			&Line1,
			ResultRow1,
			ResultColumn1,
			2,
			&Line1Row11,
			&Line1Column11,
			&Line1Row21,
			&Line1Column21);

		// ���ֱ�� 2
		// �ұ�Ե
		HXLDCont Regions2;
		HTuple ResultRow2, ResultColumn2;
		rake(
			result_img,
			&Regions2,
			10,
			30,
			15,
			1,
			20,
			"all",
			"max",
			NewLine2ROIRow[0],
			NewLine2ROICol[0],
			NewLine2ROIRow[1],
			NewLine2ROICol[1],
			&ResultRow2,
			&ResultColumn2);

		// ���
		HXLDCont Line2;
		HTuple 	Line2Row11, Line2Column11, Line2Row21, Line2Column21;
		pts_to_best_line(
			&Line2,
			ResultRow2,
			ResultColumn2,
			2,
			&Line2Row11,
			&Line2Column11,
			&Line2Row21,
			&Line2Column21);

		// ���Բ
		// �ұ�Ե
		HXLDCont Regions3;
		HTuple ResultRow3, ResultColumn3, ArcType;
		spoke(
			result_img,
			&Regions3,
			30,
			30,
			15,
			1,
			20,
			"all",
			"max",
			NewCircleROIRow,
			NewCircleROICol,
			"inner",
			&ResultRow3,
			&ResultColumn3,
			&ArcType);

		// ���
		HXLDCont Circle1;
		HTuple RowCenter, ColCenter, Radius, StartPhi, EndPhi, PointOrder;
		pts_to_best_circle(
			&Circle1,
			ResultRow3,
			ResultColumn3,
			3,
			"circle",
			&RowCenter,
			&ColCenter,
			&Radius,
			&StartPhi,
			&EndPhi,
			&PointOrder);

		// ����Բ��
		HObject Cross;
		GenCrossContourXld(&Cross, RowCenter, ColCenter, 16, 0);

		// 7. ����, ���������ص���, �Ѳ��������ͼ������ת����������ϵ�ռ�

		// ��ֱ��1��ֱ��2�ľ���
		// ��ֱ��2���е�, ���е㵽ֱ��1�ľ�����Ϊ2��ֱ�ߵľ���
		HTuple Line2CenterY = (Line2Row11 + Line2Row21)*0.5;
		HTuple Line2CenterX = (Line2Column11 + Line2Column21)*0.5;
		HTuple Line2ToLine1Distance;

		DistancePl(Line2CenterY, Line2CenterX,
			Line1Row11, Line1Column11,
			Line1Row21, Line1Column21,
			&Line2ToLine1Distance);

		// �Ѿ����ͼ��ռ�ת��������ռ�,��λ mm
		HTuple Res = 1.0;
		Line2ToLine1Distance = Line2ToLine1Distance * Res;
		
		// ��ֱ��2��ֱ��1�ļн�,��λ: ����
		HTuple Line2ToLine1Angle;
		AngleLl(
			Line2Row11,
			Line2Column11,
			Line2Row21,
			Line2Column21,
			Line1Row11,
			Line1Column11,
			Line1Row21,
			Line1Column21,
			&Line2ToLine1Angle);

		// ��ֱ��1��ֱ��2�ļн�,��λ: ����
		HTuple Line1ToLine2Angle;
		AngleLl(
			Line1Row11,
			Line1Column11,
			Line1Row21,
			Line1Column21,
			Line2Row11,
			Line2Column11,
			Line2Row21,
			Line2Column21,
			&Line1ToLine2Angle);

		// Բ�İ뾶, ��ͼ��ռ�ת��������ռ� ��λ:mm
		HTuple RadiusWord = Radius*Res;

		// ��ʾͼ��ͽ��
		this->m_pMHWinMain->SetLineWidth(3);
		this->m_pMHWinMain->DispObj(result_img);

		// ��ʾֱ��1
		this->m_pMHWinMain->SetColor("red");
		this->m_pMHWinMain->DispObj(Line1);

		// ��ʾֱ��2
		this->m_pMHWinMain->SetColor("green");
		this->m_pMHWinMain->DispObj(Line2);

		// ��ʾԲ��Բ��
		this->m_pMHWinMain->SetColor("blue");
		this->m_pMHWinMain->DispObj(Circle1);
		this->m_pMHWinMain->DispObj(Cross);

		// ��ʾ�������
		write_message(200, 20, "ֱ��2��ֱ��1�ľ���Ϊ(mm)" + Line2ToLine1Distance, true);
		write_message(240, 20, "ֱ��2��ֱ��1�ļн� ����:" + Line2ToLine1Angle, true);
		write_message(280, 20, "ֱ��1��ֱ��2�ļн� ����:" + Line1ToLine2Angle, true);
		write_message(320, 20, "Բ�뾶 mm:" + RadiusWord);

		//if (ModelID > -1) {
		//	ClearShapeModel(ModelID);
		//}
		this->hv_Model.Clear();
		
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
	}

}
