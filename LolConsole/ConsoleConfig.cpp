#include "ConsoleConfig.h"

#define _SELF L"ConsoleConfig.cpp"

CConsoleConfig::CConsoleConfig()
{
}

CConsoleConfig::~CConsoleConfig()
{
}

BOOL CConsoleConfig::WriteFightCount(DWORD dwCount)
{
	WCHAR wszValue[32] = { 0 };
	WCHAR wszLoginPath[MAX_PATH] = { 0 };


	::GetCurrentDirectoryW(MAX_PATH, wszLoginPath);
	lstrcatW(wszLoginPath, L"\\Login.ini");

	// format
	wsprintfW(wszValue, L"%d", dwCount);

	return WritePrivateProfileStringW(L"Config", L"FightCount", wszValue, wszLoginPath);
}

DWORD CConsoleConfig::GetFightCount()
{
	WCHAR wszValue[32] = { 0 };
	WCHAR wszLoginPath[MAX_PATH] = { 0 };


	::GetCurrentDirectoryW(MAX_PATH, wszLoginPath);
	lstrcatW(wszLoginPath, L"\\Login.ini");

	if (!GetPrivateProfileStringW(L"Config", L"FightCount", L"Error", wszValue, 32, wszLoginPath) || CCharacter::wstrcmp_my(wszValue, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡLogin.ini -> %sʧ��!", L"FightCount");
		return 0;
	}

	return (DWORD)_wtoi(wszValue);
}

BOOL CConsoleConfig::WriteGameResult()
{
	auto fnAppendFile = [](LPCSTR pszText)
	{
		FILE* pFile = NULL;
		fopen_s(&pFile, "GameResult.txt", "a+");
		if (pFile == NULL)
			return false;

		fseek(pFile, 0, SEEK_END);
		fwrite(pszText, sizeof(char), strlen(pszText) + 1, pFile);
		fclose(pFile);
		return true;
	};

	SYSTEMTIME st;
	static CHAR szText[64] = { 0 };

	::GetLocalTime(&st);
	wsprintfA(szText, "%4d-%2d-%2d %2d:%2d:%2d ս�����:", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	HANDLE hMutexGameResult_Win = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_GameResult_Win);
	if (hMutexGameResult_Win != NULL)
	{
		lstrcatA(szText, "ʤ��\r\n");
		fnAppendFile(szText);
		::ReleaseMutex(hMutexGameResult_Win);
		::CloseHandle(hMutexGameResult_Win);
		hMutexGameResult_Win = NULL;
		return TRUE;
	}

	HANDLE hMutexGameResult_Lose = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_GameResult_Lose);
	if (hMutexGameResult_Lose != NULL)
	{
		lstrcatA(szText, "ʧ��\r\n");
		fnAppendFile(szText);
		::ReleaseMutex(hMutexGameResult_Lose);
		::CloseHandle(hMutexGameResult_Lose);
		hMutexGameResult_Lose = NULL;
		return TRUE;
	}

	lstrcatA(szText, "δ֪\r\n");
	fnAppendFile(szText);
	return FALSE;
}

BOOL CConsoleConfig::SaveGameMap(LPCWSTR pwszMapName)
{
	WCHAR wszValue[32] = { 0 };
	WCHAR wszConfigPath[MAX_PATH] = { 0 };

	::GetCurrentDirectoryW(MAX_PATH, wszConfigPath);
	lstrcatW(wszConfigPath, L"\\Config.ini");

	return WritePrivateProfileStringW(L"Login", L"Map", pwszMapName, wszConfigPath);
}

BOOL CConsoleConfig::SetGameMap(LPCWSTR wszMapName)
{
	if (!GetGameMap_By_Name(wszMapName, g_MapType))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡ��ͼ����ʧ��,wszMapName=%s", wszMapName);
		return FALSE;
	}
	else
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"SetGameMap:%s", wszMapName);
	}
	
	auto CloseGameMapMutex = [](LPCWSTR pwszMutexName)
	{
		HANDLE hGameMapMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, pwszMutexName);
		if (hGameMapMutex != NULL)
		{
			::ReleaseMutex(hGameMapMutex);
			::CloseHandle(hGameMapMutex);
			hGameMapMutex = NULL;
		}
	};

	auto CreateGameMapMutex = [](const MapType& Mt)
	{
		if (Mt == MapType_�ٻ�ʦϿ��)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_0");
		else if (Mt == MapType_ˮ��֮��)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_1");
		else if (Mt == MapType_Ť������)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_2");
		else if (Mt == MapType_������Ԩ)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_3");
	};

	CloseGameMapMutex(L"CL_LOL_Game_MapType_0");
	CloseGameMapMutex(L"CL_LOL_Game_MapType_1");
	CloseGameMapMutex(L"CL_LOL_Game_MapType_2");
	CloseGameMapMutex(L"CL_LOL_Game_MapType_3");
	CreateGameMapMutex(g_MapType);
	return TRUE;
}

BOOL CConsoleConfig::GetGameMap_By_Name(LPCWSTR pwszMapName, MapType& Mt)
{
	BOOL bRetCode = FALSE;
	if (CCharacter::wstrcmp_my(pwszMapName, L"�ٻ�ʦϿ��"))
	{
		Mt = MapType_�ٻ�ʦϿ��;
		bRetCode = TRUE;
	}
	else if (CCharacter::wstrcmp_my(pwszMapName, L"ˮ��֮��"))
	{
		Mt = MapType_ˮ��֮��;
		bRetCode = TRUE;
	}
	else if (CCharacter::wstrcmp_my(pwszMapName, L"Ť������"))
	{
		Mt = MapType_Ť������;
		bRetCode = TRUE;
	}
	else if (CCharacter::wstrcmp_my(pwszMapName, L"������Ԩ"))
	{
		Mt = MapType_������Ԩ;
		bRetCode = TRUE;
	}
	return bRetCode;
}

BOOL CConsoleConfig::UpdateHosts()
{
	FILE* pFile = NULL;
	fopen_s(&pFile, "C:\\Windows\\System32\\drivers\\etc\\hosts", "w");
	if (pFile == NULL)
		return FALSE;

	//char szText[] = { "127.0.0.1 lol.qq.com" };
	char szText[] = { "" };
	fwrite(szText, 1, strlen(szText) + 1, pFile);
	fclose(pFile);
	return TRUE;
}

BOOL CConsoleConfig::GetTGPPath_By_Reg(LPWSTR pwszTGPPath)
{
	BOOL bRetCode = FALSE;
	HKEY hKey;
	DWORD dwRet;
	DWORD dwLen = MAX_PATH;

	dwRet = RegOpenKey(HKEY_CURRENT_USER, L"Software\\Tencent\\bugReport\\TGP\\", &hKey); //��ע��������
	if (dwRet == ERROR_SUCCESS)
	{
		dwRet = RegQueryValueExW(hKey, L"InstallDir", NULL, NULL, (BYTE *)pwszTGPPath, &dwLen);  //��ȡ�Ҳ���ַ���ֵ
		if (dwRet == ERROR_SUCCESS)
		{
			//������pwszTGPPath�������Ҫ��ȡ��ֵ
			bRetCode = TRUE;
		}
	}
	RegCloseKey(hKey);
	return bRetCode;
}

BOOL CConsoleConfig::GetGamePath_By_Process(LPWSTR pwszGamePath)
{
	if (CLProcess::Is_Exist_Process_For_ProcName(LOL_CLIENTPROC_NAME))
	{
		MODULEENTRY32 ME32 = { 0 };
		CLProcess::GetProcessModule_For_Name(LOL_CLIENTPROC_NAME, LOL_CLIENTPROC_NAME, ME32);

		CCharacter::GetRemoveLeft(ME32.szExePath, L"\\Air", pwszGamePath);
		return TRUE;
	}
	return FALSE;
}