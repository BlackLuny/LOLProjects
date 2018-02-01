#include "stdafx.h"
#include "GameRes.h"

#define _SELF L"GameRes.cpp"

CGameRes::CGameRes()
{
}

CGameRes::~CGameRes()
{
}

Point CGameRes::GetBlueBuffPoint(em_Camp emCamp)
{
	if (emCamp == em_Camp_Blue)
		return Point(3548.00f, 8206.00f, 51.19f);

	return Point(11402.00f, 6532.00f, 51.70f);
}

Point CGameRes::GetRedBuffPoint(em_Camp emCamp)
{
	if (emCamp == em_Camp_Blue)
		return Point(7488.00f, 3716.00f, 52.00f);

	return Point(7312.00f, 11140.00f, 55.15f);
}

Point CGameRes::GetDragonPoint()
{
	return Point(10186.00f, 4882.00f, -71.24f);
}

Point CGameRes::GetSpringPoint(em_Camp emCamp)
{
	if (g_MapType == MapType_�ٻ�ʦϿ��)
	{
		if (emCamp == em_Camp_Blue)
			return Point(105.93f, 134.49f, 33.34f);
		else if (emCamp == em_Camp_Red)
			return Point(14576.36f, 14693.83f, 466.96f);
	}
	else if (g_MapType == MapType_ˮ��֮��)
	{
		if (emCamp == em_Camp_Blue)
			return Point(577.00f, 4085.00f, -34.82f);
		else if (emCamp == em_Camp_Red)
			return Point(13247.00f, 4101.00f, -37.15f);
	}
	else if (g_MapType == MapType_������Ԩ)
	{
		if (emCamp == em_Camp_Blue)
			return Point(1331.00f, 1455.00f, -131.68f);
		else if (emCamp == em_Camp_Red)
			return Point(11850.09f, 11596.16f, -132.18f);
	}

	return Point();
}

LPCWSTR CGameRes::GetSpringName(em_Camp emCamp)
{
	if (emCamp == em_Camp_Blue)
	{
		static WCHAR wszBlueSpringName[32] = { L"Turret_OrderTurretShrine_A" };
		return wszBlueSpringName;
	}
	static WCHAR wszRedSpringName[32] = { L"Turret_ChaosTurretShrine_A" };
	return wszRedSpringName;
}

em_Skill_Flag CGameRes::FindSkillPoint_For_Hero(em_Hero_Pro emHero, DWORD dwLevel)
{
	auto fn_StudySkill_Q = [](DWORD dwLevel)
	{
		switch (dwLevel)
		{
		case 1: case 4: case 8: case 12: case 15:
			return em_Skill_Flag_Q;
		case 2: case 5: case 9: case 13: case 17:
			return em_Skill_Flag_W;
		case 3: case 7: case 10: case 14: case 18:
			return em_Skill_Flag_E;
		case 6: case 11: case 16:
			return em_Skill_Flag_R;
		default:
			break;
		}
		return em_Skill_Flag_Q;
	};

	auto fn_StudySkill_W = [](DWORD dwLevel)
	{
		switch (dwLevel)
		{
		case 2: case 5: case 9: case 13: case 17:
			return em_Skill_Flag_Q;
		case 1: case 4: case 8: case 12: case 15:
			return em_Skill_Flag_W;
		case 3: case 7: case 10: case 14: case 18:
			return em_Skill_Flag_E;
		case 6: case 11: case 16:
			return em_Skill_Flag_R;
		default:
			break;
		}
		return em_Skill_Flag_Q;
	};

	auto fn_StudySkill_E = [](DWORD dwLevel)
	{
		switch (dwLevel)
		{
		case 3: case 4: case 5: case 7: case 9:
			return em_Skill_Flag_Q;
		case 2: case 8: case 12: case 14: case 17:
			return em_Skill_Flag_W;
		case 1: case 10: case 13: case 15: case 18:
			return em_Skill_Flag_E;
		case 6: case 11: case 16:
			return em_Skill_Flag_R;
		default:
			break;
		}
		return em_Skill_Flag_Q;
	};

	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Garen: case em_Hero_Pro_MasterYi:  case em_Hero_Pro_Nunu: case em_Hero_Pro_Veigar: case em_Hero_Pro_Lucian:
	case em_Hero_Pro_Ezreal: case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Swain: case em_Hero_Pro_Graves:case em_Hero_Pro_Chogath:
	case em_Hero_Pro_Trundle: case em_Hero_Pro_Syndra: case em_Hero_Pro_Taric: case em_Hero_Pro_Rumble: case em_Hero_Pro_Diana:
	case em_Hero_Pro_Caitlyn: case em_Hero_Pro_Ahri: case em_Hero_Pro_Annie:
		return fn_StudySkill_Q(dwLevel);
	case em_Hero_Pro_MissFortune: case em_Hero_Pro_Ryze: case em_Hero_Pro_Malzahar: case em_Hero_Pro_Maokai:  case em_Hero_Pro_Tristana:
	case em_Hero_Pro_Galio: case em_Hero_Pro_Aatrox: case em_Hero_Pro_Urgot: case em_Hero_Pro_Teemo:
		return fn_StudySkill_E(dwLevel);
	case em_Hero_Pro_Vayne: case em_Hero_Pro_Ashe: case em_Hero_Pro_Sejuani:
		return fn_StudySkill_W(dwLevel);
	default:
		return fn_StudySkill_Q(dwLevel);
		break;
	}

	// ȫ��Ĭ�ϼӵ�һ��
	return em_Skill_Flag_Q;
}

ResItem* CGameRes::GetEuqmentArray(em_Hero_Pro emHero, int& nMaxCount)
{
	auto fn_BuyItem_Ryze = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x403, 350 },// ��ˮ��
			{ 0xBFE, 400 },// Ů��֮��
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0xBC2, 1200 },// �߻���ʯ
			{ 0xC27, 800 },// ˮ��֮ѥ
			{ 0x532, 450 },// ��ħ����
			{ 0xBD3, 1800 },// ʱ��֮��
			{ 0x805, 1200 },// ���ݶ���
			{ 0xDB8, 1500 },// ������
			{ 0x3F3, 1000 },// ��������
			{ 0xC2C, 2200 },// �����ı�������
			{ 0xBBB, 2350 },// ����ʹ֮��
			{ 0xC26, 2800 }// ��˪֮��
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_BuyItem_Master = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x403, 350 },// ��ˮ��
			{ 0xBFE, 400 },// Ů��֮��
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0xC27, 800 },// ˮ��֮ѥ
			{ 0x532, 450 },// ��ħ����
			{ 0xBC2, 1200 },// �߻���ʯ
			{ 0xBD3, 1800 },// ʱ��֮��
			{ 0x805, 1200 },// ���ݶ���
			{ 0xDB8, 1500 },// ������
			{ 0x3F3, 1000 },// ��������
			{ 0xC2C, 2200 },// �����ı�������
			{ 0xBBB, 2350 },// ����ʹ֮��
			{ 0xC26, 2800 }// ��˪֮��
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_Equment_Melee = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0xBBE, 800 },// ��սʿ�ּ�
			{ 0x514, 450 },// ��ħ����
			{ 0x40E, 1300 },// ����֮��
			{ 0xC00, 2400 },// ��Ѫ��
			{ 0x40E, 1300 },// ����֮��
			{ 0xBD7, 2300 }, // �޾�֮��
			{ 0xC0E, 1300 },// ����
			{ 0xBE6, 1500 },// ��Ӱ֮��
			{ 0x805, 1200 },// ���ݶ���
			{ 0xDB8, 1500 },// ������
			{ 0xC1E, 2900 }// Ů����ɴ
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_BuyItem_Garen = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x404, 400 },// ��ˮ��
			{ 0xEA7, 700 },// �߱ȵ�����
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0x407, 800 },// ���Ӽ�
			{ 0xBFC, 800 },// ���׶���
			{ 0xC27, 800 },// ˮ��֮ѥ
			{ 0x532, 450 },// ��ħ����
			{ 0x805, 1200 },// ���ݶ���
			{ 0xDB8, 1500 },// ������
			{ 0xBFB, 800 },// ȼ�ձ�ʯ
			{ 0xC8B, 1100 },// �Ļ궷��
			{ 0xBF9, 900 },// ��ܿ���
			{ 0xC1E, 2900 },// Ů����ɴ
			{ 0x40E, 1300 },// ����֮��
			{ 0xC00, 2400 },// ��Ѫ��
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_BuyItem_Master_Fight = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x403, 350 },// ��ˮ��
			{ 0xC01, 400 },// Ů��֮��
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0xC27, 800 },// ˮ��֮ѥ
			{ 0x532, 450 },// ��ħ����
			{ 0x3F3, 1000 },// ��������
			{ 0xC2C, 2200 },// �����ı�������
			{ 0xC1E, 2900 },// Ů����ɴ
			{ 0xBBF, 2350 },// ����ʹ֮��
			{ 0xC26, 2800 }// ��˪֮��
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_Equment_Melee_Fight = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0xBBE, 800 },// ��սʿ�ּ�
			{ 0x514, 450 },// ��ħ����
			{ 0xC0E, 1200 },// ����
			{ 0xC0F, 1300 },// ��Ӱ֮��
			{ 0xBFB, 800 },// ȼ�ձ�ʯ
			{ 0xC8B, 1100 },// �Ļ궷��
			{ 0xBF9, 900 },// ��ܿ���
			{ 0x41D, 900 },// ��Ѫ�����
			{ 0xC6D, 1500 }, // Ѫɫ֮��
			{ 0xBE4, 1250 }, // ��ʴ
			{ 0xBFF, 2250 } //��ɫ�и���
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_BuyItem_Shield_Fight = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x404, 400 },// ��ˮ��
			{ 0xEA7, 700 },// �߱ȵ�����
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0x407, 800 },// ���Ӽ�
			{ 0xBFC, 800 },// ���׶���
			{ 0xC27, 800 },// ˮ��֮ѥ
			{ 0x532, 450 },// ��ħ����
			{ 0xBFB, 800 },// ȼ�ձ�ʯ
			{ 0xC8B, 1100 },// �Ļ궷��
			{ 0xBF9, 900 },// ��ܿ���
			{ 0x41D, 900 },// ��Ѫ�����
			{ 0xC6D, 1500 }, // Ѫɫ֮��
			{ 0xBE4, 1250}, // ��ʴ
			{ 0xBFF, 2250 } //��ɫ�и���
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_Equment_Master_Scuffle = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x403, 350 },// ��ˮ��
			{ 0xBFE, 400 },// Ů��֮��
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0xBC2, 1200 },// �߻���ʯ
			{ 0xC27, 800 },// ˮ��֮ѥ
			{ 0xBD3, 1800 },// ʱ��֮��
			{ 0x532, 450 },// ��ħ����
			{ 0xBD4, 1000 },// ��гʥ��
			{ 0xC66, 1800 },// �ŵ��ȵ�а��ʥ��
			{ 0xBBB, 2350 },// ����ʹ֮��
			{ 0xC26, 2800 },// ��˪֮��
			{ 0xC2C, 3200 },// �����ı�������
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_Equment_Melee_Scuffle = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0xBBE, 800 },// ��սʿ�ּ�
			{ 0xC0E, 1200 },// ����
			{ 0x40D, 875 },// ʮ�ָ�
			{ 0xC34, 1625 },// �����Ŀ�֮��
			{ 0x514, 450 },// ��ħ����
			{ 0xBE6, 1500 },// ��Ӱ֮��
			{ 0x407, 800 },// ���Ӽ�
			{ 0xE9E, 2000 },// ���ߵİ��
			{ 0x40E, 1300 },// ����֮��
			{ 0xC00, 2400 },// ��Ѫ��
			{ 0xBD7, 3600 }, // �޾�֮��
			
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	auto fn_Equment_Shield_Scuffle = [](__out int& nMaxCount)
	{
		static ResItem ItemArray[] =
		{
			{ 0x404, 400 },// ��ˮ��
			{ 0xEA7, 700 },// �߱ȵ�����
			{ 0x3E9, 300 },// �ٶ�֮ѥ
			{ 0x407, 800 },// ���Ӽ�
			{ 0xBFC, 800 },// ���׶���
			{ 0x409, 450 },// ��ħ����
			{ 0xC27, 350 },// ˮ��֮ѥ
			{ 0xBE4, 1250 }, // ��ʴ
			{ 0xBFF, 2250 }, //��ɫ�и���
			{ 0x528, 450 },// ��ħ����
			{ 0xC8B, 1100 },// �Ļ궷��
			{ 0xBF9, 1700 },// ��ܿ���
			{ 0x407, 800 },// ���Ӽ�
			{ 0xC03, 1550 },// ����֮��
			{ 0xBED, 2700 },// ��˹���˵���ս����
		};
		nMaxCount = sizeof(ItemArray) / sizeof(ItemArray[0]);
		return ItemArray;
	};

	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Ryze: case em_Hero_Pro_Swain:
		if (g_MapType == MapType_�ٻ�ʦϿ��)
			return fn_BuyItem_Ryze(nMaxCount);
		else if (g_MapType == MapType_ˮ��֮��)
			return fn_BuyItem_Master_Fight(nMaxCount);
		else if (g_MapType == MapType_������Ԩ)
			return fn_Equment_Master_Scuffle(nMaxCount);

	case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Ezreal: case em_Hero_Pro_Malzahar: case em_Hero_Pro_Graves: case em_Hero_Pro_Chogath: case em_Hero_Pro_Nunu: case em_Hero_Pro_Maokai:
	case em_Hero_Pro_Veigar: case em_Hero_Pro_Galio: case em_Hero_Pro_Syndra: case em_Hero_Pro_Rumble: case em_Hero_Pro_Diana: case em_Hero_Pro_Corki: case em_Hero_Pro_Kennen: case em_Hero_Pro_Mordekaiser:
	case em_Hero_Pro_Thresh: case em_Hero_Pro_TwistedFate: case em_Hero_Pro_Ahri: case em_Hero_Pro_Annie:
		if (g_MapType == MapType_�ٻ�ʦϿ��)
			return fn_BuyItem_Master(nMaxCount);
		else if (g_MapType == MapType_ˮ��֮��)
			return fn_BuyItem_Master_Fight(nMaxCount);
		else if (g_MapType == MapType_������Ԩ)
			return fn_Equment_Master_Scuffle(nMaxCount);

	case em_Hero_Pro_MissFortune: case em_Hero_Pro_Ashe: case em_Hero_Pro_MasterYi: case em_Hero_Pro_Vayne: case em_Hero_Pro_Tristana: case em_Hero_Pro_Lucian: case em_Hero_Pro_Aatrox:
	case em_Hero_Pro_Urgot: case em_Hero_Pro_Teemo: case em_Hero_Pro_Caitlyn: case em_Hero_Pro_Fiora: case em_Hero_Pro_Sivir: case em_Hero_Pro_KogMaw:
		if (g_MapType == MapType_�ٻ�ʦϿ��)
			return fn_Equment_Melee(nMaxCount);
		else if (g_MapType == MapType_ˮ��֮��)
			return fn_Equment_Melee_Fight(nMaxCount);
		else if (g_MapType == MapType_������Ԩ)
			return fn_Equment_Melee_Scuffle(nMaxCount);

	case em_Hero_Pro_Garen: case em_Hero_Pro_Sejuani: case em_Hero_Pro_Trundle: case em_Hero_Pro_Taric: case em_Hero_Pro_Nautilus: case em_Hero_Pro_RekSai:
		if (g_MapType == MapType_�ٻ�ʦϿ��)
			return fn_BuyItem_Garen(nMaxCount);
		else if (g_MapType == MapType_ˮ��֮��)
			return fn_BuyItem_Shield_Fight(nMaxCount);
		else if (g_MapType == MapType_������Ԩ)
			return fn_Equment_Shield_Scuffle(nMaxCount);

	default:
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"��֧�ָ�Ӣ�۷���,emHeroPro=%X", g_pAccountGame->MyAcount_Info.emHeroPro);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		break;
	}
	return NULL;
}

ResItem* CGameRes::GetNextEqument(em_Hero_Pro emHero, DWORD& dwIndex)
{
	static ResItem* pItemArray = NULL;
	static int		nMaxLen = 0;

	pItemArray = GetEuqmentArray(emHero, nMaxLen);
	if (pItemArray == NULL)
		return NULL;

	// װ����������
	if (dwIndex == nMaxLen)
		return &pItemArray[nMaxLen - 1];

	return &pItemArray[dwIndex];
}

DWORD CGameRes::GetIndex_By_Equment(em_Hero_Pro emHero, vector<DWORD>& vlst)
{
	static ResItem* pItemArray = NULL;
	static int		nMaxLen = 0;

	pItemArray = GetEuqmentArray(emHero, nMaxLen);
	if (pItemArray == NULL)
		return NULL;
	if (vlst.size() == 0)
		return 0;

	for (int i = nMaxLen - 1; i >= 0; --i)
	{
		for (auto& dwEuqmentId : vlst)
		{
			if (dwEuqmentId == pItemArray[i].dwItemId)
			{
				// i == 0 ��ʾʲô����û��
				// i != 0 ��ʾ�ö����Ѿ�����, ����i+1 ��ʾֱ������һ��װ��
				return i + 1;
			}
		}
	}
	return 0;
}

TurretSrt* CGameRes::GetTurretArray()
{
	static TurretSrt TurretArray[] = {
		// ������·
		{ L"Turret_T1_L_03_A", em_Camp_Blue, em_Path_Flag_Top },
		{ L"Turret_T1_L_02_A", em_Camp_Blue, em_Path_Flag_Top },
		{ L"Turret_T1_C_06_A", em_Camp_Blue, em_Path_Flag_Top },

		// ������·
		{ L"Turret_T1_C_05_A", em_Camp_Blue, em_Path_Flag_Middle },
		{ L"Turret_T1_C_04_A", em_Camp_Blue, em_Path_Flag_Middle },
		{ L"Turret_T1_C_03_A", em_Camp_Blue, em_Path_Flag_Middle },

		// ������·
		{ L"Turret_T1_R_03_A", em_Camp_Blue, em_Path_Flag_Buttom },
		{ L"Turret_T1_R_02_A", em_Camp_Blue, em_Path_Flag_Buttom },
		{ L"Turret_T1_C_07_A", em_Camp_Blue, em_Path_Flag_Buttom },


		// �췽��·
		{ L"Turret_T2_L_03_A", em_Camp_Red, em_Path_Flag_Top },
		{ L"Turret_T2_L_02_A", em_Camp_Red, em_Path_Flag_Top },
		{ L"Turret_T2_L_01_A", em_Camp_Red, em_Path_Flag_Top },

		// �췽��·
		{ L"Turret_T2_C_05_A", em_Camp_Red, em_Path_Flag_Middle },
		{ L"Turret_T2_C_04_A", em_Camp_Red, em_Path_Flag_Middle },
		{ L"Turret_T2_C_03_A", em_Camp_Red, em_Path_Flag_Middle },

		// �췽��·
		{ L"Turret_T2_R_03_A", em_Camp_Red, em_Path_Flag_Buttom },
		{ L"Turret_T2_R_02_A", em_Camp_Red, em_Path_Flag_Buttom },
		{ L"Turret_T2_R_01_A", em_Camp_Red, em_Path_Flag_Buttom },
	};
	return TurretArray;
}

TurretSrt* CGameRes::GetCrystalTurretArray(em_Camp emCamp, em_Path_Flag emPathFlag)
{
	static TurretSrt TurretArray[] = {
		// ������·
		{ L"Barracks_T1_L1", em_Camp_Blue, em_Path_Flag_Top },

		// ������·
		{ L"Barracks_T1_C1", em_Camp_Blue, em_Path_Flag_Middle },

		// ������·
		{ L"Barracks_T1_R1", em_Camp_Blue, em_Path_Flag_Buttom },


		// �췽��·
		{ L"Barracks_T2_L1", em_Camp_Red, em_Path_Flag_Top },

		// �췽��·
		{ L"Barracks_T2_C1", em_Camp_Red, em_Path_Flag_Middle },

		// �췽��·
		{ L"Barracks_T2_R1", em_Camp_Red, em_Path_Flag_Buttom }
	};

	for (int i = 0; i < 6; ++i)
	{
		auto pTurretSrt = &TurretArray[i];
		if (pTurretSrt->emCamp == emCamp && pTurretSrt->emPathFlag == emPathFlag)
			return pTurretSrt;
	}

	return NULL;
}

SkillConfig* CGameRes::FindSkillConfig(LPCWSTR pwszName)
{
	const int nMaxLen = 50;
	static SkillConfig SkillConfigList[nMaxLen] =
	{
		// ���˷�ʦ
		{ 0, L"Ryze", em_SkillType_UnDirectional, 7.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Buff_Self, 7.0f, em_SkillType_Directional, 0.0f, em_SkillType_Directional, 6.0f },
		// ��������
		{ 1, L"Garen", em_SkillType_Buff_Self, 2.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 2.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��������
		{ 2, L"Ashe", em_SkillType_Buff_Self, 7.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 100.0f, em_SkillType_UnDirectional, 100.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �ͽ�����
		{ 3, L"MissFortune", em_SkillType_Directional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ̽�ռ�
		{ 4, L"Ezreal", em_SkillType_UnDirectional, 8.0f, em_SkillType_UnDirectional, 8.0f, em_SkillType_UnDirectional, 15.0f, em_SkillType_UnDirectional, 15.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �޼���ʥ
		{ 5, L"MasterYi", em_SkillType_Directional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �����֪
		{ 6, L"Malzahar", em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��տ־�
		{ 7, L"Chogath", em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ѩ����ʿ
		{ 8, L"Nunu", em_SkillType_Directional, 2.0f, em_SkillType_Directional, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ������
		{ 9, L"Heimerdinger", em_SkillType_Self_UnDirectional, 5.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �����ͽ
		{ 10, L"Graves", em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��ʿͳ��
		{ 11, L"Swain", em_SkillType_Directional, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �ڰ�֮Ů
		{ 12, L"Annie", em_SkillType_Directional, 5.0f, em_SkillType_UnDirectional, 3.0f, em_SkillType_Buff_Self, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��ҹ����
		{ 13, L"Vayne", em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// Ť������
		{ 14, L"Maokai", em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_UnDirectional, 10.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// а��С��ʦ
		{ 15, L"Veigar", em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �ݶ�֮ŭ
		{ 16, L"Sejuani", em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��������
		{ 17, L"Tristana", em_SkillType_Buff_Self, 7.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �ڱ�֮��
		{ 18, L"Galio", em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ʥǹ����
		{ 19, L"Lucian", em_SkillType_Directional, 5.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 3.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��ħ֮��
		{ 20, L"Trundle", em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 10.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ����Ԫ��
		{ 21, L"Syndra", em_SkillType_UnDirectional, 5.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ���ὣħ
		{ 22, L"Aatrox", em_SkillType_UnDirectional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��ʯ��ʿ
		{ 23, L"Taric", em_SkillType_Directional, 5.0f, em_SkillType_Buff_Self, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��е����
		{ 24, L"Rumble", em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��Ů��
		{ 25, L"Diana", em_SkillType_UnDirectional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ����֮��
		{ 26, L"Urgot", em_SkillType_UnDirectional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// Ѹ�ݳ��
		{ 27, L"Teemo", em_SkillType_Directional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 3.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// Ƥ��Ů��
		{ 28, L"Caitlyn", em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 20.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// Ӣ��Ͷ����
		{ 29, L"Corki", em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 3.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��˫����
		{ 30, L"Fiora", em_SkillType_Directional, 3.0f, em_SkillType_UnDirectional, 10.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��֮��
		{ 31, L"Kennen", em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ����ڤ��
		{ 32, L"Mordekaiser", em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 3.0f, em_SkillType_Directional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �̩̹
		{ 33, L"Nautilus", em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��նݵ���
		{ 34, L"RekSai", em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 3.0f, em_SkillType_Directional, 20.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ս��Ů��
		{ 35, L"Sivir", em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ����������
		{ 36, L"Thresh", em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 3.0f, em_SkillType_UnDirectional, 3.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ���ƴ�ʦ
		{ 37, L"TwistedFate", em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��Ԩ�޿�
		{ 38, L"KogMaw", em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_UnDirectional, 5.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// ��β����
		{ 39, L"Ahri", em_SkillType_UnDirectional, 5.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 7.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
		// �ڰ�֮Ů
		{ 40, L"Annie", em_SkillType_Directional, 5.0f, em_SkillType_UnDirectional, 2.0f, em_SkillType_Buff_Self, 0.0f, em_SkillType_UnDirectional, 3.0f, em_SkillType_Directional, 7.0f, em_SkillType_Directional, 7.0f },
	};

	auto fn_FindConfig = [&nMaxLen](LPCWSTR pwszText)
	{
		for (int i = 0; i < nMaxLen; ++i)
		{
			if (CCharacter::wstrcmp_my(pwszText, SkillConfigList[i].wszOccName))
			{
				return &SkillConfigList[i];
			}
		}
		return (SkillConfig*)NULL;
	};


	return fn_FindConfig(pwszName);
}

ResPoint* CGameRes::GetResPointArray(em_Path_Flag emPathFlag, __out int& nRetMaxLen)
{
	auto fn_Top_Path_ResPoint = [](__out int& nRetMaxLen)
	{
		static ResPoint ResPointArray[] = {
			{ 0, Point(604.00f, 612.00f, 183.57f), PointType_Normal, em_Camp_Blue },
			{ 1, Point(1154.00f, 3910.00f, 95.75f), PointType_HighTurret, em_Camp_Blue },
			{ 2, Point(1400.00f, 6086.00f, 53.76f), PointType_2Turret, em_Camp_Blue },
			{ 3, Point(1018.00f, 9678.00f, 52.84f), PointType_1Turret, em_Camp_Blue },
			{ 4, Point(1490.00f, 11490.00f, 52.84f), PointType_Normal, em_Camp_Blue },
			{ 5, Point(2224.00f, 12508.00f, 52.84f), PointType_Normal, em_Camp_Blue },
			{ 6, Point(3328.00f, 13268.00f, 52.84f), PointType_Normal, em_Camp_Red },
			{ 7, Point(4828.00f, 13892.00f, 52.84f), PointType_1Turret, em_Camp_Red },
			{ 8, Point(8568.00f, 13390.00f, 52.84f), PointType_2Turret, em_Camp_Red },
			{ 9, Point(10836.00f, 13670.00f, 91.43f), PointType_HighTurret, em_Camp_Red },
			{ 10, Point(14576.36f, 14693.83f, 171.98f), PointType_Normal, em_Camp_Red }
		};
		nRetMaxLen = sizeof(ResPointArray) / sizeof(ResPointArray[0]);
		return ResPointArray;
	};

	auto fn_Middle_Path_ResPoint = [](__out int& nRetMaxLen)
	{
		static ResPoint ResPointArray[] = {
			{ 0, Point(604.00f, 612.00f, 183.57f), PointType_Normal, em_Camp_Blue },
			{ 1, Point(1210.00f, 1240.00f, 95.75f), PointType_Normal, em_Camp_Blue },
			{ 2, Point(2064.00f, 2730.00f, 95.75f), PointType_Normal, em_Camp_Blue },
			{ 3, Point(3350.00f, 3468.00f, 95.75f), PointType_HighTurret, em_Camp_Blue },
			{ 4, Point(4338.00f, 4412.00f, 71.06f), PointType_2Turret, em_Camp_Blue },
			{ 5, Point(5472.00f, 6044.00f, 51.14f), PointType_1Turret, em_Camp_Blue },
			{ 6, Point(6784.00f, 6776.00f, 51.89f), PointType_Normal, em_Camp_Blue },
			{ 7, Point(7434.00f, 7332.00f, 53.84f), PointType_Normal, em_Camp_Blue },
			{ 8, Point(8168.00f, 8032.00f, 53.15f), PointType_Normal, em_Camp_Red },
			{ 9, Point(9154.00f, 9020.00f, 54.01f), PointType_1Turret, em_Camp_Red },
			{ 10, Point(10190.00f, 10432.00f, 52.30f), PointType_2Turret, em_Camp_Red },
			{ 11, Point(11362.00f, 11360.00f, 91.43f), PointType_HighTurret, em_Camp_Red },
			{ 12, Point(12630.00f, 11684.00f, 91.43f), PointType_Normal, em_Camp_Red },
			{ 13, Point(14576.36f, 14693.83f, 171.98f), PointType_Normal, em_Camp_Red }
		};
		nRetMaxLen = sizeof(ResPointArray) / sizeof(ResPointArray[0]);
		return ResPointArray;
	};

	auto fn_Buttom_Path_ResPoint = [](__out int& nRetMaxLen)
	{
		static ResPoint ResPointArray[] = {
			{ 0, Point(604.00f, 612.00f, 183.57f), PointType_Normal, em_Camp_Blue },
			{ 1, Point(1508.00f, 658.00f, 95.75f), PointType_Normal, em_Camp_Blue },
			{ 2, Point(2840.00f, 1200.00f, 95.75f), PointType_Normal, em_Camp_Blue },
			{ 3, Point(3926.00f, 1216.00f, 95.75f), PointType_HighTurret, em_Camp_Blue },
			{ 4, Point(6516.00f, 1388.00f, 49.50f), PointType_2Turret, em_Camp_Blue },
			{ 5, Point(9920.00f, 1310.00f, 51.30f), PointType_1Turret, em_Camp_Blue },
			{ 6, Point(11690.00f, 1590.00f, 50.39f), PointType_Normal, em_Camp_Blue },
			{ 7, Point(12378.00f, 2234.00f, 52.17f), PointType_Normal, em_Camp_Blue },
			{ 8, Point(13446.00f, 3576.00f, 51.37f), PointType_Normal, em_Camp_Red },
			{ 9, Point(13720.00f, 4948.00f, 52.76f), PointType_1Turret, em_Camp_Red },
			{ 10, Point(13402.00f, 8752.00f, 52.31f), PointType_2Turret, em_Camp_Red },
			{ 11, Point(13624.00f, 10906.00f, 91.43f), PointType_HighTurret, em_Camp_Red },
			{ 12, Point(13578.00f, 11752.00f, 91.43f), PointType_Normal, em_Camp_Red },
			{ 13, Point(13976.00f, 13098.00f, 91.43f), PointType_Normal, em_Camp_Red },
			{ 14, Point(14576.36f, 14693.83f, 171.98f), PointType_Normal, em_Camp_Red }
		};
		nRetMaxLen = sizeof(ResPointArray) / sizeof(ResPointArray[0]);
		return ResPointArray;
	};

	auto fn_Path_ResPoint_ˮ��֮�� = [](__out int& nRetMaxLen)
	{
		static ResPoint ResPointArray[] = {
			{ 0, Point(4911.00f, 3121.00f, -188.57f), PointType_Normal, em_Camp_Blue },// ���±�
			{ 1, Point(3377.00f, 7875.00f, -188.57f), PointType_Normal, em_Camp_Blue }, //���Ͻ�
			{ 2, Point(9057.00f, 3137.00f, -188.57f), PointType_Normal, em_Camp_Blue },// ���½�
			{ 3, Point(10625.00f, 8047.00f, -188.57f), PointType_Normal, em_Camp_Blue },// ���Ͻ�
			{ 4, Point(6431.00f, 10273.00f, -188.57f), PointType_Normal, em_Camp_Blue }// ��
		};
		nRetMaxLen = sizeof(ResPointArray) / sizeof(ResPointArray[0]);
		return ResPointArray;
	};

	auto fn_Path_ResPoint_������Ԩ = [](__out int& nRetMaxLen)
	{
		static ResPoint ResPointArray[] = {
			{ 0, Point(1331.00f, 1455.00f, -131.68f), PointType_Normal, em_Camp_Blue },
			{ 1, Point(2747.00f, 2891.00f, -175.78f), PointType_Normal, em_Camp_Blue },
			{ 2, Point(4217.00f, 4263.00f, -179.13f), PointType_Normal, em_Camp_Blue },
			{ 3, Point(5505.00f, 5473.00f, -178.34f), PointType_Normal, em_Camp_Blue },
			{ 4, Point(6603.00f, 6537.00f, -178.34f), PointType_Normal, em_Camp_Blue },
			{ 5, Point(7447.00f, 7237.00f, -178.34f), PointType_Normal, em_Camp_Blue },
			{ 6, Point(8451.00f, 8313.00f, -179.22f), PointType_Normal, em_Camp_Blue },
			{ 7, Point(9453.00f, 9293.00f, -170.55f), PointType_Normal, em_Camp_Blue },
			{ 8, Point(10573.00f, 10377.00f, -177.90f), PointType_Normal, em_Camp_Blue },
			{ 9, Point(11850.09f, 11596.16f, -132.18f), PointType_Normal, em_Camp_Blue }
		};
		nRetMaxLen = sizeof(ResPointArray) / sizeof(ResPointArray[0]);
		return ResPointArray;
	};

	if (g_MapType == MapType_�ٻ�ʦϿ��)
	{
		switch (emPathFlag)
		{
		case em_Path_Flag_Top:
			return fn_Top_Path_ResPoint(nRetMaxLen);
		case em_Path_Flag_Middle:
			return fn_Middle_Path_ResPoint(nRetMaxLen);
		case em_Path_Flag_Buttom:
			return fn_Buttom_Path_ResPoint(nRetMaxLen);
		default:
			break;
		}
	}
	else if (g_MapType == MapType_ˮ��֮��)
	{
		return fn_Path_ResPoint_ˮ��֮��(nRetMaxLen);
	}
	else if (g_MapType == MapType_������Ԩ)
	{
		return fn_Path_ResPoint_������Ԩ(nRetMaxLen);
	}
	return NULL;
}

ResPoint* CGameRes::FindRecentlyResPoint(Point& NowPoint, em_Path_Flag emPathFlag, em_Camp emCamp, BOOL bNextPoint)
{
	auto fn_GetRecentlyIndex = [](ResPoint* pResPointArray,int nMaxLen, Point& NowPoint)
	{
		int nIndex = -1;
		float fMaxDis = 100000.0f;
		for (int i = 0; i < nMaxLen; ++i)
		{
			float fDis = CGameSend::Get2DDistance(&pResPointArray[i].Pit, &NowPoint);
			if (fDis < fMaxDis)
			{
				fMaxDis = fDis;
				nIndex = i;
			}
		}
		return nIndex;
	};

	int nMaxLen = 0;
	ResPoint* pResPointArray = GetResPointArray(emPathFlag, nMaxLen);
	if (pResPointArray == NULL)
		return NULL;

	int nIndex = fn_GetRecentlyIndex(pResPointArray, nMaxLen, NowPoint);
	if (nIndex == -1)
		return NULL;

	if (g_MapType == MapType_�ٻ�ʦϿ�� || g_MapType == MapType_������Ԩ)
	{
		// �����ɫ��
		if (emCamp == em_Camp_Blue)
		{
			if (nIndex == nMaxLen - 1)
				return &pResPointArray[nIndex];
			else
				return &pResPointArray[bNextPoint ? nIndex + 1 : nIndex - 1];
		}

		// �ұߺ�ɫ��
		if (nIndex == 0)
			return &pResPointArray[nIndex];

		return &pResPointArray[bNextPoint ? nIndex - 1 : nIndex + 1];
	}
	else if (g_MapType == MapType_ˮ��֮��)
	{
		// �����ɫ��
		if (emCamp == em_Camp_Blue)
		{
			if (nIndex == nMaxLen - 1)
				return &pResPointArray[nIndex];
			else
				return &pResPointArray[bNextPoint ? nIndex + 1 : nIndex];
		}

		// �ұߺ�ɫ��
		if (nIndex == 0)
			return &pResPointArray[nIndex];

		return &pResPointArray[bNextPoint ? nIndex + 1: nIndex];
	}

	
	return &pResPointArray[bNextPoint ? nIndex - 1 : nIndex + 1];
	/*if (nIndex == 0 || nIndex == nMaxLen - 1)// ͷ�� or β������
		return &pResPointArray[nIndex];
	else if (emCamp == em_Camp_Blue)// ��ߵ���Ҫ -1 ������һ��
		return &pResPointArray[bNextPoint ? nIndex + 1 : nIndex - 1];

	// �ұߵ���Ҫ +1 ��ǰ��һ��
	return &pResPointArray[bNextPoint ? nIndex - 1 : nIndex + 1];*/
}

ResPoint* CGameRes::GetResPoint_By_Index(DWORD dwIndex, em_Camp emCamp)
{
	int nMaxLen = 0;
	ResPoint* pResPointArray = GetResPointArray(em_Path_Flag_Middle, nMaxLen);
	if (pResPointArray == NULL)
		return NULL;

	// 2��ʾ��·��
	if (dwIndex == 2)
		return &pResPointArray[4];

	if (dwIndex >= 3)
	{
		if (emCamp == em_Camp_Blue)
			dwIndex = dwIndex - 1;
		else
			dwIndex = dwIndex - 3;

		return &pResPointArray[dwIndex];
	}

	// ��ɫ�ĵ� ��Ҫ + 2
	if (emCamp == em_Camp_Red)
		dwIndex += 2;

	return &pResPointArray[dwIndex];
}

ResPoint* CGameRes::FindResPoint_For_Turret(em_Camp emCamp, em_Path_Flag emPathFlag, PointType PitType)
{
	int nMaxLen = 0;
	ResPoint* pResPointArray = GetResPointArray(emPathFlag, nMaxLen);
	if (pResPointArray == NULL)
		return NULL;

	for (int i = 0; i < nMaxLen; ++i)
	{
		auto pResPoint = &pResPointArray[i];
		if (pResPoint->emCamp == emCamp && pResPoint->PitType == PitType)
			return pResPoint;
	}

	return NULL;
}