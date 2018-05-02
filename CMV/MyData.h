#pragma once

#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

#include "Common.h"
#include <fstream>
#include <sstream>
#include <xstring>
#include <afxtempl.h>

#include "SQLiteCpp/SQLiteCpp.h"
#include "SQLiteCpp/VariadicBind.h"

#include <map>

class CMainFrame;
class CProjectTree;
class ST_MYDISP;

namespace UCI {

	class Option;
	class ST_USER;
	class SQLBASE;
	class RIBBON_MENU;
	class stElement;

	/// Custom comparator because UCI options should be case insensitive
	//struct CaseInsensitiveLess {
	//	bool operator() (const std::string&, const std::string&) const;
	//};

	/// Our options container is actually a std::map
	typedef std::map<CString, Option> OptionsMap;


	class RIBBON_MENU {

	public:

		RIBBON_MENU();

		bool Rib_CALI_CAPTURE;                                // �궨ͼ��ʱ���Ƿ��ץͼ��
		bool bIsInROIdrawing;                                 // ��ǰ�ǲ����� ROI ��ͼ��

		eCOMBO_MATCH_ROI_TYPE iCOMBO_MATCH_ROI_TYPE;          // ��ǰ������
	};

	class ST_PROJECT
	{
	public:

		ST_PROJECT(int id) { this->ID = id; };

		int ID;
		std::string ProjectName;
		std::string ProjectNote;
		std::string datatime;
	};

	class SQLBASE {


	public:
		//SQLBASE(std::string dbname);

	public:
		void SetDBName(std::string name);
		int MyNewProject(CString name, int& NewProjectID);                     // �����ݿ����½�һ��������Ŀ
		int MyProjectRename(CString newName, int NewProjectID);                // 

		//////////////////////////////////////////////////////////////
		/// Ԫ�������ݿ����
		int MyResetProjectElements(int p);                                      // �ѵ�ǰ��Ŀ������Ԫ��ɾ��������д��
		int RecordOneProdcut(int& productID);                                   //
		int RecordOneProdcutNotUpDateGUI(int& productID);                                   //
		int GetProductTotalAndGoodBy(int PrID, int start, int& total, int &good);
		int MySaveAllElement();                                                 // 
		int MySaveAllElement(int prID, stElement* e);
		int MySaveOneElement(OptionsMap& o, int);                               // ����һ��Ԫ�ص�Ԫ�ر���ȥ
		int MySaveOneElement(OptionsMap& o, int, SQLite::Database* pdb);

		int MyLoadElementFromDatabase(int prID);                                // ������Ŀ�е�Ԫ��
		int ClearProductResutlByProjectID(int prID); 
		int MyReadSettingFromSqlite(CString  CamName);                          // ������Ӧ����ĵ�ǰȫ������
		int ProjectFillByID(ST_PROJECT* pro);                                   // �������ݿ������Ŀ��Ϣ		
		int MySQLFillProjectTree(CProjectTree * cvt);                           // 
		int MySaveSettingToSqlite(OptionsMap& om);                              // ����ȫ�ֲ���
	
		int SetOneCamePara(CString Name, UCI::Option o);
		BOOL isCamParaCanOut(CString Name);                                     // ����Ĳ����ɲ��������
		int SetCamPara(OptionsMap& camo);                                       // ���ݵ�ǰ������Ĳ������ID���������	
		int DelAllElement(int prID);                                            // ɾ�����е�Ԫ��
		BOOL isCanlogin(CString name, CString pass);
		int GetLastProductTestNumber();
		void PrepareCamDeviceCatalog(int prID);                                  // ������������½�������Ŀʱ����׼����Ӧ��Ŀ¼

		int Log(CString log, eLogType e = LOG_LOG);                              // ����һ����־�ļ�
		int Log(HalconCpp::HException& except, CString str);                     // ��¼һ�� HALCON �Ĵ���
		int DeleteProject(int prID);                                             // ɾ��һ����Ŀ

		int MyFillUser(CList <CString, CString> *User);
		int GetUserInfoByName(ST_USER* pUser);
		int UpdateUserInfoBy(ST_USER* pUser);
		int AddUser(ST_USER* pUser);
		int DeleteUserByName(ST_USER* pUser);

		//SQLite::Database* db; // (this->dbname, SQLite::OPEN_READWRITE);

	private:

		

		std::string dbname;

		int SaveOneProductResult(int &ProductID);                              // ����һ����Ʒ������¼
		int UpdateOneProductResult(int ProductID, BOOL result);                // ���������Ʒ�Ĳ������
		int SaveOneElementResult(int ProductID, int index, OptionsMap& ores);

		
		int DeleteOneELementByName(CString name);  
		int DeleteOneGobalParaByName(CString name);

	};




	
	/// Option class implements an option as defined by UCI protocol
	class Option {

		typedef void(*OnChange)(Option&);     // ֵ�ı��ˣ�Ҫ������Ӧ�ĺ���ȥ����һ��

	public:
		
		Option(OnChange = nullptr);
		// Option(bool v, OnChange = nullptr);	
		Option(CString sname, const CString v, const CString note = L"Paramter Note",bool show = true, OnChange = nullptr);
		Option(CString sname, int v, int min, int max, const CString note = L"Paramter Note", bool show = true, OnChange = nullptr);
		Option(CString sname, double v, double dmin, double dmax, const CString note = L"Paramter Note", bool show = true, OnChange = nullptr);

		//Option(CString sname, int v, int min, int max, bool iscout, const CString note = L"Paramter Note", bool show = true, OnChange = nullptr);
		//Option(CString sname, const CString v, bool iscout, const CString note = L"Paramter Note", bool show = true, OnChange = nullptr);
		//Option(CString sname, double v, double dmin, double dmax, bool iscout, const CString note = L"Paramter Note", bool show = true, OnChange = nullptr);

		Option& operator=(const CString&);
		Option& operator=(const int&);
		Option& operator=(const double&);
		Option& operator=(const HalconCpp::HTuple&);

		//Option& operator=(const std::string&, const std::string&);
		//Option& operator=(const int&, const std::string&);
		//Option& operator=(const double&, const std::string&);

		void operator<<(const Option&);
		operator int() const;
		operator double() const;
		operator CString() const;	
		operator HalconCpp::HTuple() const;

		
		size_t getIdx() { return this->idx; };
		CString getName() { return this->name; };
		CString getType() { return this->type; };
		CString getcurrentValue() { return this->currentValue; };
		CString getNote() { return this->explain; };
		int getimin() { return this->imin; };
		int getimax() { return this->imax; };
		bool getshow() { return this->bshow; };
		//bool getCamIsOut() { return this->camIsOut; };

		void setNote(const CString str) { this->explain = str; };

		static size_t insert_order;

	private:
		//friend std::ostream& operator<<(std::ostream&, const OptionsMap&);

		
		CString name, defaultValue, currentValue, type, explain;
		int imin, imax;
		double dmin, dmax;
		size_t idx;
		bool bshow;                // �Ƿ���ʾ
		//bool camIsOut = false;     // �Ƿ�Ҫ��������������ȥ
		OnChange on_change;        // ��������
	};

	class stElement {

	public:		
		//friend int SQLDATA::MyLoadElementFromDatabase();
		//friend int CProjectView::UpdataByList();

	public:

		OptionsMap* add_One_Element(int iProject, eELEMENT_TYPE);                          // ���һ��Ԫ��

		BOOL isELeListOKandDelBad(OptionsMap *o);

		int LoadElementFromDatabase(int prID);                                             // �����ݿ��ж�����ǰ��Ŀ��Ԫ��
		int UpdateProjectByList();                                                         // ������Ŀ���ĵ�ǰ��Ŀ
		OptionsMap* initOptionMapElemnet(eELEMENT_TYPE t, int ProjectID);                  // Ԫ�س�ʼ��
		int saveOneOptionsMap(OptionsMap& o, int index);                                   // ����һ��Ԫ��Ҫ���ݿ���ȥ

		BOOL isHaveSaveElementName(CString str);                                            // �Ƿ�����ͬ������
		
		BOOL RemoveOneElement(CString stEleName);    
		OptionsMap *GetOptionsMapByEletype(eELEMENT_TYPE e);

		OptionsMap* GetOptionsMapByEleName(CString name);                                   // ͨ��Ԫ�����Ʋ���Ԫ��
		POSITION GetOptionsPosByEleName(CString name);


		// ����
	public:
		CList <OptionsMap, OptionsMap> EleList;                 // ����Ԫ��   
	private:
		void initOptionMapElemnetLine(OptionsMap& o);
		void initOptionMapElemnetCircle(OptionsMap& o);
		void initOptionMapElemnetResult(OptionsMap& o);
		void initOptionMapElemnetAngle(OptionsMap& o);             // ������ֱ��Ԫ�صĽǶ�
		void initOptionMapElemnetCamera(OptionsMap& o);            // ��ʼ��һ���������
		void initOptionMapElemnetDistance(OptionsMap& o);

		void initOptionMapFUNCTION(OptionsMap& o);
		void initOptionMapMATCH(OptionsMap& o);

		void initOptionMapElemnetMM(OptionsMap& o);
		void initOptionMapElemnetPeek(OptionsMap& o);
		void initOptionMapELEMENT_MEASURE_RECTANGLE2(OptionsMap& o);
		void initOptionMapBalloonFlaw(OptionsMap& o);
		void initOptionMapROI(OptionsMap& o);
		void initOptionMapBalloonLength(OptionsMap& o);
	};	

	void initFirst(CMainFrame* pm);              // �ȳ�ʼһЩ�����ı���
	void initOptionsMap(OptionsMap&);

	int MySaveSettingToSqlite();
	int MyReadSettingFromSqlite();

	void GetHTupleFromString(HalconCpp::HTuple&, std::string&);
	/// =====================================================================
	/// ȫ�ֺ���	

	void Speech(CString str, bool clear = false);
	void Speech(const char* cstr, bool clear = false); //{ Speech(CA2W(cstr).m_psz, clear); };
	void MyLog(CString str, bool isSave = true);
	void MyLog(const char* cst, bool isSave = true);	

	void MyLogStatus(CString str); 
	void MyLogWinHead(CString str);
	void MyLogWinHead(CString name, int id);

	void LoadCaliPoseAndParamter();           // ������̬������ڲ�
	BOOL CopyCaliPoseAndParamterFile(int oldPr, int newPr);

	int FindSpecialFile(CString stPath,      // ���ҵ�ǰĿ¼ָ����׺���ļ�����
		CString stExt, CArray<CString, CString&>* caPathFile, bool isDel = false);

    CString Utf8ToTChar(const std::string& utf8);
	std::string TCharToUtf8(const CString& tchar);

	void InitEmptyProject(int prID);

	/// =====================================================================
	/// ���ñ����ı�ʱ�Ĵ�����
	void on_camera_para(Option&);

	/// =====================================================================
	/// Halcon ����
	void action_wafer_dicing();					    // 4.1 �뵼�徧Ƭ�и�
	void action_read_serila_number();				// 4.2 ��ȡ����ϵ�ϵ�к�
	void action_inspection_of_saw_blades();         // 4.3 ��ȡ����ϵ�ϵ�к�
	void action_pring_inspection();                 // 4.4 ӡˢ���
	void action_BGA_inspection();                   // 4.5 BGA ���
	void action_sureface_inspection();              // 4.6 ������
	void action_park_plug_measuring();				// 4.7 ��𻨲���	
	void action_mold_flash_detection();             // 4.8 ģ��Ʒ������
	void action_puched_sheet_detection();           // 4.9 ��װ���
	void action_3D_reconstrucion();                 // 4.10 ˫Ŀ�����Ӿ���άƽ���ع�
	void action_pose_verfication_of_res();          // 4.11 ������̬����
	void action_classification_fabrics();           // 4.12 ��֯������
	

	void reopen_window_fit(HalconCpp::HObject ho_Image, HalconCpp::HTuple hv_MaxExtent, HalconCpp::HTuple hv_Row, HalconCpp::HTuple hv_Col,
		HalconCpp::HTuple *hv_WindowHandle);

	void init_font(HalconCpp::HTuple hv_WindowHandle, HalconCpp::HTuple hv_Font, HalconCpp::HTuple hv_Size);

	void dev_update_all(HalconCpp::HTuple hv_DisplayMode);

	void write_message(HalconCpp::HTuple hv_WindowHandle, HalconCpp::HTuple hv_Row, HalconCpp::HTuple hv_Column, HalconCpp::HTuple hv_String,
		HalconCpp::HTuple hv_WindowCoordinates);

	void get_shape_model_contour_ref(HalconCpp::HObject ho_ModelRegion, HalconCpp::HObject *ho_ModelContoursTrans,
		HalconCpp::HTuple hv_ModelID);

	void gen_arrow_contour_xld(HalconCpp::HObject *ho_Arrow, HalconCpp::HTuple hv_Row1, HalconCpp::HTuple hv_Col1, HalconCpp::HTuple hv_Row2,
		HalconCpp::HTuple hv_Col2, HalconCpp::HTuple hv_Size);

	void plot_tuple(HalconCpp::HTuple hv_WindowHandle, HalconCpp::HTuple hv_Tuple, HalconCpp::HTuple hv_TextXAxis, HalconCpp::HTuple hv_TextYAxis);

	extern BOOL bIsCanOperate(CString Operation);             // 

	extern stElement ELE;	

	extern OptionsMap OPTIONS;                                // ȫ�ֱ���

	//extern OptionsMap OutCam;                                 // �Ƿ����������е�Ԫ��

	extern CMainFrame *pMain;
	extern CString strMoudlePath;                             // AppPath ��װĿ¼
	//extern ST_USER User;                                    // ��ǰ��¼�û�
	extern bool MSSpeedInitOK;

	extern ST_USER User;
	extern SQLBASE sDB;
	extern RIBBON_MENU RIBmenu;                              // �˵���ʾ�ýṹ����          

	extern ST_MYDISP* pDis;                                  // ��ʾ�����ýṹ����;

	extern CString stPictureSavePath;                        // ͼ�񱣴��Ŀ¼
	extern CString stShutterSavePath;                        // ���ع��л��ļ�����Ŀ¼

} // namespace UCI

 






#endif