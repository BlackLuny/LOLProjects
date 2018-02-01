#include "stdafx.h"
#include "Monster.h"

#define _SELF L"Monster.cpp"

CMonster::~CMonster()
{
}

DWORD CMonster::GetBase()
{
	if (bPerson)
		return ReadDWORD(�����ַ);

	return dwMonsterBase;
}

BOOL CMonster::Init(DWORD dwBase)
{
	fDis = 0;
	bPerson = FALSE;
	dwMonsterBase = dwBase;
	ZeroMemory(wszName, sizeof(wszName));
	dwMonsterParm = NULL;

	return TRUE;
}

VOID CMonster::SetPerson()
{
	bPerson = TRUE;
}

DWORD CMonster::GetId()
{
	return ReadDWORD(GetBase() + 0x8);
}

//////////////////////////////////////////////////////////////////////////
DWORD CMonster::GetHp()
{
	return (DWORD)ReadFloat(GetBase() + ����Ѫƫ��);
}

DWORD CMonster::GetMaxHp()
{
	return (DWORD)ReadFloat(GetBase() + �������Ѫƫ��);
}

DWORD CMonster::GetPercentHp()
{
	__try
	{
		return (DWORD)((float)GetHp() / (float)GetMaxHp() * 100);
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"GetPercentHp�������쳣");
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
DWORD CMonster::GetMp()
{
	return (DWORD)ReadFloat(GetBase() + ������ƫ��);
}

DWORD CMonster::GetMaxMp()
{
	return (DWORD)ReadFloat(GetBase() + ���������ƫ��);
}

DWORD CMonster::GetPercentMp()
{
	__try
	{
		if (GetMaxMp() == 0)
			return 100;

		return (DWORD)((float)GetMp() / (float)GetMaxMp() * 100);
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"GetPercentMp�������쳣");
	}
	return 0;
}

float CMonster::GetDis(Point& TarPoint)
{
	fDis = CGameSend::Get2DDistance(&TarPoint, &GetPoint());
	return fDis;
}

Point CMonster::GetPoint()
{
	Point MonsterPoint;
	MonsterPoint.X = ReadFloat(GetBase() + ����X����ƫ��);
	MonsterPoint.Z = ReadFloat(GetBase() + ����Y����ƫ��);
	MonsterPoint.Y = ReadFloat(GetBase() + ����Z����ƫ��);

	return MonsterPoint;
}

LPWSTR CMonster::GetName()
{
	if (!CCharacter::wstrcmp_my(wszName, L""))
		return wszName;

	CHAR szText[64] = { 0 };
	if (ReadDWORD(GetBase() + ��������ƫ�� + 0x14) != 0xF)
		CCharacter::strcpy_my(szText, (LPCSTR)ReadDWORD(GetBase() + ��������ƫ��));
	else
		CCharacter::strcpy_my(szText, (LPCSTR)(GetBase() + ��������ƫ��));

	CCharacter::UTF8ToUnicode(wszName, szText);
	return wszName;

	/*if (!CCharacter::wstrcmp_my(wszName, L""))
		return wszName;

	DWORD dwResult = CGameSend::CALL_GetMonsterName(GetBase());
	if (dwResult == NULL)
		return wszName;

	CHAR szText[64] = { 0 };
	if (ReadDWORD(dwResult + 0x10) > 0xF)
		CCharacter::strcpy_my(szText, (LPCSTR)ReadDWORD(dwResult));
	else
		CCharacter::strcpy_my(szText, (LPCSTR)dwResult);

	UTF8ToUnicode(wszName, szText);
	return wszName;*/
}

em_Camp CMonster::GetCamp()
{
	switch (ReadDWORD(GetBase() + 0x14))
	{
	case 0x64:
		return em_Camp_Blue;
	case 0xC8:
		return em_Camp_Red;
	case 0x12C:
		return em_Camp_Neutral;
	}
	return em_Camp_UnKnow;
}

em_Camp CMonster::GetEnemyCamp()
{
	return GetCamp() == em_Camp_Blue ? em_Camp_Red : em_Camp_Blue;
}

bool CMonster::operator<(const CMonster& Obj)
{
	return this->fDis < Obj.fDis;
}

BOOL CMonster::IsSuperSoilder()
{
	return TRUE;
}

em_MonsterType CMonster::GetMonsterType()
{
	switch (ReadDWORD(GetBase() + 0x18))
	{
	case 0x20005: case 0x2401:
		return em_MonsterType_Turret;
	case 0x1401:
		return em_MonsterType_Hero;
	case 0xC01:
		return em_MonsterType_Soldier;
	default:
		break;
	}
	return em_MonsterType_UnKnow;
}

BOOL CMonster::IsCrystalTurret()
{
	return ReadDWORD(GetBase() + 0x18) == 0x20005 ? TRUE : FALSE;
}

DWORD CMonster::GetLevel()
{
	if (GetMonsterType() != em_MonsterType_Hero)
		return 0;

	__try
	{
		DWORD dwBase = GetBase();
		DWORD dwLevel = 0;
		_asm
		{
			MOV ECX, dwBase
				MOV EDX, DWORD PTR DS : [ECX]
				MOV EAX, DWORD PTR DS : [EDX + HeroLevel_Offset]
				CALL EAX
				MOV dwLevel,EAX
		}

		return dwLevel;
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"GetLevel�������쳣");
	}
	return 0;
}

DWORD CMonster::GetObj()
{
	__try
	{
		DWORD dwBase = GetBase();
		DWORD dwResult = 0;
		_asm
		{
			MOV ECX, dwBase
				MOV EDX, DWORD PTR DS : [ECX]
				MOV EAX, DWORD PTR DS : [EDX + 0xBC]
				CALL EAX
				MOV dwResult,EAX
		}
		return dwResult;
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"GetObj�������쳣");
	}
	return 0;
}

DWORD CMonster::GetSkillPoint()
{
	if (!bPerson)
		return 0;

	return ReadDWORD(GetObj() + 0x34);
}

DWORD CMonster::GetMoney()
{
	if (!bPerson)
		return 0;

	return (DWORD)ReadFloat(GetBase() + �����Ǯƫ��);
}

DWORD CMonster::GetTargetId()
{
	return ReadDWORD(GetBase() + ����Ŀ��ID);
}

LPWSTR CMonster::GetHeroName()
{
	CHAR szText[32] = { 0 };
	CCharacter::strcpy_my(szText, (LPCSTR)(GetBase() + yingxiong_Title));
	CCharacter::UTF8ToUnicode(wszHeroName, szText);
	return wszHeroName;
}

DWORD CMonster::GetMonsterParm()
{
	if (dwMonsterParm != NULL)
		return dwMonsterParm;

	__try
	{
		DWORD dwBase = GetBase();
		DWORD dwResult = 0;
		_asm
		{
			MOV ECX,dwBase
			MOV EDI, DWORD PTR DS : [ECX]
			MOV EDI, DWORD PTR DS : [EDI + �����������ƫ��]
			CALL EDI
			MOV dwResult,EAX
		}
		dwMonsterParm = dwResult;
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"�������쳣");
	}
	return dwMonsterParm;
}

BOOL CMonster::IsShow()
{
	if (GetMonsterType() == em_MonsterType_Turret)
		return TRUE;
	if (GetCamp() == PERSON->GetCamp())
		return TRUE;

	return ReadBYTE(GetMonsterParm() + ��������ƫ��1 + ��������ƫ��2) == 1 ? FALSE : TRUE;
}

DWORD CMonster::GetPersonAttackPower()
{
	//dd [[[0x154D050]+1c]+14]+10
	return (DWORD)ReadFloat(ReadDWORD(ReadDWORD(ReadDWORD(�������Ի�ַ) + 0x1C) + 0x14) + 0x10);
}

BOOL CMonster::NormalAttack()
{
	g_AttackPoint = GetPoint();
	g_dwNormalAttackBase = GetBase();
	CGameSend::Sleep(WAITTIME);
	return TRUE;
}

BOOL CMonster::Occupy()
{
	g_dwOccupyObjBase = GetBase();
	CGameSend::Sleep(WAITTIME);
	return TRUE;
}

float CMonster::GetAttackDis()
{
	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Garen: case em_Hero_Pro_MasterYi: case em_Hero_Pro_Chogath: case em_Hero_Pro_Nunu:  case em_Hero_Pro_Maokai:case em_Hero_Pro_Sejuani: case em_Hero_Pro_Galio:
	case em_Hero_Pro_Taric: case em_Hero_Pro_Trundle: case em_Hero_Pro_Aatrox: case em_Hero_Pro_Rumble: case em_Hero_Pro_Diana: case em_Hero_Pro_Urgot: case em_Hero_Pro_Fiora:
	case em_Hero_Pro_Mordekaiser: case em_Hero_Pro_Nautilus: case em_Hero_Pro_RekSai: case em_Hero_Pro_Sivir: case em_Hero_Pro_Thresh:
		return 2.0f;
	case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Annie:
		return 5.0f;
	default:
		return 7.0f;
	}
	return 7.0f;
}

CMonster* CMonster::GetPerson()
{
	static CMonster Person(0);
	static BOOL bInit = FALSE;

	if (bInit)
		return &Person;
	
	// ��ʼ��
	Person.SetPerson();
	bInit = TRUE;
	return &Person;
}

UINT CMonster::GetAroundObjectList(vector<CMonster>& vlst)
{
	vlst.clear();
	DWORD dwStart = ReadDWORD(��Χ���������ַ + 0x0);
	DWORD dwEnd = ReadDWORD(��Χ���������ַ + 0x4);
	DWORD dwCount = (dwEnd - dwStart) / 4;

	for (DWORD i = 0; i < dwCount; ++i)
	{
		DWORD dwObj = ((DWORD*)dwStart)[i];
		if (ReadDWORD(dwObj + 0x110) == 0)
			continue;

		CMonster Monster(dwObj);
		if (Monster.GetHp() == NULL/* || Monster.IsDead()*/)
			continue;

		Monster.GetDis(GetPerson()->GetPoint());
		vlst.push_back(Monster);
	}


	sort(vlst.begin(), vlst.end());
	return vlst.size();
}

BOOL CMonster::IsAnemia()
{
	if (GetMonsterType() == em_MonsterType_Hero)
		return GetPercentHp() <= 40 ? TRUE : FALSE;
	else if (GetMonsterType() == em_MonsterType_Soldier)
		return GetPercentHp() <= 20 ? TRUE : FALSE;

	return FALSE;
}

BOOL CMonster::IsLackBlood()
{
	return (GetPercentHp() <= 30) ? TRUE : FALSE;
}

BOOL CMonster::IsDead()
{
	return ReadDWORD(GetBase() + ����ƫ��) != 0 ? TRUE : FALSE;
}