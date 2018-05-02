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

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "CMV.h"
#include "MyData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace HalconCpp;
using namespace UCI;
using namespace std;

IMPLEMENT_DYNAMIC(NodeCAMProperty, CMFCPropertyGridProperty)

BOOL NodeCAMProperty::HasButton() const
{
	return(m_dwFlags & AFX_PROP_HAS_LIST) ||
		(m_dwFlags & AFX_PROP_HAS_BUTTON);
}


//enum ePROPERY_TYPE {
//	PROPRETY_GLOBAL,             // ȫ������ 
//	PROPRETY_CAMMRA,             // �������
//	PROPRETY_ELEMENT             // Ԫ������
//};
// ��ʼ�����Ա�
NodeCAMProperty::NodeCAMProperty(UCI::OptionsMap & om, ePROPERY_TYPE pType)
	:CMFCPropertyGridProperty(_T("tmp"))
{
	
	switch (pType) {
	case PROPRETY_GLOBAL:
		this->SetName(L"ȫ�ֱ���");
		initGobleProperty(om);
		break;
	case PROPRETY_ELEMENT:
		this->SetName(L"Ԫ������");
		initProtertyProperty(om);
		break;
	default:
		break;
	}
}
void NodeCAMProperty::initGobleProperty(UCI::OptionsMap & om)
{
	try {

		CMFCPropertyGridProperty* pProp;
		for (size_t idx = 0; idx < om.size(); ++idx) {
			for (auto&it : om) {
				if (it.second.getIdx() == idx) {

					UCI::Option o = it.second;

					bool show = o.getshow();
					if (show == true) {

						CString name = it.first;
						CString type = o.getType();
						CString val = o.getcurrentValue();
						CString note = o.getNote();
						int index = (int)o.getIdx() + 100;

						if (type == L"spin") {
							pProp = new CMFCPropertyGridProperty(name,
								(_variant_t)_ttoi(val), note, index);
							pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
							this->AddSubItem(pProp);
						}
						else if (type == L"double") {
							pProp = new CMFCPropertyGridProperty(name,
								(_variant_t)_ttof(val), note, index);
							this->AddSubItem(pProp);
						}
						else if (type == L"string") {
							pProp = new CMFCPropertyGridProperty(name,
								(_variant_t)val, note, index);

							pProp->AllowEdit(TRUE);

							if (name == L"���Э��") {
								pProp->AddOption(_T("MindVision12_X64"));
								pProp->AddOption(_T("DirectShow"));
								pProp->AllowEdit(FALSE);
							}
							else if (name == L"�������") {
								CString sCamInterFace = UCI::OPTIONS[L"���Э��"];
								HTuple CamInfo;
								HTuple CamValues;
								InfoFramegrabber(CT2A(sCamInterFace).m_psz, "device", &CamInfo, &CamValues);
								int b = (int)CamValues.Length();   // 
								for (int i = 0; i < b; i++) {
									HTuple t = CamValues[i];
									CString CSt;
									CSt.Format(L"%S", t.SArr()[0]);
									pProp->AddOption(CSt);
								}
							}						   
							this->AddSubItem(pProp);
						}						
					}
				}
			}
		}
	}
	catch (HalconCpp::HException& except) {
		UCI::sDB.Log(except, L"nitGobleProperty��");

	}
}


// ��ʼ��Ԫ�ص������б�
void NodeCAMProperty::initProtertyProperty(UCI::OptionsMap & om)
{
	
	// ÿ��Ԫ�ص����Ͳ�һ�����ֱ����һ��
	int etype = (om)[L"Ԫ������"];
	switch (etype) {
	case ELEMENT_LINE:
		initProtertyLine(om);
		break;
	case ELEMENT_CIRCLE:
		initProtertyCircle(om);
		break;
	case ELEMENT_ANGLE:
		initProtertyAngle(om);
		break;
	case ELEMENT_DISTANCE:           // ����
		initProtertyDistance(om);
		break;
	case ELEMENT_CAMERA:
		initProtertyCamera(om);
		break;
	case ELEMENT_RESULT:
		initProtertyResult(om);
		break;
	case ELEMENT_MM:
		initProtertyMM(om);
		break;
	case ELEMENT_PEAK:
		initProtertyPeek(om);
		break;
	case ELEMENT_BALLOONLENGTH:
		initProtertyBalloonLength(om);
		break;
	case ELEMENT_FUNCTION:
		initProtertyFUNCTION(om);
		break;
	case ELEMENT_MATCH:
		initProtertyMATCH(om);
		break;
	case ELEMENT_MEASURE_RECTANGLE2:
		initProtertyMEASURE_RECTANGLE2(om);
		break;
	case ELEMENT_BALLOON_FLAW:
		initPropertyBalloonFlaw(om);
		break;
	case ELEMENT_ROI:
		initPropertyROI(om);
		break;
	default:
		break;
	}
	
	
}

void NodeCAMProperty::initProtertyCamera(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {


					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ����༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ����༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(true);    // ����༭
					    if (name == L"ɫ�ʿռ�") {							
							pProp->AddOption(L"Gray");
							pProp->AddOption(L"BGR24");							
						}
						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyLine(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {

					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
						if (name == L"����������")
						{
							pProp->AllowEdit(TRUE);                      // ����༭
						}
						else if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyCircle(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {
					CString  name = it.first;
					CString  type = o.getType();
					CString  val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ����༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
						if (name == L"����������")
						{
							pProp->AllowEdit(TRUE);                      // ����༭
						}
						else if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Spoke_Circle_Direction") {							//pProp->AllowEdit(TRUE);
							pProp->AddOption(L"inner");
							pProp->AddOption(L"outer");
						}

						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

// ����
void NodeCAMProperty::initProtertyPeek(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {

					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());

						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(true);    // ������༭
						if (name == L"����������")
						{
							pProp->AllowEdit(TRUE);                      // ����༭
						}
						else if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

// ���ҳ���
void NodeCAMProperty::initProtertyBalloonLength(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {
					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(false);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());	
						if (name == L"USE_Peek1_Indentify") {
							pProp->SetData(USE_Peek1_Element_Name_ID);
						}
						else if (name == L"USE_Peek2_Indentify") {
							pProp->SetData(USE_Peek2_Element_Name_ID);
						}
						else if (name == L"USE_Peek3_Indentify") {
							pProp->SetData(USE_Peek3_Element_Name_ID);
						}
						else if (name == L"USE_Peek4_Indentify") {
							pProp->SetData(USE_Peek4_Element_Name_ID);
						}

						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭

						if (name == L"RES_���ҳ���") {
							pProp->AllowEdit(false);
						}

						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(TRUE);    // ����༭						
				        if (name == L"���ö���1" // ��ֱ�߲���
							|| name == L"���ö���2"
							|| name == L"���ö���3"
							|| name == L"���ö���4")
						{							
							pProp->AllowEdit(false);
							insertCurrentPosCanUseElement(pProp, om);   // ���ҵ�ǰ�����п����Ԫ��	
						}
						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyAngle(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {


					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(false);    // ������༭
						//pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
                        if (name == L"USE_LINE1_Indentify") {
							pProp->SetData(USE_LINE1_Name_Identify);
						}
						else if (name == L"USE_LINE2_Indentify") {
							pProp->SetData(USE_LINE2_Name_Identify);
						}
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(false);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
						if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"USE_LINE1_Parament_Name" // ��ֱ�߲���
							|| name == L"USE_LINE2_Parament_Name")
						{
							insertCurrentPosCanUseElement(pProp, om);   // ���ҵ�ǰ�����п����Ԫ��		
						}
						this->AddSubItem(pProp);

					}

				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyDistance(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {


					CString  name = it.first;
					CString  type = o.getType();
					CString  val = o.getcurrentValue();
					CString  note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);						
						
						if (name == L"����Ԫ��1 ID") {
							pProp->SetData(USE_Distance1_Name_Identify);
							pProp->AllowEdit(false);    // ������༭
						}
						else if (name == L"����Ԫ��2 ID") {
							pProp->SetData(USE_Distance2_Name_Identify);
							pProp->AllowEdit(false);    // ������༭
						}
						else {
							pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
							pProp->AllowEdit(true);    // ����༭
						}
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);

						if (name == L"RES_����") {
							pProp->AllowEdit(false);    // ������༭
						}
						else {
							pProp->AllowEdit(true);    // ����༭
						}						
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
                        if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"����Ԫ������1" // ��ֱ�߲���
							|| name == L"����Ԫ������2")
						{
							insertCurrentPosCanUseElement(pProp, om);   // ���ҵ�ǰ�����п����Ԫ��	
						}
						else if (name == L"����Ԫ��1������") {
							pProp->SetData(USE_Distance1_Parament_Name_ID);
						}
						else if(name == L"����Ԫ��2������") {   // Ҫѡ���Ԫ���������
							pProp->SetData(USE_Distance2_Parament_Name_ID);
						}

						this->AddSubItem(pProp);

					}


				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyFUNCTION(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {


					CString  name = it.first;
					CString  type = o.getType();
					CString  val = o.getcurrentValue();
					CString  note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
						if (name == L"����Ԫ��1 ID") {
							pProp->SetData(USE_Distance1_Name_Identify);
						}
						else if (name == L"����Ԫ��2 ID") {
							pProp->SetData(USE_Distance2_Name_Identify);
						}
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(true);    // ������༭
						if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						this->AddSubItem(pProp);

					}


				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyMATCH(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {


					CString  name = it.first;
					CString  type = o.getType();
					CString  val = o.getcurrentValue();
					CString  note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());

						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(true);    // ������༭
						if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}

						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyResult(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {
					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
						if (name == L"�����") {
							//pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());							
						}
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						if (name == L"����ı�׼ֵ"
							|| name == L"OUT_Elemnt_Upper_Limit"
							|| name == L"OUT_Elemnt_Lower_Limit") {
							//pProp->AllowEdit(TRUE);    // ����༭
						}
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
						if (name == L"����������")
						{
							pProp->AllowEdit(TRUE);                      // ����༭
						}
						else if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
							pProp->SetData(OUT_Element_Note_ID);
						}
						else if (name == L"���Ԫ������")		// RESULT_ELEMENT Ԫ��					
						{
							insertCurrentPosCanUseElement(pProp, om);   // ���ҵ�ǰ�����п����Ԫ��
						}
						else if (name == L"���Ԫ�صĲ���") {   // Ҫѡ���Ԫ���������
							pProp->SetData(OUT_Element_Name_ID);
						}

						this->AddSubItem(pProp);

					}
				}
			}
		}
	}
}

void NodeCAMProperty::initProtertyMM(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {

					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());
						if (name == L"�����") {
							//pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());							
						}
						else if (name == L"У��Ԫ�ص�ID X") {
							pProp->SetData(OUT_Element_Name_IdentifyX);
						}
						else if (name == L"У��Ԫ�ص�ID Y") {
							pProp->SetData(OUT_Element_Name_IdentifyY);
						}
						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						if (name == L"����ı�׼ֵ")
					   {
							//pProp->AllowEdit(TRUE);    // ����༭
						}
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
						if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
							pProp->SetData(OUT_Element_Note_ID);
						}
						else if (name == L"У��Ԫ�� X")		// RESULT_ELEMENT Ԫ��					
						{
							insertCurrentPosCanUseElement(pProp, om);   // ���ҵ�ǰ�����п����Ԫ��	
						}
						else if (name == L"У��Ԫ�صĲ��� X") {   // Ҫѡ���Ԫ���������
							pProp->SetData(OUT_Element_Name_IDX);
						}
						else if (name == L"У��Ԫ�� Y")		// RESULT_ELEMENT Ԫ��					
						{
							insertCurrentPosCanUseElement(pProp, om);   // ���ҵ�ǰ�����п����Ԫ��	
						}
						else if (name == L"У��Ԫ�صĲ��� Y") {   // Ҫѡ���Ԫ���������
							pProp->SetData(OUT_Element_Name_IDY);
						}

						this->AddSubItem(pProp);

					}
				}
			}
		}
	}
}

// 
void NodeCAMProperty::initProtertyMEASURE_RECTANGLE2(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {

					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());

						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
						if (name == L"����������")
						{
							pProp->AllowEdit(TRUE);                      // ����༭
						}
						else if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

void NodeCAMProperty::initPropertyBalloonFlaw(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {

					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());

						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(FALSE);    // ������༭
						if (name == L"����������")
						{
							pProp->AllowEdit(TRUE);                      // ����༭
						}
						else if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

void NodeCAMProperty::initPropertyROI(UCI::OptionsMap & om)
{
	CMFCPropertyGridProperty* pProp;

	for (size_t idx = 0; idx < om.size(); ++idx) {
		for (auto&it : om) {
			if (it.second.getIdx() == idx) {

				UCI::Option o = it.second;

				bool show = o.getshow();
				if (show == true) {

					CString name = it.first;
					CString type = o.getType();
					CString val = o.getcurrentValue();
					CString note = o.getNote();
					int index = (int)o.getIdx() + 100;

					if (type == L"spin") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttoi(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						pProp->EnableSpinControl(TRUE, o.getimin(), o.getimax());

						this->AddSubItem(pProp);
					}
					else if (type == L"double") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)_ttof(val), note, index);
						pProp->AllowEdit(true);    // ������༭
						this->AddSubItem(pProp);
					}
					else if (type == L"string") {
						pProp = new CMFCPropertyGridProperty(name,
							(_variant_t)val, note, index);
						pProp->AllowEdit(true);    // ������༭
						if (name == L"����������")
						{
							pProp->AllowEdit(TRUE);                      // ����༭
						}
						else if (name == L"������")
						{
							pProp->AllowEdit(TRUE);
						}
						else if (name == L"Ԫ�ر�ע")
						{
							pProp->AllowEdit(TRUE);
						}
						this->AddSubItem(pProp);
					}
				}
			}
		}
	}
}

void NodeCAMProperty::insertCurrentPosCanUseElement(CMFCPropertyGridProperty * pProp, UCI::OptionsMap & om)
{
	int emtype = om[L"Ԫ������"];
	CString omName = om[L"Ԫ������"];

	switch (emtype) {
	//case ELEMENT_BALLOON_FLAW:
	//{
	//	POSITION pos = UCI::ELE.EleList.GetHeadPosition();
	//	while (pos != NULL) {
	//		UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
	//		CString otName = (*ot)[L"Ԫ������"];
	//		if (omName == otName)
	//			break;
	//		int  otType = (*ot)[L"Ԫ������"];
	//		if (otType == ELEMENT_PEAK) {
	//			pProp->AddOption(otName);
	//		}
	//	}
	//}
		//break;
	case ELEMENT_BALLOONLENGTH:
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString otName = (*ot)[L"Ԫ������"];
			if (omName == otName) 
				break;
			int  otType = (*ot)[L"Ԫ������"];
			if (otType == ELEMENT_PEAK) {
				pProp->AddOption(otName);
			}
		}
	}
		break;
	case ELEMENT_ANGLE:
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString otName = (*ot)[L"Ԫ������"];
			if (omName == otName)
				break;
			int  otType = (*ot)[L"Ԫ������"];
			if (otType == ELEMENT_LINE) {
				pProp->AddOption(otName);
			}
		}
	}
		break;
	case ELEMENT_DISTANCE:
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString otName = (*ot)[L"Ԫ������"];
			if (omName == otName)
				break;
			int  otType = (*ot)[L"Ԫ������"];
			if (otType == ELEMENT_LINE || otType == ELEMENT_CIRCLE || otType == ELEMENT_ANGLE) {
				pProp->AddOption(otName);
			}
		}
	}
		break;
	case  ELEMENT_RESULT:
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString otName = (*ot)[L"Ԫ������"];
			if (omName == otName)
				break;
			int  otType = (*ot)[L"Ԫ������"];
			if (otType == ELEMENT_LINE
				|| otType == ELEMENT_DISTANCE
				|| otType == ELEMENT_ANGLE
				|| otType == ELEMENT_BALLOONLENGTH
				|| otType == ELEMENT_MEASURE_RECTANGLE2
				|| otType == ELEMENT_BALLOON_FLAW
				|| otType == ELEMENT_CIRCLE) {
				pProp->AddOption(otName);
			}
		}
	}
		break;
	case ELEMENT_MM:
	{
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString otName = (*ot)[L"Ԫ������"];
			if (omName == otName)
				break;
			int  otType = (*ot)[L"Ԫ������"];
			if (otType == ELEMENT_RESULT) {
				pProp->AddOption(otName);
			}
		}
	}
		break;
	case ELEMENT_MEASURE_RECTANGLE2:
	{

	}
		break;
	default:
		break;
	}
}

NodeCAMProperty::NodeCAMProperty(int ID, double x, double y, double z)
	:CMFCPropertyGridProperty(_T("tmp"))
{
	CString name(_T("�ڵ�"));
	CString tmpStr;
	tmpStr.Format(_T("%i,(%lf,%lf,%lf)"), ID, x, y, z);
	name += tmpStr;
	this->SetName(name);

	CString valueStr;
	valueStr.Format(_T("%lf,%lf,%lf"), x, y, z);
	COleVariant varCoord(valueStr);
	//this->SetValue(var);

	CString idStr;
	idStr.Format(_T("%i"), ID);
	COleVariant varID(idStr);

	this->AddSubItem(new CMFCPropertyGridProperty(_T("�ڵ���"), varID));
	this->AddSubItem(new CMFCPropertyGridProperty(_T("����"), varCoord));
};


