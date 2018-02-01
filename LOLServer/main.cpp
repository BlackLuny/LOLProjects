#include "VerSocket.h"
#define _SELF L"Socket_main.cpp"

extern "C" __declspec(dllexport) int __cdecl StartSocket()
{
	static AffairThreadSrc ATS;
	ZeroMemory(&ATS, sizeof(ATS));
	ATS.bExit = FALSE;

	static WCHAR wszValue[32];
	static WCHAR wszIniPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszIniPath);
	lstrcatW(wszIniPath, L"\\Config.ini");

	WCHAR wszServerIp[32] = { 0 };
	if (!GetPrivateProfileStringW(L"Login", L"ServerIp", L"Error", wszServerIp, 32, wszIniPath) || CCharacter::wstrcmp_my(wszServerIp, L"Error"))
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��ȡ������IPʧ��!");
		return FALSE;
	}
	
	CHAR szIP[32];
	CCharacter::UnicodeToASCII(wszServerIp, szIP);

	static st_service_pump service_pump;
	static st_server_base<CMy_st_server> server_(service_pump);
	server_.set_server_addr(12365, szIP/*SOCKET_IP*/);
	service_pump.start_service();
	
	if (service_pump.is_running())
	{
		//CVerificationSystem::InitCardInfo();
		CVerificationSystem::_InitGroup();
		// ��ʼ���ٽ���
		InitializeCriticalSection(&g_CrSection);
		InitializeCriticalSection(&g_LstCrSection);

		HANDLE hThread = chBEGINTHREADEX(NULL, NULL, HandleAffairThread, &ATS, NULL, NULL);
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"�������!");
		return TRUE;
	}
	return FALSE;
}

extern "C" __declspec(dllexport) int __cdecl PauseSocket()
{
	g_SocketStatus = SocketStatus_Pause;
	return TRUE;
}

extern "C" __declspec(dllexport) int __cdecl RecoverySocket()
{
	g_SocketStatus = SocketStatus_Normal;
	return TRUE;
}

extern "C" __declspec(dllexport) void __cdecl CloseALLClient()
{
	g_bCloseClient = TRUE;
}

extern "C" __declspec(dllexport) int __cdecl SetAccountList(const vector<ServerAccountInfo>& vlst)
{
	// �˴�������Crc��ֵ����, Ȼ���ö��ַ�����,���Ż�Ч��

	vector<ServerAccountInfo> tmplst;
	AccountListLock.Access([&tmplst]{
		tmplst.assign(AccountList.begin(), AccountList.end());
	});

	auto fnGetIndex = [&tmplst](DWORD dwQqCrc)
	{
		for (int i = 0; i < (int)tmplst.size(); ++i)
		{
			if (tmplst.at(i).CompQqCrc(dwQqCrc))
			{
				return i;
			}
		}
		return -1;
	};

	for (auto& tmp1 : vlst)
	{
		int nIndex = fnGetIndex(tmp1.dwQqCrc);
		if (nIndex == -1)
		{
			// ���һ���ʺ�
			tmplst.push_back(tmp1);
		}
		else
		{
			// �޸�һ��������
			auto pServerAccountInfo = &tmplst.at(nIndex);
			CCharacter::wstrcpy_my(pServerAccountInfo->wszServer, tmp1.wszServer);	// ����
			CCharacter::wstrcpy_my(pServerAccountInfo->wszMapName, tmp1.wszMapName); // ��ͼ
			pServerAccountInfo->dwMaxGold = tmp1.dwMaxGold;							// ���
			pServerAccountInfo->dwMaxLevel = tmp1.dwMaxLevel;						// �ȼ�
			pServerAccountInfo->PlayMode = tmp1.PlayMode;							// ģʽ
		}
	}

	for (int i = 0; i < (int)tmplst.size(); ++i)
	{
		BOOL bExist = FALSE;
		auto pServerAccountInfo = &tmplst.at(i);
		for (auto& Item : vlst)
		{
			if (pServerAccountInfo->CompQqCrc(Item.dwQqCrc))
			{
				bExist = TRUE;
				break;
			}
		}

		if (!bExist)
		{
			// ���ʺű�ɾ����
			tmplst.erase(tmplst.begin() + i--);
		}
	}

	AccountListLock.Access([&tmplst]{
		AccountList.clear();
		AccountList.assign(tmplst.begin(), tmplst.end());
	});

	return TRUE;
}

extern "C" __declspec(dllexport) int __cdecl GetAccountList(vector<ServerAccountInfo>& vlst)
{
	vlst.clear();

	AccountListLock.Access([&vlst]{
		vlst.assign(AccountList.begin(), AccountList.end());
	});
	return TRUE;
}

int main()
{
	StartSocket();
	Sleep(-1);
	return 0;
}