#include "stdafx.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "ProjectView.h"

#include <algorithm>
#include <sstream>

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;

using namespace std;
using std::string;

namespace UCI {

	size_t Option::insert_order = 0;
	
	////////////////////////////////////////////////////////////////////////////////////
	/// ��ʼ��ȫ�ֱ�����ȱʡֵ
	void initOptionsMap(OptionsMap& o) {

		Option::insert_order = 0;       // ��Ŀ������ʾ��

		o[L"��Ŀ���"] << Option(L"��Ŀ���", 1, 1, 200000,                             // ��ǰ����Ŀ���
			L"project ID", false);	

		//o[L"��ǰ����ID"] << Option(L"��ǰ����ID", 1, 1, 200000,                         // ��ǰ���������б�����ʾ��ID
		//	L"��ǰ����ID", true);

		o[L"ͼ�����"] << Option(L"dRadio", 600.0f/800.0f, 0.5f, 2.0f,                            // ͼ�񴰿ڱ���
			L"Picture Ration height/width [0.5, 2.0] default 0.75");													

		o[L"������ʱ"] << Option(L"iSoftTimeCamCapture", 20, 10, 2000,                         // �����Ķ�ʱʱ��
			L"In soft trigger mode,Camera capture one picture time [10,2000] defautl 20ms");   

		o[L"������ʽ"] << Option(L" trigger_mode", 0, 0, 2,                                      // ������ʽ
			L"trigger mode 0: continues 1: soft trigger 2: hard trigger", 
			true,on_camera_para);

		o[L"���Э��"] << Option(L"CAMinterfaceName",L"MindVision12_X64",                      // �ӿ����� File, DirectShow ...
			L"Camer interface: MindVision12_X64 or DirectShow");  

		o[L"�������"] << Option(L"CAMDeviceName", L"CM300",                                    // ��ǰѡ����������
			L"Camera DeviceName");  

		o[L"���񱣴��ʽ"] << Option(L"PictureSaveFormat", L"png",                              // ͼ�񱣴��ʽ
			L"Picture Save Format",false);

		//o[L"CAMCurGroupID"] << Option(L"CAMCurGroupID", 1, 1, 10,                               // ��ǰ������������	
		//	"CAMCurGroup number", false);      

		// �������
		o[L"GLO_Measure_OK"] << Option(L"GLO_Measure_OK", 0, 0, 1,                               // ���һ�β������
			L"Last Measure Is OK", false);

		o[L"GLO_Measure_Always_OK"] << Option(L"GLO_Measure_Always_OK", 0, 0, 1,                 // ���һ�β������
			L"Last Measure Always Is OK", false);

		o[L"ת������ɹ�"] << Option(L"GLO_HomMat2D_OK", 0, 0, 1,                                // �ҵ�ģ�ͣ���������ת�������ˡ�
			L"Fine model and already creat HomMat2D Is OK", true);

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// �ڲα궨ȫ�ֱ���
		o[L"�궨��Ϣ OK"] << Option(L"LoadCalibrateOK", 0, 0, 1,                                 // ���ر궨��ϢOK��
			L"Load Calibrate Pose and Parameters OK", true);

		o[L"ƥ��ģ�� OK"] << Option(L"Load_ROI_OK", 0, 0, 1,                                     // ����ģ��ƥ����ϢOK��
			L"����ģ��ƥ����ϢOK��", true);
		
		o[L"����궨��ʱ"] << Option(L"iSoftTimeCamCalibrate", 50, 10, 20000,                    // �궨������ʱʱ��
			L"In soft trigger mode,Camera Calibrate timer [10,20000] defautl 50ms");

		o[L"CamCalibratePath"] << Option(L"CamCalibratePath", L"Calibration File",                // ����궨�洢·��
			L"Camera Calibrate used file store path",false);

		o[L"CamCalibrateModel"] << Option(L"CamCalibrateModel", L"area_scan_polynomial",          // ����궨ģ��
			L"CamCalibrate Model area_scan_polynomial, ",false);

		o[L"Զ�ľ�ͷ"] << Option(L"LensTelecentric", 0, 0, 1,                                   // �Ƿ�Զ�ľ�ͷ
			L"is camera lens telecentric: 0: no, 1: yes"	);

		o[L"��б��ͷ"] << Option(L"LensSlope", 0, 0, 1,                                         // �Ƿ���б��ͷ
			L"is camera lens slope: 0: no, 1: yes");

		o[L"���ؿ��"] << Option(L"LensFSx", 2.2f, 1.00f, 200.0f,                                 // ���ؿ��
			L"lens pixel x size [1.0, 200.0] um");

		o[L"���ظ߶�"] << Option(L"LensFSy", 2.2f, 1.00f, 200.0f,                                 // ���ظ߶�
			L"lens pixel y size [1.0, 200.0] um");

		o[L"��ͷ����"] << Option(L"LensFocus", 8.0f, 1.00f, 200.0f,                             // ��ͷ����
			L"lens Focus [1.0, 200.0] mm");
		
		o[L"�궨����"] << Option(L"CaliPlateThick", 0.001f, 0.000001f, 10.0f,               // �궨����
			L"Calibrate Plate Thicnkness [0.000001, 10.0] m");

		o[L"ӳ����"] << Option(L"RectificationWidth", 75.0f, 0.1f, 1000.0f,                      // ӳ����
			L"RectificationWidth [0.1, 1000.0] mm");

		o[L"�궨ͼƬ����"] << Option(L"CaliTotalPicture", 8, 5, 20,                            // ���궨ͼƬ����
			L"CaliTotalPicture: [5, 20], default: 8");

		o[L"�궨�������"] << Option(L"CaliPicMaxError", 0.55f, 0.01f, 10.0f,                   // �궨�������
			L"CaliPicMaxError [0.01, 10.0] pix default: 0.55");

		o[L"�궨��ǰ���"] << Option(L"CaliCurrentError", 0.0f, 0.001f, 10.0f,                  // �궨��ǰ���
			L"CaliPic Current Error [0.01, 10.0] pix default: 0.55");

		o[L"�궨�����������"] << Option(L"�궨�����������", 1.0f, 0.1f, 2.0f,                  
			L"�궨gap_tolerance default 1.0f min 0.1 max 2.0");
		
		o[L"�궨ƽ��alpha"] << Option(L"�궨ƽ��alpha", 0.50f, 0.1f, 2.0f,                  
			L"�궨alpha default 1.0f min 0.1 max 2.0");
		
		o[L"�궨����־��ֱ��"] << Option(L"�궨����־��ֱ��", 350.0f, 1.0f, 500.0f,                  
			L"�궨����־��ֱ�� default 1.0f min 1.0 max 500.0");

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// �궨�ļ�������
		o[L"ͼ����"] << Option(L"ORGwidth", 2592, 200, INT_MAX,
			L"width [100,2592] pixel");                                                           // "ͼ����"
		o[L"ͼ��߶�"] << Option(L"ORGheight", 1944, 200, INT_MAX,
			L"height [100,1944] pixel");

		o[L"�ߴ��ûҶ�"] << Option(L"�ߴ��ûҶ�", 250, 0, 255,
			L"�����ߴ�ʱ�����ͼ��ʱʹ�õĻҶ�");

		o[L"ORGwhByCamera"] << Option(L"ORGwhByCamera", 0, 0, 1,
			L"ORG width and height is get from Camera ", false);

		o[L"CaliFileSaveName"] << Option(L"CaliFileSaveName", L"calitabsave",                       // ����궨�����ļ�
			L"CamCalibrate save file name ",false);

		o[L"�궨�����ļ�"] << Option(L"CaliDescripteName", L"caltab_30mm.descr",                   // ����궨�����ļ�
			L"CamCalibrate describe file name ");

		//o[L"�������س���"] << Option(L"OnePixelDis", 0.013f, 0.00000001f, 1.0f,                     // ���ñ궨���ͼ��һ�����ش����ʵ�ʾ��루�ף�
		//	L"Cali One Pixel Distane [0.00000001, 1.0] m default: 0.00013");

		o[L"�궨ԭ�� X ����"] << Option(L"CaliOriginPoseX", -0.5f, -2.0f, 2.0f,                // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			L"Cali One CaliOriginPose X to Width [FLT_MIN, FLT_MAX] m default: -0.04");

		o[L"�궨ԭ�� Y ����"] << Option(L"CaliOriginPoseY", -0.375f, -2.0f, 2.0f,                 // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			L"Cali One CaliOriginPose Y of Height [FLT_MIN, FLT_MAX] m default: -0.03");

		o[L"�궨ԭ�� Z ����"] << Option(L"CaliOriginPoseZ",  0.00f, -100.9f, 100.1f,                  // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			L"Cali One CaliOriginPose Z [FLT_MIN, FLT_MAX] m default: -0.03");

		
		o[L"�궨 X �� �Ƕ�"] << Option(L"CalibRotateAngle X", 0.0f, -360.0f, 360.0f,            // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			L"Calib Rotate Angle X [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o[L"�궨 Y �� �Ƕ�"] << Option(L"CalibRotateAngle Y", 0.0f, -360.0f, 360.0f,           // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			L"Calib Rotate Angle Y [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o[L"�궨 Z �� �Ƕ�"] << Option(L"CalibRotateAngle Z", 180.0f, -360.0f, 360.0f,          // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			L"Calib Rotate Angle Z [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o[L"ӳ����"] << Option(L"RectificationWidth", 40.0f, 5.0f, FLT_MAX,                    // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			L"RectificationWidth [0.01F, FLT_MAX] degree default: 40.0");

		o[L"�������س���mm"] << Option(L"OnePixelScalemm", 0.030f, 0.0001f, FLT_MAX,               // ����һ�����ش���ĳߴ�
			L"OnePixel Scale mm [0.0001, FLT_MAX] degree default: 0.030");


	
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// ��������
		o[L"TestUseStorePicture"] << Option(L"TestUseStorePicture", 0, 0, 1,                                                         
			L"Test Use prestored Picture", true);

		o[L"TestUseStorePicName"] << Option(L"TestUseStorePicName", L"roimatch.png",                                                 
			L"Test Use prestored Picture Name ", true);

		o[L"ElementDoubleClickColor"] << Option(L"ElementDoubleClickColor", L"magenta",                                              
			L"Suggested values: black, #ffb529 ", true);

		o[L"ElementDoubleClickLineWidth"] << Option(L"ElementDoubleClickLineWidth", 3, 1, 2000,                                      
			L"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o[L"ElementDoubleClickDistanceColor"] << Option(L"ElementDoubleClickDistanceColor", L"red",                                  
			L"Suggested values: black, #ffb529 ", true);

		o[L"ElementDoubleClickDistancWidth"] << Option(L"ElementDoubleClickDistancWidth", 3, 1, 2000,                                
			L"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o[L"ElementDoubleClickResultColor"] << Option(L"ElementDoubleClickResultColor", L"medium slate blue",                        
			L"Suggested values: black,  #ffb529 ", true);

		o[L"ElementDoubleClickResultWidth"] << Option(L"ElementDoubleClickResultWidth", 3, 1, 2000,                                  
			L"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o[L"ElementDimColor"] << Option(L"ElementDimColor", L"white",                                             
			L"Suggested values: #ffb529 ", true);

		o[L"ElementDimWidth"] << Option(L"ElementDimWidth", 1, 1, 2000,                                           
			L"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o[L"ElementHDLXColor"] << Option(L"ElementHDLXColor", L"green",                                           
			L"Suggested values:black ffb529", true);

		o[L"ElementHDLXWidth"] << Option(L"ElementHDLXWidth", 1, 1, 2000,                                         
			L"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o[L"ElementArrowSize"] << Option(L"ElementArrowSize",5.0f, 1.0f, 2000.0f,                                 
			L"Element Arrow Size display ", true);

		o[L"UserID"] << Option(L"UserID", 1, 1, INT_MAX,                                                            // 
			L"Current logged User iD ", false); 

		o[L"UserLogName"] << Option(L"UserLogName", L"NoUser",                                                      // �û���¼����
			L"UserLogName ", true);

		o[L"UserLogDateTime"] << Option(L"UserLogDateTime", L"20170816 08:00",                                       // �û���¼ʱ��
			L"UserLogDateTime ", true);

		o[L"ProdutctBatchNumber"] << Option(L"ProdutctBatchNumber", L"20170816",                                     // ��Ʒ����
			L"ProdutctBatchNumber", true);

		o[L"ProdutctExportStartNo"] << Option(L"ProdutctExportStartNo", 1, 1, INT_MAX,                             // ������EXCEL�Ĳ�Ʒ��ʼ��� 
			L"Produtct Export Start Number", true);

		o[L"ProdutctOKAutoSave"] << Option(L"ProdutctOKAutoSave", 1, 0, 1,                                       // ������EXCEL�Ĳ�Ʒ��ʼ��� 
			L"Produtct OK AutoSave", true);

		o[L"AutoMeasureDelayTime"] << Option(L"AutoMeasureDelayTime", 50, 20, 2000,                              // 
			L"Auto Measure Delay Time ms", true);

		o[L"������ʱ"] << Option(L"������ʱ", 600, 10, 5000,                                                      // 
			L"������ĵ�һ��ʱ����ʱ",
			true);

		o[L"��ʾԭͼ"] << Option(L"��ʾԭͼ", 0, 0, 1,                                                            // 
			L"�������ǲ�����ʾ�ɼ���ԭͼ",
			true);

		o[L"�����ɼ�Ч��ʹ��"] << Option(L"�����ɼ�Ч��ʹ��", 1, 0, 1,                                            // 
			L"�����ɼ�ͼ��, Ӧ�����Ч�����Ӵ��������ʾͼ��",
			true);

		o[L"Dummy"] << Option(L"Dummy", L"Dummy",                                                                  // ʹ�ñ����ͼƬ���ļ���
			L"Dummy why not show ", false);
		
	}	
		
	//  ֱ�� 
	void stElement::initOptionMapElemnetLine(OptionsMap& o)
	{
		
		// ���������
		o[L"ROI_Row1"] << Option(L"ROI_Row1", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			L"ROI_Row1", false);
		o[L"ROI_Column1"] << Option(L"ROI_Column1", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			L"ROI_Column1", false);
		o[L"ROI_Row2"] << Option(L"ROI_Row2", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			L"ROI_Row2", false);
		o[L"ROI_Column2"] << Option(L"ROI_Column2", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			L"ROI_Column2", false);

			
		
		o[L"Rakeֱ�߼�����"] << Option(L"Rake_Line_Elements", 10, 1, 1000,                                              // CreateShapeModel double AngleStart
		    L"Rake Line Elements number [1 20] default 10 ", true);

		o[L"Rakeֱ�߼��߶�"] << Option(L"Rake_Line_Detect_Height", 30.0f, 1.0f, 500.0f,                             // CreateShapeModel double AngleStart
			L"Rake_Line_Detect_Height [1.0f, 500.0f] default 30.0 ", true);

		o[L"Rakeֱ�߼����"] << Option(L"Rake_Line_Detect_Width", 15.0f, 1.0f, 500.0f,                                // CreateShapeModel double AngleStart
			L"Rake_Line_Detect_Height [1.0f, 500.0f] default 15.0 ", true);

		o[L"Rakeֱ�߼��Sigma"] << Option(L"Rake_Line_Detect_Sigma", 1.0f, 0.0f, 500.0f,                                // CreateShapeModel double AngleStart
			L"Rake_Line_Detect_Sigma [0.0f, 500.0f] default 1.0 ", true);

		o[L"Rakeֱ�߼����ֵ"] << Option(L"Rake_Line_Detect_Threshold", 20.0f, 1.0f, 500.0f,                        // CreateShapeModel double AngleStart
			L"Rake_Line_Detect_Threshold [1.0f, 500.0f] default 20.0 ", true);

		o[L"Rake_Line_Detect_Transition"] << Option(L"Rake_Line_Detect_Transition", L"all",                                    // Ԫ�ص������
			L"Rake_Line_Detect_Transition", true);

		o[L"Rake_Line_Detect_Select"] << Option(L"Rake_Line_Detect_Select", L"first",                                           // Ԫ�ص������
			L"Rake_Line_Detect_Select", true);

		o[L"ֱ�������С����"] << Option(L"pts_to_best_line_ActiveNum", 2, 2, 1000,                                 // CreateShapeModel double AngleStart
			L"pts_to_best_line_ActiveNum [2 1000] default 2 ", true);

		// �洢ֱ�ߵĲ�����Ϣ
		o[L"_RES_Line_Row1"] << Option(L"RES_Line_Row1", 0.0f, -FLT_MAX, FLT_MAX,                                               // CreateShapeModel double AngleStart
			L"RES_Line_Row1 [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"_RES_Line_Row2"] << Option(L"RES_Line_Row2", 0.0f, -FLT_MAX, FLT_MAX,                                               // CreateShapeModel double AngleStart
			L"RES_Line_Row2 [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"_RES_Line_Col1"] << Option(L"RES_Line_Col1", 0.0f, -FLT_MAX, FLT_MAX,                                                // CreateShapeModel double AngleStart
			L"RES_Line_Col1 [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"_RES_Line_Col2"] << Option(L"RES_Line_Col2", 0.0f, -FLT_MAX, FLT_MAX,                                               // CreateShapeModel double AngleStart
			L"RES_Line_Col2 [FLT_MIN, FLT_MAX] default 0.0 ", true);
	}

	//  Բ
	void stElement::initOptionMapElemnetCircle(OptionsMap&  o)
	{
		
		// ROI ����洢��Ϣ

		o[L"HTupleArcOrCircle"] << Option(L"HTupleArcOrCircle", 0,0,1,                            // ��Բ�����ǻ� 0: 
			L"HTupleArcOrCircle ��Բ�����ǻ� 0: Բ, 1: ��", true);

		o[L"ROI_RowC"] << Option(L"ROI_RowC", 0.0f, -FLT_MAX, FLT_MAX,                              // Բ������ Y
			L"hv_RowC [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"ROI_ColumnC"] << Option(L"ROI_ColumnC", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"hv_ColumnC [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"ROI_Radius"] << Option(L"ROI_Radius", 0.0f, -FLT_MAX, FLT_MAX,                           //
			L"hv_Radius [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"ROI_StartPhi"] << Option(L"ROI_StartPhi", 0.0f, -FLT_MAX, FLT_MAX,                        //
			L"hv_StartPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"ROI_EndPhi"] << Option(L"ROI_EndPhi", 0.0f, -FLT_MAX, FLT_MAX,                           //
			L"hv_EndPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"ROI_PointOrder"] << Option(L"ROI_PointOrder", L"",                                       // 
			L"hv_PointOrder [FLT_MIN, FLT_MAX] default 0.0 ", true);

		
		o[L"Spoke_ʮ�ֳߴ�"] << Option(L"Spoke_ʮ�ֳߴ�", 10, 1, 1000,                                             // CreateShapeModel double AngleStart
			L"Spoke_ʮ�ֳߴ� ��ʾ��С", true);

		// ���������Ϣ		
		o[L"Spoke_Circle_Direction"] << Option(L"Spoke_Circle_Direction", L"inner",                                             // Ԫ�صķ���
			L"Direction of ROI default inner, inner, outer", true);

		o[L"Spoke_Circle_Elements"] << Option(L"Spoke_Circle_Elements", 30, 3, 1000,                                             // CreateShapeModel double AngleStart
			L"Spoke Circle Elements number [1 20] default 10", true);

		o[L"Spoke_Circle_Detect_Height"] << Option(L"Spoke_Circle_Detect_Height", 50.0f, 1.0f, 500.0f,                            // CreateShapeModel double AngleStart
			L"Spoke Circle Elements number [1.0f, 500.0f] default 30.0 ", true);

		o[L"Spoke_Circle_Detect_Width"] << Option(L"Spoke_Circle_Detect_Width", 35.0f, 1.0f, 500.0f,                             // CreateShapeModel double AngleStart
			L"Spoke Circle Elements number [1.0f, 500.0f] default 15.0 ", true);

		o[L"Spoke_Circle_Detect_Sigma"] << Option(L"Spoke_Circle_Detect_Sigma", 1.0f, 0.0f, 500.0f,                                // CreateShapeModel double AngleStart
			L"Spoke_Circle_Detect_Sigma [0.0f, 500.0f] default 1.0 ", true);

		o[L"Spoke_Circle_Detect_Threshold"] << Option(L"Spoke_Circle_Detect_Threshold", 20.0f, 1.0f, 500.0f,                        // CreateShapeModel double AngleStart
			L"Spoke_Circle_Detect_Threshold [1.0f, 500.0f] default 20.0 ", true);

		o[L"Spoke_Circle_Detect_Transition"] << Option(L"Spoke_Circle_Detect_Transition", L"all",                                    
			L"Spoke_Circle_Detect_Transition", true);

		o[L"Spoke_Circle_Detect_Select"] << Option(L"Spoke_Circle_Detect_Select", L"first",                                          
			L"Spoke_Circle_Detect_Select", true);

		o[L"Circle_Fit_Min_Points_Num"] << Option(L"Circle_Fit_Min_Points_Num", 10, 5, 1000,                                         // ���Բ���ٵ���
			L"Circle_Fit_Min_Points_Num [2 1000] default 5 ", true);


		// �洢Բ�Ĳ�����Ϣ
		o[L"RES_Բ�뾶"] << Option(L"RES_Բ�뾶", 0.0f, -FLT_MAX, FLT_MAX,                                                         // Բ�뾶
			L"RES_Circle_Radius [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"RES_Բֱ��"] << Option(L"RES_Բֱ��", 0.0f, -FLT_MAX, FLT_MAX,                                                         // Բ�뾶
			L"RES_Circle_Diameter [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"RES_Circle_Center_Row"] << Option(L"RES_Circle_Center_Row", 0.0f, -FLT_MAX, FLT_MAX,                                   // Բ������ X
			L"RES_Circle_Row [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"RES_Circle_Center_Column"] << Option(L"RES_Circle_Center_Column", 0.0f, -FLT_MAX, FLT_MAX,                             // Բ������ Y
			L"RES_Circle_Column [FLT_MIN, FLT_MAX] default 0.0 ", true);
		

		o[L"RES_Circle_StartPhi"] << Option(L"RES_Circle_StartPhi", 0.0f, -FLT_MAX, FLT_MAX,                                        // Բ����ʼ��
			L"RES_Circle_StartPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"RES_Circle_EndPhi"] << Option(L"RES_Circle_EndPhi", 0.0f, -FLT_MAX, FLT_MAX,                                            // Բ�Ľ�����
			L"RES_Circle_EndPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"RES_Circle_Type"] << Option(L"RES_Circle_Type", L"",                                                                    // Բ�����ͣ�Բ����
			L"RES_Circle_Type: Circle, Arc ", true);

		o[L"RES_Circle_PointOrders"] << Option(L"RES_Circle_PointOrders", L"",                                                      // Ԫ�ص�Rows����
			L"RES_Circle_PointOrders: negative, positive ", true);
	}
	
	// ���
	void stElement::initOptionMapElemnetResult(OptionsMap& o)
	{
		o[L"���Ԫ������"] << Option(L"OUT_Elemnt_Name", L"",                                           //  Ԫ�ص�����
			L"���Ԫ������", true);
		
		o[L"���Ԫ�صĲ���"] << Option(L"OUT_Elemnt_Parament_Name", L"",                                 // Ԫ�صĲ�����
			L"���Ԫ�صĲ���", true);

		o[L"����Ĳ���ֵ"] << Option(L" OUT_Elemnt_Data", 0.0f, -FLT_MAX, FLT_MAX,                      // ����ֵ
			L"OUT_Elemnt_Parament_Data", true);

		o[L"����ı�׼ֵ"] << Option(L" OUT_Elemnt_Standard", 0.0f, -FLT_MAX, FLT_MAX,                      // ��׼ֵ
			L"OUT_Elemnt_Parament_Standard", true);

		o[L"������Ϲ���"] << Option(L"OUT_Elemnt_Upper_Tol", 0.5f, -FLT_MAX, FLT_MAX,                     // ����
			L"OUT_Elemnt_Upper_Tolerance", true);

		o[L"������¹���"] << Option(L"OUT_Elemnt_Lower_Tol", -0.5f, -FLT_MAX, FLT_MAX,                     // ����
			L"OUT_Elemnt_Lower_Tolerance", true);

		o[L"�����������"] << Option(L"OUT_Elemnt_Result", L"",                                        // ����, OK, �³��ޣ��ϳ���
			L"OUT_Elemnt_Result: OK, Lower, Upper", true);

		o[L"�����"] << Option(L"OUT_Elemnt_Is_Alarm", 0, 0, 1,                                      // �����ǲ��Ǳ���
			L"�����", true);
	}

	// �Ƕ�
	void stElement::initOptionMapElemnetAngle(OptionsMap & o)
	{
		
		o[L"USE_LINE1_Parament_Name"] << Option(L"USE_LINE1_Parament_Name", L"",                                 // ʹ��ֱ��1����
			L"USE_LINE1_Parament_Name", true);

		//o[L"USE_LINE1_Indentify"] << Option(L"USE_LINE1_Indentify", 0, 0, INT_MAX,                              // ʹ��ֱ��1 ID
		//	L"USE_LINE1_Indentify", true);

		o[L"USE_LINE2_Parament_Name"] << Option(L"USE_LINE2_Parament_Name", L"",                                 // ʹ��ֱ��2����
			L"USE_LINE2_Parament_Name", true);

		//o[L"USE_LINE2_Indentify"] << Option(L"USE_LINE2_Indentify", 0, 0, INT_MAX,                              // ʹ��ֱ��2 ID
		//	L"USE_LINE2_Indentify", true);

		o[L"RES_2LINE_Angle_Rad"] << Option(L"RES_2LINE_Angle_Rad", 0.0f, -FLT_MAX, FLT_MAX,                     // ����ֱ�ߵĽǶ�
			L"RES_2LINE_Angle_Rad [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"RES_�н�"] << Option(L"RES_�н�", 0.0f, -FLT_MAX, FLT_MAX,                                           // ����ֱ�ߵĽǶ�
			L"RES_2LINE_Angle_Deg [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"RES_Intersection_Row"] << Option(L"RES_Intersection_Row", 0.0f, -FLT_MAX, FLT_MAX,                   // ����ֱ�ߵĽ���� ROW
			L"RES_Intersection_Row ", true);

		o[L"RES_Intersection_Column"] << Option(L"RES_Intersection_Column", 0.0f, -FLT_MAX, FLT_MAX,             // ����ֱ�ߵĽ���� Column
			L"RES_Intersection_Column ", true);

		o[L"RES_Intersection_Points"] << Option(L"RES_Intersection_Points", L"",                                 // ����ֱ�ߵĽ���� Points
			L"RES_Intersection_Points", true);
	}

	// ���
	void stElement::initOptionMapElemnetCamera(OptionsMap & o)
	{
		(o)[L"��ע"] << Option(L"��ע", L"Camera Parameter group description",     // ���������˵��
			L"Camera Parameter group description");

		(o)[L"ͼ�������ʱ"] << Option(L"grab_timeout", 300, 50, 100000,                    // �ռ�һ��ͼ��ʱ���� ms
			L"grab_timeout [50, 100000] ms default 50ms",
			true);
		(o)[L"����ʱ��"] << Option(L"shutter", 20000, 50, 1000000,                           // ����ʱ�� us
			L"shutter time  [50, 1000000] ��s default 20000 20ms",
			true );
		(o)[L"�Աȶ�"] << Option(L"contrast", 100, 1, 200,                                // �Աȶ�
			L"LUT diagram contrast [1, 200] defalut 100",
			true );
		(o)[L"gamma"] << Option(L"gamma", 100, 1, 1000,                                     // gamma
			L"LUT diagram gamma [1,1000] default 100",
			true );
		(o)[L"����ģʽ"] << Option(L"exposure_mode", 0, 0, 1,                              // ���ⷽʽ
			L"exposure mode 0: manual, 1: automatic",
			true );
		(o)[L"֡����"] << Option(L"frame_speed", 0, 0, 2,                                  // ֡����
			L"frame speed 0: low speed, 1: mid speed, 2: high speed",
			true );
		(o)[L"anti_flick"] << Option(L"anti_flick", 0, 0, 2,                                // ������
			L"anti_flick 0��not use, 1: anti 50HZ, 2: anti 60HZ",
			false );
		(o)[L"resolution"] << Option(L"resolution", 0, 0, 255,                              // �ֱ���
			L"resolution [0, 255] default 0", false );
		(o)[L"resolution_mod"] << Option(L"resolution_mod", L"Normal",                       // �ֱ淽ʽ Normal BINSUM2X BINAVR2X Uknown
			L"resolution mod [Normal,BINSUM2X BINAVR2X Uknown] default Normal",
			false );
		(o)[L"offsetX"] << Option(L"offsetX", 0, 0, 2592,                                   // 
			L"offsetX [0, 2592] default 0",
			true );                             // 
		(o)[L"offsetY"] << Option(L"offsetY", 0, 0, 1944,
			L"offsetY [0, 1944] default 0",
			true );                             // 
		(o)[L"�������"] << Option(L"width", 2592, 100, INT_MAX,
			L"width [100,2592] pixel",
			true );                                 // 
		(o)[L"�����߶�"] << Option(L"height", 1944, 100, INT_MAX,
			L"height [100,1944] pixel",
			true );                                // 

		(o)[L"ɫ��"] << Option(L"color_temperature", 0, 0, 3,                                 // ɫ��
			L"color temperature 0: auto 1: D65 2: 5500K (pure white) 3: cloudy sky in door",
			true );
		(o)[L"��ɫ����"] << Option(L"red_gain", 1.00f, 0.10f, 4.0f,                             // ��ɫ����
			L"red gain [0.0, 3.99]",
			true );
		(o)[L"��ɫ����"] << Option(L"green_gain", 1.00f, 0.10f, 4.0f,                         // ��ɫ����
			L"green gain [0.0, 3.99]",
			true );
		(o)[L"��ɫ����"] << Option(L"blue_gain", 1.00f, 0.10f, 4.0f,                           // ��ɫ����
			L"blue gain [0.0, 3.99]",
			true );
		(o)[L"���Ͷ�"] << Option(L"saturation", 0, 0, 200,                                  // ���Ͷ�
			L"saturation [0, 200]",
			true );
		(o)[L"ͼ������"] << Option(L"gain", 1.25f, 1.25f, 50.0f,                                   // ����
			L"gain [1.25, 50.0]",
			true );
		(o)[L"ɫ�ʿռ�"] << Option(L"color_space", L"Gray",                                  // ɫ�ʿռ� BGR24
			L"color space [Gray, BGR24]",
			true );
		(o)[L"������ʱ"] << Option(L"������ʱ", 0, 0, 2000,                                  
			L"����ǰ��ʱ [0, 1200] ms", true);
	}

	// ����
	void stElement::initOptionMapElemnetDistance(OptionsMap & o)
	{
		
		//o[L"IsDisplayDimension"] << Option(L"IsDisplayDimension", 1, 0, 1,                                             // �Ƿ���ʾ�ߴ���ֵ
		//	L"IsDisplayDimension", false);

		
		o[L"����Ԫ������1"] << Option(L"USE_Distance1_Element_Name", L"",                                     // ʹ��ֱ��1����
			L"����Ԫ������1", true);

		o[L"����Ԫ��1������"] << Option(L" USE_Distance1_Parament_Name", L"",                                 // ʹ��ֱ��1����
			L"����Ԫ��1������", true);

		//o[L"����Ԫ��1 ID"] << Option(L" USE_Distance1_Indentify", 0, 0, INT_MAX,                              // ʹ��ֱ��1 ID
		//	L"����Ԫ��1 ID", true);

		o[L"����Ԫ������2"] << Option(L"USE_Distance2_Element_Name", L"",                                     // ʹ��ֱ��2����
			L"����Ԫ������2", true);
		
		o[L"����Ԫ��2������"] << Option(L"USE_Distance2_Parament_Name",L"",                                   // ʹ��ֱ��2����
			L"����Ԫ��2������", true);

		//o[L"����Ԫ��2 ID"] << Option(L"USE_Distance2_Indentify", 0, 0, INT_MAX,                               // ʹ��ֱ��2 ID
		//	L"����Ԫ��2 ID", true);

		// �������Ƕ�����
		o[L"����Row1"] << Option(L"����Row1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Row1", false);
		o[L"����Col1"] << Option(L"����Col1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Col1", false);

		o[L"����Row2"] << Option(L"����Row2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Row1", false);
		o[L"����Col2"] << Option(L"����Col2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Col2", false);

		o[L"�ߴ���ʾƫ��X"] << Option(L"�ߴ���ʾƫ��X", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"�ߴ���ʾƫ��X", true);
		o[L"�ߴ���ʾƫ��Y"] << Option(L"�ߴ���ʾƫ��Y", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"�ߴ���ʾƫ��Y", true);


		o[L"�ߴ��ͷ����"] << Option(L"�ߴ��ͷ����", 20.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			L"Arrow_HeadLength Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o[L"�ߴ��ͷ���"] << Option(L"�ߴ��ͷ���", 20.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			L"The width of the arrow head in pixels Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o[L"RES_����"] << Option(L"RES_Distance", 0.0f, -FLT_MAX, FLT_MAX,                                            // ����ֱ�ߵľ���
			L"RES_Distance [FLT_MIN, FLT_MAX] default 0.0 ", true);

	

	}

	// ����
	void stElement::initOptionMapFUNCTION(OptionsMap & o)
	{		

		//==========================================
		
		
		//==========================================
		o[L"USE_mean_image"] << Option(L"USE_mean_image", 1, 0, 1,                                              // FindShapeModleMinScore
			L"Smooth by averaging. ");

		(o)[L"mean_image_MaskWidth"] << Option(L"mean_image_MaskWidth", 9, 1, 501,                           // 
			L"Width of filter mask. Default value: 9 Suggested values: 3, 5, 7, 9, 11, 15, 23, 31, 43, 61, 101",
			true);

		(o)[L"mean_image_MaskHeight"] << Option(L"mean_image_MaskHeight", 9, 1, 501,                           // 
			L"Height of filter mask. Default value: 9 Suggested values: 3, 5, 7, 9, 11, 15, 23, 31, 43, 61, 101",
			true);


		//==========================================
		//o[L"USE_scale_image"] << Option(L"USE_scale_image", 0, 0, 1,                                          // FindShapeModleMinScore
		//	L"ʹ��ͼ��Ҷ����칦�� Scale the gray values of an image scale_image(Image : ImageScaled : Mult, Add : ) g1 := g * Mult + Add");

		//(o)[L"scale_image_Mult"] << Option(L"scale_image_Mult", 0.01f, 0.001f, 1.0f,                           // 
		//	L"Scale factor. Default value: 0.01 Suggested values: 0.001, 0.003, 0.005, 0.008, 0.01, 0.02, 0.03, 0.05, 0.08, 0.1, 0.5, 1.0",
		//	true);

		//(o)[L"scale_image_Add"] << Option(L"scale_image_Add", 50.0f, 0.01f, 500.0f,                           // 
		//	L"scale_image_Add. Default value: 0 Suggested values: 0, 10, 50, 100, 200, 500",
		//	true);
		//==========================================
		o[L"USE_equ_histo_image"] << Option(L"USE_equ_histo_image", 1, 0, 1,                                              // FindShapeModleMinScore
			L"Histogram linearisation of images The operator equ_histo_image primarily serves for optical processing of images for a human viewer. For example, the (local) contrast spreading can lead to a detection of fictitious edges.");
		

		//==========================================
		o[L"USE_emphasize"] << Option(L"USE_emphasize", 1, 0, 1,                                              // FindShapeModleMinScore
			L"ʹ��ͼ����ǿ���� Enhance contrast of the image. emphasize(Image : ImageEmphasize : MaskWidth, MaskHeight, Factor : ) res := round((orig - mean) * Factor) + orig");

		(o)[L"emphasize_MaskWidth"] << Option(L"emphasize_MaskWidth", 7, 3, 201,                           // 
			L"Width of low pass mask.efault value: 7 Suggested values: 3, 5, 7, 9, 11, 15, 21, 25, 31, 39  Typical range of values: 3 �� MaskWidth �� 201 Minimum increment: 2",
			true);
		
		(o)[L"emphasize_MaskHeight"] << Option(L"emphasize_MaskHeight", 7, 3, 201,                           // 
			L"Width of low pass mask.efault value: 7 Suggested values: 3, 5, 7, 9, 11, 15, 21, 25, 31, 39  Typical range of values: 3 �� MaskHeight �� 201 Minimum increment: 2",
			true);

		(o)[L"emphasize_Factor"] << Option(L"emphasize_Factor", 1.0f, 0.01f, 20.0f,                           // 
			L"Intensity of contrast emphasis. Default value: 1.0 Suggested values: 0.3, 0.5, 0.7, 1.0, 1.4, 1.8, 2.0 Minimum increment: 0.01",
			true);

		


		//==========================================
		o[L"USE_threshold"] << Option(L"USE_threshold", 0, 0, 1,                                              // FindShapeModleMinScore
			L"USE_threshold Segment an image using global threshold. threshold(Image : Region : MinGray, MaxGray : ) ");

		(o)[L"threshold_MinGray"] << Option(L"threshold_MinGray", 180.0f, 0.00f, 255.0f,                           // 
			L"Lower threshold for the gray values. Default value: 128.0 Suggested values: 0.0, 10.0, 30.0, 64.0, 128.0, 200.0, 220.0, 255.0",
			true);

		(o)[L"threshold_MaxGray"] << Option(L"threshold_MaxGray", 255.0f, 0.00f, 255.0f,                           // 
			L"Lower threshold for the gray values. Default value: 128.0 Suggested values: 0.0, 10.0, 30.0, 64.0, 128.0, 200.0, 220.0, 255.0",
			true);

		
		o[L"USE_dilation_circle"] << Option(L"USE_dilation_circle", 0, 0, 1,                                              // FindShapeModleMinScore
			L"USE_dilation_circle");
		(o)[L"dilation_circle_radius"] << Option(L"dilation_circle_radius", 3.5f, 1.50f, 511.0f,                           // 
			L"Radius of the circular structuring element.. efault value: 3.5 uggested values: 1.5, 2.5, 3.5, 4.5, 5.5, 7.5, 9.5, 12.5, 15.5, 19.5, 25.5, 33.5, 45.5, 60.5, 110.5",
			true);

		o[L"USE_fill_up"] << Option(L"USE_fill_up", 0, 0, 1,                                              // FindShapeModleMinScore
			L"USE_fill_up fill_up fills up holes in regions. The number of regions remains unchanged. The neighborhood type is set via set_system(neighborhood,<4/8>) (default: 8-neighborhood).");

		o[L"USE_erosion_circle"] << Option(L"USE_erosion_circle", 0, 0, 1,                                              // FindShapeModleMinScore
			L"USE_erosion_circle");
		(o)[L"erosion_circle_radius"] << Option(L"erosion_circle_radius", 3.5f, 1.50f, 511.0f,                           // 
			L"Radius of the circular structuring element.. efault value: 3.5 uggested values: 1.5, 2.5, 3.5, 4.5, 5.5, 7.5, 9.5, 12.5, 15.5, 19.5, 25.5, 33.5, 45.5, 60.5, 110.5",
			true);


		//==========================================
		o[L"USE_reduce_domain"] << Option(L"USE_reduce_domain", 0, 0, 1,                                      // 
			L"USE_reduce_domain reduce_domain(Image, Region : ImageReduced : : )");

		o[L"USE_reduce_domain_Gray"] << Option(L"USE_reduce_domain_Gray", 0, 0, 1,                           // 
			L"USE_reduce_domain_Gray image to replace the product to measure dimension");
		
		
	}

	// ƥ��Ԫ��
	void stElement::initOptionMapMATCH(OptionsMap & o)
	{
		
		o[L"CreateShapeModleNumLevels"] << Option(L"NumLevels", L"auto",                                              // FindShapeModleMinScore
			L"FindShapeModle NumLevels Default value: auto  List of values: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 auto");
		
		o[L"CreateShapeModleAngleStart"] << Option(L"AngleStart", -0.39f, -3.14f, 3.14f,                             // CreateShapeModel double AngleStart
			L"CreateShapeModle double AngleStart RAD Default value: -0.39 Suggested values: -3.14, -1.57, -0.78, -0.39, -0.20, 0.0 ");
		
		o[L"CreateShapeModleAngleExtend"] << Option(L"AngleExtend", 0.78f, 0.0f, 6.29f,                               // CreateShapeModel double AngleExtend
			L"CreateShapeModle double AngleExtend Default value: 0.78 Suggested values: 6.29, 3.14, 1.57, 0.78, 0.39, 0.0 ");

		o[L"CreateShapeModleAngleStep"] << Option(L"AngleStep", L"auto",                                               // CreateShapeModel double AngleExtend
			L"CreateShapeModle double AngleStep Default value: auto Suggested values: 0.0175, 0.0349, 0.0524, 0.0698, 0.0873 Restriction: (AngleStep >= 0) && (AngleStep <= (pi / 16))");

		o[L"CreateShapeModleOptimization"] << Option(L"Optimization", L"auto",                                         // FindShapeModleMinScore
			L"Kind of optimization and optionally method used for generating the model. List of values: auto, no_pregeneration, none, point_reduction_high, point_reduction_low, point_reduction_medium, pregeneration");

		o[L"CreateShapeModleMetric"] << Option(L"Metric", L"use_polarity",                                         // FindShapeModleMinScore
			L"Match metric. Default value: use_polarityList of values: ignore_color_polarity, ignore_global_polarity, ignore_local_polarity, use_polarity");

		o[L"CreateShapeModleContrast"] << Option(L"Contrast", L"auto",                                         // FindShapeModleMinScore
			L"Threshold or hysteresis thresholds for the contrast of the object in the template image and optionally minimum size of the object parts. Default value: auto Suggested values: auto, auto_contrast, auto_contrast_hyst, auto_min_size, 10, 20, 30, 40, 60, 80, 100, 120, 140, 160");
		
		o[L"CreateShapeModleMinContrast"] << Option(L"MinContrast", L"auto",                                         // FindShapeModleMinScore
			L"Minimum contrast of the objects in the search images. Default value: auto Suggested values: auto, 1, 2, 3, 5, 7, 10, 20, 30, 40 estriction: MinContrast < Contrast");


		o[L"FindShapeModleAngleStart"] << Option(L"AngleStart", -0.39f, -3.14f, 3.14f,                             // CreateShapeModel double AngleStart
			L"FindShapeModle double AngleStart RAD Default value: -0.39 Suggested values: -3.14, -1.57, -0.78, -0.39, -0.20, 0.0 ");

		o[L"FindShapeModleAngleExtend"] << Option(L"AngleExtend", 0.78f, 0.0f, 6.29f,                               // CreateShapeModel double AngleExtend
			L"FindShapeModle double AngleExtend Default value: 0.78 Suggested values: 6.29, 3.14, 1.57, 0.78, 0.39, 0.0 ");

		o[L"FindShapeModleMinScore"] << Option(L"MinScore", 0.85f, 0.30f, 1.0f,                                      // FindShapeModleMinScore
			L"FindShapeModle MinScore  Default value: 0.5 Suggested values: 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0");

		o[L"FindShapeModleNumMatches"] << Option(L"NumMatches", 1, 0, 20,                                              // FindShapeModleMinScore
			L"FindShapeModleNumMatches  Default value: 1 Suggested values: 0, 1, 2, 3, 4, 5, 10, 20");

		o[L"FindShapeModleMaxOverlap"] << Option(L"MaxOverlap", 0.5f, 0.0f, 1.00f,                                      // FindShapeModleMinScore
			L"FindShapeModle MaxOverlap double  Default value: 0.5 Suggested values: 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0");

		o[L"FindShapeModleSubPixel"] << Option(L"SubPixel", L"least_squares",                                         // FindShapeModleMinScore
			L"FindShapeModle least_squares Default value: least_squares List of values: interpolation, least_squares, least_squares_high, least_squares_very_high, max_deformation 1, max_deformation 2, max_deformation 3, max_deformation 4, max_deformation 5, max_deformation 6, none");

		o[L"FindShapeModleNumLevels"] << Option(L"NumLevels", 0, 0, 10,                                              // FindShapeModleMinScore
			L"FindShapeModle NumLevels Default value: 0  List of values: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10");

		o[L"FindShapeModleGreediness"] << Option(L"Greediness", 0.9f, 0.0f, 1.0f,                                              // FindShapeModleMinScore
			L"FindShapeModle Default value: 0.9 Suggested values: 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 Greediness of the search heuristic (0: safe but slow; 1: fast but matches may be missed).");
		
		o[L"DispShapeColor"] << Option(L"DispShapeColor", L"red",                                         // FindShapeModleMinScore
			L"dev_display_shape_matching_results color");

		o[L"RES_FindShapeRow"] << Option(L"RES_FindShapeRow", L"",                                         // FindShapeModleMinScore
			L"RES_FindShapeRow");

		o[L"RES_FindShapeColumn"] << Option(L"RES_FindShapeColumn", L"",                                         // FindShapeModleMinScore
			L"RES_FindShapeColumn");

		o[L"RES_FindShapeAngle"] << Option(L"RES_FindShapeAngle", L"",                                         // FindShapeModleMinScore
			L"RES_FindShapeAngle");

		o[L"RES_FindShapeScore"] << Option(L"RES_FindShapScore", L"",                                         // FindShapeModleMinScore
			L"RES_FindShapeScore");

		o[L"RES_FindShapeCenterRow"] << Option(L"RES_FindShapeCenterRow", L"",                                         // FindShapeModleMinScore
			L"RES_FindShapeCenterRow");

		o[L"RES_FindShapeCenterColumn"] << Option(L"RES_FindShapeCenterColumn", L"",                                         // FindShapeModleMinScore
			L"RES_FindShapeCenterColumn");

	}

	// У�� mm 
	void stElement::initOptionMapElemnetMM(OptionsMap & o)
	{
		o[L"У��Ԫ�� X"] << Option(L"OUT_Elemnt_NameX", L"",                                                 //  Ԫ�ص�����
			L"У��Ԫ�� X", true);

		//o[L"У��Ԫ�صĲ��� X"] << Option(L"OUT_Elemnt_Parament_NameX", L"",                               // Ԫ�صĲ�����
		//	L"���Ԫ�صĲ���", true);

		//o[L"У��Ԫ�ص�ID X"] << Option(L"OUT_Elemnt_IndentifyX", 0, 0, INT_MAX,                             // ���Ԫ�ص�Ψһ���
		//	L"У��Ԫ�ص�ID X", true);

		//o[L"У��Ԫ�صĲ���ֵ X"] << Option(L"OUT_Elemnt_DataX", 0.0f, -FLT_MAX, FLT_MAX,                             // ����ֵ
		//	L"OUT_Elemnt_Parament_DataX", true);

		//o[L"У��Ԫ�صı�׼ֵ X"] << Option(L"OUT_Elemnt_StandardX", 0.0f, -FLT_MAX, FLT_MAX,                     // ��׼ֵ
		//	L"OUT_Elemnt_Parament_StandardX", true);

		o[L"У��Ԫ�� Y"] << Option(L" OUT_Elemnt_NameY", L"",                                                 //  Ԫ�ص�����
			L"У��Ԫ�� Y", true);

		//o[L"У��Ԫ�صĲ��� Y"] << Option(L"OUT_Elemnt_Parament_NameY", L"",                               // Ԫ�صĲ�����
		//	L"���Ԫ�صĲ���", true);

		//o[L"У��Ԫ�ص�ID Y"] << Option(L"OUT_Elemnt_IndentifyY", 0, 0, INT_MAX,                            // ���Ԫ�ص�Ψһ���
		//	L"У��Ԫ�ص�ID Y", true);

		//o[L"У��Ԫ�صĲ���ֵ Y"] << Option(L"OUT_Elemnt_DataY", 0.0f, -FLT_MAX, FLT_MAX,                            // ����ֵ
		//	L"OUT_Elemnt_Parament_DataY", true);

		//o[L"У��Ԫ�صı�׼ֵ Y"] << Option(L"OUT_Elemnt_StandardY", 0.0f, -FLT_MAX, FLT_MAX,                    // ��׼ֵ
		//	L"OUT_Elemnt_Parament_StandardY", true);
	}

	// ����Ԫ��
	void stElement::initOptionMapElemnetPeek(OptionsMap & o)
	{
		o[L"Peak_Rectangle2_Row"] << Option(L"Peak_Rectangle2_Row", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Peak_Rectangle2_Colum"] << Option(L"Peak_Rectangle2_Colum", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak Rectangle2 Colum  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Peak_Rectangle2_Phi"] << Option(L"Peak_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak Rectangle2 Phi  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Peak_Rectangle2_Length1"] << Option(L"Peak_Rectangle2_Length1", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak Rectangle2 Length1  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Peak_Rectangle2_Length2"] << Option(L"Peak_Rectangle2_Length2", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak Rectangle2 Length2  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Peak_DetectWidth"] << Option(L"Peak_DetectWidth", 2.0f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak_DetectWidth  [FLT_MIN, FLT_MAX] default 2.0 ", true);

		o[L"Peak_Sigma"] << Option(L"Peak_Sigma", 0.41f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak_Sigma [FLT_MIN, FLT_MAX] default 0.41 ", true);

		o[L"Peak_Threshold"] << Option(L"Peak_Threshold", 20.0f, -FLT_MAX, FLT_MAX,                                        // 
			L"Peak_Threshold[FLT_MIN, FLT_MAX] default 20.0 ", true);


		// �洢���,��������ж��
		o[L"Peak_cross_size"] << Option(L"Peak_cross_size", 20.0f, -FLT_MAX, FLT_MAX,
			L"����ʮ���ߵĳߴ�", true);

		o[L"Peak_cross_angle"] << Option(L"Peak_cross_angle", 0.785398f, -FLT_MAX, FLT_MAX,
			L"����ʮ���ߵĽǶ� Default value: 0.785398 Suggested values: 0.0, 0.785398", true);

		// �洢���,��������ж��
		o[L"Peak_Transition"] << Option(L"Peak_Transition", L"positive",                                                     
			L"Peak ransition negative: �ڶ��� positive���׶���", true);

		o[L"Peak_Select"] << Option(L"Peak_Select", L"first",
			L"Peak Select", true);
		

		// �洢���,��������ж��
		//o[L"nRES_hv_EdgesY"] << Option(L"RES_hv_EdgesY", "",                                                      // Ԫ�صĽ�����б�Ե
		//	"RES_hv EdgesY ", true);

		//o[L"nRES_hv_EdgesX"] << Option(L"RES_hv_EdgesX", "",                                                      // Ԫ�صĽ������
		//	"RES_hv EdgesX ", true);

		o[L"RES_hv_ResultRow"] << Option(L"RES_hv_ResultRow", L"",                                                  // Ԫ�صĽ������
			L"RES_hv ResultRow ", true);

		o[L"RES_hv_ResultColumn"] << Option(L"RES_hv_ResultColumn", L"",                                            // Ԫ�صĽ������
			L"RES_hv ResultColumn ", true);

	}

	// һά����
	void stElement::initOptionMapELEMENT_MEASURE_RECTANGLE2(OptionsMap & o)
	{
		// ����
		
		o[L"Measure_Rectangle2_Row"] << Option(L"Measure_Rectangle2_Row", 20.0f, 0.0f, FLT_MAX,                  // Row coordinate of the center of the rectangle.
			L"Measure Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Measure_Rectangle2_Column"] << Option(L"Measure_Rectangle2_Column", 20.0f, 0.0f, FLT_MAX,           // Column coordinate of the center of the rectangle.
			L"Measure Rectangle2 Column  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Measure_Rectangle2_Phi"] << Option(L"Measure_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Measure Rectangle2 Phi  Typical range of values: -1.178097 �� Phi �� 1.178097 (lin)", true);

		o[L"Measure_Rectangle2_Length1"] << Option(L"Measure_Rectangle2_Length1", 20.0f, 1.0f, FLT_MAX,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Measure Rectangle2 Length1  Restriction: Length1 >= 1.0", true);

		o[L"Measure_Rectangle2_Length2"] << Option(L"Measure_Rectangle2_Length2", 10.0f, 1.0f, FLT_MAX,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Measure Rectangle2 Length2  Restriction: Length1 >= 1.0", true);

		o[L"Measure_Rectangle2_Interpolation"] << Option(L"Measure_Rectangle2_Interpolation", L"bilinear",            // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Measure Rectangle2 Interpolation  bicubic, bilinear, nearest_neighbor", true);

		// ���
		// measure_pairs ����
		o[L"measure_pairs_Sigma"] << Option(L"measure_pairs_Sigma", 1.0f, 0.4f, 100.0f,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Sigma of gaussian smoothing.Restriction: Sigma >= 0.4 ", true);
		
		o[L"measure_pairs_Threshold"] << Option(L"measure_pairs_Threshold", 30.0f, 1.0f, 255.0f,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Minimum edge amplitude.Default value: 30.0 Suggested values: 5.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 90.0, 110.0  ", true);
		
		o[L"measure_pairs_Transition"] << Option(L"measure_pairs_Transition", L"all",            // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Type of gray value transition that determines how edges are grouped to edge pairs. List of values: all, all_strongest, negative, negative_strongest, positive, positive_strongest", true);
	
		o[L"measure_pairs_Select"] << Option(L"measure_pairs_Select", L"all",            // Angle of longitudinal axis of the rectangle to horizontal (radians).
			L"Selection of edge pairs. Default value: all. List of values: all, first, last", true);

		// ���
		o[L"measure_pairs_RowEdgeFirst"] << Option(L"measure_pairs_RowEdgeFirst", L"",            
			L"Row coordinate of the center of the first edge.", true);

		o[L"measure_pairs_ColumnEdgeFirst"] << Option(L"measure_pairs_ColumnEdgeFirst", L"",      
			L"Column coordinate of the center of the first edge.", true);

		o[L"measure_pairs_AmplitudeFirst"] << Option(L"measure_pairs_AmplitudeFirst", L"",        
			L"Edge amplitude of the first edge (with sign).", true);

		o[L"measure_pairs_RowEdgeSecond"] << Option(L"measure_pairs_RowEdgeSecond", L"",
			L"Row coordinate of the center of the second edge.", true);

		o[L"measure_pairs_ColumnEdgeSecond"] << Option(L"measure_pairs_ColumnEdgeSecond", L"",
			L"Column coordinate of the center of the second edge.", true);

		o[L"measure_pairs_AmplitudeSecond"] << Option(L"measure_pairs_AmplitudeSecond", L"",
			L"Edge amplitude of the second edge (with sign).", true);

		o[L"RES_measure_pairs_IntraDistance"] << Option(L"RES_measure_pairs_IntraDistance", L"",
			L"Distance between edges of an edge pair.", true);

		o[L"RES_measure_pairs_InterDistance"] << Option(L"RES_measure_pairs_InterDistance", L"",
			L"Distance between consecutive edge pairs.", true);

		o[L"RES_һά�ܳ�"] << Option(L"RES_һά�ܳ�", 0.0f, -FLT_MAX, FLT_MAX,
			L"RES_measure_pairs_Balloon.", true);

		o[L"�ߴ���ʾƫ��X"] << Option(L"�ߴ���ʾƫ��X", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"�ߴ���ʾƫ��X", true);
		o[L"�ߴ���ʾƫ��Y"] << Option(L"�ߴ���ʾƫ��Y", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"�ߴ���ʾƫ��Y", true);
	}

	// ����ȱ�� 
	void stElement::initOptionMapBalloonFlaw(OptionsMap & o)
	{
		// ����
		o[L"Flaw_Rectangle2_Row"] << Option(L"Flaw_Rectangle2_Row", 400.0f, 0.0f, FLT_MAX,                 
			L"Flaw Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Flaw_Rectangle2_Column"] << Option(L"Flaw_Rectangle2_Column", 400.0f, 0.0f, FLT_MAX,           
			L"Flaw Rectangle2 Column  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"Flaw_Rectangle2_Phi"] << Option(L"Flaw_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,              
			L"Typical range of values: -1.178097 �� Phi �� 1.178097 (lin)", true);

		o[L"Flaw_Rectangle2_Length1"] << Option(L"Flaw_Rectangle2_Length1", 400.0f, 10.0f, FLT_MAX,         
			L"Restriction: Length1 >= 1.0", true);

		o[L"Flaw_Rectangle2_Length2"] << Option(L"Flaw_Rectangle2_Length2", 300.0f, 10.0f, FLT_MAX,         
			L"Restriction: Length1 >= 1.0", true);

		// mean_image ��ֵ
		o[L"Flaw_mean_image_MaskWidth"] << Option(L"Flaw_mean_image_MaskWidth", 43, 1, 501,
			L"Typical, Default value: 9, range of values: 1 �� MaskHeight �� 501", true);

		o[L"Flaw_mean_image_MaskHeight"] << Option(L"Flaw_mean_image_MaskHeight", 43, 1, 501,
			L"Typical, Default value: 9, range of values: 1 �� MaskHeight �� 501", true);

		// dyn_threshold ��̬��ֵ
		o[L"Flaw_dyn_threshold_Offset"] << Option(L"Flaw_dyn_threshold_Offset", 50.0, -255.0f, 255.0f,
			L"Default value: 5.0, Suggested values: 1.0, 3.0, 5.0, 7.0, 10.0, 20.0, 30.0 Typical range of values: -255.0 �� Offset �� 255.0 (lin) ", true);

		o[L"Flaw_dyn_threshold_LightDark"] << Option(L"Flaw_dyn_threshold_LightDark", L"dark",
			L"Extract light, dark or similar areas? Default value: light, List of values: dark, equal, light, not_equal  ", true);

		// closing_circle
		o[L"Flaw_closing_circle_Radius"] << Option(L"Flaw_closing_circle_Radius", 3.5f, 0.5f, 511.0f,
			L"Radius of the circular structuring element.  Default value: 3.5 Suggested values: 1.5, 2.5, 3.5, 4.5, 5.5, 7.5, 9.5, 12.5, 15.5, 19.5, 25.5, 33.5, 45.5, 60.5, 110.5", true);

		// select_shape
		o[L"Flaw_select_shape_Features"] << Option(L"Flaw_select_shape_Features", L"area",
			L"Shape features to be checked. Default value: area, List of values: anisometry, area, area_holes, bulkiness, circularity, column, column1, column2, compactness, connect_num, contlength, convexity, dist_deviation, dist_mean, euler_number, height, holes_num, inner_height, inner_radius, inner_width, max_diameter, moments_i1, ", true);
		
		o[L"Flaw_select_shape_Operation"] << Option(L"Flaw_select_shape_Operation", L"and",
			L"Linkage type of the individual features.Default value: and, List of values: and, or ", true);


		o[L"Flaw_select_shape_Min"] << Option(L"Flaw_select_shape_Min", 15.0f, 1.0f, FLT_MAX,
			L"Lower limits of the features or min.", true);

		o[L"Flaw_select_shape_Max"] << Option(L"Flaw_select_shape_Max", 99999.0f, 1.0f, FLT_MAX,
			L"Lower limits of the features or max.", true);


		o[L"RES_ȱ������"] << Option(L"RES_ȱ������", 0.0f, 0.0f, FLT_MAX,
			L"�����RES_ȱ������", true);

		o[L"RES_ȱ��������"] << Option(L"RES_ȱ��������", 0.0f, 0.0f, FLT_MAX,
			L"RES_ȱ��������", true);

		o[L"RES_ȱ����С���"] << Option(L"RES_ȱ����С���", 0.0f, 0.0f, FLT_MAX,
			L"RES_ȱ����С���", true);
	}

	// ROI
	void stElement::initOptionMapROI(OptionsMap & o)
	{
		
		// ��У��������ROI
		o[L"USE_map_image"] << Option(L"USE_map_image", 1, 0, 1,                               // �����ȴ�����У��
			L"ʹ�������У����Ϣ��ͼ����л���У��.");
		
		// ����
		o[L"ROI_Rectangle2_Row"] << Option(L"ROI_Rectangle2_Row", 400.0f, 0.0f, FLT_MAX,
			L"ROI Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"ROI_Rectangle2_Column"] << Option(L"ROI_Rectangle2_Column", 400.0f, 0.0f, FLT_MAX,
			L"ROI Rectangle2 Column  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o[L"ROI_Rectangle2_Phi"] << Option(L"ROI_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,
			L"Typical range of values: -1.178097 �� Phi �� 1.178097 (lin)", true);

		o[L"ROI_Rectangle2_Length1"] << Option(L"ROI_Rectangle2_Length1", 200.0f, 1.0f, FLT_MAX,
			L"Restriction: Length1 >= 1.0", true);

		o[L"ROI_Rectangle2_Length2"] << Option(L"ROI_Rectangle2_Length2", 200.0f, 1.0f, FLT_MAX,
			L"Restriction: Length1 >= 1.0", true);
	}

	// ���ҳ���
	void stElement::initOptionMapBalloonLength(OptionsMap & o)
	{
		o[L"IsDisplayDimension"] << Option(L"IsDisplayDimension", 1, 0, 1,                                     // �Ƿ���ʾ�ߴ���ֵ
			L"IsDisplayDimension", false);

		o[L"���ö���1"] << Option(L"���ö���1", L"",                                  // ʹ�ö���1����
			L"���ö���1", true);

		//o[L"USE_Peek1_Indentify"] << Option(L"USE_Peek1_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���1 ID
		//	L"USE_Peek1_Indentify", true);
		
		o[L"���ö���2"] << Option(L"���ö���2", L"",                                  // ʹ�ö���1����
			L"���ö���2", true);

		//o[L"USE_Peek2_Indentify"] << Option(L"USE_Peek2_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���1 ID
		//	L"USE_Peek2_Indentify", true);

		o[L"���ö���3"] << Option(L"���ö���3", L"",                                  // ʹ�ö���1����
			L"���ö���3", true);

		//o[L"USE_Peek3_Indentify"] << Option(L"USE_Peek3_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���1 ID
		//	L"USE_Peek3_Indentify", true);

		o[L"���ö���4"] << Option(L"���ö���4", L"",                                  // ʹ�ö���4����
			L"���ö���4", true);

		//o[L"USE_Peek4_Indentify"] << Option(L"USE_Peek4_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���4 ID
		//	L"USE_Peek4_Indentify", true);

		o[L"�ߴ���ʾƫ��X"] << Option(L"�ߴ���ʾƫ��X", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"�ߴ���ʾƫ��X", true);
		o[L"�ߴ���ʾƫ��Y"] << Option(L"�ߴ���ʾƫ��Y", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"�ߴ���ʾƫ��Y", true);

		o[L"Balloon����Row1"] << Option(L"����Row1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Row1", false);
		o[L"Balloon����Col1"] << Option(L"����Col1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Col1", false);

		o[L"Balloon����Row2"] << Option(L"����Row2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Row1", false);
		o[L"Balloon����Col2"] << Option(L"����Col2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			L"����Col2", false);

		o[L"�ߴ��ͷ����"] << Option(L"�ߴ��ͷ����", 10.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			L"Arrow_HeadLength Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o[L"�ߴ��ͷ���"] << Option(L"�ߴ��ͷ���", 10.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			L"The width of the arrow head in pixels Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o[L"RES_���ҳ���"] << Option(L"RES_Balloon_Length", 0.0f, -FLT_MAX, FLT_MAX,                            // ���ҵĳ���
			L"RES_Balloon_Length [FLT_MIN, FLT_MAX] default 0.0 ", true);

	}

	////////////////////////////////////////////////////////////////////////////////////
	// ��ʼ��һ��Ԫ�ر���
	OptionsMap* stElement::initOptionMapElemnet(eELEMENT_TYPE t, int iPID)
	{
		
		OptionsMap o;
		Option::insert_order = 0;

		
		// ����Ψһ��ţ�ֻ��Ψһ����
		CString name = L""; 
		for (int i = 1; i < INT_MAX; i++) {
			name.Format(L"%s %d", EleName[t], i);
			if (!UCI::ELE.isHaveSaveElementName(name)) {
				break;
			}
		}

		//int index = (int)ELE.EleList.GetSize() + 1;

		// ��ʼ��һЩ��������                                                   
		o[L"��Ŀ���"] << Option(L"��Ŀ���", iPID, 1, INT_MAX,                         // ��ǰ�����������Ŀ���
			L"Current Project ID", false);

		o[L"Ԫ������"] << Option(L"Ԫ������", t, 0, ELEMNET_END - 1,                    // ��ǰ����Ԫ������
			L"Current Elemnet Type",false);
		
		o[L"Ԫ������"] << Option(L"Ԫ������", name,                                     // Ԫ�ص�����
			L"Element Name", true);	
		

		o[L"Ԫ����ƥ��"] << Option(L"Ԫ����ƥ��", 0, 0, 1,                           // Ԫ�ص����ҵ�
			L"Element is Finded", false);

		switch (t) {
		case ELEMENT_LINE:      // ֱ��
			initOptionMapElemnetLine(o);
			break;
		case ELEMENT_CIRCLE:    // Բ 
			initOptionMapElemnetCircle(o);
			break;

		case ELEMENT_ELLIPSE:   // ��Բ

			break;
		case ELEMENT_ANGLE:     // �Ƕ�
			initOptionMapElemnetAngle(o);
			break;
		case ELEMENT_RESULT:    // ������
		{
			o[L"����������"] << Option(L"����������", name,             // Ҫ�����Ԫ�ص�����
				L"���������ʱ��������ʾ������", true);
		}

			initOptionMapElemnetResult(o);	
			break;

		case ELEMENT_MM:
			initOptionMapElemnetMM(o);
			break;
		case ELEMENT_DISTANCE:
			initOptionMapElemnetDistance(o);
			break;
		case ELEMENT_CAMERA:
			initOptionMapElemnetCamera(o);
			break;
		case ELEMENT_PEAK:
			initOptionMapElemnetPeek(o);
			break;
		case ELEMENT_BALLOONLENGTH:
			initOptionMapBalloonLength(o);
			break;
		case ELEMENT_FUNCTION:
			initOptionMapFUNCTION(o);
			break;
		case ELEMENT_MATCH:
			initOptionMapMATCH(o);
			break;
		case ELEMENT_MEASURE_RECTANGLE2:
			initOptionMapELEMENT_MEASURE_RECTANGLE2(o);
			break;
		case ELEMENT_BALLOON_FLAW:
			initOptionMapBalloonFlaw(o);
			break;
		case ELEMENT_ROI:
			initOptionMapROI(o);
			break;
		default:			
			break;
		}

		o[L"������"] << Option(L"������", L"none", L"Command Macro", true);                           // Ԫ�ص������
		o[L"Ԫ�ر�ע"] << Option(L"Ԫ�ر�ע", L"note", L"Element Note", true);                        // Ԫ�صı�ע
		
		ELE.EleList.AddTail(o);
		return &ELE.EleList.GetTail();
	}

	
}




