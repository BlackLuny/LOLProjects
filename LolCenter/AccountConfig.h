#ifndef __LOL_CENTER_ACCOUNTCONFIG_H__
#define __LOL_CENTER_ACCOUNTCONFIG_H__

#include "AccountModel.h"

typedef struct _AccountConfigInfo
{
	WCHAR wszQQ[16];			// QQ
	WCHAR wszPass[32];			// QQ����
	WCHAR wszServer[8];			// �ĸ���
	WCHAR wszMapName[8];		// ��ͼ
	DWORD dwConditionLevel;		// ��������ĵȼ�
	DWORD dwConditionGold;		// ��������Ľ��
	AccountPlayMode PlayMode;
	WCHAR wszComputerName[16];	// ID
}AccountConfigInfo;

class CAccountConfig
{
public:
	CAccountConfig();
	~CAccountConfig();

	// ��ȡ�ļ�
	static BOOL ReadAccountConfig(__out vector<AccountConfigInfo>& vlst);

	// �����Ѿ�ˢ����ʺ�,(����ǰ�ǵ�ʹ��CAccountModel::RemoveAccount)
	static BOOL SaveFinishAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	// ���������ʺ�,(����ǰ�ǵ�ʹ��CAccountModel::RemoveAccount)
	static BOOL SaveErrorAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	// �����ŵ��ʺ�,(����ǰ�ǵ�ʹ��CAccountModel::RemoveAccount)
	static BOOL SaveIdSealedAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	// ���汸������
	static BOOL SaveAccountConfig();
	
	// ��ʽ��1������
	static LPCWSTR FormatNumber(DWORD dwNumber);

	// ��ʽ��2������
	static LPCWSTR FormatNumber(DWORD dwNumber1, DWORD dwNumber2);

	// ���ص�ǰʱ��
	static LPCWSTR FormatNowTime();

	// ����AccountInfo
	static BOOL GetAccountInfo_By_Text(__in LPCWSTR pwszText, __out AccountConfigInfo& ConfigInfo);


private:
	static BOOL ReadAccountConfigFile(__out string& strOutText);
	
	static BOOL WriteFile(const string& strText, LPCSTR pszFileName, LPCSTR pszMode);
	
};



#endif