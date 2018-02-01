#include "CreatePlayer.h"

#define _SELF L"CreatePlayer.cpp"

CCreatePlayer::CCreatePlayer()
{
	// ��ȡ����λ��
	CSearchPic::GetGameRect(m_LauncherRect);

	// ��ʼ��ͼɫ�ж�
	WCHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	m_BmpFind.SetPath(wszPath);

	m_BmpFind.BindWindow(CSearchPic::GetGameHwnd());
}

CCreatePlayer::~CCreatePlayer()
{
}

BOOL CCreatePlayer::ClickPoint(int x, int y)
{
	for (int i = 0; i < 1; ++i)
	{
		// �������
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, NULL, NULL);
		mouse_event(MOUSEEVENTF_LEFTUP, x, y, NULL, NULL);
	}
	//CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"ClickPoint");
	return TRUE;
}

BOOL CCreatePlayer::WriteCreatePlayerConfig(BOOL bCreate)
{
	static WCHAR wszIniPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszIniPath);
	lstrcatW(wszIniPath, L"\\Login.ini");

	::WritePrivateProfileStringW(L"Config", L"PlayerEmpty", bCreate ? L"1" : L"0", wszIniPath);
	return TRUE;
}

BOOL CCreatePlayer::IsCreatePlayerSucc()
{
	int x, y;
	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"ChoosePlayerIco.bmp", x, y))
		return TRUE;

	return FALSE;
}

BOOL CCreatePlayer::IsLauncherMainPage()
{
	int x, y;
	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"4.bmp", x, y))
		return TRUE;

	return FALSE;
}

BOOL CCreatePlayer::CreatePlayer()
{
	int x = 0, y = 0;
	CSearchPic::GetGameRect(m_LauncherRect);

	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"HideMsg1.bmp", x, y) > 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��Ҫ��·!HideMsg1");
		ClickPoint(m_LauncherRect.left + x + 20, m_LauncherRect.top + y + 20);
		Sleep(5000);
	}

	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"CreatePlayer.bmp", x, y) > 0)
	{
	lb_Start:;
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"�Զ�������ɫ����!");
		//52*250
		ClickPoint(m_LauncherRect.left + 620, m_LauncherRect.top + 494);
		for (int j = 0; j < 20; ++j)
			CLPublic::SendKey(8);

		Sleep(500);
		WCHAR wszName[32] = { 0 };
		GetRandName(wszName);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"wszName=%s", wszName);
		CLPublic::SendKeys(wszName);
		Sleep(1000);
		ClickPoint(m_LauncherRect.left + 649, m_LauncherRect.top + 573);
		Sleep(3000);

		// �Ƿ񴴽���ɫ�ɹ�
		if (!IsCreatePlayerSucc())
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"������ɫʧ��, ������������!");
			ClickPoint(m_LauncherRect.left + 647, m_LauncherRect.top + 438);
			Sleep(2000);
			goto lb_Start;
		}

		// ѡ���ɫͼ��
		Sleep(1000);
		ClickPoint(m_LauncherRect.left + 385, m_LauncherRect.top + 324);
		Sleep(500);
		ClickPoint(m_LauncherRect.left + 385, m_LauncherRect.top + 324);

		// ѡ���ɫͼ����ȷ��
		Sleep(1000);
		ClickPoint(m_LauncherRect.left + 971, m_LauncherRect.top + 550);
		Sleep(3000);

		// ѡ����Ϸ����Ϥ�̶�
		ClickPoint(m_LauncherRect.left + 865, m_LauncherRect.top + 365);
		Sleep(500);
		ClickPoint(m_LauncherRect.left + 900, m_LauncherRect.top + 545);
		Sleep(2000);

		// �Ƿ���ʵսѵ��Ӫ����Լ�
		ClickPoint(m_LauncherRect.left + 700, m_LauncherRect.top + 512);
		Sleep(5000);
	}

	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"12.bmp", x, y) > 0)
	{
		ClickPoint(m_LauncherRect.left + 865, m_LauncherRect.top + 365);
		Sleep(500);
		ClickPoint(m_LauncherRect.left + 900, m_LauncherRect.top + 545);
		Sleep(2000);

		ClickPoint(m_LauncherRect.left + 700, m_LauncherRect.top + 512);
		Sleep(5000);
	}

	if (TRUE || IsLauncherMainPage())
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"������ɫ���!");
	}

	// ��������
	//CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
	//Sleep(3000);

	// ���ڻ����ڵĻ�, ���Է��͹ر���Ϣ������
	//HWND m_hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net �ͻ���");
	//if (m_hWndLuncher != NULL)
		//::PostMessageW(m_hWndLuncher, WM_CLOSE, NULL, NULL);

//	Sleep(2000);
	return TRUE;
}

BOOL CCreatePlayer::IsCreate()
{
	static WCHAR wszValue[32];
	static WCHAR wszIniPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszIniPath);
	lstrcatW(wszIniPath, L"\\Login.ini");

	// ��ȡ�����ļ�, �Ƿ���Ҫ������ɫ
	if (!GetPrivateProfileStringW(L"Config", L"PlayerEmpty", L"Error", wszValue, 32, wszIniPath) || CCharacter::wstrcmp_my(wszValue, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ȡPlayerEmptyʧ��!");
		::TerminateProcess(::GetCurrentProcess(), 0);
		return FALSE;
	}

	// PlayerEmpty=0, ��ʾ����������µ�, ������Ҫ������ɫ
	if (CCharacter::wstrcmp_my(wszValue, L"0"))
		return TRUE;

	int x = 0, y = 0;
	m_BmpFind.BindWindow(CSearchPic::GetGameHwnd());
	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"CreatePlayer.bmp", x, y) > 0) // ����Ҫ������ɫ
		return TRUE;

	return FALSE;
}

BOOL CCreatePlayer::GetRandName(LPWSTR pwszText)
{
	const WCHAR wszName1[26][2]
	{
		L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o", L"p", L"q", L"r",
			L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z"
	};

	const WCHAR wszName2[3][2]
	{
		L"��", L"ؼ", L"��"
	};

	const WCHAR wszName3[26][2]
	{
		L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R",
		L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z"
	};

	const int nMaxLen = 544;
	static WCHAR szName4[nMaxLen][5] = {
		L"��", L"Ǭ", L"�", L"�溺", L"��Ӣ", L"����", L"һЦ",
		L"ѷ", L"�", L"�", L"�꺣", L"�ɰ�", L"ڤ��", L"����",
		L"��", L"��", L"��", L"����", L"����", L"ʿ��", L"ɽ��",
		L"��", L"��", L"��", L"����", L"��Σ", L"�е�", L"����",
		L"��", L"��", L"��", L"��б", L"�ɷ�", L"����", L"һն",
		L"��", L"��", L"��", L"����", L"����", L"����", L"����",
		L"��", L"ڤ", L"��", L"����", L"��ʩ", L"���", L"�޽�",
		L"��", L"��", L"��", L"����", L"��б", L"����", L"����",
		L"��", L"Ӻ", L"��", L"����", L"����", L"����", L"Զ��",
		L"��", L"��", L"ذ", L"ǧ��", L"����", L"ǧ��", L"һ��",
		L"��", L"��", L"��", L"����", L"����", L"����", L"�̿�",
		L"��", L"��", L"��", L"һ��", L"��֮", L"��Ե", L"��Ѫ",
		L"��", L"��", L"��", L"����", L"����", L"��֮", L"����",
		L"��", L"��", L"��", L"����", L"�˴�", L"����", L"����",
		L"��", L"��", L"��", L"һ��", L"��Ӣ", L"����", L"����",
		L"��", L"��", L"��", L"�Ͼ�", L"�˽�", L"����", L"��ɽ",
		L"��", L"��", L"��", L"����", L"����", L"����", L"����",
		L"��", L"��", L"��", L"����", L"Ԫ��", L"����", L"�׺�",
		L"��", L"��", L"��", L"ʮ��", L"����", L"��ʩ", L"���",
		L"��", L"��", L"��", L"�籪", L"��Э", L"�˷�", L"��Թ",
		L"��", L"��", L"��", L"����", L"����", L"����", L"���",
		L"��", L"��", L"��", L"��ʨ", L"����", L"����", L"����",
		L"��", L"��", L"��", L"���", L"��ɽ", L"����", L"����",
		L"�", L"��", L"��", L"����", L"����", L"Զ��", L"����",
		L"��", L"��", L"��", L"ΰ��", L"׷��", L"Զ��", L"�޵�",
		L"�", L"ң", L"��", L"����", L"�ѵ�", L"Զɽ", L"���",
		L"��", L"��", L"��", L"�߷�", L"����", L"��Ե", L"����",
		L"ʤ", L"��", L"ϡ", L"����", L"�Ѵ�", L"����", L"����",
		L"ӥ", L"��", L"��", L"�촨", L"��ʤ", L"����", L"����",
		L"��", L"��", L"��", L"��", L"�µ�", L"����", L"��Ѫ",
		L"��", L"��", L"��", L"����", L"����", L"����", L"����",
		L"��", L"��", L"��", L"����", L"��ƽ", L"һ��", L"�绨",
		L"��", L"��", L"��", L"ţ��", L"����", L"����", L"����",
		L"��", L"��", L"��", L"��ɽ", L"����", L"Ԫ��", L"��Ц",
		L"��", L"ն", L"���", L"�ж�", L"����", L"��Ѫ",
		L"ԫ", L"��", L"����", L"�Ӷ�", L"ʿ��", L"����",
		L"ʦ", L"��", L"�Ϻ�", L"�ٴ�", L"����", L"�޼�",
		L"��", L"��", L"��а", L"����", L"����", L"����",
		L"��", L"��", L"��ɽ", L"����", L"����", L"����",
		L"��", L"��", L"һ��", L"���", L"��ͷ", L"����",
		L"��", L"��", L"ʮ��", L"�ж�", L"��ͨ",
		L"��", L"Ө", L"��", L"����", L"����", L"һһ", L"����",
		L"��", L"ӱ", L"��", L"����", L"ǧ��", L"ҡ٤", L"����",
		L"��", L"ӯ", L"ܾ", L"Ī��", L"����", L"��ӥ", L"����",
		L"��", L"ӣ", L"��", L"��ɫ", L"Ӣ��", L"����", L"����",
		L"�", L"ӫ", L"��", L"����", L"����", L"����", L"����",
		L"ͮ", L"ө", L"�", L"����", L"�ɷ�", L"����", L"���",
		L"ͩ", L"ݺ", L"��", L"����", L"����", L"����", L"��ħ",
		L"֥", L"��", L"��", L"����", L"ɺɺ", L"���", L"��",
		L"��", L"�", L"��", L"����", L"��ѩ", L"Ψѩ", L"��Ѫ",
		L"��", L"ܽ", L"��", L"����", L"ѩһ", L"����", L"����",
		L"��", L"��", L"��", L"����", L"����", L"���", L"Ѫ��",
		L"��", L"��", L"��", L"����", L"ӫӫ", L"���", L"����",
		L"��", L"��", L"��", L"̤��", L"����", L"����", L"��Ѫ",
		L"��", L"��", L"��", L"ܽ��", L"����", L"һ��", L"ѩڤ",
		L"��", L"��", L"ɺ", L"�㿪", L"��һ", L"���", L"����",
		L"��", L"��", L"��", L"����", L"����", L"����", L"����",
		L"��", L"�", L"��", L"����", L"��Ө", L"���", L"а��",
		L"��", L"��", L"��", L"����", L"����", L"̫��", L"��ķ",
		L"��", L"˿", L"��", L"��Ӣ", L"��ϧ", L"Ī��", L"����",
		L"��", L"�", L"����", L"����", L"ĪӢ", L"���",
		L"��", L"��", L"����", L"����", L"��ѩ", L"һ��",
		L"��", L"�", L"�߻�", L"����", L"��̫", L"����",
		L"��", L"�", L"����", L"�ɳ�", L"����", L"��Ȼ",
		L"��", L"��", L"ʢ��", L"��ڤ", L"���", L"�ñ�",
		L"��", L"��", L"���", L"���", L"Ե��", L"��ѩ",
		L"ݸ", L"��", L"����", L"����", L"ڤ��", L"����",
		L"��", L"��", L"һ��", L"����", L"Ϫ��", L"����",
		L"��", L"��", L"�绨", L"����", L"����", L"���",
		L"�", L"��", L"����", L"��ͥ", L"̫Ӣ", L"ҹ��",
		L"�", L"�", L"����", L"��«", L"̫��", L"�ؽ�",
		L"��", L"��", L"����", L"�ɷ�", L"���", L"�ü�",
		L"��", L"ܿ", L"����", L"�޴�", L"�Ƴ�", L"����",
		L"��", L"��", L"ɵ��", L"����", L"����", L"��ħ",
		L"�", L"��", L"�Է�", L"˫˫", L"��Ȼ", L"����",
		L"��", L"о", L"Ө֥", L"����", L"����", L"�ػ�",
		L"��", L"��", L"����", L"����", L"����", L"����",
		L"��", L"��", L"̫��", L"��Ƚ", L"����", L"һ��",
		L"��", L"��", L"�Ѹ�", L"�ﾡ", L"����", L"ܿң",
		L"Ƽ", L"��", L"����", L"����", L"����", L"����",
		L"�", L"��", L"��ƽ", L"����", L"����", L"����",
		L"Ӥ", L"��", L"����", L"����", L"Է��", L"СС"
	};

	srand(::GetTickCount());
	int nR1 = 0 + rand() % (26 - 1 - 0 + 1);
	int nR2 = 0 + rand() % (3 - 1 - 0 + 1);
	int nR3 = 0 + rand() % (26 - 1 - 0 + 1);
	int nR4 = 0 + rand() % (544 - 1 - 0 + 1);

	int nR5 = 0 + rand() % (4 - 1 - 0 + 1);
	switch (nR5)
	{
	case 0:
		lstrcatW(pwszText, szName4[nR4]);
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, wszName3[nR3]);
		lstrcatW(pwszText, wszName1[nR1]);
		break;
	case 1:
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, wszName3[nR3]);
		lstrcatW(pwszText, wszName1[nR1]);
		lstrcatW(pwszText, szName4[nR4]);
		break;
	case 2:
		lstrcatW(pwszText, wszName1[nR3]);
		lstrcatW(pwszText, wszName3[nR1]);
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, szName4[nR4]);
		break;
	default:
		lstrcatW(pwszText, szName4[nR4]);
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, wszName1[nR3]);
		lstrcatW(pwszText, wszName3[nR1]);
		break;
	}
	return TRUE;
	//////////////////////////////////////////////////////////////////////////
	/*CHAR szText[32] = { 0 };
	while (strlen(szText) < 10)
	{
		int nR4 = 0 + rand() % (nMaxLen - 1 - 0 + 1);
		lstrcatA(szText, szNameArray[nR4]);
		Sleep(500);
	}

	WCHAR wszTemp[32] = { 0 };
	ASCIIToUnicode(szText, wszTemp);
	lstrcatW(pwszText, wszTemp);*/

	return TRUE;
}