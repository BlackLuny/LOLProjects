#include "stdafx.h"
#include "Equment.h"

#define _SELF L"Equment.cpp"

CEqument::CEqument(DWORD dwBase)
{
	m_dwBase = dwBase;
}

CEqument::~CEqument()
{

}

DWORD CEqument::GetBase()
{
	return m_dwBase;
}

DWORD CEqument::GetCount()
{
	DWORD dwSkillCount = ReadDWORD(GetBase() + 8);
	if (dwSkillCount == 0)
		return ReadDWORD(GetBase() + 4);

	return dwSkillCount;
}

DWORD CEqument::GetParm()
{
	return ReadDWORD(ReadDWORD(GetBase()) + 0xC);
}

DWORD CEqument::GetId()
{
	return ReadDWORD(GetParm() + Role_Equip_ID);
}

UINT CEqument::GetEuqmentList(vector<CEqument>& vlst)
{
	vlst.clear();

	DWORD dwRoot = ReadDWORD(�����ַ) + ����װ��ƫ�� + 0x198;
	for (int i = 0; i < 8; ++i)
	{
		CEqument Equment(((DWORD*)dwRoot)[i]);
		if (Equment.GetCount() > 0)
			vlst.push_back(Equment);
	}
	return vlst.size();
}

BOOL CEqument::ExistItem(DWORD dwItemId)
{
	static vector<CEqument> vlst;
	GetEuqmentList(vlst);

	for (auto& Item : vlst)
	{
		if (Item.GetId() == dwItemId)
			return TRUE;
	}

	return FALSE;
}

BOOL CEqument::BuyEqument()
{
	try
	{
		static vector<CEqument> vEqumentLst;
		if (CEqument::GetEuqmentList(vEqumentLst) == 8)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"������~!");
			return TRUE;
		}

		auto pResItem = CGameRes::GetNextEqument(g_pAccountGame->MyAcount_Info.emHeroPro, g_pAccountGame->MyAcount_Info.dwEqumentIndex);
		if (pResItem == NULL)
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��֧�ָ�Ӣ�۵�װ������!");
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
			StopGame;
			return FALSE;
		}
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwMoney=%d, dwPrice=%d", PERSON->GetMoney(), pResItem->dwPrice);
		// �ж��ǲ��ǹ�Ǯ��
		if (PERSON->GetMoney() >= pResItem->dwPrice)
		{
			CGameSend::BuyItem(pResItem->dwItemId);
			g_pAccountGame->MyAcount_Info.dwEqumentIndex++;
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"����һ��װ��:%X", pResItem->dwItemId);
			CGameSend::Sleep(1000);
			return BuyEqument();
		}

		// ���Hpҩ��Mpҩ
		if (PERSON->GetLevel() < 10 && !CEqument::ExistItem(0x7D3)) // �ж���Ʒ�Ƿ������
		{
			for (int i = 0; i < 2 && PERSON->GetMoney() >= 50 && GameStatus_Run; ++i)
			{
				//CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ҩ~��");
				CGameSend::BuyItem(0x7D3);
			}
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"BuyEqument�����쳣");
	}

	return TRUE;
}

BOOL CEqument::SetEqumentIndex(em_Hero_Pro emHero)
{
	vector<CEqument> Equvlst;
	vector<DWORD>	 vlst;
	GetEuqmentList(Equvlst);

	for_each(Equvlst.begin(), Equvlst.end(), [&vlst](CEqument& Equ){vlst.push_back(Equ.GetId()); });
	g_pAccountGame->MyAcount_Info.dwEqumentIndex = CGameRes::GetIndex_By_Equment(emHero, vlst);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwEqumentIndex=%d", g_pAccountGame->MyAcount_Info.dwEqumentIndex);
	return TRUE;
}