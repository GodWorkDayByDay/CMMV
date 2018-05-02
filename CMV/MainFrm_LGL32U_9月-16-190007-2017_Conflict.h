// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "ProjectView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "PictureView.h"
#include "CalendarBar.h"
#include "Resource.h"

#include "MyData.h"
#include "MyDispLay.h"


#include "sapi.h"    
#pragma comment(lib, "sapi.lib")

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:
	//ST_MYDISP* GetpDis();                 // �õ���ǰ���ڵ���ʾ����

	void Speech(CString str, bool clear = false) {
		if (UCI::MSSpeedInitOK) {
			if (clear) {
				pSpVoice->Speak(str, SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);       
			}
			else {
				pSpVoice->Speak(str, SPF_ASYNC, NULL);     // �ʶ����ĺ�Ӣ�ĵĻ���ַ���   
			}
		}
	}

	// �õ���ǰ��ѡ��Ľڵ�
	HTREEITEM GetCurrentSelHTREEITEM() {
		return this->m_wndProjectView.GetCurrentSelHTREEITEM();
	};

	void SetPropertyCurOptionsMap(UCI::OptionsMap* CurOption) {
		this->m_wndProperties.CurOption = CurOption;
	};

	UCI::OptionsMap* GetPropertyCurOptionsMap() {
		return this->m_wndProperties.CurOption;
	};

	// �õ���ǰѡ��Ԫ�ص�ID
	//int GetCurrentSelElementID() {
	//	return this->m_wndProjectView.GetCurrentSelElementID();
	//};

	//UCI::OptionsMap* getCurPropertyMap() { return this->m_wndProjectView.getCurProperyMap(); };
	//void setCurPropertyMap(UCI::OptionsMap* o) { this->m_wndProjectView.setCurProperyMap(o); };

	//int GetCurElementPropertyIndex() { return this->m_wndProjectView.GetCurElementPropertyIndex(); };
	//void SetCurElementPropertyIndex(int cur) { this->m_wndProjectView.SetCurElementPropertyIndex(cur); };

	void ResDeleAllItems() { this->m_wndOutput.ResDeleAllItems(); };
	void insertOneResult(UCI::OptionsMap* o) { this->m_wndOutput.insertOneResult(o); };

	void Init_wndPictureView();

	void Speech(char * cstr, bool clear = false) { Speech(CA2W(cstr).m_psz, clear); };
	void LogOut(CString str, bool isSave = true) { m_wndOutput.LogOutBuild(str, isSave); };
	//void LogOut(char * cstr) { CString str = CA2W(cstr); LogOut(str); };

	void LogOut(char * cstr, bool isSave = true) { LogOut(CA2W(cstr).m_psz, isSave); };
	
	// ״̬��
	void StatusSetLogName(CString Name);
	
	void StatusCommon(CString str);

	void UpDateStatusProductInfo();
	// 

	void LogWinHead(CString str) { m_wndOutput.SetWindowText(str); };

	void ProPertyUpdate() { m_wndProperties.InitPropList(); };                          // ��������ҳ
	void SetPettyCurSel(ePROPLIST e) { m_wndProperties.SetCurList(e); };
	
	int MProjectTreeFillByPrID() { return m_wndProjectView.UpdataProjectTreeByList();  };
	int EmptyProjectTree(int prID) {
		return m_wndProjectView.EmptyProjectTree(prID);
	};

	void FillProjectTreeView() {		m_wndProjectView.FillProjectTreeView();	};
	
	UCI::OptionsMap*  MProjectTreeAddElement(UCI::OptionsMap* o) { return m_wndProjectView.AddOneElementToProjectTree(o); };

	CMFCRibbonBar* get_m_wndRibbonBar() { return &this->m_wndRibbonBar; };

	

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;     // 
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;          // ״̬��
	CProjectView         m_wndProjectView;        // ��Ŀ��ͼ
	CClassView        m_wndClassView;             // ����ͼ
	COutputWnd        m_wndOutput;                // �����ͼ
	CPropertiesWnd    m_wndProperties;            // ������ͼ
	CPictureView      m_wndPictureView;           // ������ͼ
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;                  // 
	CCalendarBar      m_wndCalendar;              // ������ͼ
	CMFCCaptionBar    m_wndCaptionBar;	          // ����

	
	CLSID CLSID_SpVoice;	                // �������
	ISpVoice *pSpVoice = NULL;

	void UpDateStatusByAppState();

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	BOOL CreateCaptionBar();

	BOOL CreateStatusBar();    // ״̬��

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;

	afx_msg LRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateRibbon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpDateStatusByAppState(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUM_LOGOUT(WPARAM wParam, LPARAM lParam);

	
	
	//LRESULT OnUpdateStatus(WPARAM /*wParam*/, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnCamOneShot();
	afx_msg void OnCamCapMulti();
	afx_msg void OnCamContinueCap();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnWinZoomOut();
	afx_msg void OnWinZoomIn();
	afx_msg void OnWinRestore();
	afx_msg void OnUserLoging();
	afx_msg void OnExportEXCEL();
	afx_msg void OnReportManage();
	afx_msg void OnSysSetting();
//	afx_msg void OnFileNew();
	afx_msg void OnCalibrate();
	afx_msg void OnCamPoseSet();
	afx_msg void OnRectityPicture();
	afx_msg void OnModelSet();
	afx_msg void OnMatchModel();

	afx_msg void OnMeasureAuto();
	afx_msg void OnMeasureStop();
	afx_msg void OnComboCameraName();
	afx_msg void OnComboCameraConnectType();
	afx_msg void OnComboCameraResolutiion();
	afx_msg void OnCheckCameraIsRgb();
	afx_msg void OnCheckCameraSoftTrig();
	afx_msg void OnEditCameraTrigTime();
	//afx_msg void OnCombo4();
	afx_msg void OnUpdateCheckCameraSoftTrig(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckCameraIsRgb(CCmdUI *pCmdUI);
	afx_msg void OnBCrossColor1();
	//afx_msg void OnCalibrateInner();
	afx_msg void OnButCaliInner();
	afx_msg void OnButCaliCapture();
	afx_msg void OnButCaliPose();
	afx_msg void OnComboCaliMapType();
	afx_msg void OnButtonGenRectifyMap();
	afx_msg void OnRoiDrawRectangle2();

	afx_msg void OnButtonTakeOne();


	
	//afx_msg void OnButton13();
	



	
	afx_msg void OnCheckCameraExposureMode();
	//afx_msg void OnCameraGainSpin();
	//afx_msg void OnCameraExposureTimeSpin();
	afx_msg void OnUpdateCheckCameraExposureMode(CCmdUI *pCmdUI);

	afx_msg void OnEditCameraGain();
	afx_msg void OnViewProjectView();
	afx_msg void OnUpdateViewProjectView(CCmdUI *pCmdUI);
	afx_msg void OnViewOutputwnd();
	afx_msg void OnUpdateViewOutputwnd(CCmdUI *pCmdUI);
	afx_msg void OnViewPropertieswnd();
	afx_msg void OnUpdateViewPropertieswnd(CCmdUI *pCmdUI);
	afx_msg void OnViewClassview();
	afx_msg void OnUpdateViewClassview(CCmdUI *pCmdUI);
	afx_msg void OnViewPictureview();
	afx_msg void OnUpdateViewPictureview(CCmdUI *pCmdUI);
	afx_msg void OnDebugStarp();
	afx_msg void OnDebugStop();
	afx_msg void OnUpdateButCaliCapture(CCmdUI *pCmdUI);
	afx_msg void OnComboMatchRoiType();
	afx_msg void OnUpdateComboMatchRoiType(CCmdUI *pCmdUI);
	afx_msg void OnButMatchLoadPic();
	afx_msg void OnMatchTakeOne();
	afx_msg void OnMatchDelAllRoi();
	afx_msg void OnRoiDrawCircle();
	afx_msg void OnRoiDrawEllipse();
	afx_msg void OnRoiDrawRectangle();
	afx_msg void OnRoiDrawAnyLine();
	afx_msg void OnRoiInfo();
	afx_msg void OnRoiFindModel();
	afx_msg void OnMeaEleLine();
	afx_msg void OnMeaEleCircle();
	afx_msg void OnMeaEleArc();
	afx_msg void OnMeaEleEllispe();
	afx_msg void OnMeaEleEllispeArc();
	afx_msg void OnMeaEleWidth();
	afx_msg void OnMeaEleFindEdge();
	afx_msg void OnMeaEleFindTop();
	afx_msg void OnMeaEleArea();
	afx_msg void OnMeaEleCounter();
	afx_msg void OnMeaEleMeasure();
	afx_msg void OnButMatchSaveModel();
	afx_msg void OnMeaEleDelAll();
	afx_msg void OnButCaliCorrect();
	afx_msg void OnMeaEleOutput();
	afx_msg void OnEleAngle();
	afx_msg void OnMeaEleStep();
	afx_msg void OnAutoStart();
	afx_msg void OnAutoStop();
	afx_msg void OnEleDistance();
	afx_msg void OnMeaEleCamera();
	afx_msg void OnMeaEleMillimeter();
	afx_msg void OnDeleteProductResult();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMeaEleBalloonLength();
	afx_msg void OnMeaBalloonCenter();
	afx_msg void OnMeaBalloonEccent();
	afx_msg void OnMeaEleBalloonFlaw();
	afx_msg void OnMeaEleFunction();
	afx_msg void OnMeaEleMatching();
	afx_msg void OnButCaliShowInfo();
	afx_msg void OnMeaEleSave();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


