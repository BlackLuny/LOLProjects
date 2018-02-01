#include "LoginManger.h"

#define _SELF L"LoginManger.cpp"

/////////////////ȫ�ֱ�����/////////////////////////////////////////////////////////
vector<LoginGameUser> CLoginManger::LgUserLst;


////////////////////����ʵ��//////////////////////////////////////////////////////
CLoginManger::CLoginManger()
{
}

CLoginManger::~CLoginManger()
{
}

LoginGameUser* CLoginManger::_ExistCard(CardStruct & Card)
{
	// ��ȡ���ŵ�Crc, ��Crcȥ����, �������ַ���ȥ������ʡʱ
	CLPublic::GetCRC32_DWORD((LPSTR)Card.szCard, wcslen(Card.szCard) + 1, Card.dwCardCode);

	for (auto& lgUser : LgUserLst)
	{
		if (lgUser.Card.dwCardCode == Card.dwCardCode && CCharacter::wstrcmp_my(lgUser.Card.szCard, Card.szCard))
			return &lgUser;
	}

	return NULL;
}

BOOL CLoginManger::_EnoughTime(LoginGameUser* pLgGameUser)
{
	if (::GetTickCount() - pLgGameUser->dwSurplusTick > pLgGameUser->dwSurplusHour * 60 * 60 * 1000)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"%s�Ŀ����Ѿ����ʳ�ʱ��,Card:%s", pLgGameUser->wszLoginName, pLgGameUser->Card.szCard);
		return FALSE;
	}
	return TRUE;
}

BOOL CLoginManger::_EnoughLoginTime(LoginGameUser* pLgGameUser)
{
	if (::GetTickCount() - pLgGameUser->dwLoginTick < 1 * 60 * 1000)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"%s�����¼��%ds֮��,Card:%s", pLgGameUser->wszLoginName, (::GetTickCount() - pLgGameUser->dwLoginTick) / 1000, pLgGameUser->Card.szCard);
		return FALSE;
	}
	pLgGameUser->dwLoginTick = ::GetTickCount();
	return TRUE;
}

BOOL CLoginManger::_SameIp(LoginGameUser* LgGameUser, LPCWSTR pwszIp)
{
	return CCharacter::wstrcmp_my(LgGameUser->wszIp, pwszIp) ? TRUE : FALSE;
}

BOOL CLoginManger::_TimeOut_ByCount(LoginGameUser* LgGameUser)
{
	// �жϴﵽ20�κ�,ʱ���Ƿ񳬹�1����
	if (++LgGameUser->dwUseCount >= 20 * 15)
	{
		if (::GetTickCount() - LgGameUser->dwTickCount > 60 * 1000)
		{
			// ����, ������0
			LgGameUser->dwUseCount = 0;
			LgGameUser->dwTickCount = ::GetTickCount();
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CLoginManger::VerCard(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg)
{
	try
	{
		/*LoginGameUser* pLgUser = _ExistCard(SLD.Card);
		if (pLgUser != NULL)
		{
			// �жϿ����Ƿ������
			if (!_EnoughTime(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"�ÿ����Ѿ�������!ʱ�䲻��!");
				goto lb_Exit;
			}

			// �ж�IP�Ƿ����ʼ����¼��һ��
			if (!_SameIp(pLgUser, pwszIp))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"CurIp:%s,LocalIp:%s", pwszIp, pLgUser->wszIp);
				wsprintfW(pwszMsg, L"�ÿ����Ѿ��ڱ�Ļ�����ʹ���ˡ������ŵ�IP��:%s,����IP:%s", pLgUser->wszIp, pwszIp);
				goto lb_Exit;
			}

			// �ж�����
			if (pLgUser->dwVerCode != SLD.dwVerCode)
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"���Ʋ�һ��,����:%s,User:%s", pLgUser->Card.szCard, pLgUser->wszLoginName);
				CCharacter::wstrcpy_my(pwszMsg, L"�����ܰ�? ��ֱ��������¼����?");
				goto lb_Exit;
			}


			//  ����ʹ��У��
			if (!_TimeOut_ByCount(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"1�����ڻ�ȡ��������!");
				goto lb_Exit;
			}

			// �ʺ�У��(���Բ�Ҫ,��������)
			/ *if (!CCharacter::wstrcmp_my(pLgUser->wszGameUserName, GameUser.wchGameUserName))
			{
			CCharacter::wstrcpy_my(pwszMsg, L"�ʺŲ�����!");
			goto lb_Exit;
			}* /

			return TRUE;
		}
		else
		{
			wsprintfW(pwszMsg, L"�˿���:%s,������", SLD.Card.szCard);
		}*/
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"VerGameUser�������쳣");
		CCharacter::wstrcpy_my(pwszMsg, L"�����VerGameUser�����쳣!");
	}
//lb_Exit:;
	return FALSE;
}

LoginGameUser* CLoginManger::InitLoginUser(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg)
{
	/*try
	{
		LoginGameUser* pLgUser = _ExistCard(SLD.Card);
		if (pLgUser != NULL)
		{
			// �жϿ����Ƿ������
			if (!_EnoughTime(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"�����Ѿ�������!");
				goto lb_Exit;
			}

			/ *if (!_EnoughLoginTime(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"��¼������Ҫ��1�����ڲ����ظ�����");
				goto lb_Exit;
			}* /

			// ����IP
			CCharacter::wstrcpy_my(pLgUser->wszIp, pwszIp);

			// ��������
			pLgUser->dwVerCode = CCharacter::GetRand_For_DWORD();
			pLgUser->dwVerTick = ::GetTickCount();

			// ������������
			pLgUser->dwUseCount = 0;
			pLgUser->dwTickCount = ::GetTickCount();

			// ��¼�ʺ�
			CCharacter::wstrcpy_my(pLgUser->wszLoginName, SLD.wszLoginUser);

			wsprintfW(pwszMsg, L"����->%X", pLgUser->dwVerCode);

			return pLgUser;
		}
		else
		{
			wsprintfW(pwszMsg, L"�˿���:%s,������", SLD.Card.szCard);
		}
	}
	catch (...)
	{
		CCharacter::wstrcpy_my(pwszMsg, L"�����CreateNewLoginGameUser�����쳣!");
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CreateNewLoginGameUser�������쳣");
	}
lb_Exit:;*/
	return NULL;
}

DWORD CLoginManger::_CheckCard(LPCWSTR pwszCard)
{
	if (wcslen(pwszCard) > 36)
		return 0;

	if (wcslen(pwszCard) < 20)
		return 0;

	WCHAR wszHour[5] = { 0 };
	for (int i = 0; i < 4; ++i)
		wszHour[i] = pwszCard[i];

	wszHour[4] = '\0';
	return _wtoi(wszHour);
}

BOOL CLoginManger::AddNewCard(CardStruct& Card, int nTime, DWORD dwToken, LPCWSTR pwszIp)
{
	try
	{
		// �жϿ����Ƿ��Ѿ������ˡ�����Ȼ�Ƕ��һ��
		BOOL bExist = FALSE;
		for (UINT i = 0; i < LgUserLst.size(); ++i)
		{
			if (CCharacter::wstrcmp_my(LgUserLst.at(i).Card.szCard, Card.szCard))
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"������Ȼ��ֵ�ɹ��Ŀ�,���Ѿ����ֹ�������̫���ܰ�!");
			return TRUE;
		}

		// ����µ�
		LoginGameUser LgGameUser;
		ZeroMemory(&LgGameUser, sizeof(LgGameUser));

		// ��CRC��ֵ��Ϊ����ȥ����
		CLPublic::GetCRC32_DWORD((LPSTR)Card.szCard, wcslen(Card.szCard) + 1, Card.dwCardCode);

		memcpy(&LgGameUser.Card, &Card, sizeof(Card));

		// д��ʣ��ʱ��
		LgGameUser.dwSurplusHour = nTime;
		LgGameUser.dwSurplusTick = ::GetTickCount();

		// ����IP
		CCharacter::wstrcpy_my(LgGameUser.wszIp, pwszIp);

		// д������
		LgGameUser.dwVerCode = dwToken;
		LgGameUser.dwVerTick = ::GetTickCount();

		// д���¼ʱ��
		LgGameUser.dwLoginTick = 0;

		LgUserLst.push_back(LgGameUser);

		return TRUE;
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"AddNewCard�������쳣");
	}
	return FALSE;
}

BOOL CLoginManger::_InitCard(vector<CardInfo>& vlst)
{
	try
	{
		for (UINT i = 0; i < vlst.size(); ++i)
		{
			AddNewCard(vlst.at(i).Card, _wtoi(vlst.at(i).wszSurplusHour), NULL, L"127.0.0.1");
		}

		return TRUE;
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"_InitCard�������쳣");
	}
	return FALSE;
}

DWORD CLoginManger::_GetCardTime(LPCWSTR pwszCardNo, DWORD dwToken, LPCWSTR pwszIp)
{
	DWORD dwTime = 0;
	try
	{
		CardStruct Card = { 0 };
		CCharacter::wstrcpy_my(Card.szCard, pwszCardNo);
		LoginGameUser* pLgu = _ExistCard(Card);
		if (pLgu != NULL)
		{
			if (dwToken != NULL && pLgu->dwVerCode != dwToken)
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"_GetCardTime Token UnComp!");
				return 0;
			}
			
			int nTime = (int)(pLgu->dwSurplusHour - ((::GetTickCount() - pLgu->dwSurplusTick) / 1000 / 60 / 60));
			if (nTime > 0)
			{
				dwTime = nTime;
				pLgu->dwVerTick = ::GetTickCount();
				CCharacter::wstrcpy_my(pLgu->wszIp, pwszIp);
			}
		}
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"_GetCardTime�������쳣");
	}
	return dwTime;
}