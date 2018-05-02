
#include "stdafx.h"
#include "CMV.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ChildView.h"
#include "VMP.h"
#include "DlgUserMange.h"

#ifdef USE_VP_PROTECT 
extern bool is_registered();
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_WM_SETTINGCHANGE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnFileSave)
	ON_COMMAND(ID_CAM_ONE_SHOT, &CMainFrame::OnCamOneShot)
	ON_COMMAND(ID_CAM_CAP_MULTI, &CMainFrame::OnCamCapMulti)
	ON_COMMAND(ID_CAM_CONTINUE_CAP, &CMainFrame::OnCamContinueCap)
	ON_WM_TIMER()
	ON_COMMAND(ID_WIN_ZOOM_OUT, &CMainFrame::OnWinZoomOut)
	ON_COMMAND(ID_WIN_ZOOM_IN, &CMainFrame::OnWinZoomIn)
	ON_COMMAND(ID_WIN_RESTORE, &CMainFrame::OnWinRestore)
	ON_COMMAND(ID_USER_LOGING, &CMainFrame::OnUserLoging)
	ON_COMMAND(ID_EXPORT_EXCEL, &CMainFrame::OnExportEXCEL)
	ON_COMMAND(ID_REPORT_MANAGE, &CMainFrame::OnReportManage)
	ON_COMMAND(ID_SYS_SETTING, &CMainFrame::OnSysSetting)
//	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
ON_COMMAND(ID_CALIBRATE, &CMainFrame::OnCalibrate)
ON_COMMAND(ID_CAM_POSE_SET, &CMainFrame::OnCamPoseSet)
ON_COMMAND(ID_RECTITY_PICTURE, &CMainFrame::OnRectityPicture)
ON_COMMAND(ID_MODEL_SET, &CMainFrame::OnModelSet)
ON_COMMAND(ID_MATCH_MODEL, &CMainFrame::OnMatchModel)

ON_COMMAND(ID_MEASURE_AUTO, &CMainFrame::OnMeasureAuto)
ON_COMMAND(ID_MEASURE_STOP, &CMainFrame::OnMeasureStop)
ON_COMMAND(ID_COMBO_CAMERA_NAME, &CMainFrame::OnComboCameraName)
ON_COMMAND(ID_COMBO_CAMERA_CONNECT_TYPE, &CMainFrame::OnComboCameraConnectType)
ON_COMMAND(ID_COMBO_CAMERA_RESOLUTIION, &CMainFrame::OnComboCameraResolutiion)
ON_COMMAND(ID_CHECK_CAMERA_IS_RGB, &CMainFrame::OnCheckCameraIsRgb)
ON_COMMAND(ID_CHECK_CAMERA_SOFT_TRIG, &CMainFrame::OnCheckCameraSoftTrig)
ON_COMMAND(ID_EDIT_CAMERA_TRIG_TIME, &CMainFrame::OnEditCameraTrigTime)
//ON_COMMAND(ID_COMBO4, &CMainFrame::OnCombo4)
ON_UPDATE_COMMAND_UI(ID_CHECK_CAMERA_SOFT_TRIG, &CMainFrame::OnUpdateCheckCameraSoftTrig)
ON_UPDATE_COMMAND_UI(ID_CHECK_CAMERA_IS_RGB, &CMainFrame::OnUpdateCheckCameraIsRgb)
//ON_COMMAND(ID_B_CROSS_COLOR_1, &CMainFrame::OnBCrossColor1)
//ON_COMMAND(ID_CALIBRATE_INNER, &CMainFrame::OnCalibrateInner)
ON_COMMAND(ID_BUT_CALI_INNER, &CMainFrame::OnButCaliInner)
ON_COMMAND(ID_BUT_CALI_CAPTURE, &CMainFrame::OnButCaliCapture)
ON_COMMAND(ID_BUT_CALI_POSE, &CMainFrame::OnButCaliPose)
ON_COMMAND(ID_COMBO_CALI_MAP_TYPE, &CMainFrame::OnComboCaliMapType)
ON_COMMAND(ID_BUTTON_GEN_RECTIFY_MAP, &CMainFrame::OnButtonGenRectifyMap)
ON_COMMAND(ID_ROI_DRAW_RECTANGLE2, &CMainFrame::OnRoiDrawRectangle2)

ON_COMMAND(ID_BUTTON_TAKE_ONE, &CMainFrame::OnButtonTakeOne)



//ON_COMMAND(ID_BUTTON13, &CMainFrame::OnButton13)




ON_COMMAND(ID_CHECK_CAMERA_EXPOSURE_MODE, &CMainFrame::OnCheckCameraExposureMode)
//ON_COMMAND(ID_CAMERA_GAIN_SPIN, &CMainFrame::OnCameraGainSpin)
//ON_COMMAND(ID_CAMERA_EXPOSURE_TIME_SPIN, &CMainFrame::OnCameraExposureTimeSpin)
ON_UPDATE_COMMAND_UI(ID_CHECK_CAMERA_EXPOSURE_MODE, &CMainFrame::OnUpdateCheckCameraExposureMode)

ON_COMMAND(ID_EDIT_CAMERA_GAIN, &CMainFrame::OnEditCameraGain)
ON_COMMAND(ID_VIEW_FILEVIEW, &CMainFrame::OnViewProjectView)
ON_UPDATE_COMMAND_UI(ID_VIEW_FILEVIEW, &CMainFrame::OnUpdateViewProjectView)
ON_COMMAND(ID_VIEW_OUTPUTWND, &CMainFrame::OnViewOutputwnd)
ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUTWND, &CMainFrame::OnUpdateViewOutputwnd)
ON_COMMAND(ID_VIEW_PROPERTIESWND, &CMainFrame::OnViewPropertieswnd)
ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIESWND, &CMainFrame::OnUpdateViewPropertieswnd)
ON_COMMAND(ID_VIEW_CLASSVIEW, &CMainFrame::OnViewClassview)
ON_UPDATE_COMMAND_UI(ID_VIEW_CLASSVIEW, &CMainFrame::OnUpdateViewClassview)
ON_COMMAND(ID_VIEW_PICTUREVIEW, &CMainFrame::OnViewPictureview)
ON_UPDATE_COMMAND_UI(ID_VIEW_PICTUREVIEW, &CMainFrame::OnUpdateViewPictureview)
ON_COMMAND(ID_DEBUG_STARP, &CMainFrame::OnDebugStarp)
ON_COMMAND(ID_DEBUG_STOP, &CMainFrame::OnDebugStop)

//ON_MESSAGE(UM_UPDATESTATUS, &CMainFrame::OnUpdateStatus)

ON_REGISTERED_MESSAGE(UM_UPDATESTATUS, &CMainFrame::OnUpdateStatus)
ON_REGISTERED_MESSAGE(UM_UPDATE_RIBBON, &CMainFrame::OnUpdateRibbon)
ON_REGISTERED_MESSAGE(UM_UPDATESTATUS_APPSTATE, &CMainFrame::OnUpDateStatusByAppState)
ON_REGISTERED_MESSAGE(UM_LOGOUT, &CMainFrame::OnUM_LOGOUT)




ON_UPDATE_COMMAND_UI(ID_BUT_CALI_CAPTURE, &CMainFrame::OnUpdateButCaliCapture)
ON_COMMAND(ID_COMBO_MATCH_ROI_TYPE, &CMainFrame::OnComboMatchRoiType)
ON_UPDATE_COMMAND_UI(ID_COMBO_MATCH_ROI_TYPE, &CMainFrame::OnUpdateComboMatchRoiType)
ON_COMMAND(ID_BUT_MATCH_LOAD_PIC, &CMainFrame::OnButMatchLoadPic)
ON_COMMAND(ID_MATCH_TAKE_ONE, &CMainFrame::OnMatchTakeOne)
ON_COMMAND(ID_MATCH_DEL_ALL_ROI, &CMainFrame::OnMatchDelAllRoi)
ON_COMMAND(ID_ROI_DRAW_CIRCLE, &CMainFrame::OnRoiDrawCircle)
ON_COMMAND(ID_ROI_DRAW_ELLIPSE, &CMainFrame::OnRoiDrawEllipse)
ON_COMMAND(ID_ROI_DRAW_RECTANGLE, &CMainFrame::OnRoiDrawRectangle)
ON_COMMAND(ID_ROI_DRAW_ANY_LINE, &CMainFrame::OnRoiDrawAnyLine)
ON_COMMAND(ID_ROI_INFO, &CMainFrame::OnRoiInfo)
ON_COMMAND(ID_ROI_FIND_MODEL, &CMainFrame::OnRoiFindModel)
ON_COMMAND(ID_MEA_ELE_LINE, &CMainFrame::OnMeaEleLine)
ON_COMMAND(ID_MEA_ELE_CIRCLE, &CMainFrame::OnMeaEleCircle)
ON_COMMAND(ID_MEA_ELE_ARC, &CMainFrame::OnMeaEleArc)
ON_COMMAND(ID_MEA_ELE_ELLISPE, &CMainFrame::OnMeaEleEllispe)
ON_COMMAND(ID_MEA_ELE_ELLISPE_ARC, &CMainFrame::OnMeaEleEllispeArc)
ON_COMMAND(ID_MEA_ELE_WIDTH, &CMainFrame::OnMeaEleWidth)
ON_COMMAND(ID_MEA_ELE_FIND_EDGE, &CMainFrame::OnMeaEleFindEdge)
ON_COMMAND(ID_MEA_ELE_FIND_TOP, &CMainFrame::OnMeaEleFindTop)
ON_COMMAND(ID_MEA_ELE_AREA, &CMainFrame::OnMeaEleArea)
ON_COMMAND(ID_MEA_ELE_COUNTER, &CMainFrame::OnMeaEleCounter)
ON_COMMAND(ID_MEA_ELE_MEASURE, &CMainFrame::OnMeaEleMeasure)
ON_COMMAND(ID_BUT_MATCH_SAVE_MODEL, &CMainFrame::OnButMatchSaveModel)
ON_COMMAND(ID_MEA_ELE_DEL_ALL, &CMainFrame::OnMeaEleDelAll)
ON_COMMAND(ID_BUT_CALI_CORRECT, &CMainFrame::OnButCaliCorrect)
ON_COMMAND(ID_MEA_ELE_OUTPUT, &CMainFrame::OnMeaEleOutput)
ON_COMMAND(ID_ELE_ANGLE, &CMainFrame::OnEleAngle)
ON_COMMAND(ID_MEA_ELE_STEP, &CMainFrame::OnMeaEleStep)
ON_COMMAND(ID_AUTO_START, &CMainFrame::OnAutoStart)
ON_COMMAND(ID_AUTO_STOP, &CMainFrame::OnAutoStop)
ON_COMMAND(ID_ELE_DISTANCE, &CMainFrame::OnEleDistance)
ON_COMMAND(ID_MEA_ELE_CAMERA, &CMainFrame::OnMeaEleCamera)
ON_COMMAND(ID_MEA_ELE_MILLIMETER, &CMainFrame::OnMeaEleMillimeter)
ON_COMMAND(ID_DELETE_PRODUCT_RESULT, &CMainFrame::OnDeleteProductResult)
//ON_COMMAND(ID_APP_ABOUT, &CMainFrame::OnAppAbout)
ON_WM_KEYDOWN()
ON_COMMAND(ID_MEA_ELE_BALLOON_LENGTH, &CMainFrame::OnMeaEleBalloonLength)
ON_COMMAND(ID_MEA_BALLOON_CENTER, &CMainFrame::OnMeaBalloonCenter)
ON_COMMAND(ID_MEA_BALLOON_ECCENT, &CMainFrame::OnMeaBalloonEccent)
ON_COMMAND(ID_MEA_ELE_BALLOON_FLAW, &CMainFrame::OnMeaEleBalloonFlaw)
ON_COMMAND(ID_MEA_ELE_function, &CMainFrame::OnMeaEleFunction)
//ON_COMMAND(ID_MEA_ELE_Matching, &CMainFrame::OnMeaEleMatching)
ON_COMMAND(ID_BUT_CALI_SHOW_INFO, &CMainFrame::OnButCaliShowInfo)
ON_COMMAND(ID_MEA_ELE_SAVE, &CMainFrame::OnMeaEleSave)
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_MEA_ELE_ROI, &CMainFrame::OnMeaEleRoi)
ON_COMMAND(ID_BUT_USER_MANAGE2, &CMainFrame::OnButUserManage2)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	EnableActiveAccessibility();
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// ��ʼȫ�ֱ���
	UCI::initFirst(this);

	

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	
	if (!this->CreateStatusBar()) {
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	
	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �������񽫴�������࣬��˽���ʱ��������ͣ��: 
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// ���������á�Outlook��������: 
	//if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	//{
	//	TRACE0("δ�ܴ�����������\n");
	//	return -1;      // δ�ܴ���
	//}

	// ����������: 
	if (!CreateCaptionBar())
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// �Ѵ��� Outlook ����Ӧ���������ͣ����
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// ���ز˵���ͼ��(�����κα�׼��������): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndProjectView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProjectView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndProjectView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);  

	m_wndPictureView.EnableDocking(CBRS_ALIGN_ANY);     // ����ͼ
	DockPane(&m_wndPictureView);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ  m_wndPictureView
	OnApplicationLook(theApp.m_nAppLook);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);	

	// �򿪵�һ��ͼ��
	((CCMVApp*)AfxGetApp())->OnFileNew();

	if (::CoInitialize(NULL) == E_INVALIDARG)
	{
		AfxMessageBox(_T("��ʼ��Comʧ��!"));
	}
	CLSIDFromProgID(_T("SAPI.SpVoice"), &CLSID_SpVoice);

	if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pSpVoice)))
	{
		AfxMessageBox(L"���ܳ�ʼ������");
	}
	else {
		UCI::MSSpeedInitOK = true;
	}

	UCI::MyReadSettingFromSqlite();

	UCI::pDis->LoadDefaultPicture();  // ���Ӵ��ڴ�һ��ȱʡͼ��

#ifdef USE_VP_PROTECT 
	if (is_registered() == false) {
		return -1;
	}
#endif

	return 0;
}

// ��ʼ������ͼ
void CMainFrame::Init_wndPictureView()
{
	this->m_wndPictureView.InitPictureStatic(UCI::pDis);

	UCI::pDis->InitThreadCommunication();
}


// ����ϵͳ״̬��ʾ
void CMainFrame::UpDateStatusByAppState()
{
	CMFCRibbonBaseElement * pElement;
	pElement = m_wndStatusBar.GetElement(2);
	pElement->SetText(STATE_NAME[UCI::pDis->GetAppState()]);	
	pElement->Redraw();
}
// һ��״̬����ʾ
void CMainFrame::StatusCommon(CString str)
{
		CMFCRibbonBaseElement * pElement;
		pElement = m_wndStatusBar.GetElement(6);
		pElement->SetText(str);
		pElement->Redraw();
		//m_wndStatusBar.RedrawWindow();
}

void CMainFrame::UpDateStatusProductInfo()
{
	int iPID = UCI::OPTIONS[L"��Ŀ���"];
	int start = UCI::OPTIONS[L"ProdutctExportStartNo"];

	int total,good;

	UCI::sDB.GetProductTotalAndGoodBy(iPID, start, total, good);
	CString str;
	str.Format(_T("%6d"), good);
	CMFCRibbonBaseElement * pElement;
	pElement = m_wndStatusBar.GetElement(10);
	pElement->SetText(str);
	pElement->Redraw();


	str.Format(_T("%6d"), total);
	pElement = m_wndStatusBar.GetElement(14);
	pElement->SetText(str);
	pElement->Redraw();

}

void CMainFrame::StatusSetLogName(CString Name)
{
	CMFCRibbonBaseElement * pElement;
	pElement = m_wndStatusBar.GetElement(4);
	pElement->SetDescription(L"��ǰ�ɹ���¼���û���");
	pElement->SetText(Name);
	pElement->Redraw();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ���������ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	// �����ļ���ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndProjectView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������Ŀ��ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	// ����������ͼ����  
	CString strPictureView;
	bNameValid = strPictureView.LoadString(IDS_PICTURE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndPictureView.Create(strPictureView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PICTURE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�����������ͼ������\n");
		return FALSE; // δ�ܴ���
	}
	// �����������
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	//m_wndOutput.SetWindowText(L"laef ");

	// �������Դ���
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ��������ԡ�����\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProjectView.SetIcon(hFileViewIcon, FALSE);

	HICON hPictureViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndPictureView.SetIcon(hPictureViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // δ�ܴ���
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// �ɸ��������Զ����أ��ɵ�����С�������ܹر�
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);

	calendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

// ������
BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("δ�ܴ���������\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// ״̬��
BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this))
	{
		return FALSE;
	}

	int ID = 120;

	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++, 
		L"��ǰ״̬", 
		TRUE,0,
		L"��ǰ״̬"),
		L"״̬��LABEL0");

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L"δ��¼", 
		TRUE,0,
		L"��ǰ״̬"),
		L"״̬��LABEL1");

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L" ",
		TRUE, 0,
		L"δ��¼"),
		L"״̬��LABEL2");          // ��¼�Ĳ���Ա

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L"��ʼ���ɹ��ˣ�",
		TRUE, 0,
		L"һ��״̬�����������һ��״̬�����������һ��״̬�����������һ��״̬�����������"),
		L"״̬��LABEL2");

	

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L"�ϸ�",
		TRUE, 0,
		L"�ϸ�"),
		L"״̬��LABEL2");

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L"000000",
		TRUE, 0,
		L"000000"),
		L"״̬��LABEL2");

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L"����",
		TRUE, 0,
		L"����"),
		L"״̬��LABEL2");

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L"000000",
		TRUE, 0,
		L"000000"),
		L"״̬��LABEL2");

	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID++,
		L"",
		TRUE, 0,
		L""),
		L"״̬��LABEL2");

	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID++, L"���ճ���", TRUE), L"���ճ���");
	
	return TRUE;
}

int CMainFrame::FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl ** ppOutlookWnd)
{
	return 0;
}

CMFCOutlookBarTabCtrl * CMainFrame::FindOutlookParent(CWnd * pWnd)
{
	return nullptr;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	//CMFCRibbonButton * pB = DYNAMIC_DOWNCAST(
	//	CMFCRibbonButton, m_wndRibbonBar.FindByID(ID_BUT_CALI_CAPTURE));
	////pB->SetText(L"eaffa");
	////pB->SetVisible(false);
	//pB->Redraw();

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);

	//this->m_wndClassView.ShowWindow(SW_SHOW);
	//RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

// �߳���״̬�������Ϣ
LRESULT CMainFrame::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	int isSave = (int)wParam;
	CString *str = (CString*)lParam;
	StatusCommon(*str);

	// ������־����Ҳ��ʾһ��
	this->LogOut(*str, isSave);

	delete str;
	return LRESULT();
}

LRESULT CMainFrame::OnUpdateRibbon(WPARAM wParam, LPARAM lParam)
{
	this->m_wndRibbonBar.RedrawWindow();
	
	return LRESULT();
}

LRESULT CMainFrame::OnUpDateStatusByAppState(WPARAM wParam, LPARAM lParam)
{
	
	this->UpDateStatusByAppState();
	return LRESULT();
}

LRESULT CMainFrame::OnUM_LOGOUT(WPARAM wParam, LPARAM lParam)
{
	int save = (int)wParam;
	LPCTSTR lpszText = (LPCTSTR)lParam;
	this->LogOut(lpszText, save);

	return LRESULT();
}

void CMainFrame::OnClose()
{

	pSpVoice->Release();
	::CoUninitialize();

	UCI::MySaveSettingToSqlite();

	CMDIFrameWndEx::OnClose();
}

// ɾ����ǰ��Ŀ�����в�����¼
void CMainFrame::OnDeleteProductResult()
{
	int prID = UCI::OPTIONS[L"��Ŀ���"];
	UCI::sDB.ClearProductResutlByProjectID(prID);
}

// ��������
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (pMsg->message) {
	case WM_KEYDOWN:
		UCI::pDis->OnDisKeyDown(pMsg);
		break;
	case WM_RBUTTONDOWN:
		UCI::pDis->OnRButtonDown(pMsg);
		break;
	default:
		break;
	}

	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}




void CMainFrame::OnMeaBalloonCenter()
{
	// TODO: �ڴ���������������
}





void CMainFrame::OnMeaEleSave()
{

	if (!UCI::pDis->TestCurrentAppState(APP_LOGGING_OK)) return;

	UCI::sDB.MySaveAllElement();
}


void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CMDIFrameWndEx::OnRButtonDown(nFlags, point);
}






void CMainFrame::OnButUserManage2()
{
	DlgUserManage dlg(this);

	if (dlg.DoModal() == IDOK) {

		//CString userName;
		//CString passWord;
		//dlg.ID_UserName.GetWindowText(userName);
		//dlg.ID_Password.GetWindowText(passWord);


	}
}
