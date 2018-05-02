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
#include "MyData.h"
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList ����

class COutputList : public CListBox
{
// ����
public:
	COutputList();

// ʵ��
public:
	virtual ~COutputList();
	

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// ����
public:
	COutputWnd();

	void UpdateFonts();

	void LogOutBuild(CString str, bool isSave) {
		int iSel = m_wndOutputBuild.AddString(str);
		m_wndOutputBuild.SetCurSel(iSel);

		if(isSave)
		   UCI::sDB.Log(str);
	};	

	void ResDeleAllItems() {
		m_wndDataList.DeleteAllItems();
	}

	void insertOneResult(UCI::OptionsMap* o);   // ���һ���������
		

// ����
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

	//CMFCPropertyGridCtrl

	CListCtrl m_wndDataList;       // ���������б�ؼ�

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// ʵ��
public:
	virtual ~COutputWnd();
	void OnViewOutput();

private:


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	

	DECLARE_MESSAGE_MAP()
};

