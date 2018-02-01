#include "stdafx.h"
#include "FightFunction.h"
#define _SELF L"FightFunction.cpp"
CFightFunction::CFightFunction()
{
	bSwitchPath = FALSE;
	bStopMoveToEnd = FALSE;
	dwKillDragonTick = NULL;
	dwKillBlueBuffTick = NULL;
	dwDragonCount = NULL;
	dwKillRedBuffTick = NULL;
}

CFightFunction::~CFightFunction()
{
}

BOOL CFightFunction::KillSolider_Skill(CMonster& Solider, BOOL bUseQ, BOOL bUseW, BOOL bUseE, BOOL bUseR)
{
	if (bUseQ && UseSkill_By_Dis(em_Skill_Flag_Q, Solider))
		return TRUE;

	if (bUseR && UseSkill_By_Dis(em_Skill_Flag_R, Solider))
		return TRUE;

	if (bUseW && UseSkill_By_Dis(em_Skill_Flag_W, Solider))
		return TRUE;

	if (bUseE && UseSkill_By_Dis(em_Skill_Flag_E, Solider))
		return TRUE;

	return Solider.NormalAttack();
}

BOOL CFightFunction::KillSolider(CMonster& Solider, BOOL bClearAround)
{
	// ��Ӣ�۷���
	UseSpecialSkill();
	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Ryze: case em_Hero_Pro_Maokai:
		return KillSolider_Skill(Solider, TRUE, FALSE, TRUE, FALSE); // QE
	case em_Hero_Pro_Ashe:
		return KillSolider_Skill(Solider, FALSE, FALSE , FALSE, FALSE);
	case em_Hero_Pro_MasterYi: case em_Hero_Pro_Swain: case em_Hero_Pro_Graves:
	case em_Hero_Pro_Teemo: case em_Hero_Pro_MissFortune:
		return KillSolider_Skill(Solider, FALSE, FALSE, FALSE, FALSE); // Nop
	case em_Hero_Pro_Garen:  case em_Hero_Pro_Tristana: case em_Hero_Pro_Galio:
	case em_Hero_Pro_Aatrox: case em_Hero_Pro_Urgot:
		return KillSolider_Skill(Solider, FALSE, FALSE, TRUE, FALSE); // E
	case em_Hero_Pro_Malzahar:
		return KillSolider_Skill(Solider, FALSE, TRUE, TRUE, FALSE); // WE
	case em_Hero_Pro_Chogath:
		return KillSolider_Skill(Solider, Solider.GetPercentHp() > 50 ? FALSE : TRUE, FALSE, FALSE, TRUE);
	case em_Hero_Pro_Vayne:  case em_Hero_Pro_Lucian: case em_Hero_Pro_Trundle:
	case em_Hero_Pro_Ezreal: case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Nunu:
	case em_Hero_Pro_Syndra: case em_Hero_Pro_Rumble: case em_Hero_Pro_Diana:
	case em_Hero_Pro_Caitlyn: case em_Hero_Pro_Ahri:
		return KillSolider_Skill(Solider, TRUE, FALSE, FALSE, FALSE); // Q
	case em_Hero_Pro_Sejuani: case em_Hero_Pro_Taric:
		return KillSolider_Skill(Solider, FALSE, TRUE, FALSE, FALSE); // W
	case em_Hero_Pro_Veigar: case em_Hero_Pro_Annie:
		return KillSolider_Skill(Solider, Solider.GetPercentHp() <= 40 ? TRUE : FALSE, FALSE, FALSE, FALSE);
	default:
		return Solider.NormalAttack();
	}

	return FALSE;
}

BOOL CFightFunction::KillHero(CMonster& Hero, BOOL bConsume)
{
	// ��Ӣ�۷���
	UseSpecialSkill();
	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Ryze:
		return KillHero_Ryze(Hero, bConsume);
	case em_Hero_Pro_Ashe:
		return KillHero_Ashe(Hero, bConsume);
	case em_Hero_Pro_MissFortune:
		return KillHero_MissFortune(Hero, bConsume);
	case em_Hero_Pro_MasterYi:
		return KillHero_MasterYi(Hero, bConsume);
	case em_Hero_Pro_Garen:
		return KillHero_Garen(Hero, bConsume);
	case em_Hero_Pro_Chogath:
		return KillHero_Chogath(Hero, bConsume);
	case em_Hero_Pro_Nunu:
		return KillHero_Nunu(Hero, bConsume);
	case em_Hero_Pro_Heimerdinger:
		return KillHero_Heimerdinger(Hero, bConsume);
	case em_Hero_Pro_Ezreal:
		return KillHero_Ezreal(Hero, bConsume);
	case em_Hero_Pro_Malzahar:
		return KillHero_Malzahar(Hero, bConsume);
	case em_Hero_Pro_Swain:
		return KillHero_Swain(Hero, bConsume);
	case em_Hero_Pro_Graves:
		return KillHero_Graves(Hero, bConsume);
	case em_Hero_Pro_Vayne:
		return KillHero_Vayne(Hero, bConsume);
	case em_Hero_Pro_Maokai:
		return KillHero_Maokai(Hero, bConsume);
	case em_Hero_Pro_Veigar:
		return KillHero_Veigar(Hero, bConsume);
	case em_Hero_Pro_Sejuani:
		return KillHero_Sejuani(Hero, bConsume);
	case em_Hero_Pro_Tristana:
		return KillHero_Tristana(Hero, bConsume);
	case em_Hero_Pro_Lucian:
		return KillHero_Lucian(Hero, bConsume);
	case em_Hero_Pro_Galio:
		return KillHero_Galio(Hero, bConsume);
	case em_Hero_Pro_Trundle:
		return KillHero_Tristana(Hero, bConsume);
	case em_Hero_Pro_Syndra:
		return KillHero_Syndra(Hero, bConsume);
	case em_Hero_Pro_Aatrox:
		return KillHero_Aatrox(Hero, bConsume);
	case em_Hero_Pro_Taric:
		return KillHero_Taric(Hero, bConsume);
	case em_Hero_Pro_Rumble:
		return KillHero_Rumble(Hero, bConsume);
	case em_Hero_Pro_Diana:
		return KillHero_Diana(Hero, bConsume);
	case em_Hero_Pro_Urgot:
		return KillHero_Urgot(Hero, bConsume);
	case em_Hero_Pro_Teemo:
		return KillHero_Teemo(Hero, bConsume);
	case em_Hero_Pro_Caitlyn:
		return KillHero_Caitlyn(Hero, bConsume);
	case em_Hero_Pro_Corki:
		return KillHero_Corki(Hero, bConsume);
	case em_Hero_Pro_Fiora:
		return KillHero_Fiora(Hero, bConsume);
	case em_Hero_Pro_Kennen:
		return KillHero_Kennen(Hero, bConsume);
	case em_Hero_Pro_Mordekaiser:
		return KillHero_Mordekaiser(Hero, bConsume);
	case em_Hero_Pro_Nautilus:
		return KillHero_Nautilus(Hero, bConsume);
	case em_Hero_Pro_RekSai:
		return KillHero_RekSai(Hero, bConsume);
	case em_Hero_Pro_Sivir:
		return KillHero_Sivir(Hero, bConsume);
	case em_Hero_Pro_Thresh:
		return KillHero_Thresh(Hero, bConsume);
	case em_Hero_Pro_TwistedFate:
		return KillHero_TwistedFate(Hero, bConsume);
	case em_Hero_Pro_KogMaw:
		return KillHero_KogMaw(Hero, bConsume);
	case em_Hero_Pro_Ahri:
		return KillHero_Ahri(Hero, bConsume);
	case em_Hero_Pro_Annie:
		return KillHero_Annie(Hero, bConsume);
	default:
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��ʱ��֧�����Ӣ��,em=%X", g_pAccountGame->MyAcount_Info.emHeroPro);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		return FALSE;
	}

	return FALSE;
}

BOOL CFightFunction::AttackSolider(BOOL bClearAround)
{
	CMonster Solider;
	if (!GetRecentlyEnemySolider(MaxDis_Attack, NULL, Solider))
		return TRUE;

	// ����С��
	float fAttackDis = PERSON->GetAttackDis();
	if (PERSON->GetDis(Solider.GetPoint()) > fAttackDis)
		return CGameSend::Move(Solider.GetPoint());

	return HitSolider(Solider, bClearAround);
}

BOOL CFightFunction::HitSolider(CMonster& Solider, BOOL bClearAround)
{
	// ��ƶѪ�������, ֻ����ͨ����, �ͱ��˷����ü�����
	if (Solider.IsAnemia())
		return Solider.NormalAttack();

	return KillSolider(Solider, bClearAround);
}

BOOL CFightFunction::AttackTurret()
{
	CMonster Turret;
	if (!GetRecentlyEnemyTurret(MaxDis_Attack, NULL, Turret))
		return TRUE;

	// ������
	float fAttackDis = PERSON->GetAttackDis();
	if (PERSON->GetDis(Turret.GetPoint()) > 7.0f)
		 CGameSend::Move(Turret.GetPoint());

	// ������¹�����Χ���б�,�����.
	//if (GetAroundEnemySoliderCount(fAttackDis, NULL) > 0)
		//return AttackSolider(TRUE);

	return HitTurret(Turret);
}

BOOL CFightFunction::HitTurret(CMonster& Turret)
{
	return Turret.NormalAttack();
}

BOOL CFightFunction::DodgeTurret()
{
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"��Ҫ���˶�����Ĺ���");
	// ������
	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(MaxDis_Attack, NULL);
	BackOff();
	// һֱ���˵����Ĺ�����Χ20m����, ��취�ȱ��߳���,�������ɱӢ��
	while (GetAroundEnemyTurretCount(MaxDis_Attack + 5.0f, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(500);
		BackOff();
	}

	// ֹͣ��, ��������ľ��Ӣ��
	if (uAroundEnemyHeroCount != 0)
	{
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"������Ӣ��, ��ֹǰ��");
		//bStopMoveToEnd = TRUE;
	}

	if (CEqument::ExistItem(0xDB8))
		CSkill::UseNormalSkill_By_Name(L"ItemVoidGate", *PERSON, em_SkillType_UnDirectional);

	/*if (uAroundEnemyHeroCount != 0 && g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ryze && bSwitchPath)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"���˶�����ʱ���־�Ȼ��Ӣ��������, �����ˣ�ȥ����·Gank");
		SwitchPath();
		return TRUE;
	}*/
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"ֹͣ����");
	// ֹͣ����
	return CGameSend::StopAction();
}

BOOL CFightFunction::HandleRegainMove()
{
	if (!bStopMoveToEnd)
		return FALSE;

	// �ж��Լ�15m���Ƿ�����
	if (GetAroundEnemyTurretCount(20.0f, NULL) == 0)
	{
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"20m�ڶ�ľ����, �ָ�ǰ��");
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	// ��ȡ�������
	static CMonster NearestTurret;
	if (!GetRecentlyEnemyTurret(20.0f, NULL, NearestTurret))
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"20m�ڶ�ľ����, �ָ�ǰ��");
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	// ��ȡ15m���Ƿ���Ӣ��
	static CMonster NearMonster;
	if (!GetRecentlyEnemyHero(25.0f, NULL, NearMonster))
	{
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"25m�ڶ�ľ��Ӣ��,�ָ�ǰ��");
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	// �ж� �������, �������Ӣ�۵ľ��볬��10m, �ͻָ�
	if (NearestTurret.GetDis(NearMonster.GetPoint()) > MaxDis_Attack)
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"����ĵз���������ĵз�Ӣ�۾���=%.2f,�ָ�ǰ��", NearestTurret.GetDis(NearMonster.GetPoint()));
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	return TRUE;
}

BOOL CFightFunction::IsEvacuate()
{
	// ���15m��Χ ��������� > ��������, �ͺ���
	float fEvacuateDis = 10.0f;
	static CMonster NearestTurret;

	// ������3m֮�ھͲ�������! �Ͼ����Ĺ�����Χ��10m, Ӣ�۵Ĺ�������һ�㶼ľ��7m
	if (GetRecentlyAllianceTurret(5.0f, NULL, NearestTurret))
		return FALSE;

	if (PERSON->GetLevel() > 7)
		fEvacuateDis = 15.0f;

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(fEvacuateDis, NULL);
	UINT uAroundAllianceHeroCount = GetAroundAllianceHeroCount(8.0f, NULL);

	/*if (g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ryze)
	{
		if (uAroundEnemyHeroCount > uAroundAllianceHeroCount + 1)// ����ֻ��4���˵�ʱ�����, 3�������ѹ�Ŵ�!
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"�з���%d����, �Լ�ֻ��%d����, ��ʱ����һ��,�͵���~!", uAroundEnemyHeroCount, uAroundAllianceHeroCount);
			return TRUE;
		}
		return FALSE;
	}*/

	// �жϵ����Ƿ�����������, �ǵĻ��Ͳ���
	static CMonster SelfTurret;
	static CMonster NearMonster;
	if (GetRecentlyAllianceTurret(fEvacuateDis, NULL, SelfTurret) && GetRecentlyEnemyHero(fEvacuateDis, NULL, NearMonster))
	{
		// �����������Լ�����, ��ë?
		if (SelfTurret.GetDis(NearMonster.GetPoint()) <= 7.0f)
		{
			return FALSE;
		}
	}

	if (g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ashe || g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Swain)
	{
		if (uAroundEnemyHeroCount <= uAroundAllianceHeroCount)
		{
			static CMonster EnemyHero;
			if (!GetRecentlyEnemyHero(fEvacuateDis, NULL, EnemyHero))
				return FALSE;

			// ����з�Ӣ���ں���2m��Χ��, �ͺ���, ��Ҫ�����ǽ���!, ��������Ѫ������20%
			float fDis = PERSON->GetDis(EnemyHero.GetPoint());
			if (fDis <= 1.5f)
			{
				if (EnemyHero.GetPercentHp() <= 30)
					return FALSE;
				else
					return TRUE;
			}
			else if (fDis >= 3.0f)
				return FALSE;

			return TRUE;
		}
		return TRUE;
	}
	else
	{
		if (uAroundEnemyHeroCount > uAroundAllianceHeroCount)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"�з���%d����, �Լ�ֻ��%d����, ��ʱ����һ��,�͵���~!", uAroundEnemyHeroCount, uAroundAllianceHeroCount);
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CFightFunction::IsEvacuate_Crystal()
{
	// ���15m��Χ ��������� > ��������, �ͺ���
	const float fEvacuateDis = 15.0f;
	static CMonster NearestTurret;

	// ������3m֮�ھͲ�������! �Ͼ����Ĺ�����Χ��7m, Ӣ�۵Ĺ�������һ�㶼ľ��7m
	if (GetRecentlyAllianceTurret(3.0f, NULL, NearestTurret))
		return FALSE;

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(fEvacuateDis, NULL);
	UINT uAroundAllianceHeroCount = GetAroundAllianceHeroCount(8.0f, NULL);

	if (uAroundEnemyHeroCount > uAroundAllianceHeroCount)// �з�����ռ���Ƶ������, �������
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::IsEvacuate_Scuffle()
{
	// ���15m��Χ ��������� > ��������, �ͺ���
	const float fEvacuateDis = 15.0f;
	static CMonster NearestTurret;

	// ������3m֮�ھͲ�������! �Ͼ����Ĺ�����Χ��10m, Ӣ�۵Ĺ�������һ�㶼ľ��7m
	if (GetRecentlyAllianceTurret(3.0f, NULL, NearestTurret))
		return FALSE;

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(fEvacuateDis, NULL);
	UINT uAroundAllianceHeroCount = GetAroundAllianceHeroCount(10.0f, NULL);

	if (uAroundEnemyHeroCount > uAroundAllianceHeroCount + 2)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"����%d��, �Լ����%d��,����!", uAroundEnemyHeroCount, uAroundAllianceHeroCount);
		return TRUE;
	}

	return FALSE;
}

BOOL CFightFunction::Evacuate()
{
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"Evacuate 5m!");
	Point NowPoint = PERSON->GetPoint();
	BackOff();

	while (GameStatus_Run && !EMPTY_PERSONHP && PERSON->GetDis(NowPoint) < 5.0f)
	{
		CGameSend::Sleep(WAITTIME);
		BackOff();
	}

	return TRUE;
	return CGameSend::StopAction();
}

BOOL CFightFunction::ConsumeEnemyHero()
{
	CMonster Hero;
	if (!GetRecentlyEnemyHero(MaxDis_Attack, NULL, Hero))
		return FALSE;

	KillHero(Hero, TRUE);
	if (Hero.GetTargetId() == PERSON->GetId())
		Evacuate();

	return TRUE;
}

BOOL CFightFunction::DodgeHero()
{
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"����Ӣ�۵Ĺ���");
	// ������
	CGameSend::StopAction();
	BackOff();

	// һֱ���˵�Ӣ�۲������Լ�Ϊֹ
	while (GetAttackSelfHeroCount(MaxDis_Attack, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
	{
		BackOff();
		CGameSend::Sleep(WAITTIME);
	}
	// ֹͣ����
	return CGameSend::StopAction();
}

BOOL CFightFunction::IsDodgeTurret()
{
	// ��ȡ�ж���Ӫ����
	static vector<CMonster> vMonsterList;
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Turret, PERSON->GetEnemyCamp(), MaxDis_Attack + 2.0f, NULL);

	if (vMonsterList.size() == 0)
		return FALSE;

	// ˮ��������
	if (vMonsterList.at(0).IsCrystalTurret())
		return FALSE;

	// �Ƿ��Ȫˮ��,15m
	/*CMonster SpringTurr;
	if (FindMonster_For_Name(CGameRes::GetSpringName(PERSON->GetEnemyCamp()), 15.0f, SpringTurr))
		return TRUE;*/

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(MaxDis_Attack, NULL);
	UINT uAroundAlliance = GetAroundAllianceHeroCount(MaxDis_Attack, NULL);

	// ����������Ŀ���ǲ����Լ�
	for (CMonster& Turret : vMonsterList)
	{
		// ����������, ��������
		if (Turret.GetTargetId() == PERSON->GetId())
			return TRUE;

		if (uAroundEnemyHeroCount == 0)
			continue;

		// ���±�Ӣ�۹�����, ��!
		if (IsDodgeHero())
			return TRUE;

		if (uAroundEnemyHeroCount == 1)
		{
			// ��һ�������������Ӣ��
			CMonster Hero;
			if (!GetRecentlyEnemyHero(MaxDis_Attack, NULL, Hero))
				continue;

			// ��������Ӣ�������ǱȽϽ�, �ǾͲ�Ҫȥ��������,��Զһ���
			if (Hero.GetDis(Turret.GetPoint()) < 7.0f)
			{
				return TRUE;
			}
		}
		else if (uAroundAlliance > uAroundEnemyHeroCount)
		{
			// ����������Ӣ�۶�������, ���顣 ֱ�Ӹ���
			continue;
		}
		else if (uAroundEnemyHeroCount > 1)
		{
			// �����2��������, �����ɡ� �ȴ�����!
			return TRUE;
		}
	}

	// 10m֮������2���Լ���ʿ��,���ȳ���
	if (GetAroundAllianceSoliderCount(PERSON->GetAttackDis() >= 7.0f ? MaxDis_Attack : MaxDis_Attack + 2.0f, &vMonsterList.at(0).GetPoint()) <= 2)
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::IsDodgeCrystalTurret()
{
	static vector<CMonster> vGuardLst;
	// ��ȡ�ж���Ӫ����
	GetMonsterList_By_Type(vGuardLst, em_MonsterType_Soldier, PERSON->GetEnemyCamp(), 15.0f, NULL);
	if (vGuardLst.size() == 0)
		return FALSE;

	// ��ȡ�ж���Ӫ��Ӣ��
	static vector<CMonster> vEnemyLst;
	GetMonsterList_By_Type(vEnemyLst, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 15.0f, NULL);
	if (vEnemyLst.size() == 0)
		return FALSE;

	// �жϵз�Ӣ���Ƿ���������, �����������, ��ô���������ǵз�Ӣ������+1��ǿ��!
	UINT uCount = 0;
	for (auto& Guard : vGuardLst)
	{
		for (auto& EnemyHero : vEnemyLst)
		{
			if (EnemyHero.GetDis(Guard.GetPoint()) <= 7.0f)
				uCount++;
		}
	}

	UINT uAllianceHeroCount = GetAroundAllianceHeroCount(10.0f, NULL);
	if (uAllianceHeroCount == 5)
		return FALSE;

	return uAllianceHeroCount >= uCount + 1;
}

BOOL CFightFunction::DodgeCrystalTurret()
{
	while (GameStatus_Run && !EMPTY_PERSONHP && IsDodgeCrystalTurret())
	{
		BackOff();
		Sleep(WAITTIME);
	}
	return TRUE;
}

BOOL CFightFunction::AttackHero()
{
	CMonster Hero;
	if (!GetRecentlyEnemyHero(g_fMaxDis_Attack, NULL, Hero))
		return TRUE;

	// ����С��
	float fAttackDis = PERSON->GetAttackDis();
	if (PERSON->GetDis(Hero.GetPoint()) > fAttackDis)
		return CGameSend::Move(Hero.GetPoint());

	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"ֱ�ӹ���Ӣ��,HP=%d", Hero.GetPercentHp());
	return KillHero(Hero, FALSE);
}

BOOL CFightFunction::UseSpecialSkill()
{
	/*auto fn_UseSkill_By_Condition = [this](bool bConditionHp,em_Skill_Flag emSkillFlag)
	{
		static vector<CMonster> vMonsterList;
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetCamp(), 5.0f, NULL);
		for (auto& tmpHero : vMonsterList)
		{
			if (bConditionHp && tmpHero.GetPercentHp() <= 30 )
			{
				CSkill::UseNormalSkill(emSkillFlag, *PERSON);
				//CSkill::UseNormalSkill_By_Name(L"summonerheal", *PERSON, em_SkillType_Buff_Self);
				break;
			}
			else if (!bConditionHp && tmpHero.GetPercentMp() <= 50 )
			{
				CSkill::UseNormalSkill(emSkillFlag, *PERSON);
				//CSkill::UseNormalSkill_By_Name(L"summonermana", *PERSON, em_SkillType_Buff_Self);
				break;
			}
		}
	};*/

	/*if (CSkill::GetSkillCool(em_Skill_Flag_1)) // 180s CD  ������, �走TXȡ����!
	{
		fn_UseSkill_By_Condition(false, em_Skill_Flag_1);
	}*/

	static DWORD dwHealTick = 0;
	if (::GetTickCount() - dwHealTick >= 240 * 1000)
	{
		if (PERSON->GetPercentHp() < 50)
		{
			CSkill::UseItemSkill(L"summonerheal");
			dwHealTick = ::GetTickCount();
			return TRUE;
		}
		static vector<CMonster> vMonsterList;
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetCamp(), 5.0f, NULL);
		for (auto& tmpHero : vMonsterList)
		{
			if (tmpHero.GetPercentHp() <= 50)
			{
				CSkill::UseItemSkill(L"summonerheal");
				break;
			}
		}
		
		dwHealTick = ::GetTickCount();
	}
	return TRUE;
}

BOOL CFightFunction::IsKillDragon()
{
	if (g_MapType != MapType_�ٻ�ʦϿ��)
		return FALSE;

	static DWORD dwWaitTick = 0;

	// �����˷�ʦȥ�������, ���󶼲�ȥ������, ѹס��·
	if (g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ryze)
		return FALSE;

	if (dwKillDragonTick != 0)
	{
		if (::GetTickCount() - dwKillDragonTick < 6 * 60 * 1000)
			return FALSE;

		CPrintLog::PrintLog_W(_SELF,__LINE__,L"6���ӵ�ˢ��ʱ�䵽��~!");
		return TRUE;
	}

	// 3s ����һ��, ��̫Ƶ��
	if (::GetTickCount() - dwWaitTick >= 3 * 1000)
	{
		dwWaitTick = ::GetTickCount();
		// ���Ҷ��ѵĵȼ�, ����Ҫ��4��5��
		if (GetAroundAllianceHeroCount_By_Level(2000.0f, 5) >= 4)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"��Χ���㹻5���Ķ���, ׼��ȥɱС��!");
			return TRUE;
		}
	}


	return FALSE;
}

BOOL CFightFunction::KillDragon()
{
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"׼��ɱ��,��ʼ�س�");
	BackHome();

	static CMonster Drogan;
	Point DragonPoint = CGameRes::GetDragonPoint();
	Point tmpPoint = DragonPoint;
	tmpPoint.X += 300.0f;
	tmpPoint.Y += 300.0f;
	static DWORD dwTickCount = 0;
	DWORD dwWaitTick = ::GetTickCount();

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"��С���ķ����ߡ���");
	

	// �ȴ����Ѽ���, ����ез�Ӣ�۳���, ˳�����������
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Move(DragonPoint);
		CGameSend::Sleep(500);

		// �жϸ����Ƿ��ез�Ӣ��
		if (GetAroundEnemyHeroCount(MaxDis_Attack, NULL) > 0)
		{
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"�����з�Ӣ��!");
			AttackHero();
			continue;
		}

		if (::GetTickCount() - dwWaitTick >= 1 * 60 * 1000)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"1�����ڶ�û����, ֱ�ӿ�����!");
			break;
		}

		if (PERSON->GetDis(DragonPoint) < 2.0f)
		{
			// 5m��Χ�ڱ���Ҫ��4�������ϵĶ���
			UINT uAroundAllianceHeroCoun = GetAroundAllianceHeroCount(MaxDis_Attack, NULL);
			if (dwKillDragonTick != NULL && uAroundAllianceHeroCoun >= 3)
				break;
			else if (uAroundAllianceHeroCoun >= 4)
				break;

			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"��������=%d", GetAroundAllianceHeroCount(MaxDis_Attack, NULL));
			if (!FindMonster_For_Name(DroganName, 10.0f, Drogan))
				goto lb_Exit;

			if (Drogan.GetPercentHp() != 100)
				break;
		}

		CGameSend::Move(tmpPoint);
	}
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"�������!,׼����ɱ");

	// �жϴ���
	if (!GameStatus_Run || EMPTY_PERSONHP)
		return FALSE;

	// ��ʼɱ��
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ʼ����С����λ��");
	if (!FindMonster_For_Name(DroganName, 10.0f, Drogan))
		goto lb_Exit;

	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"����!");
	while (Drogan.GetHp() != NULL  && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(100);
		KillHero(Drogan, FALSE);
	}

lb_Exit:;
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"����С����!");
	BackHome();
	dwKillDragonTick = ::GetTickCount();
	SwitchPath();
	return TRUE;
}

BOOL CFightFunction::IsKillBlueBuff()
{
	if (g_MapType != MapType_�ٻ�ʦϿ��)
		return FALSE;

	// ������6mˢ��һ�ε�,

	// ֻ�����˷�ʦ ����>=6 ����ȥˢ��BUFF
	if (g_pAccountGame->MyAcount_Info.emHeroPro != em_Hero_Pro_Ryze)
		return FALSE;

	if (PERSON->GetLevel() < 6)
		return FALSE;

	// ��һ��6��ȥˢ, ʱ��϶�����ˢ����BUFF��ʱ����
	if ( dwKillBlueBuffTick == 0)
		return TRUE;

	if (::GetTickCount() - dwKillBlueBuffTick >= 6 * 60 * 1000)
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::KillBlueBuff()
{
	// ��ȡ��BUFF������
	Point BluePoint = CGameRes::GetBlueBuffPoint(PERSON->GetCamp());

	// �ȴ����Ѽ���, ����ез�Ӣ�۳���, ˳�����������
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Move(BluePoint);
		CGameSend::Sleep(500);

		// �жϸ����Ƿ��ез�Ӣ��
		while (GetAroundEnemyHeroCount(MaxDis_Attack, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
		{
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"�����з�Ӣ��!");
			AttackHero();
			continue;
		}

		if (PERSON->GetDis(BluePoint) < 2.0f)
			break;
	}
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"�������!,׼����ɱ");

	// �жϴ���
	if (!GameStatus_Run || EMPTY_PERSONHP)
		return FALSE;

	// ��ʼɱ��
	static CMonster BlueBuff;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ʼ������BUFF��λ��");
	if (!FindMonster_For_Name(BlueBuffName_Top, 10.0f, BlueBuff) && !FindMonster_For_Name(BlueBuffName2_Buttom, 10.0f, BlueBuff))
		goto lb_Exit;

	MoveToPoint(BlueBuff.GetPoint(), 3.0f);
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"����!");
	while (BlueBuff.GetHp() != NULL  && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(100);
		KillHero(BlueBuff, FALSE);
	}

	// ��������СҰ��
	KillAroundSRU(5.0f);

lb_Exit:;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"������BUFF��!");
	//BackHome();
	dwKillBlueBuffTick = ::GetTickCount();
	return TRUE;
}

BOOL CFightFunction::IsKillRedBuff()
{
	if (g_MapType != MapType_�ٻ�ʦϿ��)
		return FALSE;

	// �ж�������ľ����BUFF
	if (CPersonBuff::ExistBuff(0x16))
		return FALSE;

	if (g_pAccountGame->MyAcount_Info.emHeroPro != em_Hero_Pro_Ryze)
		return FALSE;

	if (PERSON->GetLevel() < 6)
		return FALSE;

	// ��һ��6��ȥˢ, ʱ��϶�����ˢ����BUFF��ʱ����
	if (dwKillRedBuffTick == 0)
		return TRUE;

	if (::GetTickCount() - dwKillRedBuffTick >= 6 * 60 * 1000)
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::KillRedBuff()
{
	// ��ȡ��BUFF������
	Point RedPoint = CGameRes::GetRedBuffPoint(PERSON->GetCamp());

	// �ȴ����Ѽ���, ����ез�Ӣ�۳���, ˳�����������
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Move(RedPoint);
		CGameSend::Sleep(500);

		// �жϸ����Ƿ��ез�Ӣ��
		while (GetAroundEnemyHeroCount(MaxDis_Attack, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
		{
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"�����з�Ӣ��!");
			AttackHero();
			continue;
		}

		if (PERSON->GetDis(RedPoint) < 2.0f)
			break;
	}
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"�������!,׼����ɱ");

	// �жϴ���
	if (!GameStatus_Run || EMPTY_PERSONHP)
		return FALSE;

	// ��ʼɱ��
	static CMonster RedBuff;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"��ʼ������BUFF��λ��");
	if (!FindMonster_For_Name(RedBuffName_Top, 10.0f, RedBuff) && !FindMonster_For_Name(RedBuffName2_Buttom, 10.0f, RedBuff))
		goto lb_Exit;

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"����!");
	MoveToPoint(RedBuff.GetPoint(), 3.0f);
	while (RedBuff.GetHp() != NULL && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(100);
		KillHero(RedBuff, FALSE);
	}

	// ��������СҰ��
	KillAroundSRU(5.0f);
lb_Exit:;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"�����BUFF��!");
	//BackHome();
	dwKillRedBuffTick = ::GetTickCount();
	return TRUE;
}

BOOL CFightFunction::KillAroundSRU(float fDis)
{
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		static vector<CMonster> vMonsterList;
		// ��ȡ�ж���Ӫ��Ұ��
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, em_Camp_Neutral, fDis, NULL);
		if (vMonsterList.size() == 0)
			break;

		for (auto& SRU : vMonsterList)
		{
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"��������Ұ��!");
			while (SRU.GetHp() != NULL && GameStatus_Run && !EMPTY_PERSONHP)
			{
				CGameSend::Sleep(100);
				KillHero(SRU, FALSE);
			}
			break;
		}
	}
	return TRUE;
}

////////////�ִδ����߼�//////////////////////////////////////////////////////////////
BOOL CFightFunction::HandleTurret()
{
	if (g_MapType == MapType_�ٻ�ʦϿ�� || g_MapType == MapType_������Ԩ)
	{
		// �Ƿ���Ҫ������ķ�Χ || ����
		if (IsDodgeTurret())
			return DodgeTurret();

		// �ǲ��ǿ�ʼ������
		if (IsAttackTurret())
			return AttackTurret();

		// �Ƿ���Իָ�ǰ��
		//HandleRegainMove();
	}
	else if (g_MapType == MapType_ˮ��֮��)
	{
		//if (IsDodgeCrystalTurret())
			//return BackOff();

		return OccupyEnemyTurret();
	}
	
	return FALSE;
}

BOOL CFightFunction::HandleHero()
{
	// �Ƿ���Ҫ����
	if (g_MapType == MapType_�ٻ�ʦϿ��)
	{
		if (IsEvacuate())
			return Evacuate();
	}
	else if (g_MapType == MapType_ˮ��֮��)
	{
		//if (IsEvacuate_Crystal())
			//return Evacuate();
	}
	else if (g_MapType == MapType_������Ԩ)
	{
		if (IsEvacuate_Scuffle())
			return Evacuate();
	}


	// �Ƿ���Ҫ���Ӣ�۵Ĺ���
	if (IsDodgeHero())
	{
		if (/*g_pAccountGame->MyAcount_Info.emHeroPro != em_Hero_Pro_Ryze && */g_MapType == MapType_�ٻ�ʦϿ�� && PERSON->GetLevel() < 6)
			return DodgeHero();

		return AttackHero();
	}


	// �Ƿ���Ҫ�ü�������Ӣ�۵�Ѫ��
	if (IsConsumeEnemyHero())
	{
		return ConsumeEnemyHero();
	}

	

	// �ǲ��ǿ�ʼ����Ӣ��
	if (IsAttackHero())
		return AttackHero();

	return FALSE;
}

BOOL CFightFunction::HandleSolider()
{
	if (g_MapType == MapType_ˮ��֮��)
		return FALSE;

	// �Ƿ���Ҫ���С���Ĺ���
	if (IsDodgeSolider())
		return DodgeEnemySolider();

	// �ǲ��ǿ�ʼ���
	if (IsClearSolider())
		return AttackSolider(FALSE);

	return FALSE;
}

BOOL CFightFunction::SwitchPath()
{
	dwDragonCount = CPersonBuff::GetBuffCount(0x1C);// S5Tooltip_DragonSlayerBuffv1
	if (dwDragonCount == 3 || !ExistMiddleCrystalTurret())
	{
		// �ֳ�2��ȥ��·��æ
		switch (g_pAccountGame->MyAcount_Info.emHeroPro)
		{
		 case em_Hero_Pro_Ashe: 
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Top;
			break;
		case em_Hero_Pro_Garen:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Buttom;
			break;
		default:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Middle;
			break;
		}
	}
	else if (dwDragonCount >= 5)
	{
		// ����Ӣ�۵ķ�·��·��
		switch (g_pAccountGame->MyAcount_Info.emHeroPro)
		{
		case em_Hero_Pro_Ryze: // ���˷�ʦ�е�
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Middle;
			break;
		case em_Hero_Pro_Swain: case em_Hero_Pro_Ashe: case em_Hero_Pro_Ezreal:  case em_Hero_Pro_Chogath:  case  em_Hero_Pro_Maokai:
		case em_Hero_Pro_MissFortune: case em_Hero_Pro_Sejuani: case em_Hero_Pro_Galio: case em_Hero_Pro_KogMaw: case em_Hero_Pro_Ahri:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Top;
			break;
		case em_Hero_Pro_Malzahar: case em_Hero_Pro_MasterYi: case em_Hero_Pro_Graves: case em_Hero_Pro_Vayne:
		case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Nunu: case em_Hero_Pro_Garen: case em_Hero_Pro_Veigar:
		case em_Hero_Pro_Tristana: case em_Hero_Pro_Lucian: case em_Hero_Pro_Corki: case em_Hero_Pro_Caitlyn:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Buttom;
			break;
		default:
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"����,���Ӣ��:%s��ʱ����֧���~!", PERSON->GetHeroName());
			break;
		}
	}
	
	

	return TRUE;
	//////////////////////////////////////////////////////////////////////////
	if (PERSON->GetLevel() < 6)
		return FALSE;

	static bool bInit = false;
	static em_Path_Flag s_emPathFlag = em_Path_Flag_Top;
	
	if (!bInit)
	{
		s_emPathFlag = g_pAccountGame->MyAcount_Info.emPathFlag;
		bInit = true;
		return TRUE;
	}

	em_Camp emEnemyCamp = PERSON->GetEnemyCamp();
	auto pLeftCrystal = CGameRes::GetCrystalTurretArray(emEnemyCamp, em_Path_Flag_Top);
	auto pCenterCrystal = CGameRes::GetCrystalTurretArray(emEnemyCamp, em_Path_Flag_Middle);
	auto pRightCrystal = CGameRes::GetCrystalTurretArray(emEnemyCamp, em_Path_Flag_Buttom);
	if (pLeftCrystal == NULL || pCenterCrystal == NULL || pRightCrystal == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"�����ܰ�!!!!!!! ��ô���Ҳ���ˮ������Դ��?");
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		return FALSE;
	}

	CMonster LeftCrystalTurret; // ��·ˮ����
	CMonster CenterCrystalTurret; // ��·ˮ����
	CMonster RightCrystalTurret; // ��·ˮ����

	// �����·��������. ȥ��·
	FindMonster_For_Name(pLeftCrystal->wszTurretName, 1000.0f, LeftCrystalTurret);
	FindMonster_For_Name(pCenterCrystal->wszTurretName, 1000.0f, CenterCrystalTurret);
	FindMonster_For_Name(pRightCrystal->wszTurretName, 1000.0f, RightCrystalTurret);

	// �����·ˮ����������, ��ȥ����·, ��·Ҳľ�о�ȥ��·
	if (CenterCrystalTurret.GetBase() == NULL)
	{
		if (LeftCrystalTurret.GetBase() != NULL)
		{
			s_emPathFlag = em_Path_Flag_Top;
		}
		else if (RightCrystalTurret.GetBase() != NULL)
		{
			s_emPathFlag = em_Path_Flag_Buttom;
		}
		else
		{
			s_emPathFlag = em_Path_Flag_Middle;
		}
	}
	else
	{
		s_emPathFlag = em_Path_Flag_Middle;
	}

	if (s_emPathFlag != g_pAccountGame->MyAcount_Info.emPathFlag)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"·���Ѿ��ı���!");
		BackHome();
		g_pAccountGame->MyAcount_Info.emPathFlag = s_emPathFlag;
	}

	return TRUE;
}

BOOL CFightFunction::HandleBackHome()
{
	// ���ش��Ҷ�������»س�Ҳ����Ѫ, �ɴ�����
	if (g_MapType == MapType_������Ԩ)
		return FALSE;

	if (IsBackHome())
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"HandleBackHome");
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"��Ҫ�س�!");
		// �ܵ�����
		//MoveToTurret(0, g_pAccountGame->MyAcount_Info.emPathFlag);

		// �ܵ����²ŷ���Ѫ���ع����˲��ûس���!
		//if (PERSON->GetLevel() < 10 && !IsBackHome())
			//return FALSE;

		BackHome();
		if (IsKillBlueBuff() && ExistMiddleCrystalTurret())
			return KillBlueBuff();
		else if (IsKillRedBuff() && ExistMiddleCrystalTurret())
			return KillRedBuff();
		return TRUE;
	}
	return FALSE;
}

BOOL CFightFunction::PartyStart()
{
	auto fn_WaitToLoad = []
	{
		while (PERSON->GetBase() == NULL && GameStatus_Run)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"��ͼ�С���");
			CGameSend::Sleep(5 * 1000);
		}

		while (GameStatus_Run && PERSON->GetMoney() == 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"ľ��Ǯ!!����");
			CGameSend::Sleep(1 * 1000);
		}
	};

	auto fn_Test = []
	{
		Point NowPoint = PERSON->GetPoint();
		Point NewPoint = NowPoint;
		NewPoint.X += 300.0f;
		NewPoint.Y += 300.0f;

		while (GameStatus_Run)
		{
			CGameSend::Move(NewPoint);
			Sleep(5000);
			CGameSend::Move(NowPoint);
			Sleep(5000);
		}
	};

	g_dwMainThreadId = CLThread::GetMainThreadId();
	fn_WaitToLoad();
	::CreateMutexW(NULL, FALSE, Mutex_GameALive_Name);
	SetGamePath();
	InitAccountGame();
	Sleep(5 * 1000);

	//fn_Test();

	CSkill::LevelUpSkill();
	CEqument::SetEqumentIndex(g_pAccountGame->MyAcount_Info.emHeroPro);
	
	CEqument::BuyEqument();
	CSkill::InitNormalSkill();
	WaitToStartGame();
	CGameSend::LockView();
	SetMatchingPath();
	SetGamePath();
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"PartyStart");
	while (GameStatus_Run)
	{
		// ������������
		//CGameSend::Sleep(50);
		if (!ExistMiddleCrystalTurret())
			SwitchPath();

		if (EMPTY_PERSONHP)
		{
			if (PERSON->GetLevel() >= 13 && CEqument::ExistItem(0xD11))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"���ϸ���!");
				CSkill::UseItemSkill(L"OdinTrinketRevive");
			}
			while (GameStatus_Run && EMPTY_PERSONHP)
			{
				CGameSend::Sleep(1000);
			}

			CPrintLog::PrintLog_W(_SELF, __LINE__, L"���︴����!");
			CEqument::BuyEqument();

			// ɱBUFF
			if (IsKillBlueBuff() && ExistMiddleCrystalTurret())
				KillBlueBuff();
			else if (IsKillRedBuff() && ExistMiddleCrystalTurret())
				KillRedBuff();

		}

		// ɱ��
		if (FALSE && ExistMiddleCrystalTurret() && IsKillDragon())
		{
			KillDragon();
			continue;
		}

		// �سǴ���
		if (HandleBackHome())
			continue;

		// ���Ĵ���
		if (HandleTurret())
			continue;

		// Ӣ�۵Ĵ���
		if (HandleHero())
			continue;

		// С���Ĵ���
		if (HandleSolider())
			continue;

		// ��·�Ĵ���
		//if (!bStopMoveToEnd)
		MoveToEndPoint();

	}

	CPrintLog::PrintLog_W(_SELF,__LINE__,L"����!");
	g_pAccountGame->MyAcount_Info.bClose = TRUE;

	return TRUE;
}

BOOL CFightFunction::TestAAA()
{
	g_dwMainThreadId = CLThread::GetMainThreadId();
	InitAccountGame();

	static vector<CMonster> vMonsterList;
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, em_Camp_Neutral, 20.0f, NULL);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"vMonsterList.size()=%d", vMonsterList.size());
	if (vMonsterList.size() == 0)
		return FALSE;

	CMonster Solider = vMonsterList.at(0);
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"Name:%s,dis=%.2f", Solider.GetName(), Solider.GetDis(PERSON->GetPoint()));
	Solider.NormalAttack();
	Sleep(1000);
	g_dwStartGameCmd = GameCmd_Stop;
	return TRUE;
}