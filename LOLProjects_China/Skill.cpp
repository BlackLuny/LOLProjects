#include "stdafx.h"
#include "Skill.h"
#define _SELF L"Skill.cpp"

vector<CSkill> CSkill::vNormalSkill;

CSkill::CSkill()
{
	dwKeyIndex = NULL;
	Init(0);
}

CSkill::CSkill(DWORD dwIndex, DWORD dwBase)
{
	dwKeyIndex = dwIndex;
	Init(dwBase);
}

CSkill::~CSkill()
{

}

VOID CSkill::Init(DWORD dwBase)
{
	ZeroMemory(wszName, sizeof(wszName));
	dwSkillBase = dwBase;
	fSkillDis = 0;
}

DWORD CSkill::GetSkillLevel()
{
	return ReadDWORD(GetBase() + 0x10);
}

LPWSTR CSkill::GetName()
{
	if (!CCharacter::wstrcmp_my(wszName, L""))
		return wszName;

	__try
	{
		CHAR szText[64] = { 0 };
		DWORD dwParm = ReadDWORD(GetBase() + UseItem_Of_SkillOffset);
		if (ReadDWORD(dwParm + 0x18 + 0x10) > 0xF)
			CCharacter::strcpy_my(szText, (LPCSTR)ReadDWORD(dwParm + 0x18));
		else
			CCharacter::strcpy_my(szText, (LPCSTR)(dwParm + 0x18));

		CCharacter::UTF8ToUnicode(wszName, szText);
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"GetName�������쳣");
	}

	return wszName;
}

BOOL CSkill::IsCool()
{
	if (ReadFloat(ReadDWORD(��Ϸ����ʱ���ַ) + 0x2C) > ReadFloat(GetBase() + 0x14))
		return TRUE;

	return FALSE;
}

DWORD CSkill::GetUseMp()
{
	return (DWORD)ReadFloat(ReadDWORD(ReadDWORD(GetBase() + UseItem_Of_SkillOffset) + 0x34) + (GetSkillLevel() * 4 + Role_skill_mp));
}

BOOL CSkill::GetUseStatus()
{
	if (GetSkillLevel() > 0 && PERSON->GetMp() >= GetUseMp() && IsCool())
		return TRUE;

	return FALSE;
}

BOOL CSkill::UseItemSkill(LPCWSTR pwszName)
{
	static vector<CSkill> vSkillLst;
	CSkill::GetSkillList(vSkillLst);

	// ��������
	for (auto& Skill : vSkillLst)
	{
		if (!CCharacter::wstrcmp_my(pwszName, Skill.GetName()))
			continue;

		CPrintLog::PrintLog_W(_SELF, __LINE__, L"ʹ�ü���:%s~!", pwszName);
		g_dwUseItemParm = ReadDWORD(Skill.GetBase() + UseItem_Of_SkillOffset);
		Sleep(WAITTIME);
		break;
	}
	return TRUE;
}

BOOL CSkill::UseSkill(CMonster& Monster, em_SkillType emSkillType)
{
	// ���õ�ǰ����
	g_Hook_UseSkillInfo.NowPoint = PERSON->GetPoint();

	// ����Ӣ�۵�Id
	auto fn_GetMonsterSkillId = [](DWORD dwBase)
	{
		return ReadDWORD(dwBase + yingxiong_ID);
	};

	switch (emSkillType)
	{
	case em_SkillType_Directional: // ָ���Լ���,  ���ǹ���Id
		g_Hook_UseSkillInfo.dwId = fn_GetMonsterSkillId(Monster.GetBase());
		g_Hook_UseSkillInfo.TarPoint = Monster.GetPoint();
		break;
	case em_SkillType_UnDirectional:// ��ָ���Լ���, Id=0
		g_Hook_UseSkillInfo.dwId = 0;
		g_Hook_UseSkillInfo.TarPoint = Monster.GetPoint();
		g_Hook_UseSkillInfo.NowPoint = Monster.GetPoint();
		break;
	case em_SkillType_Buff_Self: // �������״̬�ļ���, Id=��������
		g_Hook_UseSkillInfo.dwId = fn_GetMonsterSkillId(PERSON->GetBase());
		g_Hook_UseSkillInfo.TarPoint = PERSON->GetPoint();
		break;
	case em_SkillType_Self_UnDirectional:
		g_Hook_UseSkillInfo.dwId = 0;
		g_Hook_UseSkillInfo.TarPoint = PERSON->GetPoint();
		g_Hook_UseSkillInfo.NowPoint = PERSON->GetPoint();
		break;
	default:
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"�ղݡ���SkillType=%X", emSkillType);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		break;
	}

	g_Hook_UseSkillInfo.dwSkillBase = GetBase();
	g_Hook_UseSkillInfo.dwIndex = dwKeyIndex;
	g_bUseSkill = TRUE;
	CGameSend::Sleep(WAITTIME);
	return TRUE;
}

BOOL CSkill::UseNormalSkill_By_Name(LPCWSTR pwszName, CMonster& Monster, em_SkillType emSkillType)
{
	static vector<CSkill> vSkillList;
	if (GetSkillList(vSkillList) < 6)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"�����б�����=%d,��������", vSkillList.size());
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		return FALSE;
	}

	for (auto& Skill : vSkillList)
	{
		if (CCharacter::wstrcmp_my(pwszName, Skill.GetName()) && Skill.GetUseStatus())
		{
			return Skill.UseSkill(Monster, emSkillType);
		}
	}

	return FALSE;
}

BOOL CSkill::InitNormalSkill()
{
	// ��ʼ������
	static vector<CSkill> vSkillList;
	if (GetSkillList(vSkillList) < 6)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"�����б�����=%d,��������", vSkillList.size());
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		return FALSE;
	}

	// ����Ӣ����Դ��
	auto pSkillConfig = CGameRes::FindSkillConfig(PERSON->GetHeroName());

	// ��ֵӢ����Դ��ļ�������
	vSkillList.at(0).SkillType = pSkillConfig->emType_Q;
	vSkillList.at(0).fSkillDis = pSkillConfig->fSkillDis_Q;

	vSkillList.at(1).SkillType = pSkillConfig->emType_W;
	vSkillList.at(1).fSkillDis = pSkillConfig->fSkillDis_W;

	vSkillList.at(2).SkillType = pSkillConfig->emType_E;
	vSkillList.at(2).fSkillDis = pSkillConfig->fSkillDis_E;

	vSkillList.at(3).SkillType = pSkillConfig->emType_R;
	vSkillList.at(3).fSkillDis = pSkillConfig->fSkillDis_R;

	// +MP:summonermana  +HP:summonerheal
	if (CCharacter::wstrcmp_my(vSkillList.at(4).GetName(),L"summonermana"))
	{
		vSkillList.at(4).SkillType = pSkillConfig->emType_1;
		vSkillList.at(4).fSkillDis = pSkillConfig->fSkillDis_1;

		vSkillList.at(5).SkillType = pSkillConfig->emType_2;
		vSkillList.at(5).fSkillDis = pSkillConfig->fSkillDis_2;
	}
	else// �п���+HP ��+MP���츳���ܶԵ�λ��
	{
		vSkillList.at(5).SkillType = pSkillConfig->emType_1;
		vSkillList.at(5).fSkillDis = pSkillConfig->fSkillDis_1;

		vSkillList.at(4).SkillType = pSkillConfig->emType_2;
		vSkillList.at(4).fSkillDis = pSkillConfig->fSkillDis_2;
	}
	

	// ֻҪǰ��4�� ����QWER
	vNormalSkill.clear();
	vNormalSkill.assign(vSkillList.begin(), vSkillList.begin() + 6);
	return TRUE;
}

BOOL CSkill::GetSkillCool(em_Skill_Flag emSkillFlag)
{
	static CSkill* pSkill = NULL;
	static DWORD dwInitTick = ::GetTickCount();

	// 1m ��ʼ��һ��
	if (::GetTickCount() - dwInitTick >= 60 * 1000)
	{
		InitNormalSkill();
		dwInitTick = ::GetTickCount();
	}

	// ��ȡ�ü���ָ��
	pSkill = &vNormalSkill.at((int)emSkillFlag);
	if (!pSkill->GetUseStatus())
		return FALSE;

	return TRUE;
}

float CSkill::GetSkillDis(em_Skill_Flag emSkillFlag)
{
	const float fDefaultSkillDis = 7.0f;

	static CSkill* pSkill = NULL;
	// ��ȡ�ü���ָ��
	pSkill = &vNormalSkill.at((int)emSkillFlag);
	if (!pSkill->GetUseStatus())
		return fDefaultSkillDis;

	return pSkill->fSkillDis;
}

BOOL CSkill::UseNormalSkill(em_Skill_Flag emSkillFlag, CMonster& Monster)
{
	static CSkill* pSkill = NULL;

	/*if (g_bUseSkill == TRUE)
	{
		static DWORD dwTick = 0;
		if (::GetTickCount() - dwTick >= 500)
		{
			dwTick = ::GetTickCount();
			g_bUseSkill = FALSE;
		}
		Sleep(100);
		return FALSE;
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"��һ�����ܶ���û�����ء���");
		//return FALSE;
	}*/

	// ��ȡ�ü���ָ��
	pSkill = &vNormalSkill.at((int)emSkillFlag);
	if (!pSkill->GetUseStatus())
		return FALSE;
	
	// ���õ�ǰ����
	g_Hook_UseSkillInfo.NowPoint = PERSON->GetPoint();

	// ����Ӣ�۵�Id
	auto fn_GetMonsterSkillId = [](DWORD dwBase)
	{
		return ReadDWORD(dwBase + yingxiong_ID);
	};

	switch (pSkill->SkillType)
	{
	case em_SkillType_Directional: // ָ���Լ���,  ���ǹ���Id
		g_Hook_UseSkillInfo.dwId = fn_GetMonsterSkillId(Monster.GetBase());
		g_Hook_UseSkillInfo.TarPoint = Monster.GetPoint();
		break;
	case em_SkillType_UnDirectional:// ��ָ���Լ���, Id=0
		g_Hook_UseSkillInfo.dwId = 0;
		g_Hook_UseSkillInfo.TarPoint = Monster.GetPoint();
		g_Hook_UseSkillInfo.NowPoint = Monster.GetPoint();
		break;
	case em_SkillType_Buff_Self: // �������״̬�ļ���, Id=��������
		g_Hook_UseSkillInfo.dwId = fn_GetMonsterSkillId(PERSON->GetBase());
		g_Hook_UseSkillInfo.TarPoint = PERSON->GetPoint();
		break;
	case em_SkillType_Self_UnDirectional:
		g_Hook_UseSkillInfo.dwId = 0;
		g_Hook_UseSkillInfo.TarPoint = PERSON->GetPoint();
		g_Hook_UseSkillInfo.NowPoint = PERSON->GetPoint();
		break;
	default:
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"�ղݡ���SkillType=%X", pSkill->SkillType);
		StopGame;
		break;
	}

	

	g_Hook_UseSkillInfo.dwSkillBase = pSkill->GetBase();
	g_Hook_UseSkillInfo.dwIndex = pSkill->dwKeyIndex;
	g_bUseSkill = TRUE;
	CGameSend::Sleep(100);
	/*RECT r;
	::GetWindowRect(g_pAccountGame->hWndGame, &r);
	switch (emSkillFlag)
	{
	case em_Skill_Flag_Q:
		CLPublic::SimulationMouse(r.left + 383, r.top + 728);
		break;
	case em_Skill_Flag_W:
		CLPublic::SimulationMouse(r.left + 430, r.top + 728);
		break;
	case em_Skill_Flag_E:
		CLPublic::SimulationMouse(r.left + 473, r.top + 728);
		break;
	case em_Skill_Flag_R:
		CLPublic::SimulationMouse(r.left + 520, r.top + 728);
		break;
	default:
		break;
	}*/
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"ʹ�����!");
	return TRUE;
}

BOOL CSkill::StudySkill(em_Skill_Flag emSkillFlag)
{
	MsgBoxDebug_W((int)emSkillFlag < 4, _SELF, __LINE__, L"�ղ�,��ļ��ܶ�ѧ��%dȥ��", (int)emSkillFlag);

	switch (emSkillFlag)
	{
	case em_Skill_Flag_Q:
		CGameSend::GameTalk(L"ѧ����,�ӵ���:Q");
		break;
	case em_Skill_Flag_W:
		CGameSend::GameTalk(L"ѧ����,�ӵ���:W");
		break;
	case em_Skill_Flag_E:
		CGameSend::GameTalk(L"ѧ����,�ӵ���:E");
		break;
	case em_Skill_Flag_R:
		CGameSend::GameTalk(L"ѧ����,�ӵ���:R");
		break;
	default:
		break;
	}

	g_dwStudySkillIndex = (int)emSkillFlag;
	CGameSend::Sleep(WAITTIME);
	return TRUE;
Function_Exit:;
	return FALSE;
}

UINT CSkill::GetSkillList(vector<CSkill>& vSkillList)
{
	vSkillList.clear();

	DWORD dwRoot = ReadDWORD(�����ַ) + ���＼��ƫ�� + ���＼��ƫ��2;

	for (int i = 0; i < 0x3F; ++i)
	{
		DWORD dwObj = ReadDWORD(dwRoot + i * 4);
		if (dwObj == 0 || ReadDWORD(dwObj + UseItem_Of_SkillOffset) == 0)
			continue;

		vSkillList.push_back(CSkill(i, dwObj));
	}

	return vSkillList.size();
}

BOOL CSkill::LevelUpSkill()
{
	DWORD dwSkillPoint = PERSON->GetSkillPoint();
	if (dwSkillPoint > 0 && GameStatus_Run)
		return StudySkill(CGameRes::FindSkillPoint_For_Hero(g_pAccountGame->MyAcount_Info.emHeroPro, PERSON->GetLevel() - dwSkillPoint + 1));

	return TRUE;
}
