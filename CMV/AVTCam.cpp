#include "stdafx.h"
#include "AVTCam.h"

// �����ɼ�ͼ��

DWORD WINAPI ContinousGrab(LPVOID pParam)
{
	// FGFRAME Frame;
	UINT32 error;

	AVTCam* pOwner = (AVTCam*)pParam;   // 
	pOwner->set_busy(TRUE);             // �������æµ״̬

	MYFRAME* pmyF = pOwner->GetPFrame();

	// ��ʼ�ɼ�
	pOwner->get_camera()->StartDevice();       // 
	while (pOwner->get_busy()) {

		// ��ȡһ��ͼ��
		error = pOwner->get_camera()->GetFrame(pmyF, 200);  // ���̲ɼ�

		// �жϲ�ͼ����ֵ�Ƿ���ȷ����ͼ�Ƿ�Ϊ��
		if (error == FCE_NOERROR){ // && Frame.pData != NULL) {

			// ��ͼ����п���
			// memcpy(pOwner->pImage, Frame.PData, pOwner->iImageHeight*pOwner->iImageWidth);
			// HalconCpp::CopyImage(Frame.ho_FRAME, pOwner->get_hv_image());

			// ʹ����Ϣ���ƽ�ͼ�񴫳�
			if (pOwner->m_fcallback != NULL) {
				pOwner->m_fcallback(pOwner->get_parent(), pOwner->get_camera_index());
			}
			// ���ͼ��׼��������һ��ͼ��
			// error = pOwner->get_camera()->PutFrame(&Frame);
		}
		else {
			// ��CPU˯�� 10ms
			Sleep(10);
		}
	}

	pOwner->get_camera()->StopDevice();

	return 0;
}


AVTCam::AVTCam(CWnd * PWnd, MYFRAME* pF)
{	
	//NodeCnt = 0;
	m_bBusy = false;
	m_iCameraIndex = -1;
	//pImage = NULL;
	m_fcallback = NULL;
	Result = FCE_NOERROR;	

	m_pParent = PWnd;
	this->pmyFrame = pF;

	// ����궨
	nTotalPicture = 8;						// �ܹ���Ҫ�ı궨ͼƬ��    
	dOnePicMaxError = 0.55;					// �궨����������������
	bOnePIcMaxErrorOK = false;              // �궨ͼ������OK

	CaliPlateThick = 0.001;					// �궨����
	RectificationWidth = 75.0;				// width of the visible area in world coordinates[mm]
											// Ŀ¼
	DescrDir = L"Calibration File";		    // �궨Ŀ¼
	DirInnerPicture = L"7x72caltabInner";	// DescrFileName + id
	DirPosePicture = L"7x72caltabPose";     // 
	DescrFileName = L"7x72caltab.descr";    // 
	DescrPsName = L"7x72caltab.ps";			// �궨 PostScrip �ļ�����

	CamCaliCcdFile = L"7x72caltab.ccd";     // ����궨�ļ����������ϴεı궨��Ϣ

	CamParaCalFile = L"7x72caltab.cal";     // 
	CamPosDatFile = L"7x72caltab.dat";      // 

	CamerModel = L"area_scan_polynomial";
	bIsYuanXin = false;
	bIsSlope = false;
	fSx = 8.3;
	fSy = 8.3;
	fFocus = 8;
}

AVTCam::~AVTCam()
{
	if (this->m_bBusy) {
		this->m_bBusy = false;
		Sleep(100);
	}
	
	SetTriggerMode(false);
	Camera.StopDevice();
	Camera.CloseCapture();

	// �ͷ�ͼ�񻺴�

	//if (pImage != NULL) {
	//	delete pImage;
	//	pImage = NULL;
	//}
}

void AVTCam::CPPExpDefaultExceptionHandler(const HalconCpp::HException & except)
{
	CString err = CA2W(except.ProcName().Text());
	err += CA2W(except.ErrorMessage());
	AfxMessageBox(err);
}

//HalconCpp::HObject * AVTCam::get_hv_image()
//{
//	return this->ho_pImage;
//}

void AVTCam::set_busy(bool b)
{
	this->m_bBusy = b;
}

bool AVTCam::get_busy()
{
	return this->m_bBusy;
}

int AVTCam::get_ImageHeight()
{
	return (int)this->iImageHeight;
}

int AVTCam::get_ImageWidth()
{
	return (int)this->iImageWidth;
}

void AVTCam::set_ImageWidth(int width)
{
	this->iImageWidth = width;
}

int AVTCam::get_camera_index()
{
	return this->m_iCameraIndex;
}

CWnd * AVTCam::get_parent()
{
	return this->m_pParent;
}

void AVTCam::set_ImageHeight(int height)
{
	this->iImageHeight = height;
}

CCamera * AVTCam::get_camera()
{
	return &this->Camera;
}

bool AVTCam::IsCaliPictureOk()
{
	return this->bOnePIcMaxErrorOK;
}

CString AVTCam::GetCamerModel()
{
	return this->CamerModel;
}

CString AVTCam::GetMoudlePath()
{
	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	CString _strMoudlePath(modulePath);
	_strMoudlePath = _strMoudlePath.Left(_strMoudlePath.ReverseFind('\\'));

	return _strMoudlePath;
}

CString AVTCam::GetCaliInnerPicPath()
{
	CString str = this->GetMoudlePath() + L"\\"
		+ this->DescrDir + L"\\" + this->DirInnerPicture;
	return str;
}

CString AVTCam::GetCaliPosePicPath()
{
	CString str = this->GetMoudlePath() + L"\\"
		+ this->DescrDir + L"\\" + this->DirPosePicture;
	return str;
}

CString AVTCam::GetCalibCcdPathFile()
{
	return  this->GetCaliInnerPicPath() +
		L"\\" + this->CamCaliCcdFile;
}

CString AVTCam::GetCaliCalPathFile()
{
	return  this->GetCaliInnerPicPath() +
		L"\\" + this->CamParaCalFile;
}

CString AVTCam::GetCaliPosePathFile()
{
	return  this->GetCaliPosePicPath() +
		L"\\" + this->CamPosDatFile;
}

bool AVTCam::Write_CalibrationCcd(HalconCpp::HTuple hv_CalibHandle)
{
	return this->Write_CalibrationCcd(hv_CalibHandle, GetCalibCcdPathFile());
}

bool AVTCam::Read_CalibrationCcd(HalconCpp::HTuple & hv_CalibHandle)
{
	return this->Read_CalibrationCcd(hv_CalibHandle, GetCalibCcdPathFile());
}

bool AVTCam::Write_CalibrationCcd(HalconCpp::HTuple hv_CalibHandle, CString Path)
{
	try {
		WriteCalibData(hv_CalibHandle, CT2A(Path).m_psz);
		return true;
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
		return false;
	}
}

bool AVTCam::Read_CalibrationCcd(HalconCpp::HTuple & hv_CalibHandle, CString Path)
{
	if (!PathFileExists(Path)) {
		return false;
	}

	try {
		ReadCalibData(CT2A(Path).m_psz, &hv_CalibHandle);
		return true;
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
		return false;
	}
}



bool AVTCam::Write_CaliPose(HalconCpp::HTuple CameraPose)
{
	return this->Write_CaliPose(CameraPose, GetCaliPosePathFile());
}

bool AVTCam::Read_CaliPose(HalconCpp::HTuple & CameraPose)
{
	return this->Read_CaliPose(CameraPose, GetCaliPosePathFile());
}

bool AVTCam::Write_CaliPose(HalconCpp::HTuple CameraPose, CString Path)
{
	try {
		HalconCpp::WritePose(CameraPose, CT2A(Path).m_psz);
		return true;
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
		return false;
	}
}

bool AVTCam::Read_CaliPose(HalconCpp::HTuple & CameraPose, CString Path)
{
	if (!PathFileExists(Path)) {
		return false;
	}

	try {
		HalconCpp::ReadPose(CT2A(Path).m_psz, &CameraPose);
		return true;
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
		return false;
	}
}

bool AVTCam::Write_CaliCal(HalconCpp::HTuple CameraParam)
{
	return this->Write_CaliCal(CameraParam, this->GetCaliCalPathFile());
}

bool AVTCam::Read_CaliCal(HalconCpp::HTuple & CameraParam)
{
	return this->Read_CaliCal(CameraParam, this->GetCaliCalPathFile());
}

bool AVTCam::Write_CaliCal(HalconCpp::HTuple CameraParam, CString Path)
{
	try {
		HalconCpp::WriteCamPar(CameraParam, CT2A(Path).m_psz);
		return true;
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
		return false;
	}
}

bool AVTCam::Read_CaliCal(HalconCpp::HTuple &CameraParam, CString Path)
{
	if (!PathFileExists(Path)) {
		return false;
	}

	try {
		HalconCpp::ReadCamPar(CT2A(Path).m_psz, &CameraParam);
		return true;
	}
	catch (HalconCpp::HException& except) {
		this->CPPExpDefaultExceptionHandler(except);
		return false;
	}
}

CString AVTCam::GetDescrFileName()
{
	return this->GetMoudlePath() + "\\" + DescrDir + "\\" + DescrFileName;
}

double AVTCam::GetFocus()
{
	return this->fFocus;
}

double AVTCam::GetFsx()
{
	return this->fSx;
}

double AVTCam::GetFsy()
{
	return this->fSy;
}

double AVTCam::GetdOnePicMaxError()
{
	return this->dOnePicMaxError;
}

double AVTCam::GetCaliPlateThick()
{
	return this->CaliPlateThick;
}

double AVTCam::GetRectificationWidth()
{
	return this->RectificationWidth;
}

void AVTCam::SetfError(double f)
{
	this->fError = f;
}


int AVTCam::GetTotalCaliPicture()
{
	return this->nTotalPicture;
}

void AVTCam::SetbOnePIcMaxErrorOK(bool b)
{
	this->bOnePIcMaxErrorOK = b;
}

// �����
bool AVTCam::open_camera()
{	
	if (this->IsOpened()) {
		AfxMessageBox(L"����Ѵ򿪣������ٴ���"); 
		return true;
	}

	// ���׼�����Ӵ�

	if (this->InitialCamera()) {
		return true;
	}
	else {
		AfxMessageBox(L"��ǰ����޷��򿪣�");
		return false;
	}	
}

bool AVTCam::InitialCamera()
{
	Result = FCE_NOERROR;
	
	m_fcallback = NULL;

	// �������
	// if (Result == FCE_NOERROR && NodeCnt) {
	if (Result == FCE_NOERROR) {
		Result = Camera.Connect();
	}
	else if (Result == HALER_BUSY || this->IsOpened()) {
		AfxMessageBox(_T("��������У�"));
		return false;
	}
	else {
		AfxMessageBox(_T("�����ʼ��ʧ�ܣ�"));
		return false;
	}

	// ���ô���ģʽ
	// SetTriggerMode(TRUE);

	// ��ȡͼ����
	Camera.GetParameter(FGP_XSIZE, &iImageWidth);
	Camera.GetParameter(FGP_YSIZE, &iImageHeight);


	// �������
	Camera.StartDevice();

	return true;
}

bool AVTCam::close_camera(){

	if (Camera.CloseCapture() == FCE_NOERROR) {
		return true;
	}
	return false;
}

bool AVTCam::OneShotA()
{
	if (m_bBusy)    // ��ǰ������������
	{
		return false;
	}

	// ��ȡһ֡ͼ�����ȴ� 1000ms,
	Result = Camera.GetFrame(this->GetPFrame(),1000);


	if (FCE_NOERROR == Result) {
		return true;
	}
	else {
		return false;
	}	
}


// �����ɼ�
bool AVTCam::Snap(void) {

	// ��ʼ�ɼ�
	Camera.StartDevice();

	// ����������Ƿ���ڵ���0�� �������Ƿ���æµ״̬
	if (!m_bBusy) {
		// ��ղɼ�����ͼ��
		// Camera.DiscardFrames();

		//FGFRAME Frame;

		// ��ȡһ��ͼ�����ȴ� 1000ms, 
		Result = Camera.GetFrame(this->GetPFrame(),1000);

		if (Result == FCE_NOERROR) {
			// ������棬�ȴ�������һ֡ͼ��
			// ����ص�������Ϊ�գ�ִ�лص�����������ͼ����
			if (m_fcallback != NULL) {
				m_fcallback(m_pParent, m_iCameraIndex);
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		AfxMessageBox(_T("��ȷ�����״̬��"));
		return false;
	}
}

// ����ͼ��ɼ�

bool AVTCam::Grab(void)
{
	// �������Ƿ���æµ״̬
	if (m_bBusy) {
		return true;
	}
	if (!this->IsOpened()) {
		AfxMessageBox(_T("��������ڣ�"));
		return true;
	}

	// ��������ɼ��߳̽��������ɼ�
	m_hThread = CreateThread(NULL, 0, ContinousGrab, this, 0, NULL);

	// �����������ʾ��Ϣ���ҷ���
	// if (FAILED((HRESULT)(m_hThread))) {
	if (m_hThread < 0) {
		AfxMessageBox(_T("����grap�߳�ʧ�ܣ�"));
		return false;
	}
	return true;
}

bool AVTCam::Freeze(void)
{
	m_bBusy = false;
	Sleep(20);
	return true;
}

bool AVTCam::SetTriggerMode(bool bEnable, int nSrc)
{
	//UINT32 nOn;
	//// �Ƿ�ʹ�ô���ģʽ
	//if (bEnable) {
	//	nOn = 1;
	//}
	//else {
	//	nOn = 0;
	//}

	//// 0=ext.trigger off 1= ext.trigger on
	//UINT32 nPolarity = 0; // 0=�͵�ƽ���� 1=�ߵ�ƽ����
	////UINT32 nSrc = 1;		// ����Դ����
	////UINT32 nSrc = 7;	    // ����Դ���� 1=�ⴥ�� 7=����
	//UINT32 nMode = 0;       // 0=��Ե���� 1=��ƽ����
	//UINT32 nParm = 0;       // Ԥ�� ��ʹ��
	//UINT32 TriggeValue = 0;

	////TriggeValue = MAKETRIGGER(nOn, nPolarity, nSrc, nMode, nParm);
	//Camera.SetParameter(FGP_TRIGGER, TriggeValue);

	// ���÷�����ʱ����ֹ�󴥷���
	//if (nSrc == 1) {
	//	Camera.WriteRegister(0xf1000840, 0x8000000A);
	//}

	return true;
}

bool AVTCam::SetShutter(int iValue)
{
	Camera.Set_Shutter(iValue);
	return true;
}

bool AVTCam::SetGain(double gain)
{
	Camera.Set_Gain(gain);
	return true;
}

int AVTCam::GetShutter(void)
{
	UINT32 iValue = 0;
	Camera.GetParameter(FGP_SHUTTER, &iValue);
	return iValue;
}

int AVTCam::GetGain(void)
{
	UINT32 iValue = 0;
	Camera.GetParameter(FGP_GAIN, &iValue);
	return iValue;
}
//
//bool AVTCam::Get_bIsHardTrigger()
//{
//	return this->bIsHardTrigger;
//}
//
//void AVTCam::Set_bIsHardTrigger(bool b)
//{
//	this->bIsHardTrigger = b;
//}

bool AVTCam::Get_m_bRGBmode()
{
	return this->Camera.Get_m_bRGBmode();
}

void AVTCam::Set_m_bRGBmode(bool b)
{
	this->Camera.Set_m_bRGBmode(b);
}

//BOOL AVTCam::Get_ExposureMode()
//{
//	return this->Camera.Get_ExposureMode();
//}

void AVTCam::Set_ExposureMode(BOOL ��)
{
	this->Camera.Set_ExposureMode(��);
}

bool AVTCam::IsOpened()
{
	return this->Camera.Get_bIsOpened();
}

void AVTCam::Set_interfaceName(CString str)
{
	this->Camera.Set_interfaceName(str);
}

CString AVTCam::Get_interfaceName()
{
	return this->Camera.Get_interfaceName(); 
}

void AVTCam::Set_deviceName(CString str)
{
	this->Camera.Set_deviceName(str);
}





