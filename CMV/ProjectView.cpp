

#include "stdafx.h"
#include "mainfrm.h"
#include "ProjectView.h"
#include "Resource.h"
#include "CMV.h"

#include "MyData.h"
#include "MyDispLay.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CFileView

CProjectView::CProjectView()
{
}

CProjectView::~CProjectView()
{
}

BEGIN_MESSAGE_MAP(CProjectView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnProjectOpen)
	//ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	//ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)           // ɾ��һ����Ŀ��ɾ��һ������Ԫ��
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	// ON_COMMAND(ID_PROJECT_RENAME, &CFileView::OnProjectRename)

	//ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	//ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)

	ON_COMMAND(ID_ProjectTreePaste, &CProjectView::OnProjectTreePaste)
END_MESSAGE_MAP()




////������Ϣ TVN_BEGINLABELEDIT 
//void CParentWnd::OnBeginEditTree(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR;
//	if (pTVDI->item.lParam == 0);
//	//�ж��Ƿ�ȡ���ò���
//	*pResult = 1;
//	else *pResult = 0;
//}
////������Ϣ TVN_BEGINLABELEDIT 
//void CParentWnd::OnBeginEditTree(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR;
//	if (pTVDI->item.pszText == NULL);
//	//�ж��Ƿ��Ѿ�ȡ��ȡ���༭ 
//	m_tree.SetItemText(pTVDI->item.hItem, pTVDI->pszText);
//	//������ʾ�ַ� 
//	*pResult = 0;
//}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CProjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS 
		| TVS_EDITLABELS;

	if (!m_wndProjectTree.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��: 
	m_ProjectViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));

	//m_ProjectViewImages.Create(IDB_MEA_ELE_LARGE, 16, 0, RGB(255, 0, 255));
	m_wndProjectTree.SetImageList(&m_ProjectViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	
	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillProjectTreeView();
	AdjustLayout();

	return 0;
}

void CProjectView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CProjectView::FillProjectTreeView()
{
	UCI::sDB.MySQLFillProjectTree(&m_wndProjectTree);
}

void CProjectView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndProjectTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������: 
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

HTREEITEM CProjectView::GetCurrentSelHTREEITEM()
{
	return this->m_wndProjectTree.GetSelectedItem();
	//return HTREEITEM();
}

// ���ص�ǰѡ���Ԫ��ID
//int CProjectView::GetCurrentSelElementID()
//{
//	HTREEITEM hCurrentSel = this->m_wndProjectTree.GetSelectedItem();
//
//	if (hCurrentSel == NULL) {
//		return -1;         // û��ѡ��
//	}
//
//	HTREEITEM parent = m_wndProjectTree.GetParentItem(hCurrentSel);
//	if (parent == NULL) {   // ����Ǹ���Ŀ
//		return -1;
//	}	
//	CString stParentName = m_wndProjectTree.GetItemText(parent);
//	if (stParentName == L"������Ŀ") {
//			return -1;     // �����Ŀ
//	}
//	
//	return (int)this->m_wndProjectTree.GetItemData(hCurrentSel);
//
//}

void CProjectView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndProjectTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CProjectView::OnProperties()
{
	AfxMessageBox(_T("����...."));

}

// ��һ��������Ŀ
void CProjectView::OnProjectOpen()
{
	HTREEITEM it = m_wndProjectTree.GetSelectedItem();

	CProjectTree* pTree = &this->m_wndProjectTree;

	HTREEITEM parent = pTree->GetParentItem(it);
	if (parent == nullptr) {   // ����Ǹ�Ŀ¼
		return;
	}

	if (pTree->GetParentItem(parent) == nullptr) {         // �������Ŀ

		CString projectName = pTree->GetItemText(it);      // ��Ŀ����

		int pold = UCI::OPTIONS[L"��Ŀ���"];

		int prID = (int)pTree->GetItemData(it);              // ��Ŀ���

		if (pold == prID) {
			return;
		}

		UCI::MyLogWinHead(projectName, prID);
		UCI::OPTIONS[L"��Ŀ���"] = prID;

		UCI::ELE.LoadElementFromDatabase(prID);                // �����ݿ��м���Ԫ��

		UCI::CopyCaliPoseAndParamterFile(pold, prID);          // ����ԭ���������̬���ڲε����ڵ��ļ�����

		//UCI::LoadCaliPoseAndParamter();                        // ������̬������ڲ�


		if (UCI::ELE.EleList.GetCount() == 0) {                 // ����������ʼԪ�أ���������ӣ�ƥ��
			UCI::InitEmptyProject(prID);
		}
		UCI::pMain->MProjectTreeFillByPrID();                   // ��ʼ����Ŀ��
	}
}

void CProjectView::OnFileOpenWith()
{
	// TODO: �ڴ˴���������������
}

void CProjectView::OnDummyCompile()
{
	// TODO: �ڴ˴���������������
}

void CProjectView::OnEditCut()
{
	// TODO: �ڴ˴���������������
}

void CProjectView::OnEditCopy()
{
	// TODO: �ڴ˴���������������
	this->m_wndProjectTree.m_CurrentSelectItem = m_wndProjectTree.GetSelectedItem();

	// UCI::MyLog(L"��ǰ�Ѹ�����һ����Ŀ��");
}


void CProjectView::OnProjectRename()
{
	// TODO: �ڴ���������������
	//AfxMessageBox(_T("����...."));

	HTREEITEM hItem = m_wndProjectTree.GetSelectedItem();

	//CString te = m_wndProjectView.GetItemText(hItem);

	//int a = 0;
	m_wndProjectTree.SetItemText(hItem, L"������");
}

// ɾ��һ����Ŀ,��ɾ��һ��Ԫ��
void CProjectView::OnEditClear()
{
	if (!UCI::pDis->TestCurrentAppState(APP_LOGGING_OK)) return;
	
	HTREEITEM hCurrentSel = m_wndProjectTree.GetSelectedItem();

	int prID = UCI::OPTIONS[L"��Ŀ���"];

	if (m_wndProjectTree.GetChildItem(hCurrentSel) == NULL) {                                 // 

		// û��չ������Ŀ��Ҳ�յ��ۣ���
		HTREEITEM parent = m_wndProjectTree.GetParentItem(hCurrentSel);
		if (parent != NULL) {
			CString stParentName = m_wndProjectTree.GetItemText(parent);
			if (stParentName == L"������Ŀ") {
				//UCI::MyLog(L"��Ŀ��ʱ����ɾ����");

				int delPrID = (int)m_wndProjectTree.GetItemData(hCurrentSel);
				if (delPrID == prID) {
					MessageBox(L"��ǰ��Ŀ����ɾ����");
					return;
				}

				// ����һ��
				int res = ::MessageBox(NULL,
					L"��� ȷ��  ɾ����\n"
					"��� ȡ��  ���ء�",
					L"������ɾ��ѡ������Ŀ��",
					MB_OKCANCEL | MB_ICONSTOP);
				if (res == IDCANCEL) return;

				UCI::sDB.DeleteProject(delPrID);
				return; 
			}
		}

		// �õ�Ԫ�ص�ELE���
		CString stCurSelName = m_wndProjectTree.GetItemText(hCurrentSel);

		UCI::ELE.RemoveOneElement(stCurSelName);                // ɾ��һ��Ԫ��

		this->UpdataProjectTreeByList();		
	}
}

void CProjectView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndProjectTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CProjectView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndProjectTree.SetFocus();
}

void CProjectView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_ProjectViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ProjectViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ProjectViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndProjectTree.SetImageList(&m_ProjectViewImages, TVSIL_NORMAL);
}

int CProjectView::EmptyProjectTree(int prID)
{
	// ���ҵ�ǰ��Ŀ���Ľڵ�
	HTREEITEM hProject = m_wndProjectTree.findProjectItemByPrID(prID);
	this->m_wndProjectTree.DelAllByPrID(prID);
	return TRUE;
}

// ˢ��һ����Ŀ�����нڵ�
int CProjectView::UpdataProjectTreeByList()
{

	int prID = UCI::OPTIONS[L"��Ŀ���"];

	// ���ҵ�ǰ��Ŀ���Ľڵ�
	HTREEITEM hProject = m_wndProjectTree.findProjectItemByPrID(prID);

	if (hProject != NULL) {

		this->m_wndProjectTree.DelAllByPrID(prID);

		//

		// �������и���Ŀ��Ԫ��
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *o = &UCI::ELE.EleList.GetNext(pos);
			this->m_wndProjectTree.AddOneElement(o, NULL);
		}
	}
	
	return 0;
}

UCI::OptionsMap* CProjectView::AddOneElementToProjectTree(UCI::OptionsMap * o)
{
	// 1. �ȵ�����ǰѡ��Ľڵ�
	HTREEITEM hCurrentSel = this->m_wndProjectTree.GetSelectedItem();
	if (hCurrentSel != NULL) {
		HTREEITEM hParent = this->m_wndProjectTree.GetParentItem(hCurrentSel);
		if (hParent != NULL) {
			CString name = this->m_wndProjectTree.GetItemText(hParent);
			if (name == L"������Ŀ") { // ����һ����Ŀ�ڵ�
			}
			else {  // ����һ��Ԫ�ؽڵ�
				//POSITION pos = (POSITION)this->m_wndProjectTree.GetItemData(hCurrentSel);
				return this->m_wndProjectTree.AddOneElement(o, hCurrentSel);
			}
		}
	}


	return this->m_wndProjectTree.AddOneElement(o,NULL);
		

}

// ճ���ڵ�
void CProjectView::OnProjectTreePaste() // ճ��һ���ڵ㣬�п�������Ŀ���п�����һ��Ԫ��
{
	// TODO: �ڴ���������������
	if (this->m_wndProjectTree.m_CurrentSelectItem != NULL) {

		HTREEITEM it = this->m_wndProjectTree.m_CurrentSelectItem;
		CProjectTree* pTree = &this->m_wndProjectTree;

		HTREEITEM parent = pTree->GetParentItem(it);

		if (parent == nullptr) {   // ��Ŀ¼���ܸ���
			return; 
		}	

		CString parentName = pTree->GetItemText(parent);
		if (parentName == L"������Ŀ") { // ��Ŀ�ڵ�
			
			CString stOld = pTree->GetItemText(it);

			int nImage, nSelectedImage;
			pTree->GetItemImage(it, nImage, nSelectedImage);

			DWORD_PTR id = pTree->GetItemData(it);

			UINT mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			CString newName = stOld + L"����";

			// Ȼ�������ݿ����һ����Ŀ
			int NewProjectID;
			UCI::sDB.MyNewProject(newName, NewProjectID);

			pTree->InsertItem(mask, newName, nImage, nSelectedImage, 0, 0, NewProjectID, parent, it);

			// �����ڵ�Ԫ�ر��浽����Ŀ��ȥ
			UCI::stElement newELE;

			POSITION pos = UCI::ELE.EleList.GetHeadPosition();
			while (pos != NULL) {
				UCI::OptionsMap ot = UCI::ELE.EleList.GetNext(pos);				
				ot[L"��Ŀ���"] = NewProjectID;
				newELE.EleList.AddTail(ot);
			}

			UCI::sDB.MySaveAllElement(NewProjectID, &newELE);
		}
		else {  // Ԫ�ؽڵ�

		}

	}
}
