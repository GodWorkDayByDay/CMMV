#include "stdafx.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "ProjectView.h"

#include <algorithm>
#include <sstream>

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;

using namespace std;
using std::string;

namespace UCI {





	// ���ϵ��²飬���������Ԫ�������˲����ڵ�Ԫ�أ�9�����Ԫ��ɾ��������FALSE;
	BOOL stElement::isELeListOKandDelBad(OptionsMap *o)
	{
		POSITION pos = ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			POSITION oldPos = pos;
			OptionsMap *o = &ELE.EleList.GetNext(pos);   // �����POSȡֵ����Զ�ָ����һ������
		int etype = (*o)[L"Ԫ������"];
			switch (etype) {
			case ELEMENT_ANGLE:     // �ɶ�������ɣ�
			{
				CString name;
				name = (*o)[L"USE_LINE1_Parament_Name"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
					UCI::ELE.EleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)[L"USE_LINE2_Parament_Name"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
				UCI::ELE.EleList.RemoveAt(oldPos);
				return FALSE;
				}
			}
			break;
			case ELEMENT_DISTANCE:
			{
				CString name;
				name = (*o)[L"����Ԫ������1"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
					UCI::ELE.EleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)[L"����Ԫ������2"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
				UCI::ELE.EleList.RemoveAt(oldPos);
				return FALSE;
				}
			}
			break;
			case ELEMENT_RESULT:               // ����������Ԫ�صĽ��
			{
				CString name;
				name = (*o)[L"���Ԫ������"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
					UCI::ELE.EleList.RemoveAt(oldPos);
					return FALSE;
				}
			}
			break;
			case ELEMENT_BALLOONLENGTH:           // ����������Ԫ�صĽ��
			{
				CString name;
				name = (*o)[L"���ö���1"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
					UCI::ELE.EleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)[L"���ö���2"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
					UCI::ELE.EleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)[L"���ö���3"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
					UCI::ELE.EleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)[L"���ö���4"];
				if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
					UCI::ELE.EleList.RemoveAt(oldPos);
					return FALSE;
				}
			}
			break;
			case ELEMENT_MM:                    // ����������Ԫ�صĽ��
			{
				//CString name;
				//name = (*o)[L"У��Ԫ�صĲ��� X"];
				//if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
				//	UCI::ELE.EleList.RemoveAt(oldPos);
				//	return FALSE;
				//}
				//name = (*o)[L"У��Ԫ�صĲ��� Y"];
				//if (name != L"" && GetOptionsMapByEleName(name) == nullptr) {
				//	UCI::ELE.EleList.RemoveAt(oldPos);
				//	return FALSE;
				//}
				//return FALSE;
			}
			break;
			default:
				break;
			}		
		}
		return TRUE;
	}

	// ɾ��һ��Ԫ��

	BOOL stElement::RemoveOneElement(CString stEleName)
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			POSITION prepos = pos;
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString otName = (*ot)[L"Ԫ������"];
			if (otName == stEleName) {
				UCI::ELE.EleList.RemoveAt(prepos);
				break;
			}
		}

		// ��Ҫɾ����ص��������Ԫ�ص�����Ԫ��
		pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			OptionsMap *o = &ELE.EleList.GetNext(pos);
			if (isELeListOKandDelBad(o) == FALSE) {    
				pos = ELE.EleList.GetHeadPosition();   // �Ѿ�ɾ���ˣ�Ҫ���¸���һ��
			}
		}		

		return TRUE;
	}

	OptionsMap * stElement::GetOptionsMapByEletype(eELEMENT_TYPE e)
	{

		POSITION pos = EleList.GetHeadPosition();
		while (pos != NULL) {
			OptionsMap *o = &EleList.GetNext(pos);
			//////////////////////////////////////////////////
			// ��ÿһ��Ԫ�ؽ��в���	
			int etype = (*o)[L"Ԫ������"];

			if (etype == e) return o;
		}
		return nullptr;
	}

	OptionsMap * stElement::GetOptionsMapByEleName(CString fname)
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString name = (*ot)[L"Ԫ������"];
			if (fname == name)
			{
				return ot;
			}
		}		
		return nullptr;
	}

	POSITION stElement::GetOptionsPosByEleName(CString fname)
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			POSITION oldpos = pos;
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString name = (*ot)[L"Ԫ������"];
			if (fname == name)
			{
				return oldpos;
			}
		}
		return NULL;
	}

}