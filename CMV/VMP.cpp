

#include "stdafx.h"

#include "VMP.h"
#include "aes.h"
#include "Base64.h"
#include "WMI_DeviceQuery.h"
#include "MyData.h"
#include "MyDispLay.h"
#include "VMProtectSDK.h"

#define  NOT_USE_VMPSDK

#define MARKER_NAME1            "CMV_ST_VMP3091"
#define MARKER_NAME2            "CMV_ST_VMP3092"
#define MARKER_NAME3            "CMV_ST_VMP3093"
#define MARKER_NAME4            "CMV_ST_VMP3094"

//#define PRINT_HELPER(state, flag) if (state & flag) printf("%s ", #flag)
//void print_state(int state)
//{
//	if (state == 0)
//	{
//		printf("your are register now! thank you\n");
//		return;
//	}
//
//	printf("state = ");
//	PRINT_HELPER(state, SERIAL_STATE_FLAG_CORRUPTED);
//	PRINT_HELPER(state, SERIAL_STATE_FLAG_INVALID);
//	PRINT_HELPER(state, SERIAL_STATE_FLAG_BLACKLISTED);
//	PRINT_HELPER(state, SERIAL_STATE_FLAG_DATE_EXPIRED);
//	PRINT_HELPER(state, SERIAL_STATE_FLAG_RUNNING_TIME_OVER);
//	PRINT_HELPER(state, SERIAL_STATE_FLAG_BAD_HWID);
//	PRINT_HELPER(state, SERIAL_STATE_FLAG_MAX_BUILD_EXPIRED);
//	printf("\n");
//}

#ifdef USE_VP_PROTECT
//char *read_serial(const char *fname)
char* read_serial(char* buf, CString Path) {

#ifndef NOT_USE_VMPSDK
	VMProtectBegin(MARKER_NAME1);
#endif

	CString KeyFile;
	KeyFile = Path + L"\\CMVreg.key";

	FILE *f;
	if (0 != _wfopen_s(&f, KeyFile, L"rb")) {
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	int s = ftell(f);
	fseek(f, 0, SEEK_SET);

	//if (s == 0) return NULL;

	//char *buf = new char[s + 1];
	fread(buf, s, 1, f);
	buf[s] = 0;
	fclose(f);

#ifndef NOT_USE_VMPSDK
	VMProtectEnd();
#endif

	return buf;
}



void xor_key2(char* hex, int len) {

#ifndef NOT_USE_VMPSDK
	VMProtectBegin(MARKER_NAME2);
#endif

	char key2[] = "efeffeqwertyushnirelitshinrgzxciouthefsfesfaffefefesaff";

	int lenk = sizeof(key2) - 5;
	int pk = 0;
	for (int i = 0; i < len; i++) {
		char c = hex[i];
		char hc = c ^ key2[pk];
		hex[i] = hc;
		pk++;
		if (pk > lenk) {
			pk = 0;
		}
	}
#ifndef NOT_USE_VMPSDK
	VMProtectEnd();
#endif
}

#define WMI_QUERY_NUM 2

bool is_registered() {

#ifndef NOT_USE_VMPSDK
	VMProtectBegin(MARKER_NAME3);
#endif

	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	CString _strMoudlePath(modulePath);
	_strMoudlePath = _strMoudlePath.Left(_strMoudlePath.ReverseFind('\\'));
	CString strMoudleP = _strMoudlePath;

	bool isOK = false;

	FILE *f;
	char strhex[2048];
	char hard[32 * WMI_QUERY_TYPENUM* WMI_QUERY_NUM];
	char* p2 = NULL;
	int res = 0;
	char cName[64];
	char cDate[64];
	char cMonth[64];
	T_DEVICE_PROPERTY idp[WMI_QUERY_TYPENUM][WMI_QUERY_NUM];

	char ppser[4096];


	//  �õ���ǰ��ע���ļ��е���Ϣ��
	char* pser = read_serial(ppser, strMoudleP);
	if (pser != NULL) {
		// ����һ�¡�
		char dstr[2048];
		char DeKey[2048];
		unsigned char key[] = "efaeaffearfefeetet345fe3rfeffaaf";
		AES aes(key);
		aes.InvCipher(pser, dstr);

		unsigned int dsize = 0;
		unsigned char* db64 = base64Decode(dstr, dsize);	

		xor_key2((char*)db64, dsize - 1);
		sprintf_s(DeKey, 2048, "%s", db64);
		
		delete db64;                                    // ���һ��Ҫ���ϣ���Ȼ�ڴ�й©

		memset(cName, 0, 64);
		memset(cDate, 0, 64);
		memset(cMonth, 0, 64);
		// �õ��û���;
		char* p1 = strstr(DeKey, "name:");
		p2 = strstr(DeKey, " date:");
		if (p1 == NULL || p2 == NULL) {
			return false;
		}
		p1 += 5;
		memcpy(cName, p1, p2 - p1);
		// �õ�ע������;
		p1 = strstr(DeKey, " date:");
		p2 = strstr(DeKey, " Month:");
		if (p1 == NULL || p2 == NULL) {
			return false;
		}
		p1 += 6;
		memcpy(cDate, p1, p2 - p1);
		// �õ�����������;
		p1 = strstr(DeKey, " Month:");
		p2 = strstr(DeKey, " (End)");
		if (p1 == NULL || p2 == NULL) {
			return false;
		}
		p1 += 7;
		memcpy(cMonth, p1, p2 - p1);

		p2 += 6; // p2 ָ���˻�����
	}



	// ��ȡ��ǰ�Ļ�����
	// ����ϵͳ��Ӳ��rx��Ϣ��
	for (int i = 0; i < WMI_QUERY_TYPENUM; i++) {
		WMI_DeviceQuery(i, idp[i], WMI_QUERY_NUM);
	}

	// ��Ӳ����Ϣд���ļ���ȥ
	CString HdidFileName;
	HdidFileName = strMoudleP + L"\\HWID.txt";

	if (0 != _wfopen_s(&f, HdidFileName, L"w")) {
		return false;
	}

	//memcpy(idpAES, idp, sizeof(idp));

	unsigned char key1[] = "efalkjaefoijageffeofjahfaeohihfeffef";

	AES aes1(key1);
	char str[32 * WMI_QUERY_TYPENUM* WMI_QUERY_NUM];

#pragma warning(disable : 4477)                  // 
	//char result[64 * WMI_QUERY_TYPENUM* WMI_QUERY_NUM];
	memset(str, 0, sizeof(str));
	//str[0] = 0;
	char buf[128];
	for (int i = 0; i < WMI_QUERY_TYPENUM; i++) {
		for (int j = 0; j < WMI_QUERY_NUM; j++) {
			//if (i == 4) continue;
			sprintf_s(buf, sizeof(buf), "%32S", idp[i][j]);
			strcat_s((str), buf);
		}
	}
	sprintf_s(buf, sizeof(buf), "%32S", L"CMV_20170818");
	strcat_s((str), buf);

	memcpy(hard, str, sizeof(str));

	int len = (int)strnlen_s(str, 2048);
	xor_key2(str, len);

	char* base64 = base64Encode(str, len);

	aes1.Cipher(base64, strhex);	

	delete base64;

	//0������ԭ��MAC��ַ
	//1��Ӳ�����к�
	//2���������к�
	//3��CPU ID
	//4��BIOS���к�
	//5�������ͺ�
	//6��������ǰMAC��ַ
	// ����Ƚ� buf �� P2
	if (pser != NULL) {
		res = 1;
		for (int i = 0; i < 32 * 5; i++) {
			if (p2[i] != hard[i]) {
				res = 0;
				break;
			}
		}
	}

	// ���û��ע��ɹ����Ͷ�����ǰ����Ϣ��������д�뵽�ļ���ȥ��
	if (res == 0) {

		fprintf_s(f, strhex);
		fflush(f);


		MessageBox(NULL, L"�뷢�� HWID.txt \" �� keersun@qq.com",
			L"�����û��ע�� \n www.czmed.com", MB_OK);

		isOK = false;
	}
	else { // ע��ɹ���
		//printf("info register ok! thank you!\n");
		//printf("regName: %s, regDate: %s, Update Month %s\n", cName, cDate, cMonth);
		//frist_init();
		isOK = true;
	}

#ifndef NOT_USE_VMPSDK
	VMProtectEnd();
#endif	

	return isOK;
}

//bool is_registered_old(){
//	VMProtectBegin(MARKER_NAME);
//	int res;
//	res = VMProtectGetSerialNumberState();
//	if(res != 0){
//		char *serial = read_serial();
//		int res = -1;
//		if(serial != NULL)
//	    {
//			res = VMProtectSetSerialNumber(serial);
//
//			VMProtectSerialNumberData sd = {0};
//			VMProtectGetSerialNumberData(&sd, sizeof(sd));
//
//			printf("info user: %ls VIP user Only\n", sd.wUserName);
//
//		    delete [] serial;
//		}
//		if(res != 0){
//
//			// Ҳ�п��ܲ��������ˣ�
//			if(res == SERIAL_STATE_FLAG_MAX_BUILD_EXPIRED){
//				VMProtectSerialNumberData sd = {0};
//				VMProtectGetSerialNumberData(&sd, sizeof(sd));
//				printf("sorry you key is expiration!  max. date: y = %d, m = %d, d = %d\n", sd.dtMaxBuild.wYear, sd.dtMaxBuild.bMonth, sd.dtMaxBuild.bDay);
//				printf("please contact the agent to renew again!\n");				
//				return false;
//			}
//
//		    printf("info please registe! visit http://www.ggchess.com or contact ggchess agent \n");
//			int nSize = VMProtectGetCurrentHWID(NULL, 0); // get number of required bytes
//			char *pBuf = new char[nSize]; // allocate buffer
//			VMProtectGetCurrentHWID(pBuf, nSize); // obtain hardeare identifier
//
//			TCHAR HdidFileName[1024];
//			FILE *f;
//			swprintf_s(HdidFileName,sizeof(HdidFileName)/sizeof(TCHAR), L"%s\\HWID.txt",installDir);
//
//			if (0 != _wfopen_s(&f, HdidFileName, L"w")){
//				return false;
//			}
//			//�õ�ע���ʱ��
//#define SIZE 256
//			time_t ltime;
//			char buf[SIZE];
//			time(&ltime);
//			errno_t err;
//			err = ctime_s( buf, 128, &ltime );
//			fprintf_s(f,buf);
//			fprintf_s(f,"your hd ID is:\n--------------------------\n%08x%s",ltime,pBuf);			
//			fflush(f);
//			delete [] pBuf; // release buffer
//
//			///////////////////////////////////////////////////////////////////////////
//			// �ټ����Լ�����һЩ��Ϣ��
//			T_DEVICE_PROPERTY idp[WMI_QUERY_TYPENUM][8];
//			for (int i = 0; i < WMI_QUERY_TYPENUM; i++){
//				WMI_DeviceQuery(i, idp[i], 8);
//			}
//
//			MessageBox(NULL,L"Please send \"HWID.txt\" file to the agent \nwhich under ggchess engine catalog.",
//				L"Sorry not registe now!\nwww.ggchess.com",MB_OK);			
//			return false;
//		}
//		else{
//			// �õ��û���ע������
//			printf("info register ok! thank you!\n");
//			// �õ����տ������汾��ʱ��
//			VMProtectSerialNumberData sd = {0};
//			VMProtectGetSerialNumberData(&sd, sizeof(sd));
//			printf("info you can update until y = %d, m = %d, d = %d\n", sd.dtMaxBuild.wYear, sd.dtMaxBuild.bMonth, sd.dtMaxBuild.bDay);
//
//			return true;
//		}
//	}
//	VMProtectEnd();
//	return false;	
//}
#endif