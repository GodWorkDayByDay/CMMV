#pragma once

#include "MyDispLay.h"

class CPictureView : public CDockablePane
{
	// ����
public:
	CPictureView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	//ST_MYDISP* pDis = NULL;



	void InitPictureStatic(ST_MYDISP* pD);

	// ����
protected:

protected:
	void FillFileView();

	// ʵ��
public:
	virtual ~CPictureView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};
