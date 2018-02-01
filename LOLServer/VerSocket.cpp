#include "VerSocket.h"
#define _SELF L"VerSocket.cpp"

//////////////��////////////////////////////////////////////////////////////
#define LOCKSTACK EnterCriticalSection(&g_CrSection)
#define UNLOCKSTACK LeaveCriticalSection(&g_CrSection)


//////////////////ȫ�ֱ�����ʼ��////////////////////////////////////////////////////////
CRITICAL_SECTION g_CrSection;
CRITICAL_SECTION g_LstCrSection;
SocketStatus g_SocketStatus = SocketStatus_Normal;
BOOL			g_bCloseClient = FALSE;



/////////////////����ʵ��/////////////////////////////////////////////////////////
bool CMy_st_server::on_msg(msg_type& msg)
{
	return false;
}

bool CMy_st_server::on_msg_handle(msg_type& msg, bool link_down)
{
	static StackMsg SM;
	ZeroMemory(&SM, sizeof(SM));
	SM.pst_server = this->shared_from_this();
	SM.msg = msg;

	LOCKSTACK;
	GetTaskQueue()->push(SM);
	UNLOCKSTACK;
	return true;
}

stack<StackMsg>* CMy_st_server::GetTaskQueue()
{
	static stack<StackMsg> TaskQueue;
	return &TaskQueue;
}

static BOOL _AnalysisPacket(DWORD dwCmd, __in ByteBuffer& Bf, __out LPVOID lpParm)
{
	auto fn_GetStr = [](ByteBuffer& Bf, wstring& wstr)
	{
		Bf >> wstr;
		if (Bf.size() == 0)
			return FALSE;
		return TRUE;
	};

	auto fn_GetDWORD = [](ByteBuffer& Bf, DWORD& dwResult)
	{
		Bf >> dwResult;
	};

	static wstring wstr;
	if (dwCmd == SOCKET_MESSAGE_RECHARGELOGINUSER || dwCmd == SOCKET_MESSAGE_KEEPALIVE || dwCmd == SOCKET_MESSAGE_GETGROUP || 
		dwCmd == SOCKET_MESSAGE_GROUP_MEMBER || dwCmd == SOCKET_MESSAGE_LEAVEGROUP || dwCmd == SOCKET_MESSAGE_GETACCOUNT ||
		dwCmd == SOCKET_MESSAGE_CLIENTMSG || dwCmd == SOCKET_MESSAGE_RECORD || dwCmd == SOCKET_MESSAGE_GAMETITLE)
	{
		SockClientGroup* pClientGroup = (SockClientGroup*)lpParm;
		if (pClientGroup == NULL)
			return FALSE;

		// wszComputerName
		if (!fn_GetStr(Bf, wstr))
			return FALSE;
		CCharacter::wstrcpy_my(pClientGroup->wszComputerName, wstr.c_str());

		// GameUser'QQ'
		if (!fn_GetStr(Bf, wstr))
			return FALSE;
		CCharacter::wstrcpy_my(pClientGroup->SLD.wszGameUser, wstr.c_str());

		// NiceName
		if (!fn_GetStr(Bf, wstr))
			return FALSE;
		CCharacter::wstrcpy_my(pClientGroup->SLD.wszNiceName, wstr.c_str());

		// ServerName
		if (!fn_GetStr(Bf, wstr))
			return FALSE;
		CCharacter::wstrcpy_my(pClientGroup->wszServerName, wstr.c_str());

		// MapName
		if (!fn_GetStr(Bf, wstr))
			return FALSE;
		CCharacter::wstrcpy_my(pClientGroup->wszMapName, wstr.c_str());

		// nGold
		Bf >> pClientGroup->SLD.nGold;

		// nLevel
		Bf >> pClientGroup->SLD.nLevel;

		// nSumWarCount
		Bf >> pClientGroup->SLD.nSumWarCount;

		// �ͻ�������Ϣ����
		if (dwCmd == SOCKET_MESSAGE_CLIENTMSG)
		{
			if (!fn_GetStr(Bf, wstr))
				return FALSE;
			CCharacter::wstrcpy_my(pClientGroup->wszClientMsg, wstr.c_str());
		}

		return TRUE;
	}

	
	return FALSE;
}

DWORD WINAPI HandleAffairThread(LPVOID lpParm)
{
	AffairThreadSrc* pThreadSrc = (AffairThreadSrc*)lpParm;
	if (pThreadSrc == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"pThreadSrc = NULL");
		return 0;
	}

	// ��ȡ�������
	auto TaskQueue = CMy_st_server::GetTaskQueue();
	UINT uVisitCount = 0;					// 5m�ڵķ�����
	UINT64 uTotalVisitCount = 0;			// �ܷ�����
	auto ulTotalTick = GetTickCount64();	// ������ʱ��
	auto dwTick = ::GetTickCount64();		// ����ʱ��
	auto dwRefushLoseTick = ::GetTickCount64();

	while (!pThreadSrc->bExit)
	{
		if (CMy_st_server::GetTaskQueue()->empty())
		{
			Sleep(10);
			continue;
		}

		LOCKSTACK;
		auto pst_server = CMy_st_server::GetTaskQueue()->top(); // ��ȡջ��Ԫ��
		CMy_st_server::GetTaskQueue()->pop();	// ��ջ
		UNLOCKSTACK;

		// ��ȡԶ�̷��ʿͻ��˵�IP
		static WCHAR wszRemoteClientIp[16];
		boost::system::error_code ec;
		CCharacter::ASCIIToUnicode(pst_server.pst_server->lowest_layer().remote_endpoint(ec).address().to_string().c_str(), wszRemoteClientIp);

		// ͳ�Ʒ��ʴ���
		uVisitCount++;
		if (::GetTickCount64() - dwTick >= 60 * 60 * 1000)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"60�����ڷ��ʴ�����=%d, ÿ�벢����=%d", uVisitCount, uVisitCount / 60 / 60 / 60);
			static WCHAR wszRunDate[32] = { 0 };
			CLPublic::FormatTime(GetTickCount64() - ulTotalTick, wszRunDate);
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"������ʱ����:%s", wszRunDate);
			dwTick = ::GetTickCount64();
			uVisitCount = 0;
		}
		if (::GetTickCount64() - dwRefushLoseTick >= 10 * 60 * 1000)
		{
			//CVerificationSystem::_RefushLoseCon();
			dwRefushLoseTick = ::GetTickCount64();
		}

		// Var
		DWORD dwCmd = 0;
		ByteBuffer Bf;
		ByteBuffer RetBf;
		SockClientGroup ClientGroup;
		ZeroMemory(&ClientGroup, sizeof(ClientGroup));

		// ��������, ֱ�ӷ�������
		if (g_bCloseClient)
		{
			RetBf << SOCKET_MESSAGE_CLOSECLIENT << SOCKET_MESSAGE_CLOSECLIENT;
			pst_server.pst_server->post_msg((CHAR*)RetBf.contents(), RetBf.size(), FALSE);
			continue;
		}

		// ��ȡSocket����
		Bf.append(pst_server.msg.data(), pst_server.msg.size());

		// ��ȡcmd���
		Bf >> dwCmd;
		RetBf << dwCmd;
		
		EnterCriticalSection(&g_LstCrSection);
		switch (dwCmd)
		{
		case SOCKET_MESSAGE_RECHARGELOGINUSER:
			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
				RetBf << SOCKET_MESSAGE_RETMSG << L"��¼�������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			else
				CVerificationSystem::RechargeLoginUser(ClientGroup.SLD, wszRemoteClientIp, RetBf);
			break;
		case SOCKET_MESSAGE_KEEPALIVE:

			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
				RetBf << SOCKET_MESSAGE_RETMSG << L"�����������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			else
				CVerificationSystem::KeepAlive(dwCmd, ClientGroup, wszRemoteClientIp, RetBf);

			break;
		case SOCKET_MESSAGE_GETGROUP:
			// �Ȱ�SockLoginData ����ṹ���������, �ٴ���SockClientGroupʣ�µ�����
			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ӽ������!�����Կ���? ʵ�ڲ��о����Ϸ�!");
				RetBf << SOCKET_MESSAGE_RETMSG << L"��ӽ������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			}
			else if (g_SocketStatus == SocketStatus_Pause)
			{
				RetBf << SOCKET_MESSAGE_WAIT; // һֱ�ȴ�!
			}
			else
			{
				CVerificationSystem::_InitSockClientGroup(ClientGroup);
				CVerificationSystem::_HandleGroup(ClientGroup, RetBf);
			}
			break;
		case SOCKET_MESSAGE_LEAVEGROUP:
			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
				RetBf << SOCKET_MESSAGE_RETMSG << L"���Ž������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			else
				CVerificationSystem::_LeaveGroup(ClientGroup, RetBf);

			break;
		case SOCKET_MESSAGE_GAMETITLE:
			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
				RetBf << SOCKET_MESSAGE_RETMSG << L"��Ž������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			else
				CVerificationSystem::_IdSealed(ClientGroup, RetBf);
			
			break;
		case SOCKET_MESSAGE_CLIENTMSG:
			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
				RetBf << SOCKET_MESSAGE_RETMSG << L"��Ϣ�������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			else
				CVerificationSystem::_ClientMsg(ClientGroup, RetBf);
			break;
		case SOCKET_MESSAGE_RECORD:
			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
				RetBf << SOCKET_MESSAGE_RETMSG << L"ս���������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			else
				CVerificationSystem::_GameRecord(ClientGroup, RetBf);
			break;
		case SOCKET_MESSAGE_GETACCOUNT:
			if (!_AnalysisPacket(dwCmd, Bf, &ClientGroup) || Bf.rpos() != Bf.wpos())
				RetBf << SOCKET_MESSAGE_RETMSG << L"��ȡ�ʺŽ������!�����Կ���? ʵ�ڲ��о����Ϸ�!";
			else
				CVerificationSystem::_GetAccount(ClientGroup, RetBf);
			break;
		default:
			RetBf << SOCKET_MESSAGE_RETMSG << L"�������ʲô��?";
			break;
		}
		LeaveCriticalSection(&g_LstCrSection);
		pst_server.pst_server->post_msg((CHAR*)RetBf.contents(), RetBf.size(), FALSE);
	}

	return 0;
}

BOOL Lol_OutputFile()
{
	// Init Var 
	vector<Server_GroupMember> tmpGroupLst;
	vector<SockClientGroup> tmpWaitGroupLst;
	
	// Lock and Copy Memory
	EnterCriticalSection(&g_LstCrSection);
	tmpGroupLst.assign(CVerificationSystem::GroupLst.begin(), CVerificationSystem::GroupLst.end());
	tmpWaitGroupLst.assign(CVerificationSystem::WaitGroupLst.begin(), CVerificationSystem::WaitGroupLst.end());
	LeaveCriticalSection(&g_LstCrSection);

	wstring strGroupLst;
	DWORD dwOnlineCount = 0;
	auto fn_Convert_SockClientGroup_Str = [&dwOnlineCount](SockClientGroup& SCG, wstring& str, BOOL bWait, LPCWSTR pwszCaptainName)
	{
		static WCHAR wszText[1024];
		wsprintfW(wszText, L"%s: Server:%s Map:%s QQ:%s LoginUser:%s NiceName:%s Level:%d Gold:%d SumWarCount:%d \r\n",
			bWait ? L"Wait" : L"Game",
			SCG.wszServerName, SCG.wszMapName,
			SCG.SLD.wszGameUser, SCG.SLD.wszLoginUser, SCG.SLD.wszNiceName,
			SCG.SLD.nLevel, SCG.SLD.nGold, SCG.SLD.nSumWarCount);
		str += wszText;
		dwOnlineCount++;
	};

	strGroupLst = L"WaitMemberList:";
	for (auto& SCG : tmpWaitGroupLst)
	{
		fn_Convert_SockClientGroup_Str(SCG, strGroupLst, TRUE, SCG.SLD.wszNiceName);
	}

	strGroupLst += L"\r\n\r\nGroupList:\r\n";

	for (auto& tmpServer_GroupMember : tmpGroupLst)
	{
		for (auto& tmpMap_GroupMember : tmpServer_GroupMember.vMaplst)
		{
			for (auto& tmpGroupList : tmpMap_GroupMember.vGrouplst)
			{
				fn_Convert_SockClientGroup_Str(tmpGroupList.Captain, strGroupLst, FALSE, tmpGroupList.Captain.SLD.wszNiceName);
				for (auto& GroupMember : tmpGroupList.MemberList)
					fn_Convert_SockClientGroup_Str(GroupMember, strGroupLst, FALSE, tmpGroupList.Captain.SLD.wszNiceName);
			}
		}
	}

	auto fn_WriteFile = [](wstring& wstr)
	{
		FILE* pFile = NULL;
		pFile = _wfopen(L"Output.txt", L"w, ccs=UNICODE");
		if (pFile == NULL)
		{
			cout << "create Output.txt faild!" << endl;
			return;
		}

		fwprintf(pFile, L"%s", wstr.c_str());
		fclose(pFile);
	};
	fn_WriteFile(strGroupLst);

	cout << "������������:" << dwOnlineCount << endl;
	return TRUE;
}