#include "stdafx.h"
#include "MainFrm.h"

#include "MyData.h"
#include "MyDispLay.h"

using namespace std;

namespace UCI {  // Namespace ���ݿ� 

	//SQLBASE::SQLBASE()
	//{
	//	//SQLite::Database db2(this->dbname, SQLite::OPEN_READWRITE);
	//	//this->
	//}
	
	// ����ȫ�ֲ���
	int SQLBASE::MySaveSettingToSqlite(OptionsMap & om)
	{
		//int iPID = UCI::OPTIONS[L"��Ŀ���"];

		CString Camera = om[L"�������"];

		try
		{
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);
			
			for (auto& it : om) {

				Option& o = it.second;

				std::string name = TCharToUtf8(it.first);
				std::string type = TCharToUtf8(o.getType());
				std::string val = TCharToUtf8(o.getcurrentValue());
				std::string note = TCharToUtf8(o.getNote());

				// 1 ���Ȳ����Ƿ�����ͬ��
				// Get a single value result with an easy to use shortcut
				//int id = db.execAndGet("");
				SQLite::Statement query(db, "SELECT ID FROM UCITAB WHERE ArgName = ? AND Camera = ?");
				query.bind(1, name);
				query.bind(2, CT2A(Camera).m_psz);

				// �ҵ���ͬ��
				if (query.executeStep()) {

					int id = query.getColumn(0);

					char     sql[1024];
					sprintf_s(sql, sizeof(sql), "UPDATE UCITAB SET ArgData = '%s', argType = '%s', Note = '%s' WHERE ID = %d",
						val.c_str(), type.c_str(), note.c_str(), id);

					int nb = (db).exec(sql);

				}
				else {

					char     sql[1024];
					sprintf_s(sql, sizeof(sql),
						"INSERT INTO UCITAB (ArgName, ArgData, ArgType,  Camera, Note) \
 VALUES('%s', '%s', '%s','%s', '%s')", name.c_str(), val.c_str(), type.c_str(), CT2A(Camera).m_psz, note.c_str());

					int nb = (db).exec(sql);
				}

			}
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	

	int SQLBASE::SetOneCamePara(CString Name, UCI::Option o)
	{
		int sleepTime = 1;
		CString stError;

		CString paraname = o.getName();
		CString type = o.getType();
		CString val = o.getcurrentValue();

		if (UCI::sDB.isCamParaCanOut(Name)) {

			if (type == L"spin") {
				int v = _ttoi(val);
				UCI::pDis->pCAM->SetParameterHTuple(paraname, v);

				// ��ȷ��һ�£��ǲ������óɹ���
				Sleep(sleepTime);
				int vin = UCI::pDis->pCAM->GetParameterInt(paraname);
				if (v != vin) {
					stError.Format(L"����趨����%s : %d", paraname, v);
					UCI::MyLog(stError);
					return FALSE;
				}
			}
			else if (type == L"double") {
				double v = _ttof(val);
				UCI::pDis->pCAM->SetParameterHTuple(paraname, v);

				// ��ȷ��һ�£��ǲ������óɹ���
				Sleep(sleepTime);
				double vin = UCI::pDis->pCAM->GetParameterDouble(paraname);
				if (v != vin) {
					stError.Format(L"����趨����%s : %f", paraname, v);
					UCI::MyLog(stError);
					return FALSE;
				}
			}
			else if (type == L"string") {
				UCI::pDis->pCAM->SetParameterHTuple(paraname, CT2A(val).m_psz);

				// ��ȷ��һ�£��ǲ������óɹ���
				Sleep(sleepTime);
				CString vin = UCI::pDis->pCAM->GetParameterString(paraname);
				if (val != vin) {
					stError.Format(L"����趨����%s : %s", paraname, val);
					UCI::MyLog(stError);
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	BOOL SQLBASE::isCamParaCanOut(CString Name)
	{
		if (Name == L"��ע") {
			return FALSE;
		}
		else if (Name == L"��Ŀ���") {
			return FALSE;
		}
		else if (Name == L"Ԫ������") {
			return FALSE;
		}
		else if (Name == L"Ԫ������") {
			return FALSE;
		}
		else if (Name == L"Ԫ����ƥ��") {
			return FALSE;
		}
		else if (Name == L"Ԫ�ر�ע") {
			return FALSE;
		}
		else if (Name == L"������") {
			return FALSE;
		}
		else if (Name == L"������ʱ") {
			return FALSE;
		}
		
		return TRUE;
	}

	int SQLBASE::SetCamPara(OptionsMap & camo)
	{
		static bool isSeting = false;

		if (isSeting) return TRUE;     // ����ֻ�趨һ��		
		
		for (auto&it : camo) {
			UCI::Option o = it.second;
			CString Name = it.first;

			this->SetOneCamePara(Name, o);
			Sleep(5);	
		}
		int delay = UCI::OPTIONS[L"������ʱ"];
		Sleep(delay);
		isSeting = true;
		
		return SQLITE_OK;
	}

	int SQLBASE::DelAllElement(int iPid)
	{
		//int iPID = UCI::OPTIONS[L"��Ŀ���"];                // ��ǰԪ�ص���Ŀ���
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "DELETE FROM ELEM WHERE ProjectID = %d", iPid);
			db.exec(sql);			
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		
		return SQLITE_OK;
	}

	BOOL SQLBASE::isCanlogin(CString name, CString pass)
	{
				
		try
		{
			SQLite::Database db(this->dbname);

			SQLite::Statement query(db, "SELECT * FROM USER WHERE USERNAME = ? AND PASSWORD = ?");
			query.bind(1, CT2A(name).m_psz);
			query.bind(2, CT2A(pass).m_psz);

			if (query.executeStep()) {
				return TRUE;
			}

			return FALSE;

		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return FALSE;
		}
		return FALSE;
	}

	int SQLBASE::GetLastProductTestNumber()
	{
		
		try
		{
			SQLite::Database db(this->dbname);

			SQLite::Statement query(db, "SELECT ID FROM PRODUCT_RESULT ORDER BY ID DESC");

			if (query.executeStep()) {

				int last = query.getColumn(0);
				return last;
			}
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}

		return 1;

	}

	// �����������������Ӧ��Ŀ¼
	void SQLBASE::PrepareCamDeviceCatalog(int prID)
	{
		// ������Ӧ��Ŀ¼
		CString proRoot;
		proRoot.Format(L"%s\\Project\\%d", UCI::strMoudlePath, prID);

		if (!PathIsDirectory(proRoot)) {
			::CreateDirectory(proRoot, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		CString CsDeviceName = UCI::OPTIONS[L"�������"];	

		CString str = proRoot + L"\\inner";
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		// �ٴ��������Ӧ��Ŀ¼
		str = str + L"\\" + CsDeviceName;
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		//str = proRoot + L"\\MODEL";
		//if (!PathIsDirectory(str)) {
		//	::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		//}

		str = proRoot + L"\\pos";
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		// �ٴ��������Ӧ��Ŀ¼
		str = str + L"\\" + CsDeviceName;
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		str = proRoot + L"\\Product";
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		// �ٴ��������Ӧ��Ŀ¼
		str = str + L"\\" + CsDeviceName;
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		str = proRoot + L"\\ROI";
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}

		// �ٴ��������Ӧ��Ŀ¼
		str = str + L"\\" + CsDeviceName;
		if (!PathIsDirectory(str)) {
			::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}
	}

	int SQLBASE::Log( CString log, eLogType e)
	{
		try
		{
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[4096];
			sprintf_s(sql, sizeof(sql),
				"INSERT INTO LOGTABLE (TYPE, LOG) VALUES(%d, '%s')", e, TCharToUtf8(log).c_str());

			int nb = (db).exec(sql);

			//int id = (int)db.getLastInsertRowid();
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();	
			return 1;
		}
		return SQLITE_OK;
	}

	int SQLBASE::Log(HalconCpp::HException & except, CString str)
	{
		//UCI::pDis->write_message(350, 20, CT2A(str).m_psz, true);
		//UCI::pDis->write_message(450, 20, HalconCpp::HTuple(except.ProcName()) + " " + except.ErrorMessage(), 0);

		string stName = except.ProcName();
		string stErr = except.ErrorMessage();

		CString procName = CA2T(stName.c_str()).m_psz;
		procName = procName + L" " + CA2T(stErr.c_str()).m_psz;		
	
		
		// ���浽��־
		Log(str + L" " + procName, LOG_HALCON_ERR);

		// ��ʾ����־
		UCI::MyLog(procName,false);
		
		return SQLITE_OK;
	}

	int SQLBASE::DeleteProject(int prID)
	{
		// 
		
		// ��ɾ����Ŀ��Ԫ��
		this->DelAllElement(prID);

		// ��ɾ������Ŀ
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "DELETE FROM Project WHERE ID = %d", prID);
			(db).exec(sql);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}

		// ��ˢ��һ����Ŀ��
		UCI::pMain->FillProjectTreeView();

		UCI::pMain->MProjectTreeFillByPrID();

		return SQLITE_OK;
	}


		
	int SQLBASE::MyResetProjectElements(int ipID)
	{
		//int ipID = UCI::OPTIONS[L"��Ŀ���"];                // ��ǰԪ�ص���Ŀ���

		this->DelAllElement(ipID);

		// �ٱ������е�Ԫ�ص����ݿ���

		this->MySaveAllElement();

		// �ٶ������е�Ԫ��
		this->MyLoadElementFromDatabase(ipID);

		return SQLITE_OK;
	}
	int SQLBASE::RecordOneProdcutNotUpDateGUI(int& productNumber) {
		
		// ����һ����Ʒ
		this->SaveOneProductResult(productNumber);   // ����һ����Ʒ    
		int elementResultIndex = 1;

		BOOL result = UCI::OPTIONS[L"GLO_Measure_OK"];

		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *o = &UCI::ELE.EleList.GetNext(pos);

			int itype = (*o)[L"Ԫ������"];
			if (itype == ELEMENT_RESULT) {

				//UCI::pMain->insertOneResult(o);
				UCI::sDB.SaveOneElementResult(productNumber, elementResultIndex, *o);
				elementResultIndex++;
			}
		}

		UpdateOneProductResult(productNumber, result);

		UCI::pMain->UpDateStatusProductInfo();

		return SQLITE_OK;

	}
	
	int SQLBASE::RecordOneProdcut(int & productNumber)
	{
		// ����һ����Ʒ
		this->SaveOneProductResult(productNumber);   // ����һ����Ʒ    
		int elementResultIndex = 1;

		BOOL result = UCI::OPTIONS[L"GLO_Measure_OK"];

		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *o = &UCI::ELE.EleList.GetNext(pos);

			int itype = (*o)[L"Ԫ������"];
			if (itype == ELEMENT_RESULT) {

				UCI::pMain->insertOneResult(o);
				UCI::sDB.SaveOneElementResult(productNumber, elementResultIndex, *o);
				elementResultIndex++;
			}
		}

		UpdateOneProductResult(productNumber, result);

		UCI::pMain->UpDateStatusProductInfo();

		return SQLITE_OK;
	}

	int SQLBASE::GetProductTotalAndGoodBy(int PrID, int start, int & total, int & good)
	{
		
		try {

			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "SELECT COUNT(*) FROM PRODUCT_RESULT "\
				"WHERE ProjectID = %d AND ID >= %d",	PrID, start);
		   total = (db).execAndGet(sql);

		   sprintf_s(sql, sizeof(sql), "SELECT COUNT(*) FROM PRODUCT_RESULT "\
			   "WHERE ProjectID = %d AND ID >= %d AND Result = 1", PrID, start);
		   good = (db).execAndGet(sql);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return SQLITE_ERROR;
		}


		// Get a single value result with an easy to use shortcut
		//const std::string value = db.execAndGet("SELECT value FROM test WHERE id=2");
		//std::cout << "execAndGet=" << value.c_str() << std::endl;
		
		return SQLITE_OK;
	}

	int SQLBASE::SaveOneProductResult(int & ProductID)
	{
		
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			int ProjectID = UCI::OPTIONS[L"��Ŀ���"];
			int TestUserID = UCI::OPTIONS[L"UserID"];
			CString BatchNumber = UCI::OPTIONS[L"ProdutctBatchNumber"];

			char     sql[1024];
			sprintf_s(sql, sizeof(sql),
				"INSERT INTO PRODUCT_RESULT (ProjectID, TestUserID, BatchNumber) \
 VALUES(%d, %d, '%s')", ProjectID, TestUserID, TCharToUtf8(BatchNumber).c_str());

			int nb = (db).exec(sql);

			ProductID = (int)(db).getLastInsertRowid();

		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return SQLITE_ERROR;
		}
		
		return SQLITE_OK;
	}

	int SQLBASE::UpdateOneProductResult(int ProductID, BOOL result)
	{
		try {

			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "UPDATE PRODUCT_RESULT SET Result = %d WHERE ID = %d",
				result, ProductID);

			int nb = (db).exec(sql);

		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return SQLITE_ERROR;
		}

		return SQLITE_OK;
	}

	int SQLBASE::MyNewProject(CString name, int & NewProjectID)
	{
		try {

			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql),
				"INSERT INTO Project (ProjectName, Note) \
 VALUES('%s', '%s')", TCharToUtf8(name).c_str(),"project Note");

			int nb = (db).exec(sql);  

			NewProjectID = (int)(db).getLastInsertRowid();
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return SQLITE_ERROR;
		}

		PrepareCamDeviceCatalog(NewProjectID);		

		return SQLITE_OK;
	}

	// ��Ŀ����
	int SQLBASE::MyProjectRename(CString newName, int NewProjectID)
	{
		try
		{
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "UPDATE Project SET ProjectName = '%s' WHERE ID = %d",
				TCharToUtf8(newName).c_str(), NewProjectID);

			int nb = (db).exec(sql);

		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return SQLITE_ERROR;
		}

		return SQLITE_OK;

	}

	// ����һ������Ԫ�صĲ������
	int SQLBASE::SaveOneElementResult(int ProductID, int index, OptionsMap & ores)
	{
		
		try {
			int prID = UCI::OPTIONS[L"��Ŀ���"];

			CString Name = (ores)[L"����������"];

			double result = (ores)[L"����Ĳ���ֵ"];
			double stand = (ores)[L"����ı�׼ֵ"];
			double upper = (ores)[L"������Ϲ���"];
			double lower = (ores)[L"������¹���"];		

			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql),
				"INSERT INTO ELEMENT_RESULT (ProResID, EleIndex, Name,Val_Result,Val_Stand, Val_Upper,Val_Lower,ProjectID) \
 VALUES(%d, %d, '%s', %f, %f, %f, %f, %d)", ProductID, index, TCharToUtf8(Name).c_str(), result, stand, upper, lower, prID);

			int nb = (db).exec(sql);

			ProductID = (int)(db).getLastInsertRowid();

		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return SQLITE_ERROR;
		}

		return SQLITE_OK;	
	}

	int SQLBASE::DeleteOneELementByName(CString name)
	{
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "DELETE FROM ELEM WHERE ArgName = '%s'", TCharToUtf8(name).c_str());
			(db).exec(sql);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	int SQLBASE::DeleteOneGobalParaByName(CString name)
	{
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "DELETE FROM UCITAB WHERE ArgName = '%s'", TCharToUtf8(name).c_str());
			(db).exec(sql);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	// �������еĲ���Ԫ�ر���
	int SQLBASE::MySaveAllElement()
	{
		
		int ipID = UCI::OPTIONS[L"��Ŀ���"];                // ��ǰԪ�ص���Ŀ���
		this->DelAllElement(ipID);

		SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);
		
		int index = 1;
		POSITION pos = ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			OptionsMap *o = &ELE.EleList.GetNext(pos);
			MySaveOneElement(*o,index, &db);
			index++;
		}
		return SQLITE_OK;
	}

	int SQLBASE::MySaveAllElement(int prID, stElement * NELE)
	{
		this->DelAllElement(prID);

		SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);
		
		int index = 1;
		POSITION pos = NELE->EleList.GetHeadPosition();
		while (pos != NULL) {
			OptionsMap *o = &NELE->EleList.GetNext(pos);
			MySaveOneElement(*o, index, &db);
			index++;
		}

		return SQLITE_OK;
	}

	// ����һ��Ԫ��
	int SQLBASE::MySaveOneElement(OptionsMap & om, int Eleindex)
	{
		int iPID = om[L"��Ŀ���"];                // ��ǰԪ�ص���Ŀ���
		int Eletype = om[L"Ԫ������"];              // ��ǰԪ�ص�����

		try
		{
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			for (auto& it : om) {

				Option& o = it.second;

				CString name = it.first;

				CString type = o.getType();
				CString val = o.getcurrentValue();
				CString note = o.getNote();

				// 1 ���Ȳ����Ƿ�����ͬ��
				// Get a single value result with an easy to use shortcut
				//int id = db.execAndGet("");
				SQLite::Statement query(db, "SELECT ID FROM ELEM WHERE ArgName = ? AND ProjectID = ? \
AND EleType = ? AND EleINdex = ?");
				query.bind(1, TCharToUtf8(name).c_str());
				query.bind(2, iPID);
				query.bind(3, Eletype);
				query.bind(4, Eleindex);

				// �ҵ���ͬ��
				if (query.executeStep()) {

					int id = query.getColumn(0);

					char     sql[1024];
					sprintf_s(sql, sizeof(sql), "UPDATE ELEM SET ArgData = '%s', argType = '%s', Note = '%s' WHERE ID = %d",
						TCharToUtf8(val).c_str(), TCharToUtf8(type).c_str(), TCharToUtf8(note).c_str(), id);

					int nb = (db).exec(sql);

					//om[L"ID"] = id;

				}
				else {

					char     sql[1024];
					sprintf_s(sql, sizeof(sql),
						"INSERT INTO ELEM (ArgName, ArgData, ArgType, ProjectID, EleType, EleIndex, Note) \
 VALUES('%s', '%s', '%s', %d, %d, %d, '%s')", TCharToUtf8(name).c_str(), 
						TCharToUtf8(val).c_str(), TCharToUtf8(type).c_str(), iPID,Eletype, Eleindex, 
						TCharToUtf8(note).c_str());

					int nb = (db).exec(sql);

					//int id = (int)db.getLastInsertRowid();
					//om[L"ID"] = id;
				}
			}
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	int SQLBASE::MySaveOneElement(OptionsMap & om, int Eleindex, SQLite::Database * db)
	{


		int iPID = om[L"��Ŀ���"];                // ��ǰԪ�ص���Ŀ���
		int Eletype = om[L"Ԫ������"];              // ��ǰԪ�ص�����

		try
		{
			//SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			for (auto& it : om) {

				Option& o = it.second;

				CString name = it.first;

				CString type = o.getType();
				CString val = o.getcurrentValue();
				CString note = o.getNote();

				// 1 ���Ȳ����Ƿ�����ͬ��
				// Get a single value result with an easy to use shortcut
				//int id = db.execAndGet("");
				SQLite::Statement query(*db, "SELECT ID FROM ELEM WHERE ArgName = ? AND ProjectID = ? \
AND EleType = ? AND EleINdex = ?");
				query.bind(1, TCharToUtf8(name).c_str());
				query.bind(2, iPID);
				query.bind(3, Eletype);
				query.bind(4, Eleindex);

				// �ҵ���ͬ��
				if (query.executeStep()) {

					int id = query.getColumn(0);

					char     sql[1024];
					sprintf_s(sql, sizeof(sql), "UPDATE ELEM SET ArgData = '%s', argType = '%s', Note = '%s' WHERE ID = %d",
						TCharToUtf8(val).c_str(), TCharToUtf8(type).c_str(), TCharToUtf8(note).c_str(), id);

					int nb = (*db).exec(sql);

					//om[L"ID"] = id;

				}
				else {

					char     sql[1024];
					sprintf_s(sql, sizeof(sql),
						"INSERT INTO ELEM (ArgName, ArgData, ArgType, ProjectID, EleType, EleIndex, Note) \
 VALUES('%s', '%s', '%s', %d, %d, %d, '%s')", TCharToUtf8(name).c_str(),
						TCharToUtf8(val).c_str(), TCharToUtf8(type).c_str(), iPID, Eletype, Eleindex,
						TCharToUtf8(note).c_str());

					int nb = (*db).exec(sql);

					//int id = (int)db.getLastInsertRowid();
					//om[L"ID"] = id;
				}
			}
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	int SQLBASE::MyFillUser(CList<CString, CString>* User)
	{
		User->RemoveAll();
		
		try {

			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);


			SQLite::Statement query(db, "SELECT * FROM USER");
			//query.bind(1, TCharToUtf8(CamName).c_str());

			while (query.executeStep()) {
				CString ArgName = Utf8ToTChar(query.getColumn(1).getString());				
				User->AddTail(ArgName);
			}
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}

		return SQLITE_OK;	
	}

	int SQLBASE::GetUserInfoByName(ST_USER * pUser)
	{
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			SQLite::Statement query(db, "SELECT * FROM USER WHERE USERNAME = ?");
			query.bind(1, TCharToUtf8(pUser->Name));

			if (!query.executeStep()) {   // û���ҵ�
				return 1;
			}

			query.reset();
			query.executeStep();

			//pUser->Name = UserName;

			pUser->Password = Utf8ToTChar(query.getColumn(2).getString());
			pUser->Permission = query.getColumn(3);
			pUser->Note = Utf8ToTChar(query.getColumn(4).getString());
			pUser->stRegTime = Utf8ToTChar(query.getColumn(5).getString());
			pUser->stLastLogTime = Utf8ToTChar(query.getColumn(6).getString());

		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	int SQLBASE::UpdateUserInfoBy(ST_USER * pUser)
	{
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char sql[1024];

			if (pUser->Password.GetLength() == 0) { // ֻ����Note
				sprintf_s(sql, sizeof(sql), "UPDATE USER SET NOTE = '%s' WHERE USERNAME = '%s'",
					TCharToUtf8(pUser->Note).c_str(), TCharToUtf8(pUser->Name).c_str());
			}
			else {
				sprintf_s(sql, sizeof(sql), "UPDATE USER SET PASSWORD = '%s', NOTE = '%s' WHERE USERNAME = '%s'",
					TCharToUtf8(pUser->Password).c_str(), TCharToUtf8(pUser->Note).c_str(), TCharToUtf8(pUser->Name).c_str());
			}

			int nb = (db).exec(sql);

		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	int SQLBASE::AddUser(ST_USER * pUser)
	{
		
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char sql[1024];
			
			sprintf_s(sql, sizeof(sql), "INSERT INTO USER (USERNAME, PASSWORD, PERMISSION, NOTE) \
VALUES('%s', '%s','%d','%s')", 
                    TCharToUtf8(pUser->Name).c_str(),
					TCharToUtf8(pUser->Password).c_str(), 
				    pUser->Permission,
				    TCharToUtf8(pUser->Note).c_str());			

			int nb = (db).exec(sql);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}

	int SQLBASE::DeleteUserByName(ST_USER * pUser)
	{
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char sql[1024];

			sprintf_s(sql, sizeof(sql), "DELETE FROM USER WHERE USERNAME = '%s'",
TCharToUtf8(pUser->Name).c_str());

			int nb = (db).exec(sql);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}


	int SQLBASE::MyLoadElementFromDatabase(int prID)
	{
		
		ELE.EleList.RemoveAll();

		CList <CString, CString> elementToBeDel;

		// int prID = UCI::OPTIONS[L"��Ŀ���"];

		try {

			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			for (int index = 1; index < 10000; index++) {    // ���10000��Ԫ��
				
				SQLite::Statement query(db, "SELECT * FROM ELEM WHERE ProjectID = ? AND EleIndex = ?");
				query.bind(1, prID);
				query.bind(2, index);

				if (!query.executeStep()) {   // û���ҵ�
					break;
				}
				query.reset();
				query.executeStep();                    // ����һ�¡��õ����Ԫ�ص�����
				int etype = query.getColumn(5);         // 
  
				OptionsMap* o = ELE.initOptionMapElemnet(eELEMENT_TYPE(etype), prID);    // ����һ��Ԫ��

				query.reset();                // ��λһ�£��Ա���ٴ�ʹ��
				while (query.executeStep()) {
					
					CString ArgName = Utf8ToTChar(query.getColumn(1).getString());
					CString ArgData = Utf8ToTChar(query.getColumn(2).getString());
					CString ArgType = Utf8ToTChar(query.getColumn(3).getString());
					//int etype = query.getColumn(5);
					CString Note = Utf8ToTChar(query.getColumn(7).getString());

					if ((*o).count(ArgName)) {

						//if (ArgData == "-1.750000") {
						//	if (ELE.EleList.GetCount() == 12) {
						//		int kkk = 0;
						//	}
						//}

						(*o)[ArgName] = ArgData;
						(*o)[ArgName].setNote(Note);
					}
					else {
						MyLog("û�����������");
						CString Arg = ArgName + L" " + ArgData + L" " + ArgType;
						MyLog(Arg);

						elementToBeDel.AddTail(ArgName);

/*						MyLog("����ɾ�����Ԫ��");
						DeleteOneELementByName(ArgName);
						MyLog("ɾ�����Ԫ�����")*/;
						
					}

				}
				// ����һ�µ�ǰ��index 
			}
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}

		POSITION pos = elementToBeDel.GetHeadPosition();
		while (pos != NULL) {
			CString ArgName = elementToBeDel.GetNext(pos);
			DeleteOneELementByName(ArgName);
		}

		return SQLITE_OK;
	}

	int SQLBASE::ClearProductResutlByProjectID(int prID)
	{
		try {
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			char     sql[1024];
			sprintf_s(sql, sizeof(sql), "DELETE FROM ELEMENT_RESULT WHERE ProjectID = %d", prID);
			(db).exec(sql);

			//char     sql[1024];
			sprintf_s(sql, sizeof(sql), "DELETE FROM PRODUCT_RESULT WHERE ProjectID = %d", prID);
			(db).exec(sql);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}

		return SQLITE_OK;
	}
	
	// ����һ��Ԫ�ص����ݿ���ȥ
	int stElement::saveOneOptionsMap(UCI::OptionsMap & o, int index)
	{
		return UCI::sDB.MySaveOneElement(o, index);
	}

	BOOL stElement::isHaveSaveElementName(CString str)
	{
		
		POSITION pos = UCI::ELE.EleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList.GetNext(pos);
			CString name = (*ot)[L"Ԫ������"];
			if (str == name) return TRUE;
		}
		
		return FALSE;
	}


	// �����ݿ��ж���ȫ������
	int SQLBASE::MyReadSettingFromSqlite(CString  CamName)
	{
		
		OptionsMap & om = UCI::OPTIONS;
		CList <CString, CString> elementToBeDel;

		try
		{
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);

			SQLite::Statement query(db, "SELECT * FROM UCITAB WHERE Camera = ?");
			query.bind(1, TCharToUtf8(CamName).c_str());

			while (query.executeStep()) {
				CString ArgName = Utf8ToTChar(query.getColumn(1).getString());
				CString ArgData = Utf8ToTChar(query.getColumn(2).getString());
				CString ArgType = Utf8ToTChar(query.getColumn(3).getString());
				CString Note = Utf8ToTChar(query.getColumn(5).getString());

				if (om.count(ArgName)) {
					om[ArgName] = ArgData;
					om[ArgName].setNote(Note);
				}
				else {
					MyLog("û�����������");
					CString Arg = ArgName + L" " + ArgData + L" " + ArgType;
					MyLog(Arg);

					elementToBeDel.AddTail(ArgName);
				}
			}			
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}

		POSITION pos = elementToBeDel.GetHeadPosition();
		while (pos != NULL) {
			CString ArgName = elementToBeDel.GetNext(pos);
			DeleteOneGobalParaByName(ArgName);
		}

		return SQLITE_OK;		
	}
	
	
	//
	int SQLBASE::ProjectFillByID(ST_PROJECT * pro)
	{
		try
		{
			SQLite::Database db(this->dbname);

			SQLite::Statement query(db, "SELECT * FROM Project WHERE ID = ?");
			query.bind(1, pro->ID);

			while (query.executeStep()) {
				pro->ProjectName = query.getColumn(1).getText();				
				pro->ProjectNote = query.getColumn(2).getText();
				pro->datatime = query.getColumn(3).getText();
			}
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			MyLog(err.c_str());
			return 1;
		}
		return SQLITE_OK;
	}
		
	//
	int MySaveSettingToSqlite()
	{
		CString lastCammer = UCI::OPTIONS[L"�������"];
		
		
		UCI::sDB.MySaveSettingToSqlite(OPTIONS);        // ����ȫ�ֲ���

		UCI::sDB.MySaveAllElement();                    // �������е�Ԫ��

		CString INIfile = UCI::strMoudlePath + L"\\config.ini";
		
		WritePrivateProfileString(L"SETTING", L"Last_Camer", lastCammer, INIfile);

		return SQLITE_OK;
	}

	//
	int MyReadSettingFromSqlite()
	{
		
		int prID = UCI::OPTIONS[L"��Ŀ���"];
		UCI::ST_PROJECT pr(prID);
		UCI::sDB.ProjectFillByID(&pr);

		UCI::OPTIONS[L"�궨��Ϣ OK"] = FALSE;
		UCI::OPTIONS[L"ORGwhByCamera"] = FALSE;

		CString lastCammer = UCI::OPTIONS[L"�������"];
		//UCI::sDB.MyReadCAMparaFromSqlite(lastCammer);        // �������	

		UCI::MyLogWinHead(Utf8ToTChar(pr.ProjectName.c_str()), pr.ID);

		//UCI::LoadCaliPoseAndParamter();           // ������̬������ڲ�

		UCI::pMain->ProPertyUpdate();              // ��ʼ�����Ա�

		if (ELE.EleList.GetCount() == 0) {         // ���Ϊ�գ��������������ӣ�ƥ����Ԫ��
			UCI::InitEmptyProject(prID);
		}
		UCI::pMain->MProjectTreeFillByPrID();      // ��ʼ����Ŀ��
		

		//UCI::pDis->set_display_font(*UCI::pDis->m_pMHWinMain, 12, "mono", "true", "false");

		UCI::pMain->UpDateStatusProductInfo();

		return SQLITE_OK;

	}
	
	//	
	int SQLBASE::MySQLFillProjectTree(CProjectTree * cvt)
	{
		// ȡ�õ�ǰ��Ŀ�Ĳ���Ԫ��
		//FillMyElementArray();
		cvt->DeleteAllItems();       // �����һ�¡�

		int prid = UCI::OPTIONS[L"��Ŀ���"];
		
		HTREEITEM hRoot = cvt->InsertItem(_T("������Ŀ"), 0, 0);
		cvt->SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
		
		try
		{
			SQLite::Database db(this->dbname, SQLite::OPEN_READWRITE);
			//HTREEITEM hSrc;
			// ================================================================
			// ����һЩ�������Ŀ
			SQLite::Statement query2(db, "SELECT * FROM Project ORDER BY ID ASC");

			UINT mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			while (query2.executeStep()) {

				int id2 = query2.getColumn(0);                      // ��Ŀ���
				std::string stProName = query2.getColumn(1);
				std::string stProNote = query2.getColumn(3);

				HTREEITEM hRc = cvt->InsertItem(mask,
					Utf8ToTChar(stProName), 1, 1, 0, 0, id2, hRoot, NULL);

				//if (prid == id2) {						

				////	for (int i = 0; i < MyELEMENT.GetCount(); i++) {

				////		stROI_ELEMENT e = MyELEMENT[i];
				////		CString name;
				////		name.Format(L"%s %d", e.getNote(), e.getID());

				////		cvt->InsertItem(mask, name, 2, 2, 0, 0, e.getID(), hRc, NULL);						
				////	}					
				//   cvt->Expand(hRc, TVE_EXPAND);
				//}
			}
			cvt->Expand(hRoot, TVE_EXPAND);
		}
		catch (std::exception& e) {
			std::string err = "SQLite exception: ";
			err += e.what();
			//MyLog(err.c_str());
			return 1;
		}

		cvt->Expand(hRoot, TVE_EXPAND);		
		
		return SQLITE_OK;
	}



} // Namespace ���ݿ� 





