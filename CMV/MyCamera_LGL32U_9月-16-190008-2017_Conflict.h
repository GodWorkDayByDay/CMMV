#pragma once

#include "FireGrab.h"




// class AVTCam;
// �Զ��������

typedef struct                                  
{
	HANDLE* phMutex;                             // ��ʾ������
	HalconCpp::HObject* ho_FRAME;                // ����һ��ͼ�����
} MYFRAME;

class  CCamera  // : public CFGCamera
{	
public:
	CCamera();
	~CCamera();

	// void CPPExpDefaultExceptionHandler(const HalconCpp::HException & except);

	//UINT32        WriteRegister(UINT32 Address, UINT32 Value);
	//
	//UINT32        ReadRegister(UINT32 Address, UINT32 *pValue);
	//UINT32        WriteBlock(UINT32 Address, UINT8 *pData, UINT32 Length);
	//UINT32        ReadBlock(UINT32 Address, UINT8 *pData, UINT32 Length);

	//UINT32        Connect(UINT32HL *pGuid);
	//UINT32        Connect(int index);
	// UINT32        Connect();                     // ��Ribbon�еõ�������Ϣ
	// UINT32        ConnectA(AVTCam* pAvt);     // ��Ribbon�еõ�������Ϣ
	//UINT32        Disconnect();

	//UINT32        SetParameter(UINT16 Which, UINT32 Value);
	



	bool SetParameterHTuple(CString str, HalconCpp::HTuple ht);

	int GetParameterInt(CString sPara);
	double GetParameterDouble(CString Spara);

	CString GetParameterString(CString str);


	//UINT32        GetParameterInfo(UINT16 Which, FGPINFO *pInfo);

	//UINT32        OpenCapture();
	//UINT32        CloseCapture();

	//UINT32        AssignUserBuffers(UINT32 Cnt, UINT32 Size, void* *ppMemArray);

	//UINT32        StartDevice();
	//UINT32        StopDevice();


	//UINT32        GetFrame(MYFRAME* myFrame, UINT32 TimeoutInMs = INFINITE);

	//UINT32        GetCameraName(char* name);

	bool Get_bIsOpened();
	void Set_bIsOpened(bool b);



	void Set_interfaceName(CString str);
	CString Get_interfaceName();




	void Set_Gain(double d);              // �������������

	BOOL Get_ExposureMode();
	void Set_ExposureMode(BOOL b);

	void Set_Shutter(int i);
	

private:
	HalconCpp::HTuple hv_AcqHandle;                       // ������		
	bool bIsOpened;



};

