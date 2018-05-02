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

#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ����ѡ�����: 
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ������ѡ�����\n");
		return -1;      // δ�ܴ���
	}

	// �����������: 
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("δ�ܴ����������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndDataList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, rectDummy,
		&m_wndTabs, 5)) 
	{
		TRACE0("δ�ܴ���ListCtrl����\n");
		return -1;      // δ�ܴ���
	}
	else
	{
		m_wndDataList.InsertColumn(0, L"���", LVCFMT_RIGHT, 60);
		m_wndDataList.InsertColumn(1, L"����", LVCFMT_RIGHT, 100);
		m_wndDataList.InsertColumn(2, L"����ֵ", LVCFMT_RIGHT, 100);
		m_wndDataList.InsertColumn(3, L"��׼ֵ", LVCFMT_RIGHT, 100);
		m_wndDataList.InsertColumn(4, L"����", LVCFMT_RIGHT, 100);
		m_wndDataList.InsertColumn(5, L"����", LVCFMT_RIGHT, 100);
		m_wndDataList.InsertColumn(6, L"�ж����", LVCFMT_RIGHT, 100);
		m_wndDataList.InsertColumn(7, L"��ע", LVCFMT_RIGHT, 300);

		m_wndDataList.SetExtendedStyle(LVS_EX_GRIDLINES);

		//m_wndDataList.DeleteAllItems();
	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// ���б��ڸ��ӵ�ѡ�: 
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);

	bNameValid = strTabName.LoadString(IDS_OUTRESULT_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndDataList, strTabName, (UINT)1);
	
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)2);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, L"����", (UINT)3);

	// ʹ��һЩ�����ı���д���ѡ�(���踴������)
	FillBuildWindow();
	FillDebugWindow();
	FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// ѡ��ؼ�Ӧ��������������: 
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
	m_wndOutputBuild.AddString(_T("�����ʼ���ɹ���"));
	//m_wndOutputBuild.AddString(_T("�������ʾ���б���ͼ������"));
	//m_wndOutputBuild.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::FillDebugWindow()
{
	//m_wndOutputDebug.AddString(_T("�����������ʾ�ڴ˴���"));
	//m_wndOutputDebug.AddString(_T("�������ʾ���б���ͼ������"));
	//m_wndOutputDebug.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::FillFindWindow()
{
	//m_wndOutputFind.AddString(_T("�����������ʾ�ڴ˴���"));
	//m_wndOutputFind.AddString(_T("�������ʾ���б���ͼ������"));
	//m_wndOutputFind.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::insertOneResult(UCI::OptionsMap * o)
{	
	CListCtrl* CL = &m_wndDataList;

	int column = 1;
	CString str;

	int count = CL->GetItemCount();

    str.Format(L"%d", count+1);
	CL->InsertItem(count, str);

	CString name = (*o)[L"����������"];
	CL->SetItemText(count, column++, name);

    double dRes = (*o)[L"����Ĳ���ֵ"];
	str.Format(L"%.3lf", dRes);
	CL->SetItemText(count, column++, str);

	dRes = (*o)[L"����ı�׼ֵ"];
	str.Format(L"%.3lf", dRes);
	CL->SetItemText(count, column++, str);

	double dLow = (*o)[L"������¹���"];
	str.Format(L"%.3lf", dRes + dLow);
	CL->SetItemText(count, column++, str);

	double dUpper = (*o)[L"������Ϲ���"];
	str.Format(L"%.3lf", dRes + dUpper);
	CL->SetItemText(count, column++, str);

	CString stRes = (*o)[L"�����������"];
	CL->SetItemText(count, column++, stRes);

	CString stNote = (*o)[L"Ԫ�ر�ע"];
	CL->SetItemText(count, column++, stNote);
}

//TCHAR * ColTitle[num] = { L"���",L"����",L"����ֵ",L"��׼ֵ",L"�Ϲ���",L"�¹���",L"�ж����",L"��ע" };
//int width[num] = { 60,60,100,100,100,100,100,200 };

void COutputWnd::OnViewOutput()
{
	if (this->IsVisible() == FALSE) {
		this->ShowPane(TRUE, FALSE, TRUE);
	}
	else {
		this->ShowPane(FALSE, FALSE, TRUE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	//ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ��Ϣ�������

void COutputList::OnContextMenu(CWnd* /*pCView*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("�������"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("������"));
}

