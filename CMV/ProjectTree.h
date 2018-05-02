
#include "MyData.h"
#pragma once


// CProjectView ��ͼ


class CProjectTree : public CTreeCtrl
{

	// ����
public:
	CProjectTree();

	//UCI::OptionsMap *CurProp = NULL;     // ��ǰԪ��
	//int curElementPropertyIndex = 0;             // ��ǰ������ID

	// ��ǰѡ���Item
	HTREEITEM m_CurrentSelectItem = NULL;	

	HTREEITEM findProjectItemByPrID(int prID);

	void DelAllByPrID(int prID);

	BOOL AddOneElementTail(UCI::OptionsMap *o);

	UCI::OptionsMap* AddOneElement(UCI::OptionsMap *o, HTREEITEM hCurrentSel);
	


	// ��д
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// ʵ��
public:
	virtual ~CProjectTree();

private:
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


