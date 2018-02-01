#include "stdafx.h"
#include "AccountConfig.h"

#define _SELF_A		"AccountConfig.cpp"
#define _SELF		L"AccountConfig.cpp"

CAccountConfig::CAccountConfig()
{
}

CAccountConfig::~CAccountConfig()
{
}

BOOL CAccountConfig::ReadAccountConfigFile(__out string& strOutText)
{
	CHAR szConfigPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryA(MAX_PATH, szConfigPath);
	lstrcatA(szConfigPath, "\\Account.txt");

	FILE* pFile = nullptr;
	fopen_s(&pFile, szConfigPath, "r");
	if (pFile == nullptr)
	{
		CPrintLog::MsgBoxLog_A(_SELF_A, __LINE__, "��ȡ�ʺ��ļ�ʧ��,·��:%s", szConfigPath);
		return FALSE;
	}

	fseek(pFile, 0, SEEK_END);
	long len = ftell(pFile);
	char* pBuff = new char[len];
	if (pBuff == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"Alloc Memory Fiald!");
		return FALSE;
	}

	fseek(pFile, 0, SEEK_SET);
	fread(pBuff, sizeof(char), (size_t)len, pFile);

	fclose(pFile);

	strOutText = pBuff;
	delete[] pBuff;

	return TRUE;
}

BOOL CAccountConfig::ReadAccountConfig(__out vector<AccountConfigInfo>& vlst)
{
	wstring wstrTemp;
	string strFileText;
	static AccountConfigInfo __AccountConfigInfo;

	if (!ReadAccountConfigFile(strFileText))
		return FALSE;

	wstring wstrText = CCharacter::ASCIIToUnicode(strFileText);
	while (true)
	{
		int nIndex = wstrText.find(L"\n");
		if (nIndex == -1)
		{
			if (wstrText.find(L",") == -1)
				break;

			wstrTemp = wstrText;
			ZeroMemory(&__AccountConfigInfo, sizeof(__AccountConfigInfo));
			if (!GetAccountInfo_By_Text(wstrTemp.c_str(), __AccountConfigInfo))
				return FALSE;

			vlst.push_back(__AccountConfigInfo);
			break;
		}

		wstrTemp = wstrText.substr(0, nIndex);
		wstrText = wstrText.substr(nIndex + 1);

		if (!GetAccountInfo_By_Text(wstrTemp.c_str(), __AccountConfigInfo))
			continue;

		vlst.push_back(__AccountConfigInfo);
	}
	return TRUE;
}

BOOL CAccountConfig::GetAccountInfo_By_Text(__in LPCWSTR pwszText, __out AccountConfigInfo& ConfigInfo)
{
	static WCHAR wszText[1024];
	static WCHAR wszTemp[32];

	CCharacter::wstrcpy_my(wszText, pwszText);

	if (CCharacter::wstrstr_my(wszText, L","))
	{
		// 1951353641,liu1155,�þ�֮��,Ĭ��ģʽ,30/32000
		CCharacter::GetRemoveLeft(wszText, L",", wszTemp);
		CCharacter::GetRemoveRight(wszText, L",", wszText);
		CCharacter::wstrcpy_my(ConfigInfo.wszQQ, wszTemp);

		if (!CCharacter::wstrstr_my(wszText, L","))
			goto lb_Exit;

		// liu1155,�þ�֮��,Ĭ��ģʽ,30/32000
		CCharacter::GetRemoveLeft(wszText, L",", wszTemp);
		CCharacter::GetRemoveRight(wszText, L",", wszText);
		CCharacter::wstrcpy_my(ConfigInfo.wszPass, wszTemp);

		if (!CCharacter::wstrstr_my(wszText, L","))
			goto lb_Exit;

		// �þ�֮��,Ĭ��ģʽ,30/32000
		CCharacter::GetRemoveLeft(wszText, L",", wszTemp);
		CCharacter::GetRemoveRight(wszText, L",", wszText);
		CCharacter::wstrcpy_my(ConfigInfo.wszServer, wszTemp);

		if (!CCharacter::wstrstr_my(wszText, L"/"))
			goto lb_Exit;

		// Ĭ��ģʽ,30/32000
		CCharacter::GetRemoveLeft(wszText, L",", wszTemp);
		CCharacter::GetRemoveRight(wszText, L",", wszText);

		
		auto fnExistMap = [](LPCWSTR pwszMapName)
		{
			if (CCharacter::wstrcmp_my(pwszMapName, L"�ٻ�ʦϿ��"))
				return TRUE;
			if (CCharacter::wstrcmp_my(pwszMapName, L"ˮ��֮��"))
				return TRUE;
			if (CCharacter::wstrcmp_my(pwszMapName, L"������Ԩ"))
				return TRUE;
			if (CCharacter::wstrcmp_my(pwszMapName, L"Ť������"))
				return TRUE;
			return FALSE;
		};

		if (!fnExistMap(wszTemp))
		{
			ConfigInfo.PlayMode = AccountPlayMode_Default;
			CCharacter::wstrcpy_my(ConfigInfo.wszMapName, L"����");
		}
		else
		{
			ConfigInfo.PlayMode = AccountPlayMode_Custome;
			CCharacter::wstrcpy_my(ConfigInfo.wszMapName, wszTemp);
		}
		

		CCharacter::GetRemoveLeft(wszText, L",", wszTemp);
		CCharacter::GetRemoveRight(wszText, L",", wszText);
		if (!CCharacter::wstrstr_my(wszTemp, L"/"))
			goto lb_Exit;

		// 30/32000
		static WCHAR wszCondition[32];
		CCharacter::GetRemoveLeft(wszTemp, L"/", wszCondition);
		ConfigInfo.dwConditionLevel = (DWORD)_wtoi(wszCondition);

		CCharacter::GetRemoveRight(wszTemp, L"/", wszCondition);
		ConfigInfo.dwConditionGold = (DWORD)_wtoi(wszCondition);

		// ���
		if (CCharacter::wstrcmp_my(wszText,L"NULL"))
			CCharacter::wstrcpy_my(ConfigInfo.wszComputerName, L"");
		else
			CCharacter::wstrcpy_my(ConfigInfo.wszComputerName, wszText);
		
		return TRUE;
	}
	
lb_Exit:;
	CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��������ݸ�ʽ:%s", pwszText);
	return FALSE;
}

BOOL CAccountConfig::WriteFile(const string& strText, LPCSTR pszFileName, LPCSTR pszMode)
{
	if (pszFileName == NULL)
		return FALSE;

	CHAR szFilePath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryA(MAX_PATH, szFilePath);
	lstrcatA(szFilePath, "\\");
	lstrcatA(szFilePath, pszFileName);

	FILE* pFile = nullptr;
	fopen_s(&pFile, szFilePath, pszMode);
	if (pFile == nullptr)
	{
		CPrintLog::MsgBoxLog_A(_SELF_A, __LINE__, "д���ʺ��ļ�ʧ��,·��:%s", szFilePath);
		return FALSE;
	}

	fseek(pFile, 0, SEEK_END);
	fwrite((LPVOID)strText.c_str(), sizeof(char), (size_t)strText.length() + 1, pFile);
	fclose(pFile);

	return TRUE;
}

LPCWSTR CAccountConfig::FormatNumber(DWORD dwNumber)
{
	static WCHAR wszText[32];
	wsprintfW(wszText, L"%d", dwNumber);
	return wszText;
}

LPCWSTR CAccountConfig::FormatNumber(DWORD dwNumber1, DWORD dwNumber2)
{
	static WCHAR wszText[32];
	wsprintfW(wszText, L"%d/%d", dwNumber1, dwNumber2);
	return wszText;
}

LPCWSTR CAccountConfig::FormatNowTime()
{
	static WCHAR wszText[64];
	__time64_t long_time;

	_time64(&long_time);

	struct tm* newtime = nullptr;
	_localtime64_s(newtime, &long_time);
	wsprintfW(wszText, L"%4d-%2d-%2d %2d:%2d:%2d", newtime->tm_year, newtime->tm_mon, 
		newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

	return wszText;
}

BOOL CAccountConfig::SaveAccountConfig()
{
	wstring wstrText;
	vector<ServerAccountInfo> vlst;

	// Copy Data
	AccountListLock.Access([&vlst]{vlst = AccountList; });

	for (auto& __ServerAccountInfo : vlst)
	{
		wstrText += __ServerAccountInfo.wszQQ; // QQ
		wstrText += L",";
		wstrText += __ServerAccountInfo.wszPass; // Pass
		wstrText += L",";
		wstrText += __ServerAccountInfo.wszServer; // ����
		wstrText += L",";

		if (__ServerAccountInfo.PlayMode == AccountPlayMode_Default) // ��ͼ&&ģʽ
			wstrText += L"Ĭ��ģʽ"; 
		else
			wstrText += __ServerAccountInfo.wszMapName;

		wstrText += L",";
		wstrText += FormatNumber(__ServerAccountInfo.dwMaxLevel, __ServerAccountInfo.dwMaxGold); // �������
		wstrText += L",";

		if (CCharacter::wstrcmp_my(__ServerAccountInfo.wszComputerName, L""))
			wstrText += L"NULL";
		else
			wstrText += __ServerAccountInfo.wszComputerName;

		wstrText += L"\n";
	}

	string strText = CCharacter::UnicodeToASCII(wstrText);
	WriteFile(strText, "Account.txt", "w");

	return TRUE;
}

BOOL CAccountConfig::SaveFinishAccount(__in const ServerAccountInfo& __ServerAccountInfo)
{
	wstring wstrText;

	wstrText += L"QQ��:";
	wstrText += __ServerAccountInfo.wszQQ;
	wstrText += L",����:";
	wstrText += __ServerAccountInfo.wszPass;
	wstrText += L",����:";
	wstrText += __ServerAccountInfo.wszServer;
	wstrText += L",�ȼ�:";
	wstrText += FormatNumber(__ServerAccountInfo.dwLevel, __ServerAccountInfo.dwMaxLevel);
	wstrText += L",���:";
	wstrText += FormatNumber(__ServerAccountInfo.dwGold, __ServerAccountInfo.dwMaxGold);
	wstrText += L"���ʱ��:";
	wstrText += FormatNowTime();
	wstrText += L"\n";

	WriteFile(CCharacter::UnicodeToASCII(wstrText), "FinishAccount.txt", "a");
	return TRUE;
}

BOOL CAccountConfig::SaveErrorAccount(__in const ServerAccountInfo& __ServerAccountInfo)
{
	wstring wstrText;

	wstrText += L"QQ��:";
	wstrText += __ServerAccountInfo.wszQQ;
	wstrText += L",����:";
	wstrText += __ServerAccountInfo.wszPass;
	wstrText += L",����:";
	wstrText += __ServerAccountInfo.wszServer;
	wstrText += L",�ȼ�:";
	wstrText += FormatNumber(__ServerAccountInfo.dwLevel, __ServerAccountInfo.dwMaxLevel);
	wstrText += L",���:";
	wstrText += FormatNumber(__ServerAccountInfo.dwGold, __ServerAccountInfo.dwMaxGold);
	wstrText += L"ʱ��:";
	wstrText += FormatNowTime();
	wstrText += L",������Ϣ:";
	wstrText += __ServerAccountInfo.wszMsg;
	wstrText += L"\n";

	WriteFile(CCharacter::UnicodeToASCII(wstrText), "ErrorAccount.txt", "a");
	return TRUE;
}

BOOL CAccountConfig::SaveIdSealedAccount(__in const ServerAccountInfo& __ServerAccountInfo)
{
	wstring wstrText;

	wstrText += L"QQ��:";
	wstrText += __ServerAccountInfo.wszQQ;
	wstrText += L",����:";
	wstrText += __ServerAccountInfo.wszPass;
	wstrText += L",����:";
	wstrText += __ServerAccountInfo.wszServer;
	wstrText += L",�ȼ�:";
	wstrText += FormatNumber(__ServerAccountInfo.dwLevel, __ServerAccountInfo.dwMaxLevel);
	wstrText += L",���:";
	wstrText += FormatNumber(__ServerAccountInfo.dwGold, __ServerAccountInfo.dwMaxGold);
	wstrText += L",���!";
	wstrText += L" ʱ��:";
	wstrText += FormatNowTime();
	wstrText += L"\n";

	WriteFile(CCharacter::UnicodeToASCII(wstrText), "IdSealedAccount.txt", "a");
	return TRUE;
}