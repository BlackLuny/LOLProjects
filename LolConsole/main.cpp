#include <iostream>
#include "VerClientSocket.h"
#include "Hall.h"
#include "CreatePlayer.h"
#include "ConsoleConfig.h"

//#include "BindStatusCallback.h"
using namespace std;
#define _SELF L"main.cpp"

static DWORD GetAreaId_ByName(LPCWSTR pwszName)
{
	typedef struct _AreaSrc
	{
		WCHAR wszAreaName[32];
		DWORD dwAreaId;
	}AreaSrc;

	const int nMaxArrayLen = 27;
	static AreaSrc AreaSrcArray[nMaxArrayLen] = {
		L"��ŷ����", 1,
		L"�ȶ�������", 2,
		L"�氲", 3,
		L"ŵ����˹", 4,
		L"��������", 5,
		L"��¶���", 6,
		L"Ƥ�����ַ�", 7,
		L"ս��ѧԺ", 8,
		L"���׶�׿��", 9,
		L"�����", 10,
		L"��ɪ�ر�", 11,
		L"��η�ȷ�", 12,
		L"�þ�֮��", 13,
		L"��ɫõ��", 14,
		L"��Ӱ��", 15,
		L"��������", 17,
		L"ˡ����", 16,
		L"�������", 19,
		L"ˮ��֮��", 18,
		L"������ר��", 21,
		L"Ӱ��", 22,
		L"����֮��", 23,
		L"Ť������", 20,
		L"����֮��", 24,
		L"��������", 25,
		L"Ƥ�Ǿ���", 27,
		L"����֮��", 26
	};

	for (int i = 0; i < nMaxArrayLen; ++i)
	{
		if (CCharacter::wstrcmp_my(pwszName, AreaSrcArray[i].wszAreaName))
			return AreaSrcArray[i].dwAreaId;
	}

	return 0;
}

static BOOL GetWebInfo()
{
	// д����ҳ��¼
	g_pSharedInfo->MyGameInfo[0].Lock();
	auto pWebLoginInfo = &g_pSharedInfo->MyGameInfo[0].webInfo;
	CCharacter::wstrcpy_my(pWebLoginInfo->wszGameUser, g_ClientGroup.SLD.wszGameUser);
	CCharacter::wstrcpy_my(pWebLoginInfo->wszGamePass, g_ClientGroup.SLD.wszPass);
	CCharacter::wstrcpy_my(pWebLoginInfo->wszdamaUser, wszdamaUser);
	CCharacter::wstrcpy_my(pWebLoginInfo->wszdamaPass, wszdamaPass);
	pWebLoginInfo->dwAreId = GetAreaId_ByName(g_ClientGroup.wszServerName);
	g_pSharedInfo->MyGameInfo[0].UnLock();
	if (pWebLoginInfo->dwAreId == 0)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��ȡ������ʧ��, �����ڷ�����:%s", g_ClientGroup.wszServerName);
		return FALSE;
	}
	

	wcout << L"�ȴ���ҳ��ȡ�ʺ��ǳơ�������" << endl;
	if (!CLProcess::Is_Exist_Process_For_ProcName(L"WebLogin.exe"))
	{
		WinExec("WebLogin.exe", SW_SHOW);
		Sleep(1000);
	}

	if (g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel == -1)
		g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel = 0;

	while (true)
	{
		g_pSharedInfo->MyGameInfo[0].Lock();
		if (g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel == (DWORD)-1)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ѯʧ�ܡ���X��");
			CLProcess::TerminateProc_For_ProcName(L"WebLogin.exe");
			return FALSE;
		}
		if (g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel > 0)
		{
			auto pWebPerson = &g_pSharedInfo->MyGameInfo[0].webPerson;
			CCharacter::wstrcpy_my(g_ClientGroup.SLD.wszNiceName, pWebPerson->wszNiceName);
			g_ClientGroup.SLD.nGold = pWebPerson->dwGold;
			g_ClientGroup.SLD.nLevel = pWebPerson->dwLevel;
			g_ClientGroup.SLD.nSumWarCount = pWebPerson->dwSumWarCount;
			break;
		}
		g_pSharedInfo->MyGameInfo[0].UnLock();
		Sleep(1000);
	}
	

	return TRUE;
}

CHAR szServerIp[32] = { 0 };
BOOL ReadConfig()
{
	WCHAR wszIniPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszIniPath);
	lstrcatW(wszIniPath, L"\\Config.ini");

	if (!CConsoleConfig::GetTGPPath_By_Reg(g_pSharedInfo->wszTGPPath))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡTGP·��ʧ��!");
		return FALSE;
	}

	WCHAR wszTmpPath[MAX_PATH];
	if (!CLPublic::FileExit(L"C:\\admin.CL"))
	{
		// �ж��Ƿ����d3d8thk.dll��d3d8th.dll
		auto fn_DeleteFile = [&wszTmpPath](LPCWSTR pwszDllName)
		{
			CCharacter::wstrcpy_my(wszTmpPath, wszGamePath);
			lstrcatW(wszTmpPath, L"\\Game\\");
			lstrcatW(wszTmpPath, pwszDllName);
			if (CLPublic::FileExit(wszTmpPath))
				::DeleteFileW(wszTmpPath);
		};
		
		//fn_DeleteFile(L"d3d8th.dll");
		//fn_DeleteFile(L"d3d8thk.dll");
	}

	CCharacter::wstrcpy_my(wszTmpPath, g_pSharedInfo->wszTGPPath);
	lstrcatW(wszTmpPath, L"\\tgp_daemon.exe");
	if (!CLPublic::FileExit(wszTmpPath))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"TGP·������=%s", wszTmpPath);
		return FALSE;
	}

	ZeroMemory(&g_ClientGroup, sizeof(g_ClientGroup));

	if (!GetPrivateProfileStringW(L"Login", L"damaUser", L"Error", wszdamaUser, MAX_PATH, wszIniPath) || CCharacter::wstrcmp_my(wszdamaUser, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡ�������ʺ�ʧ��!");
		return FALSE;
	}

	if (!GetPrivateProfileStringW(L"Login", L"damaPass", L"Error", wszdamaPass, MAX_PATH, wszIniPath) || CCharacter::wstrcmp_my(wszdamaPass, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡ����������ʧ��!");
		return FALSE;
	}

	if (!GetPrivateProfileStringW(L"Login", L"ID", L"Error", g_ClientGroup.wszComputerName, 32, wszIniPath) || CCharacter::wstrcmp_my(g_ClientGroup.wszComputerName, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡ���ʧ��!");
		return FALSE;
	}

	WCHAR wszServerIp[32] = { 0 };
	if (!GetPrivateProfileStringW(L"Login", L"ServerIp", L"Error", wszServerIp, 32, wszIniPath) || CCharacter::wstrcmp_my(wszServerIp, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡ������Ipʧ��!");
		return FALSE;
	}
	CCharacter::UnicodeToASCII(wszServerIp, szServerIp);

	return TRUE;
}

static bool CreateMapView()
{
	//����ļ�ӳ��
	HANDLE hFileMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZFILE_NAME_SHAREDINFO);
	if (hFileMap == NULL)
	{
		hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024 * 1024, SZFILE_NAME_SHAREDINFO);
		if (hFileMap == NULL)
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"���ļ�ӳ��ʧ��");
			return false;
		}
	}

	BYTE * pCheckBuf = (BYTE *)MapViewOfFile(hFileMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
	if (pCheckBuf == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"ӳ���ļ�ʧ��");
		return false;
	}

	wchar_t szFilePath[MAX_PATH] = { 0 };
	wsprintf(szFilePath, L"%s.SharedMem", SZFILE_NAME_SHAREDINFO);
	HANDLE hFileSharedInfo = CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE,          // open for reading
		FILE_SHARE_READ | FILE_SHARE_WRITE,       // share for reading
		NULL,                  // default security
		CREATE_ALWAYS,         // existing file only
		FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, // normal file
		NULL);
	if (hFileSharedInfo == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"ӳ���ļ�������");
		return FALSE;
	}

	hFileMap = CreateFileMapping(hFileSharedInfo, NULL, PAGE_READWRITE, 0, sizeof(SHARED_INFO), SZFILE_NAME_SHAREDINFO);
	if (hFileMap == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"����ӳ��ʧ��!");
		return FALSE;
	}

	//�ر��ļ����
	::CloseHandle(hFileSharedInfo);

	g_pSharedInfo = (PSHARED_INFO)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, sizeof(SHARED_INFO));
	if (!g_pSharedInfo)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��֪��˵ʲô���ˡ���");
		return false;
	}

	//��ʼ�������ڴ�ռ�
	memset(g_pSharedInfo, 0, sizeof(SHARED_INFO));
	g_pSharedInfo->nRunTGPCount = 0;
	g_pSharedInfo->nAccountCount = 1;

	//���ÿ���̨��·������ϷDLL
	/*CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPPath, L"F:\\Program Files\\Tencent\\TGP");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameUser, L"1743051060");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGamePass, L"liu1155");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaUser, L"chchch25");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaPass, L"123456");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameServer, L"�þ�֮��");
	g_pSharedInfo->MyGameInfo[0].webInfo.dwAreId = GetAreaId_ByName(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameServer);*/

	return true;
}

DWORD WINAPI WorkThread(LPVOID lpParm)
{
	static st_service_pump service_pump;
	static CVerClient client(service_pump);
	static CVerClientSocket ClientSocket;
	static CCreatePlayer CCP;
	static CHall Hall(&ClientSocket);

	//////////����socket////////////////////////////////////////////////////////////////
	cout << "ServerIP:" << szServerIp << endl;
	ClientSocket.pVerClient = &client;
	ClientSocket.pservice_pump = &service_pump;
	client.set_server_addr(12365, szServerIp);
	service_pump.start_service(1);
	if (!service_pump.is_running())
	{
		service_pump.stop_service();
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"�޷����ӷ�����!");
		goto Function_Exit;
	}
	ClientSocket.Start();
	Sleep(5 * 1000);

	while (!client.is_connected())
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ȴ���������Ӧ����");
		Sleep(10 * 1000);
	}

	////////�ӷ���˻�ȡ�ʺ�//////////////////////////////////////////////////////////////////
	ClientSocket._Send_GetAccount();

lb_Loop:;
	//////////����TGP��LOL����////////////////////////////////////////////////////////////////
	hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net �ͻ���");
	if (hWndLuncher == NULL)
	{
		Hall.RunLolClient();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"����, �Ѿ�����LOL!");
	}

	////////////������ɫ//////////////////////////////////////////////////////////////
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ж��Ƿ���Ҫ������ɫ");
	if (!GetWebInfo())
	{
		// ��ʼ������ɫ
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��Ҫ������ɫ");
		CCP.CreatePlayer();

		if (!GetWebInfo())
		{
			static int nCount = 0;
			if (++nCount >= 3)
			{
				CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"����������ɫ3�ζ�ʧ��!");
			}
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡ��ɫ����ʧ��!");
			goto lb_Loop;
		}
	}

	/////////�Ŷ�/////////////////////////////////////////////////////////////////
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"�ж��Ŷ�!");
	while (Hall.IsQueuing())
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"�Ŷ��С���");
		Sleep(5 * 1000);
		continue;
	}

	// �Ƿ񱻷��
	if (Hall.IsSealUser())
	{
		// ���ͷ����Ϣ 
		ClientSocket._Send_IdSealed();

		// �ر�socket
		ClientSocket.pVerClient->disconnect(false);
		ClientSocket.pVerClient->force_close(false);

		// �ر�WebLogin.exe
		CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);

		Sleep(5000);
		exit(0);
		goto lb_Loop;
	}

	////////ս��ѭ��//////////////////////////////////////////////////////////////////
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ʼѭ��!");
	while (TRUE)
	{
		// ��������
		if (Hall.IsLoseCon())
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"�����������Ͳ�Ҫ���������!��");
			ClientSocket._Send_ClientMsg(L"��������������Ϸ!");
			goto lb_CreateTeam;
		}

		
		/*if (g_ClientGroup.bMatching)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"��������²���ӡ���");
			goto lb_CreateTeam;
		}*/

		// �ȴ����������
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ȴ�����������С���");
		//ClientSocket._Send_ClientMsg(L"��ʼ�ȴ����������!");
		//if (!ClientSocket._Send_JoinGroup())
		//{
		//	g_bCloseGame = TRUE;
		//	goto lb_LeaveGroup;
		//}

		g_ClientGroup.bMatching = TRUE;
		g_ClientGroup.bMatchMachine = TRUE;
		g_ClientGroup.Difficulty = GameDifficulty_Normal;
		if (g_ClientGroup.SLD.nSumWarCount >= 30)
		{
			if (rand() % 5 == 2)
				CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, L"�ٻ�ʦϿ��");
			else
				CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, L"ˮ��֮��");

		}
		else
		{
			CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, L"�ٻ�ʦϿ��");
		}
		CConsoleConfig::SetGameMap(g_ClientGroup.wszMapName);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ǳ�:%s,��ͼ:%s,�ʺ�:%s,nSumWarCount=%d", g_ClientGroup.SLD.wszNiceName, g_ClientGroup.wszMapName, g_ClientGroup.SLD.wszGameUser, g_ClientGroup.SLD.nSumWarCount);

	lb_CreateTeam:;
		HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Game_Load_Wait");
		if (hMutex != NULL)
		{
			::ReleaseMutex(hMutex);
			::CloseHandle(hMutex);
			hMutex = NULL;
		}

		Hall.Init();
		if (g_bCaptain || g_ClientGroup.bMatching)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"���Ƕӳ�,׼�����ӳ��Ĳ���!");
			Hall.CaptainHanle();
		}
		else
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ֻ�Ƕ�Ա����, ��������");
			Hall.MemberHandle();
		}

//	lb_LeaveGroup:;
		// ս�����������!
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"ս�����������");
		//ClientSocket._Send_LeaveGroup();

		// �Ƿ���
		/*if (g_bGameTitle)
		{
			// ���ͷ����Ϣ 
			ClientSocket._Send_IdSealed();

			// �ر�socket
			ClientSocket.pVerClient->disconnect(false);
			ClientSocket.pVerClient->force_close(false);

			// �ر�WebLogin.exe
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);

			Sleep(5000);
			exit(0);
		}*/

		// ���»�ȡ�ʺ�
		//ClientSocket._Send_GetAccount();
		if (g_bSwitchAccount && FALSE)
		{
			// ��Ҫ�����ʺ�, �п����ǵȼ�ˢ����, �п����Ǻű�������, �п����Ǵ˺��ڷ������Ѿ���������
			// �ر�TGP
			CLProcess::TerminateProc_For_ProcName(L"tgp_render.exe");
			CLProcess::TerminateProc_For_ProcName(L"tgp_daemon.exe");
			CLProcess::TerminateProc_For_ProcName(L"tcls_core.exe");
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(L"WebLogin.exe");
			CLProcess::TerminateProc_For_ProcName(L"TGPLogin.exe");
			Sleep(3 * 1000);
			goto lb_Loop;
		}

		if (g_bCloseGame)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ҪX����Ϸ,X������̨~!");
			g_bCloseGame = FALSE;
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
		}

		
		Hall.RunLolClient();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ȴ�0���Ӻ�, ����������������!");
		//Sleep(2 * 60 * 1000);
	}

	CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��Ϸ������ˡ���������~!");

Function_Exit:;
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"�̲߳����Ѿ�ֹͣ��~!");
	hWorkThread = NULL;
	return 0;
}

DWORD WINAPI RemoteInjectorThread(LPVOID lpParm)
{
	BOOL bExistProc = FALSE;
	while (true)
	{
		Sleep(1000);
		if (!bExistProc)
		{
			// �ȴ����̳���, Ȼ��ע��
			if (CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"׼��ע��DLL");
				Sleep(20 * 1000);

				// ����ǰ��
				auto fn_SwitchWindow = [](HWND hWnd)
				{
					if (hWnd != NULL)
					{
						ShowWindow(hWnd, SW_RESTORE);
						::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						SwitchToThisWindow(hWnd, TRUE);
					}
				};
				fn_SwitchWindow(::FindWindowW(L"RiotWindowClass", NULL));

				static WCHAR wszDLLPath[MAX_PATH];
				::GetCurrentDirectoryW(MAX_PATH, wszDLLPath);
				lstrcatW(wszDLLPath, L"\\d3d8th.dll");
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ʼע��!");
				if (CLProcess::LoadRemoteDLL(LOL_GAMEPROC_NAME, wszDLLPath))
					bExistProc = TRUE;
				else
					CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"ע��ʧ��!");
			}
		}
		else
		{
			// �жϽ����Ƿ���ʧ����
			if (!CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"������ʧ�ˡ������ſ�ʼ����!");
				bExistProc = FALSE;
			}
		}

	}
	return 0;
}

static BOOL RunDebugLog()
{
	CHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH, wszPath);
	lstrcatA(wszPath, "\\Log\\DebugLog.exe");

	if (!CLProcess::Is_Exist_Process_For_ProcName(L"DebugLog.exe"))
	{
		cout << "����DebugLog.exe" << endl;
		WinExec(wszPath, SW_SHOW);
		Sleep(5 * 1000);
	}
	return TRUE;
}

static VOID ClearFile()
{
	auto fnDeleteFile = [](LPCWSTR pwszFileName)
	{
		static WCHAR wszCurrentPath[MAX_PATH];

		::GetCurrentDirectoryW(MAX_PATH, wszCurrentPath);
		lstrcatW(wszCurrentPath, L"\\");
		lstrcatW(wszCurrentPath, pwszFileName);

		if (CLPublic::FileExit(wszCurrentPath))
			::DeleteFileW(wszCurrentPath);
	};

	fnDeleteFile(L"CrackCaptcha.log");
	fnDeleteFile(L"LOLServer.dll");
	fnDeleteFile(L"Login.ini");
	fnDeleteFile(L"������־.rtf");
}

int main(int argc, CHAR* argv[])
{
	RunDebugLog();
	HANDLE hConsoleMutex = ::CreateMutexW(NULL, FALSE, L"Lol_More_Console_Mutex");
	if (hConsoleMutex != NULL && ::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"ֻ�ܿ�1��LolConsole.exe����!");
		return 0;
	}

	if (argc <= 1)
		return 0;

	if (CCharacter::strcmp_my(argv[1], "-test"))
	{
		CBmpFind dm;
		int x, y;
		WCHAR wszPath[MAX_PATH];
		::GetCurrentDirectoryW(MAX_PATH, wszPath);
		lstrcatW(wszPath, L"\\Bmp");
		dm.SetPath(wszPath);
		dm.BindWindow(::GetDesktopWindow());
		if (dm.FindPic(0, 0, 1920, 1080, L"test.bmp", x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"1111111111");
		}
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"ret");
		return 0;
	}
	else if (CCharacter::strcmp_my(argv[1], "-map"))
	{
		if (CCharacter::strcmp_my(argv[2], "-�ٻ�ʦϿ��"))
		{
			CConsoleConfig::SetGameMap(L"�ٻ�ʦϿ��");
		}
		else if (CCharacter::strcmp_my(argv[2], "-ˮ��֮��"))
		{
			CConsoleConfig::SetGameMap(L"ˮ��֮��");
		}
		else if (CCharacter::strcmp_my(argv[2], "-������Ԩ"))
		{
			CConsoleConfig::SetGameMap(L"������Ԩ");
		}
		Sleep(-1);
	}
	else if (CCharacter::strcmp_my(argv[1], "-start"))
	{
		if (!CreateMapView())
			return 0;

		// ��������
		::CreateMutexW(NULL, FALSE, L"CL_LOL_Console_Mutex");

		// ����UniCode���
		SetConsole_Language_CHINA;

		while (true)
		{
			cout << "Give Me Order!" << endl;
			char szText[32] = { 0 };
			//cin >> szText;

			if (CCharacter::strcmp_my(szText, "-start") || TRUE)
			{
				if (hWorkThread != NULL)
				{
					cout << "�Ѿ����������ˡ�������, ���ظ�����,֪����?" << endl;
					continue;
				}
				if (!CLProcess::Is_Exist_Process_For_ProcName(L"QQ.exe"))
				{
					cout << "��������QQ�ſ���!" << endl;
					//continue;
				}

				// �ر���Ϸ!
				CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
				CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
				CLProcess::TerminateProc_For_ProcName(L"tgp_daemon.exe");
				if (!ReadConfig())
				{
					cout << "��ȡ�����ļ�ʧ��" << endl;
					Sleep(-1);
				}

				// �޸�hosts
				if (!CConsoleConfig::UpdateHosts())
				{
					cout << "�޸�hosts�ļ�ʧ��, �ǲ���û����ԱȨ��?" << endl;
					Sleep(-1);
				}

				// �����Ϲ������������ļ�
				ClearFile();

				cout << "�Ѿ�����ƥ���߳�! ʣ�µ���Ϣ��ȫ�����Դ���־�й۲�~!" << endl;
				g_dwCmd = GameCmd_Start;
				hWorkThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkThread, NULL, NULL, NULL);
				///::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RemoteInjectorThread, NULL, NULL, NULL);
			}
			else if (CCharacter::strcmp_my(szText, "-stop"))
			{
				StopGame;
				if (WaitForSingleObject(hWorkThread, 3000) == WAIT_TIMEOUT)
				{
					::TerminateThread(hWorkThread, 0);
					::CloseHandle(hWorkThread);
				}
				hWorkThread = NULL;
				cout << "�ɹ��������߳���!" << endl;
			}
			Sleep(-1);
		}
	}
	

	
	
	return 0;
}
