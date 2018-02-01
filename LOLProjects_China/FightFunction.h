#ifndef __FIGHTFUNCTION_H__
#define __FIGHTFUNCTION_H__

#include "GameFunction.h"
#include "PersonBuff.h"

class CFightFunction : CGameFunction
{
public:
	CFightFunction();
	~CFightFunction();

	///////С��///////////////////////////////////////////////////////////////////

	BOOL KillSolider_Skill(CMonster& Solider, BOOL bUseQ, BOOL bUseW, BOOL bUseE, BOOL bUseR);

	BOOL KillSolider(CMonster& Solider, BOOL bClearAround);

	///////Ӣ��///////////////////////////////////////////////////////////////////

	// ���˷�ʦ
	BOOL KillHero_Ryze(CMonster& Hero, BOOL bConsume/*�Ƿ�ֻ������һ��Ѫ��*/);

	// ��������
	BOOL KillHero_Ashe(CMonster& Hero, BOOL bConsume);

	// �޼���ʥ
	BOOL KillHero_MasterYi(CMonster& Hero, BOOL bConsume);

	// �ͽ�����
	BOOL KillHero_MissFortune(CMonster& Hero, BOOL bConsume);

	// ��������
	BOOL KillHero_Garen(CMonster& Hero, BOOL bConsume);

	// �����֪
	BOOL KillHero_Malzahar(CMonster& Hero, BOOL bConsume);

	// ̽�ռ�
	BOOL KillHero_Ezreal(CMonster& Hero, BOOL bConsume);

	// ��տ־�
	BOOL KillHero_Chogath(CMonster& Hero, BOOL bConsume);

	// ѩ����ʿ
	BOOL KillHero_Nunu(CMonster& Hero, BOOL bConsume);

	// ������
	BOOL KillHero_Heimerdinger(CMonster& Hero, BOOL bConsume);

	// �����ͽ
	BOOL KillHero_Graves(CMonster& Hero, BOOL bConsume);

	// ��ʿͳ��
	BOOL KillHero_Swain(CMonster& Hero, BOOL bConsume);

	// ��ҹ����
	BOOL KillHero_Vayne(CMonster& Hero, BOOL bConsume);

	// Ť������
	BOOL KillHero_Maokai(CMonster& Hero, BOOL bConsum);

	// а��С��ʦ
	BOOL KillHero_Veigar(CMonster& Hero, BOOL bConsum);

	// �ݶ�֮ŭ
	BOOL KillHero_Sejuani(CMonster& Hero, BOOL bConsum);

	// ��������
	BOOL KillHero_Tristana(CMonster& Hero, BOOL bConsum);

	// �ڱ�֮��
	BOOL KillHero_Galio(CMonster& Hero, BOOL bConsum);

	// ʥǹ����
	BOOL KillHero_Lucian(CMonster& Hero, BOOL bConsum);

	// ��ħ֮��
	BOOL KillHero_Trundle(CMonster& Hero, BOOL bConsum);

	// ����Ԫ��
	BOOL KillHero_Syndra(CMonster& Hero, BOOL bConsum);

	// ���ὣħ
	BOOL KillHero_Aatrox(CMonster& Hero, BOOL bConsum);

	// ��ʯ��ʿ
	BOOL KillHero_Taric(CMonster& Hero, BOOL bConsum);

	// ��е����
	BOOL KillHero_Rumble(CMonster& Hero, BOOL bConsum);

	// ��Ů��
	BOOL KillHero_Diana(CMonster& Hero, BOOL bConsum);

	// ����֮��
	BOOL KillHero_Urgot(CMonster& Hero, BOOL bConsum);

	// Ѹ�ݳ��
	BOOL KillHero_Teemo(CMonster& Hero, BOOL bConsum);

	// Ƥ��Ů��
	BOOL KillHero_Caitlyn(CMonster& Hero, BOOL bConsum);

	// Ӣ��Ͷ����
	BOOL KillHero_Corki(CMonster& Hero, BOOL bConsum);

	// ��˫����
	BOOL KillHero_Fiora(CMonster& Hero, BOOL bConsum);

	// ��֮��
	BOOL KillHero_Kennen(CMonster& Hero, BOOL bConsum);

	// ����ڤ��
	BOOL KillHero_Mordekaiser(CMonster& Hero, BOOL bConsum);

	// �̩̹
	BOOL KillHero_Nautilus(CMonster& Hero, BOOL bConsum);

	// ��նݵ���
	BOOL KillHero_RekSai(CMonster& Hero, BOOL bConsum);

	// ս��Ů��
	BOOL KillHero_Sivir(CMonster& Hero, BOOL bConsum);

	// ����������
	BOOL KillHero_Thresh(CMonster& Hero, BOOL bConsum);

	// ���ƴ�ʦ
	BOOL KillHero_TwistedFate(CMonster& Hero, BOOL bConsum);

	// ��Ԩ�޿�
	BOOL KillHero_KogMaw(CMonster& Hero, BOOL bConsum);

	// ��β����
	BOOL KillHero_Ahri(CMonster& Hero, BOOL bConsum);

	// �ڰ�֮Ů
	BOOL KillHero_Annie(CMonster& Hero, BOOL bConsum);

	BOOL KillHero(CMonster& Hero, BOOL bConsume);

	////////�������߼�//////////////////////////////////////////////////////////////////
	// ����С��
	BOOL AttackSolider(BOOL bClearAround);
	BOOL HitSolider(CMonster& Solider, BOOL bClearAround);

	// ����
	BOOL AttackTurret();
	BOOL HitTurret(CMonster& Turret);
	BOOL IsDodgeTurret();
	BOOL IsDodgeCrystalTurret();
	BOOL DodgeCrystalTurret();
	BOOL DodgeTurret();
	BOOL HandleRegainMove();

	// �ǲ��Ƕ����˶� , Ӧ�ú���!
	BOOL IsEvacuate();
	BOOL IsEvacuate_Crystal();
	BOOL IsEvacuate_Scuffle();
	BOOL Evacuate();

	// ���ĵз�Ӣ�۵�Ѫ��
	BOOL ConsumeEnemyHero();
	// �����з�Ӣ��
	BOOL AttackHero();
	// ʹ�ûָ��Եļ���
	BOOL UseSpecialSkill();	
	// ���ܵз�Ӣ�۵Ĺ���
	BOOL DodgeHero();

	// �ǲ��Ǹ�ȥ������
	BOOL IsKillDragon();
	BOOL KillDragon();

	// �ǲ��Ǹ�ȥ����BUFF��
	BOOL IsKillBlueBuff();
	BOOL KillBlueBuff();

	// �ǲ��Ǹ�ȥ�պ�BUFF��
	BOOL IsKillRedBuff();
	BOOL KillRedBuff();

	// �帽����Ұ��
	BOOL KillAroundSRU(float fDis);

	BOOL HandleBackHome();

	////////��������߼�//////////////////////////////////////////////////////////////////
	BOOL HandleTurret();
	BOOL HandleHero();
	BOOL HandleSolider();
	BOOL SwitchPath();

	// Party��ʼ!
	BOOL PartyStart();

	BOOL TestAAA();
private:
	BOOL bStopMoveToEnd;
	DWORD dwKillDragonTick;
	DWORD dwKillBlueBuffTick;
	DWORD dwKillRedBuffTick;
	BOOL bSwitchPath;
	DWORD dwDragonCount;
};



#endif