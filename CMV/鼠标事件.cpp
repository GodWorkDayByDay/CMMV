// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "CMV.h"
#include "ChildView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace HalconCpp;

// ������
//
//BOOL CChildView::OnMouseWheel2(UINT nFlags, short zDelta, CPoint pt)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	
//	
//
//	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
//}


//// �������
//
//void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	//CPoint pt;
//	//GetCursorPos(&pt);
//	//ScreenToClient(&pt);
//
//	//int num;
//	//if (this->MouseInWhichDISP(point, num)) {
//
//	//	this->DISP[num].DisOnMouseLeftDown(nFlags, point);
//	//}
//
//	DISP.DisOnMouseLeftDown(nFlags, point);
//
//	CWnd::OnLButtonDown(nFlags, point);
//}
//
//// ���˫��
//
//void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	//int num;
//	//if (this->MouseInWhichDISP(point, num)) {
//
//	//	if (this->eScreenMode == ScreenOneLarge) {
//	//		if (this->iScreenWhich == num) {
//	//			this->eScreenMode = ScreenALlSame;
//	//		}
//	//	}
//	//	else {
//	//		this->eScreenMode = ScreenOneLarge;
//	//	}
//
//	//	this->iScreenWhich = num;
//	//	DISP[num].bMouseLBdown = false;
//
//	//	this->FitImageToClient();
//	//}
//
//
//	CWnd::OnLButtonDblClk(nFlags, point);
//}
//
//// ����ƶ�
//
//void CChildView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	// CClientDC DC(this);
//
//	//int num;
//	//if (this->MouseInWhichDISP(point, num)) {
//	//	this->DISP[num].DisOnMouseMove(nFlags, point);
//	//}
//
//	CWnd::OnMouseMove(nFlags, point);
//}
//
//// �Ҽ�����
//
//void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	//int num;
//	//if (this->MouseInWhichDISP(point, num)) {
//	//	this->DISP[num].DisOnRButtonUp(nFlags, point);
//	//}
//
//	CWnd::OnRButtonUp(nFlags, point);
//}