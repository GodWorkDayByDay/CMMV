
#pragma once

#include "ProjectTree.h"
#include "MyDispLay.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CProjectView : public CDockablePane
{
// ����
public:
	CProjectView();

	//UCI::OptionsMap* getCurProperyMap() { return this->m_wndProjectTree.CurProp; };
	//void setCurProperyMap(UCI::OptionsMap* o) { this->m_wndProjectTree.CurProp = o; };


	/*int GetCurElementPropertyIndex() { return this->m_wndProjectTree.curElementPropertyIndex; };
	void SetCurElementPropertyIndex(int cur) { this->m_wndProjectTree.curElementPropertyIndex = cur; };*/

	HTREEITEM GetCurrentSelHTREEITEM();        // ���ص�ǰѡ���Ҷ�ڵ�
	//int GetCurrentSelElementID();              // �õ���ǰѡ���Ԫ�ص�ID

	void AdjustLayout();
	void OnChangeVisualStyle();

	int EmptyProjectTree(int prID);           // �����Ŀ��
	int UpdataProjectTreeByList();
	UCI::OptionsMap* AddOneElementToProjectTree(UCI::OptionsMap* o);

// ����
protected:

	CProjectTree m_wndProjectTree;
	CImageList m_ProjectViewImages;
	CFileViewToolBar m_wndToolBar;

public:
	void FillProjectTreeView();

// ʵ��
public:
	virtual ~CProjectView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnProjectOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnProjectRename();

	//void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnProjectTreePaste();
};

