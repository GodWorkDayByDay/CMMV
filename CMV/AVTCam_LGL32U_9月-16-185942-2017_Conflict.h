#pragma once

#include "Resource.h"
#include "MyCamera.h"

//#define WM_RECEIVE WM_USER+100
#define USE_WINDOWS_CAMERA_HALCON




#ifdef USE_WINDOWS_CAMERA_HALCON

class AVTCam {

	
public:	

	AVTCam(CWnd* PWnd, MYFRAME* pF);   //
	~AVTCam();

	//--------------------------------------------------------------------------------
	// ����

	

	//HalconCpp::HObject* get_hv_image();

	void set_busy(bool b);
	bool get_busy();

	int get_ImageHeight();
	void set_ImageHeight(int height);	

	int get_ImageWidth();
	void set_ImageWidth(int width);

	int get_camera_index();

	CWnd* get_parent();
	CCamera* get_camera();

	bool IsCaliPictureOk();                                            // ��ǰ�궨ͼ�������ǲ��Ǻϸ�
	
	CString GetCamerModel();                                           // �õ�����ı궨ģ�ͣ�
	CString GetMoudlePath();                                           // �õ���ǰ������е�Ŀ¼
	CString GetCaliInnerPicPath();                                     // �õ��궨 inner ͼƬ��Ŀ¼
	CString GetCaliPosePicPath();                                      // �õ��궨 pose ͼƬ��Ŀ¼
	CString GetCalibCcdPathFile();      
	CString GetCaliCalPathFile();                                      // camera inner parameters
	CString GetCaliPosePathFile();

	bool Write_CalibrationCcd(HalconCpp::HTuple hv_CalibHandle);
	bool Read_CalibrationCcd(HalconCpp::HTuple &hv_CalibHandle);

	bool Write_CalibrationCcd(HalconCpp::HTuple hv_CalibHandle, CString Path);
	bool Read_CalibrationCcd(HalconCpp::HTuple &hv_CalibHandle, CString Path);

	bool Write_CaliPose(HalconCpp::HTuple CameraPose); // write the internal camera parameters
	bool Read_CaliPose(HalconCpp::HTuple &CameraPose);
	bool Write_CaliPose(HalconCpp::HTuple CameraPose, CString Path); // write the internal camera parameters
	bool Read_CaliPose(HalconCpp::HTuple &CameraPose, CString Path);

	bool Write_CaliCal(HalconCpp::HTuple CameraParam); // write the internal camera parameters
	bool Read_CaliCal(HalconCpp::HTuple &CameraParam);
	bool Write_CaliCal(HalconCpp::HTuple CameraParam, CString Path); // write the internal camera parameters
	bool Read_CaliCal(HalconCpp::HTuple &CameraParam, CString Path);

	CString GetDescrFileName();

	double GetFocus();                                                 // �õ���ǰ�ľ�ͷ�Ľ���
	double GetFsx();                                                   // �õ����صĿ��
	double GetFsy();                                                   // �õ����صĿ��
	double GetdOnePicMaxError();                                       // �õ���ǰͼ�������������
	double GetCaliPlateThick();                                        // 
	double GetRectificationWidth();

	void SetbOnePIcMaxErrorOK(bool b);

	void SetfError(double f);

	int GetTotalCaliPicture();

	bool open_camera();
	bool close_camera();

	//--------------------------------------------------------------------------------
	



	bool Snap(void);                             // �����ɼ�
	bool Grab(void);                             // �����ɼ�
	bool Freeze(void);                           // ֹͣ�����ɼ�
	bool SetTriggerMode(bool bEnable, int nSrc = 7);
	// ���ô���ģʽ Mode = 1 �ҿ������� Mode = 0 �رմ���
	// ���ô���Դ nSrc = 1 �ⴥ�� nSrc = 7 ����
	bool SetShutter(int iValue);                 // �����ع�ʱ��
	bool SetGain(double iValue);                 // ��������
	int GetShutter(void);                        // ��ȡ�ع�ʱ��
	int GetGain(void);                           // ��ȡ����
				
	//bool Get_bIsHardTrigger();
	//void Set_bIsHardTrigger(bool b);
									             
	bool Get_m_bRGBmode();
	void Set_m_bRGBmode(bool b);

	void Set_ExposureMode(BOOL ��);

	CCamera* GetCam() { return &this->Camera; };

	bool IsOpened();                             // ֻ��

	void Set_interfaceName(CString str);         // ����ӿ�
	CString Get_interfaceName();



	void Set_deviceName(CString str);
	//CString Get_deviceName() { return this->Camera.Get_deviceName();};

	int GetParameterInt(CString str) { return this->Camera.GetParameterInt(str); };



private:

	HANDLE m_hThread;                            // �ɼ��߳�
	UINT32 Result;                               // ��� api ��������ֵ


	UINT32 iImageWidth;
	UINT32 iImageHeight;
	int m_iCameraIndex;                          // ͼ������

	bool m_bBusy;                                // ����Ƿ���æµ״̬
	CWnd * m_pParent;                            // ������ָ��
	CCamera Camera;                              // firegrab �������

	//-----------------------------------------------------------------------
	// ����Ϊ�궨������
	CString CamerModel;                          // ��ɨ�裬��������
	bool bIsYuanXin;                             // �ǲ���Զо��ͷ
	bool bIsSlope;                               // �ǲ�����б
	double fSx;                                  // ������Ԫ�Ŀ� um
	double fSy;                                  // ������Ԫ�ĸ� um
	double fFocus;                               // ����Ľ��� mm

	//int id;										// ���    ����ʹ��	
	//int CammerPort;								// ����˿�
	int nTotalPicture;							 // �ܹ���Ҫ�ı궨ͼƬ��    
	CString DescrFileName;    					 // �궨�����ļ�����
	double fError;							     // ͼ������	 
	CString DescrPsName;						 // �궨 PostScrip �ļ�����
	CString DescrDir;							 // �궨�����ļ�Ŀ¼
	CString DirInnerPicture;					 // �궨 Inner ͼ��Ŀ¼	
	CString DirPosePicture;                      // �궨 pose ͼ��Ŀ¼	
	CString CamCaliCcdFile;				         // ����궨����ļ� .ccd

	CString CamParaCalFile;                      // 
	CString CamPosDatFile;                       // 
	
	double CaliPlateThick;						 // �궨����
	double dOnePicMaxError;						 // �궨������������	
	bool bOnePIcMaxErrorOK;						 // ���α궨ͼ����Ч
	double RectificationWidth;					 // width of the visible area in world coordinates[mm]

};


#else

#endif



