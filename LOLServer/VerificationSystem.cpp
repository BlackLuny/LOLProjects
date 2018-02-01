#include "VerificationSystem.h"
#include <algorithm>
#define _SELF L"SvrVS.cpp"

vector<Server_GroupMember> CVerificationSystem::GroupLst;
vector<SockClientGroup> CVerificationSystem::WaitGroupLst;
vector<SockKeepConSrc> CVerificationSystem::SocketConectLst;

vector<ServerAccountInfo> AccountList;
CLLock AccountListLock;

CVerificationSystem::CVerificationSystem()
{

}
CVerificationSystem::~CVerificationSystem()
{

}

BOOL CVerificationSystem::InitCardInfo()
{
	vector<CardInfo> vlst;
	CDataBaseMdel::GetCardInfo(vlst);

	CPrintLog::PrintLog_W(_SELF, __LINE__, L"���ݿ���Ч����.size()=%d", vlst.size());
	CLoginManger::_InitCard(vlst);
	return TRUE;
}

BOOL CVerificationSystem::RechargeLoginUser(SockLoginData& SLD, LPCWSTR pwszIp, __out ByteBuffer& RetBf)
{
	/*if (wcslen(SLD.Card.szCard) > (sizeof(SLD.Card.szCard) / sizeof(WCHAR)))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"RechargeLoginUser.�޷����յ���ȷ�Ŀ���!");
		RetBf << SOCKET_MESSAGE_RETMSG << L"�޷����յ���Ч�Ŀ���!";
		return TRUE;
	}

	DWORD dwCardHour = CLoginManger::_CheckCard(SLD.Card.szCard);
	if (dwCardHour == 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"RechargeLoginUser.��������!!");
		RetBf << SOCKET_MESSAGE_RETMSG << L"�޷����յ���Ч�Ŀ���!��������!";
		return TRUE;
	}

	// Init Card Lst
	WCHAR wszSQL[128] = { 0 };
	wsprintfW(wszSQL, L"exec [RechargeCard] '%s', '%s', %d, '%s'", SLD.Card.szCard, SLD.wszLoginUser, dwCardHour, pwszIp);
	WCHAR wszRetMsg[64] = { 0 };
	if (!CDataBaseMdel::ExcuteSQL(wszSQL, wszRetMsg))
	{
		RetBf << SOCKET_MESSAGE_RETMSG << L"�޷����ӵ������������ݿ�~!��ϵ����~!����:0x6";
		return TRUE;
	}

	if (CCharacter::wstrstr_my(wszRetMsg, L"�ɹ�"))
	{
		DWORD dwTime = CLoginManger::_CheckCard(SLD.Card.szCard);
		DWORD dwToken = CCharacter::GetRand_For_DWORD();
		CLoginManger::AddNewCard(SLD.Card, dwTime, dwToken, pwszIp);
		wsprintfW(wszRetMsg, L"����->%X", dwToken);
		RetBf << SOCKET_MESSAGE_RECHARGELOGINUSER << dwTime << wszRetMsg;
		return TRUE;
	}

	LoginGameUser* pLgu = CLoginManger::InitLoginUser(SLD, pwszIp, wszRetMsg);
	if (pLgu == NULL)
	{
		RetBf << SOCKET_MESSAGE_RETMSG << wszRetMsg;
		return TRUE;
	}

	DWORD dwTime = (DWORD)(pLgu->dwSurplusHour - ((::GetTickCount() - pLgu->dwSurplusTick) / 1000 / 60 / 60));
	RetBf << SOCKET_MESSAGE_RECHARGELOGINUSER << dwTime << wszRetMsg;*/
	return TRUE;
}

BOOL CVerificationSystem::GetCardInfo_By_LoginUser(SockLoginData& SLD, LPCWSTR pwszIp, __out ByteBuffer& RetBf)
{
	return TRUE;
}

BOOL CVerificationSystem::SetSockKeepCon(__in const SockClientGroup& ClientGroup)
{
	DWORD dwComputerCrc = CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.wszComputerName, wcslen(ClientGroup.wszComputerName) * 2 + 1);

	BOOL bExist = FALSE;
	for (auto& __SockKeepConSrc : SocketConectLst)
	{
		if (__SockKeepConSrc.dwComputerCrc == dwComputerCrc)
		{
			__SockKeepConSrc.dwQqCrc = ClientGroup.dwGameUserCrc;
			__SockKeepConSrc.ulTick = ::GetTickCount64();
			bExist = TRUE;
		}
	}

	if (!bExist)
	{
		SockKeepConSrc SKC = { 0 };
		SKC.ulTick = ::GetTickCount64();
		SKC.dwQqCrc = ClientGroup.dwGameUserCrc;
		SKC.dwComputerCrc = dwComputerCrc;
		SocketConectLst.push_back(SKC);
	}

	return TRUE;
}

BOOL CVerificationSystem::KeepAlive(DWORD dwCmd, SockClientGroup& ClientGroup, LPCWSTR pwszIp, __out ByteBuffer& RetBf)
{
	/*if (CLoginManger::_GetCardTime(ClientGroup.SLD.Card.szCard, ClientGroup.SLD.dwVerCode, pwszIp) == 0)
		{
		RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"WTF? ����,������ʱ�� ò�ƿ��Ų���ʱ����!")/ *�˿������ڻ��߱�������* /;
		return TRUE;
		}*/

	// ��ʼ��Crc
	_InitSockClientGroup(ClientGroup);

	// ����
	SetSockKeepCon(ClientGroup);

	// ��ȡ��Ӧ�ķ������б�
	auto pGroupServer = _ExistGroupServer(ClientGroup.wszServerName);
	if (pGroupServer == NULL)
	{
		RetBf << SOCKET_MESSAGE_KEEPALIVE;
		//RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"����, �������ʲô�Ʒ�����,���������û���ѡ��!");
		return TRUE;
	}

	// ��ȡ��Ӧ�ĵ�ͼ�б�
	auto pGroupMap = _ExistGroupMap(pGroupServer, ClientGroup.wszMapName);
	if (pGroupMap == NULL)
	{
		RetBf << SOCKET_MESSAGE_KEEPALIVE;
		//RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"����, ��������ʲô�Ƶ�ͼ? ��Ӿ�4�ŵ�ͼ,���ڸ�ʲô�һ�?");
		return TRUE;
	}

	for (auto& Group : pGroupMap->vGrouplst)
	{
		// �������(���, �ȼ�, �ܳ���)
		if (Group.Captain == ClientGroup)
		{
			Group.Captain.SLD = ClientGroup.SLD;
			Group.Captain.ulTick = ::GetTickCount64();
		}
		else
		{
			BOOL bExist = FALSE;
			for (auto& GroupMember : Group.MemberList)
			{
				if (GroupMember == ClientGroup)
				{
					bExist = TRUE;
					GroupMember.SLD = ClientGroup.SLD;
					GroupMember.ulTick = ::GetTickCount64();
					break;
				}
			}

			if (bExist)
				break;
		}
	}

	// �ط�����ͷ ������ʾ����
	RetBf << SOCKET_MESSAGE_KEEPALIVE;
	return TRUE;
}

////////����//////////////////////////////////////////////////////////////////

BOOL CVerificationSystem::_InitSockClientGroup(SockClientGroup& ClientGroup)
{
	CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.SLD.wszGameUser, wcslen(ClientGroup.SLD.wszGameUser) * 2 + 1, ClientGroup.dwGameUserCrc);
	CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.SLD.wszLoginUser, wcslen(ClientGroup.SLD.wszLoginUser) * 2 + 1, ClientGroup.dwLoginUserCrc);
	CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.SLD.wszNiceName, wcslen(ClientGroup.SLD.wszNiceName) * 2 + 1, ClientGroup.dwNiceNameCrc);
	ClientGroup.ulTick = ::GetTickCount64();
	return TRUE;
}

BOOL CVerificationSystem::_InitGroup()
{
	auto fn_GetCrc_By_ServerName = [](LPCWSTR pwszServerName)
	{
		static Srt_ServerGroup SSG;
		ZeroMemory(&SSG, sizeof(SSG));

		CCharacter::wstrcpy_my(SSG.wszServerName, pwszServerName);
		CLPublic::GetCRC32_DWORD((LPSTR)pwszServerName, wcslen(pwszServerName) * 2 + 1, SSG.dwServerCrc);
		return &SSG;
	};

	/////////����/////////////////////////////////////////////////////////////////
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��ŷ����"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"�氲"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"ŵ����˹"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��¶���"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"Ƥ�����ַ�"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"ս��ѧԺ"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"�����"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��ɪ�ر�"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��������"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"�þ�֮��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��ɫõ��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��Ӱ��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"�������"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"ˮ��֮��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"Ӱ��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"����֮��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"����֮��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��������"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"Ƥ�Ǿ���"));

	////////��ͨ//////////////////////////////////////////////////////////////////
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"�ȶ�������"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��������"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"���׶�׿��"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"��η�ȷ�"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"ˡ����"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"Ť������"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"����֮��"));

	/////////����/////////////////////////////////////////////////////////////////
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"������ר��"));

	return TRUE;
}

BOOL CVerificationSystem::_CreateGroupServer(Srt_ServerGroup* pServerGroup)
{
	// ��ȡ��ͼCrc
	auto fn_GetCrc_By_MapName = [](LPCWSTR pwszMapName)
	{
		static Srt_MapGroup SMG;
		ZeroMemory(&SMG, sizeof(SMG));

		CCharacter::wstrcpy_my(SMG.wszMapName, pwszMapName);
		CLPublic::GetCRC32_DWORD((LPSTR)pwszMapName, wcslen(pwszMapName) * 2 + 1, SMG.dwMapCrc);
		return &SMG;
	};

	// ��ʼ��
	static Server_GroupMember SGM;
	vector<Map_GroupMember> vMaplst;
	ZeroMemory(&SGM, sizeof(SGM));

	// ��ÿһ���������¶�����4����ͼ
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"�ٻ�ʦϿ��"));
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"ˮ��֮��"));
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"Ť������"));
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"������Ԩ"));

	// ��ӵ��������б�
	SGM.Srt_Server = *pServerGroup;
	SGM.vMaplst = vMaplst;
	GroupLst.push_back(SGM);
	return TRUE;
}

BOOL CVerificationSystem::_CreateGroupMap(vector<Map_GroupMember>& vMaplst, Srt_MapGroup* pMapGroup)
{
	static Map_GroupMember MGM;
	ZeroMemory(&MGM, sizeof(MGM));
	MGM.Srt_Map = *pMapGroup;
	//MGM.vGrouplst
	vMaplst.push_back(MGM);
	return TRUE;
}

Server_GroupMember* CVerificationSystem::_ExistGroupServer(LPCWSTR pwszServerName)
{
	DWORD dwServerCrc = 0;
	CLPublic::GetCRC32_DWORD((LPSTR)pwszServerName, wcslen(pwszServerName) * 2 + 1, dwServerCrc);

	for (auto& GroupServer : GroupLst)
	{
		if (GroupServer.Srt_Server.dwServerCrc == dwServerCrc && CCharacter::wstrcmp_my(pwszServerName, GroupServer.Srt_Server.wszServerName))
			return &GroupServer;
	}

	return NULL;
}

Map_GroupMember* CVerificationSystem::_ExistGroupMap(Server_GroupMember* pGroupServer, LPCWSTR pwszMapName)
{
	if (pGroupServer == NULL || pwszMapName == NULL)
		return NULL;

	DWORD dwMapCrc = 0;
	CLPublic::GetCRC32_DWORD((LPSTR)pwszMapName, wcslen(pwszMapName) * 2 + 1, dwMapCrc);

	for (auto& GroupMap : pGroupServer->vMaplst)
	{
		if (GroupMap.Srt_Map.dwMapCrc == dwMapCrc && CCharacter::wstrcmp_my(pwszMapName, GroupMap.Srt_Map.wszMapName))
			return &GroupMap;
	}

	return NULL;
}

em_GroupPlayer CVerificationSystem::_ExistGroupUser(Map_GroupMember* pGroupMap, SockClientGroup& ClientGroup)
{
	try
	{
		// �����õ�ͼ�� ���еĶ���, �Ƿ���������
		for (auto& Group : pGroupMap->vGrouplst)
		{
			// �Ӷӳ�����������ľ�������
			if (Group.Captain == ClientGroup && !Group.Captain.bLeave)
			{
				_InitSockClientGroup(ClientGroup);
				Group.Captain.ulTick = ::GetTickCount64();
				ClientGroup = Group.Captain;
				return em_GroupPlayer_Captain;
			}

			// ��������4λ��Ա
			for (auto& GroupMember : Group.MemberList)
			{
				if (GroupMember == ClientGroup && !GroupMember.bLeave)
				{
					GroupMember.ulTick = ::GetTickCount64();
					ClientGroup = GroupMember;
					return em_GroupPlayer_Member;
				}
			}
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"_ExistGroupUser�����쳣");
	}

	return em_GroupPlayer_None;
}

BOOL CVerificationSystem::_AddWaitGroupMember(__in Map_GroupMember* pMapGroup, __in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	//////��ʱ���ò���, ����������////////////////////////////////////////////////////////////////////
	return FALSE;

	BOOL bExist = FALSE;
	for (UINT i = 0; i < WaitGroupLst.size(); ++i)
	{
		if (WaitGroupLst.at(i) == ClientGroup)
		{
			WaitGroupLst.erase(WaitGroupLst.begin() + i); // ɾ���ȴ��б�Ķӳ�
			bExist = TRUE;
			break;
		}
	}

	// �����ھ�ֱ�ӷ��ز����κβ���
	if (!bExist)
		return FALSE;

	// ����Ѿ�������, ��ô�Լ��϶��Ƕӳ�!
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"�������Ķӳ���:%s,size()=%d", ClientGroup.SLD.wszNiceName, WaitGroupLst.size());
	random_shuffle(WaitGroupLst.begin(), WaitGroupLst.end()); // �����һ��

	// ȡǰ���4��������
	if (WaitGroupLst.size() < 4)
	{
		WaitGroupLst.clear();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"���ǵȺ�������:%d", WaitGroupLst.size());
		return FALSE;
	}

	// �ж�WaitGroupLst����ͬ��ͬ��ͼ������, �ٴ�ɸѡ, �����ͻ
	vector<SockClientGroup> vlst;
	for (int i = 0; i < (INT)WaitGroupLst.size(); ++i)
	{
		auto& SCG = WaitGroupLst.at(i);
		if (CCharacter::wstrcmp_my(SCG.wszServerName, ClientGroup.wszServerName) && CCharacter::wstrcmp_my(SCG.wszMapName, ClientGroup.wszMapName))
		{
			vlst.push_back(SCG);
			WaitGroupLst.erase(WaitGroupLst.begin() + i--);
		}
	}

	if (vlst.size() < 4)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"���ǵȺ�������:%d -> %d", WaitGroupLst.size(), vlst.size());
		return FALSE;
	}

	// �ӳ������µĶ���
	GroupList GL;
	ZeroMemory(&GL, sizeof(GL));

	GL.InitHero();
	_InitHero(GL.emList, ClientGroup);	// �ӳ�������
	GL.Captain = ClientGroup;
	GL.Captain.ulTick = ::GetTickCount64();
	GL.Captain.bLeave = FALSE;

	// �ȴ��б��ǰ4�������¶���
	for (UINT i = 0; i < 4; ++i)
	{
		SockClientGroup SCG = vlst.at(i);
		_InitHero(GL.emList, SCG);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"����%d��:%s, Ӣ��:%d,%d", i + 1, SCG.SLD.wszNiceName, (int)SCG.emHero, (int)SCG.emRetainHero);
		SCG.bAlive = FALSE;
		SCG.ulTick = ::GetTickCount64();
		SCG.bLeave = FALSE;
		GL.MemberList.push_back(SCG);
	}

	pMapGroup->vGrouplst.push_back(GL);
	// ���Լ������ֻط�, ��ʾ�Լ������ų�!
	//RetBf << SOCKET_MESSAGE_GETGROUP << wstring(ClientGroup.SLD.wszNiceName) << (DWORD)ClientGroup.emHero << (DWORD)ClientGroup.emRetainHero; // ��������Ӣ��
	RetBf << SOCKET_MESSAGE_WAIT;
	return TRUE;
}

BOOL CVerificationSystem::_HandleGroup(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	try
	{
		// ��ȡ��Ӧ�ķ������б�
		auto pGroupServer = _ExistGroupServer(ClientGroup.wszServerName);
		if (pGroupServer == NULL)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"����, �������ʲô�Ʒ�����:%s,���������û���ѡ��!", ClientGroup.wszServerName);
			RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"����, �������ʲô�Ʒ�����,���������û���ѡ��!");
			return TRUE;
		}

		// �ж�ģʽ
		ServerAccountInfo __ServerAccountInfo = { 0 };
		__ServerAccountInfo.dwQqCrc = ClientGroup.dwGameUserCrc;

		AccountListLock.Access([&__ServerAccountInfo]{
			for (auto& tmpServerAccountInfo : AccountList)
			{
				if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
				{
					__ServerAccountInfo = tmpServerAccountInfo;
					break;
				}
			}
		});

		if (__ServerAccountInfo.dwComputerNameCrc == NULL)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"����ʺű�ɾ����!,����λ�������»�ȡ!");
			RetBf << SOCKET_MESSAGE_GETACCOUNT;
			return TRUE;
		}

		if (__ServerAccountInfo.PlayMode == AccountPlayMode_Default)
		{
			// �Զ���ģʽ��, 7����ˢ�˻��ٻ�ʦϿ��,7����ˢͳ��ս��
			if (ClientGroup.SLD.nSumWarCount >= 40)
				CCharacter::wstrcpy_my(ClientGroup.wszMapName, L"ˮ��֮��");
			else
				CCharacter::wstrcpy_my(ClientGroup.wszMapName, L"�ٻ�ʦϿ��");
		}
		else if (__ServerAccountInfo.PlayMode == AccountPlayMode_Custome)
		{
			// �Զ���ģʽ���� һֱˢĳ����ͼ
			CCharacter::wstrcpy_my(ClientGroup.wszMapName, __ServerAccountInfo.wszMapName);
		}

		// ��ȡ��Ӧ�ĵ�ͼ�б�
		auto pGroupMap = _ExistGroupMap(pGroupServer, ClientGroup.wszMapName);
		if (pGroupMap == NULL)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"����, ��������ʲô�Ƶ�ͼ:%s? ��Ӿ�4�ŵ�ͼ,���ڸ�ʲô�һ�?", ClientGroup.wszMapName);
			RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"����, ��������ʲô�Ƶ�ͼ? ��Ӿ�4�ŵ�ͼ,���ڸ�ʲô�һ�?");
			return TRUE;
		}

		// �ж��Ƿ�ȴ��б���,�ǵĻ�, �Ͱ�4�����Ѽӽ�ȥ!
		if (_AddWaitGroupMember(pGroupMap, ClientGroup, RetBf))
			return TRUE;

		// �ж��Լ��ǲ����Ѿ� �ڶ�������, ��Ҫ�ظ�����2������!
		em_GroupPlayer emPlayer = _ExistGroupUser(pGroupMap, ClientGroup);

		if (emPlayer == em_GroupPlayer_Captain)
		{
			// ���η����˵Ķӳ�
			_RefushGroupCaptain(pGroupMap, ClientGroup, RetBf);
		}
		else if (emPlayer == em_GroupPlayer_Member)
		{
			// ���η��ʵĶ�Ա
			_RefushGroupMember_To_Captain(pGroupMap, ClientGroup, RetBf);
		}
		else // None
		{
			// һ�η���
			_JoinGroup(pGroupMap, __ServerAccountInfo, ClientGroup);
			RetBf << SOCKET_MESSAGE_WAIT; // һֱ�ȴ�!
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"_HandleGroup�����쳣");
	}

	return TRUE;
}

BOOL CVerificationSystem::_InitHero(__in vector<int>& vlst, __out SockClientGroup& ClientGroup)
{
	static em_Hero_Pro emHeroArray[] = {
		em_Hero_Pro_Ryze,
		em_Hero_Pro_Garen, em_Hero_Pro_Ashe,
		em_Hero_Pro_MissFortune, em_Hero_Pro_MasterYi
	};

	static em_Hero_Pro emRetainHeroArray[] = {
		em_Hero_Pro_Ryze,
		em_Hero_Pro_Garen, em_Hero_Pro_Ashe,
		em_Hero_Pro_Caitlyn, em_Hero_Pro_Ahri
	};

	if (vlst.size() == 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ղ�, vlst.size=0");
		return FALSE;
	}
	random_shuffle(vlst.begin(), vlst.end());

	ClientGroup.nHeroIndex = vlst.at(0);
	if (ClientGroup.nHeroIndex >= 5)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"ClientGroup.nHeroIndex=%d", ClientGroup.nHeroIndex);
		ClientGroup.nHeroIndex %= 5;
	}

	ClientGroup.emHero = emHeroArray[ClientGroup.nHeroIndex];
	ClientGroup.emRetainHero = emRetainHeroArray[ClientGroup.nHeroIndex];
	vlst.erase(vlst.begin());
	return TRUE;
}

BOOL CVerificationSystem::_JoinGroup(Map_GroupMember* pMapGroup, __in const ServerAccountInfo& __ServerAccountInfo, SockClientGroup& ClientGroup)
{
	try
	{
		// ������ľ���Ŷ���û��
		for (auto& Group : pMapGroup->vGrouplst)
		{
			// �˼Ҷ����Ѿ����ˡ���
			if (Group.MemberList.size() >= 4)
				continue;

			// �Է���һ, ľ�жӳ���Ҳ������(�����ǵ�����)
			if (Group.Captain.dwNiceNameCrc == NULL)
				continue;

			// �ж�����ӳ���ģʽ, �ǲ��Ǹ��Լ�һ��ģʽ
			if (Group.PlayMode != __ServerAccountInfo.PlayMode)
				continue;

			// ��, �������������λ�� Ҳ�жӳ�, ������֧����, +1��ָ, �ӳ���Զ�������˷�ʦ
			if (!_InitHero(Group.emList, ClientGroup))
				continue;

			ClientGroup.bAlive = FALSE;
			ClientGroup.bLeave = FALSE;
			Group.MemberList.push_back(ClientGroup);
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"�������, �ӳ���:%s,�Լ���:%s,Ӣ��:%d,%d", Group.Captain.SLD.wszNiceName, ClientGroup.SLD.wszNiceName, ClientGroup.emHero, ClientGroup.emRetainHero);
			return TRUE;
		}

		// ��Ȼľ������Ķ���, ��ô�Լ�����һ��
		static GroupList GL;
		ZeroMemory(&GL, sizeof(GL));
		GL.InitHero();

		_InitHero(GL.emList, ClientGroup);	// �ӳ�������
		GL.Captain = ClientGroup;
		GL.Captain.bLeave = FALSE;
		GL.PlayMode = __ServerAccountInfo.PlayMode;

		if (GL.PlayMode == AccountPlayMode_Default && ClientGroup.SLD.nSumWarCount % 5 == 0)
			CCharacter::wstrcpy_my(GL.Captain.wszMapName, L"ˮ��֮��");

		pMapGroup->vGrouplst.push_back(GL);

		// ���Լ������ֻط�, ��ʾ�Լ������ų�!
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"��������!,�ӳ���:%s,Ӣ��:%d,%d", ClientGroup.SLD.wszNiceName, ClientGroup.emHero, ClientGroup.emRetainHero);
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"_JoinGroup�����쳣");
	}
	return TRUE;
}

BOOL CVerificationSystem::_RefushGroupMember_To_Captain(Map_GroupMember* pMapGroup, const SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	for (int i = 0; i < (int)pMapGroup->vGrouplst.size(); ++i)
	{
		for (auto& GroupMember : pMapGroup->vGrouplst.at(i).MemberList)
		{
			if (GroupMember == ClientGroup)
			{
				auto& Group = pMapGroup->vGrouplst.at(i);
				// �ж϶ӳ��Ƿ�������ʱ!
				if (::GetTickCount64() - Group.Captain.ulTick >= 30 * 1000)
				{
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ղ�! �ӳ�%s������ʱ��, ��ɢ����!", Group.Captain.SLD.wszNiceName);
					pMapGroup->vGrouplst.erase(pMapGroup->vGrouplst.begin() + i);
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}
				if (Group.MemberList.size() < 4)
				{
					// �����鲻��4�˵�ʱ��, ��һֱ�ڵ�, ��嶯
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}
				GroupMember.bAlive = TRUE; // ״̬����!

				// �ж���4���Ƿ��Ǽ���״̬
				BOOL bAlive = TRUE;
				if (!Group.MemberList.at(0).bAlive || !Group.MemberList.at(1).bAlive || !Group.MemberList.at(2).bAlive || !Group.MemberList.at(3).bAlive)
					bAlive = FALSE;

				// �ж���4���Ƿ����������
				if (Group.MemberList.at(0).bLeave || Group.MemberList.at(1).bLeave || Group.MemberList.at(2).bLeave || Group.MemberList.at(3).bLeave)
					bAlive = FALSE;

				// ���˻���δ����״̬,��!
				if (!bAlive)
				{
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}

				/////�����ͼ����״̬/////////////////////////////////////////////////////////////////////
				_HandleServerStatus(ClientGroup, FALSE);

				// Ȼ��ط� �ų�������
				RetBf << SOCKET_MESSAGE_GROUP_MEMBER << pMapGroup->vGrouplst.at(i).Captain.wszMapName << (DWORD)ClientGroup.emHero << (DWORD)ClientGroup.emRetainHero; // ��������Ӣ��;
				return TRUE;
			}
		}
	}
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ղ�!, ��Ȼ�Ҳ����ӳ�:%s", ClientGroup.SLD.wszNiceName);
	return TRUE;
}

BOOL CVerificationSystem::_HandleServerStatus(__in const SockClientGroup& ClientGroup, BOOL bLeave)
{
	ServerAccountInfo __ServerAccountInfo = { 0 };

	__ServerAccountInfo.dwQqCrc = ClientGroup.dwGameUserCrc;
	CCharacter::wstrcpy_my(__ServerAccountInfo.wszStatus, bLeave ? L"����" : L"�ж���");
	CCharacter::wstrcpy_my(__ServerAccountInfo.wszMapName, ClientGroup.wszMapName);

	AccountListLock.Access([&__ServerAccountInfo]{
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
			{
				// ֻ����һ��״̬����!
				CCharacter::wstrcpy_my(tmpServerAccountInfo.wszStatus, __ServerAccountInfo.wszStatus);
				CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMapName, __ServerAccountInfo.wszMapName);
				break;
			}
		}
	});

	return TRUE;
}

BOOL CVerificationSystem::_RefushGroupCaptain(Map_GroupMember* pMapGroup, const SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	UINT uGroupIndex = 0;
	for (auto& Group : pMapGroup->vGrouplst)
	{
		uGroupIndex++;
		// �ж����
		if (Group.Captain == ClientGroup && !Group.Captain.bLeave)
		{
			// �ж��Ƿ��ж��ѳ�ʱ��!
			for (int i = 0; i < (int)Group.MemberList.size(); ++i)
			{
				if (::GetTickCount64() - Group.MemberList.at(i).ulTick >= 30 * 1000)
				{
					// �Ȱ�Ӣ�۹黹���б�
					Group.emList.push_back(Group.MemberList.at(i).nHeroIndex);

					// ��ɾ
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"��Ա%s ������ʱ! ��ɢ����!", Group.MemberList.at(i).SLD.wszNiceName);
					pMapGroup->vGrouplst.erase(pMapGroup->vGrouplst.begin() + (uGroupIndex - 1));
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}
			}

			// Ҫ�չ�4���˲����ط�!
			if (Group.MemberList.size() != 4)
				break;

			// �ж���4���Ƿ��Ǽ���״̬
			BOOL bAlive = TRUE;
			if (!Group.MemberList.at(0).bAlive || !Group.MemberList.at(1).bAlive || !Group.MemberList.at(2).bAlive || !Group.MemberList.at(3).bAlive)
				bAlive = FALSE;

			// �ж���4���Ƿ����������
			if (Group.MemberList.at(0).bLeave || Group.MemberList.at(1).bLeave || Group.MemberList.at(2).bLeave || Group.MemberList.at(3).bLeave)
				bAlive = FALSE;

			// ���˻���δ����״̬,��!
			if (!bAlive)
				break;

			//////////////////////////////////////////////////////////////////////////
			/*if (CCharacter::wstrcmp_my(ClientGroup.wszMapName, L"ˮ��֮��")) // ��ʾ5���Ŷ���ˮ��֮��, ��ôÿ5�ѿ�����һ���ٻ�ʦϿ��
			{
				if (ClientGroup.SLD.nSumWarCount % 5 == 0)
					CCharacter::wstrcpy_my(Group.Captain.wszMapName, L"�ٻ�ʦϿ��");
			}*/

			/////�����ͼ����״̬/////////////////////////////////////////////////////////////////////
			_HandleServerStatus(ClientGroup, FALSE);

			// Ȼ��ط� ������Ӣ��
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ط��ӳ��ͻ���������4������! �ӳ���:%s", ClientGroup.SLD.wszNiceName);
			RetBf << SOCKET_MESSAGE_GROUP_CAPTAIN << Group.Captain.wszMapName << (DWORD)Group.Captain.emHero << (DWORD)Group.Captain.emRetainHero; // ��������Ӣ��;
			for (auto GroupMember : Group.MemberList)
				RetBf << wstring(GroupMember.SLD.wszNiceName);

			return TRUE;
		}
	}

	RetBf << SOCKET_MESSAGE_WAIT;
	return TRUE;
}

BOOL CVerificationSystem::_LeaveGroup(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	/*if (CLoginManger::_GetCardTime(ClientGroup.SLD.Card.szCard, ClientGroup.SLD.dwVerCode, pwszIp) == 0)
	{
	RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"WTF? ����,������ʱ�� ò�ƿ��Ų���ʱ����!")/ *�˿������ڻ��߱�������* /;
	return TRUE;
	}*/

	// ��ʼ��Crc
	_InitSockClientGroup(ClientGroup);

	// ��ȡ��Ӧ�ķ������б�
	auto pGroupServer = _ExistGroupServer(ClientGroup.wszServerName);
	if (pGroupServer == NULL)
	{
		RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"����, �������ʲô�Ʒ�����,���������û���ѡ��!");
		return TRUE;
	}

	// ��ȡ��Ӧ�ĵ�ͼ�б�
	auto pGroupMap = _ExistGroupMap(pGroupServer, ClientGroup.wszMapName);
	if (pGroupMap == NULL)
	{
		RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"����, ��������ʲô�Ƶ�ͼ? ��Ӿ�4�ŵ�ͼ,���ڸ�ʲô�һ�?");
		return TRUE;
	}

	for (INT i = 0; i < (INT)pGroupMap->vGrouplst.size(); ++i)
	{
		auto Group = pGroupMap->vGrouplst.at(i);
		// ������ֶ����еĶӳ�Ҫ���
		if (Group.Captain == ClientGroup)
		{
			Group.Captain.bLeave = TRUE;
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ɢ����,�ӳ���:%s", Group.Captain.SLD.wszNiceName);
			pGroupMap->vGrouplst.erase(pGroupMap->vGrouplst.begin() + i--);
			break;
		}

		// ������ֶ����еĶ�ԱҪ���
		BOOL bExist = FALSE;
		for (auto& GroupMember : Group.MemberList)
		{
			if (GroupMember == ClientGroup)
			{
				GroupMember.bLeave = TRUE;
				bExist = TRUE;
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ɢ����,�ӳ���:%s", Group.Captain.SLD.wszNiceName);
				pGroupMap->vGrouplst.erase(pGroupMap->vGrouplst.begin() + i--);
				break;
			}
		}

		if (bExist)
			break;
	}

	// ���뵽�ȴ��б�
	_JoinWaitList(ClientGroup);

	/////�����ͼ����״̬/////////////////////////////////////////////////////////////////////
	_HandleServerStatus(ClientGroup, TRUE);

	RetBf << SOCKET_MESSAGE_LEAVEGROUP;
	return TRUE;
}

BOOL CVerificationSystem::_JoinWaitList(__in SockClientGroup& SCG)
{
	////////�ȴ��б���ʱ������//////////////////////////////////////////////////////////////////
	return FALSE;

	BOOL bExist = FALSE;
	for (auto& GroupMember : WaitGroupLst)
	{
		if (GroupMember == SCG)
		{
			bExist = TRUE;
			break;
		}
	}

	if (!bExist)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"%s����ȴ��б�", SCG.SLD.wszNiceName);
		WaitGroupLst.push_back(SCG);
	}

	return TRUE;
}

BOOL CVerificationSystem::_DeleteGroupList(__in SockClientGroup& ClientGroup)
{
	// �����ض���ͼ�µ����ж���
	/*for (int i = 0; i < (int)pMapGroup->vGrouplst.size(); ++i)
	{
		// �������
		UINT uLeaveCount = 0;
		auto pGL = &pMapGroup->vGrouplst.at(i);
		if (pGL->Captain.bLeave) // calc Captain
			uLeaveCount++;

		// calc Team Member Leave Count
		for (auto& GroupMember : pGL->MemberList)
		{
			if (GroupMember.bLeave)
				uLeaveCount++;
		}

		// All of it -> delete
		if (uLeaveCount >= 1)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ɢ����,�ӳ���:%s", pGL->Captain.SLD.wszNiceName);
			pMapGroup->vGrouplst.erase(pMapGroup->vGrouplst.begin() + i--);
			break;
		}
	}*/
	return TRUE;
}

BOOL CVerificationSystem::_IdSealed(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"�ʺ�:%s �ǳ�:%s �������", ClientGroup.SLD.wszGameUser, ClientGroup.SLD.wszNiceName);

	_InitSockClientGroup(ClientGroup);

	AccountListLock.Access([&ClientGroup]{
		for (auto& __ServerAccountInfo : AccountList)
		{
			if (__ServerAccountInfo.CompQqCrc(ClientGroup.dwGameUserCrc))
			{
				CCharacter::wstrcpy_my(__ServerAccountInfo.wszMsg, L"���!");
				__ServerAccountInfo.bIdSealed = TRUE;
				break;
			}
		}
	});

	// ��GorupListɾ��
	
	RetBf << SOCKET_MESSAGE_GAMETITLE;
	return TRUE;
}

BOOL CVerificationSystem::_GetAccount(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	ServerAccountInfo __ServerAccountInfo = { 0 };

	if (CCharacter::wstrcmp_my(ClientGroup.wszComputerName, L"test"))
	{
		RetBf << SOCKET_MESSAGE_RETMSG << L"����ô���ٵø�һ��ID��? ��Ȼ�ظ�ID����ô����Ū?";
		return TRUE;
	}

	_InitSockClientGroup(ClientGroup);
	__ServerAccountInfo.dwComputerNameCrc = CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.wszComputerName, wcslen(ClientGroup.wszComputerName) * 2 + 1);
	
	AccountListLock.Access([&__ServerAccountInfo]{
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo == __ServerAccountInfo)
			{
				__ServerAccountInfo = tmpServerAccountInfo;
				break;
			}
		}
	});

	// �����NULL || ��� || ���� || ������� �Ļ�, �ͱ�ʾ���ʺŲ�����
	if (__ServerAccountInfo.dwQqCrc == NULL || __ServerAccountInfo.bIdSealed || __ServerAccountInfo.bError || __ServerAccountInfo.IsFinish())
	{
		// ���»�ȡһ���ʺ�
		//ZeroMemory(&__ServerAccountInfo, sizeof(__ServerAccountInfo));
		CCharacter::wstrcpy_my(__ServerAccountInfo.wszComputerName, ClientGroup.wszComputerName);

		AccountListLock.Access([&__ServerAccountInfo]{
			for (auto& tmpServerAccountInfo : AccountList)
			{
				// ����ľ�б�ű�ռ�� && !��� && !���� && !�������
				if (tmpServerAccountInfo.dwComputerNameCrc == NULL && !tmpServerAccountInfo.bIdSealed && !tmpServerAccountInfo.bError && !tmpServerAccountInfo.IsFinish())
				{
					// ��ֵ���, �Ϳ��Բ��ñ���ʺ��ظ���ȡ
					tmpServerAccountInfo.dwComputerNameCrc = CLPublic::GetCRC32_DWORD((LPSTR)__ServerAccountInfo.wszComputerName, wcslen(__ServerAccountInfo.wszComputerName) * 2 + 1);
					CCharacter::wstrcpy_my(tmpServerAccountInfo.wszComputerName, __ServerAccountInfo.wszComputerName);
					CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMsg, L"��ͻ��˳ɹ�����!");

					// ���ظ��˱�ŵĵ���
					__ServerAccountInfo = tmpServerAccountInfo;
					break;
				}
			}
		});

		if (__ServerAccountInfo.dwComputerNameCrc == NULL)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"�ղ�, �ʺŲ���!!!");
			return SOCKET_MESSAGE_WAIT;
		}
		// Qq >> Pass >> Server >> Map
		RetBf << SOCKET_MESSAGE_GETACCOUNT << __ServerAccountInfo.wszQQ << __ServerAccountInfo.wszPass << __ServerAccountInfo.wszServer << __ServerAccountInfo.wszMapName;
	}
	else
	{
		// ����ԭ�����ʺ�
		RetBf << SOCKET_MESSAGE_GETACCOUNT << __ServerAccountInfo.wszQQ << __ServerAccountInfo.wszPass << __ServerAccountInfo.wszServer << __ServerAccountInfo.wszMapName;
	}

	return TRUE;
}

BOOL CVerificationSystem::_ClientMsg(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	ServerAccountInfo __ServerAccountInfo = { 0 };

	_InitSockClientGroup(ClientGroup);
	__ServerAccountInfo.dwQqCrc = ClientGroup.dwGameUserCrc;

	AccountListLock.Access([&ClientGroup]{
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(ClientGroup.dwGameUserCrc))
			{
				if (CCharacter::wstrstr_my(ClientGroup.wszClientMsg, L"����:"))
					tmpServerAccountInfo.bError = TRUE;

				// ��Ϣ
				CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMsg, ClientGroup.wszClientMsg);

				// ˳������һ�½�ɫս��������
				if (ClientGroup.SLD.nLevel != 0)
				{
					CCharacter::wstrcpy_my(tmpServerAccountInfo.wszNiceName, ClientGroup.SLD.wszNiceName);
					tmpServerAccountInfo.dwFightCount = ClientGroup.SLD.nSumWarCount;
					tmpServerAccountInfo.dwGold = ClientGroup.SLD.nGold;
					tmpServerAccountInfo.dwLevel = ClientGroup.SLD.nLevel;
				}
				break;
			}
		}
	});
	
	// �ط���Ϣ��ʾһ������!
	RetBf << SOCKET_MESSAGE_CLIENTMSG;
	return TRUE;
}

BOOL CVerificationSystem::_GameRecord(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	RetBf << SOCKET_MESSAGE_RECORD;
	return TRUE;
}

BOOL CVerificationSystem::_RefushLoseCon()
{
	for (int i = 0; i < (int)SocketConectLst.size(); ++i)
	{
		auto pSockKeepConSrc = &SocketConectLst.at(i);
		if (::GetTickCount64() - pSockKeepConSrc->ulTick >= 30 * 1000)
		{
			// ���˵�����!
			AccountListLock.Access([&pSockKeepConSrc]{
				for (auto& tmpServerAccountInfo : AccountList)
				{
					if (tmpServerAccountInfo.dwComputerNameCrc == pSockKeepConSrc->dwComputerCrc)
					{
						CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMsg, L"����: ��ͻ��˶Ͽ�����!");
						tmpServerAccountInfo.bError = TRUE;
					}
				}
			});
			SocketConectLst.erase(SocketConectLst.begin() + i--);
		}
	}

	return TRUE;
}