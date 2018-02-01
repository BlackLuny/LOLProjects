#ifndef __GAMEFUNCTION_H__
#define __GAMEFUNCTION_H__

#include "GameSend.h"
#include "Monster.h"
#include "Skill.h"
#include <memory>
#include "GameRes.h"
#include "Equment.h"
#include "Field.h"
#define EMPTY_PERSONHP (PERSON->GetHp() == NULL)

class CGameFunction
{
public:
	CGameFunction();
	~CGameFunction();
public:
	BOOL InitAccountGame();

	BOOL InitTimer();

	BOOL TerminateTestThread();

	///////Move///////////////////////////////////////////////////////////////////
	// ��·,һֱ�ߵ��յ�����Ϊֹ
	BOOL MoveToPoint(Point& EndPoint, float fDis);

	// ������
	BOOL BackOff();

	// �ߵ��յ�
	BOOL MoveEnd();

	BOOL MoveToEndPoint();

	BOOL MoveToEnemyTurrent();

	// �жϾ�����ʹ�ü���
	BOOL UseSkill_By_Dis(em_Skill_Flag emSkillFlag, CMonster& Obj);

	/////////Other Function/////////////////////////////////////////////////////////////////
	// �Ƿ���Ϸ������
	BOOL IsGameOver();

	// �ǲ��Ǹûس���
	BOOL IsBackHome();

	// �ؼҲ��� or ����״̬
	BOOL BackHome();

	// �ָ�����ֵ��ȥ
	BOOL Rest();

	// �ȴ���Ϸ��ʼ
	BOOL WaitToStartGame();

	// ѧϰ���ܴ���
	BOOL HandleSkillLevelUp();

	////////////AroundObject//////////////////////////////////////////////////////////////
	UINT GetMonsterList_By_Type(vector<CMonster>& vMonsterList, em_MonsterType emType, em_Camp emCamp, float fDis, Point* pFixPoint);

	// Ѱ�ҹ���
	BOOL FindMonster_For_Name(__in LPCWSTR pwszName, __in float fDis,__out CMonster& Monster);

	// ����ƶѪ������
	int GetAnemiaIndex(vector<CMonster>& vMonsterList);

	//////////Soilder////////////////////////////////////////////////////////////////
	// ��ȡ�����ĵж�С������
	UINT GetAroundEnemySoliderCount(float fDis, Point* pFixPoint);

	// ��ȡ������ͬ��С������
	UINT GetAroundAllianceSoliderCount(float fDis, Point* pFixPoint);

	// ��ȡҪ������С��
	BOOL GetRecentlyEnemySolider(float fDis, Point* pFixPoint, CMonster& Monster);

	// �ж��Ƿ�Ҫ���
	BOOL IsClearSolider();

	// �ж��Ƿ���Ҫ���С��
	BOOL IsDodgeSolider();

	// ��ȡ�����Լ���С������
	UINT GetAttackSelfSoliderCount(float fDis, Point* pFixPoint);

	// ���ܵз�С���Ĺ���
	BOOL DodgeEnemySolider();

	// ͳ��ս�� ��ȡ���������Լ������������
	UINT GetAroundEnemyGuardian(vector<CMonster>& vlst, float fDis, Point* pFixPoint);

	// ռ����
	BOOL OccupyEnemyTurret();

	///////Hero///////////////////////////////////////////////////////////////////
	// ��ȡ�����ĵж�Ӣ�۵�����
	UINT GetAroundEnemyHeroCount(float fDis, Point* pFixPoint);

	// ��ȡ������ͬ��Ӣ�۵�����
	UINT GetAroundAllianceHeroCount(float fDis, Point* pFixPoint);

	// ��ȡҪ������Ӣ��
	BOOL GetRecentlyEnemyHero(float fDis, Point* pFixPoint, CMonster& Monster);

	// �Ƿ����ܵз�Ӣ�۵Ĺ���
	BOOL IsDodgeHero();

	// ��ȡ�����Լ���Ӣ������
	UINT GetAttackSelfHeroCount(float fDis, Point* pFixPoint);

	// �Ƿ����������з�Ӣ��
	BOOL IsAttackHero();

	// �Ƿ����ĵз�Ӣ�۵�Ѫ��
	BOOL IsCoolConsumeSkill(DWORD dwLevel);
	BOOL IsConsumeEnemyHero();

	// �жϷ�Χ��,  �ж��ٸ����Ѵﵽx������
	UINT GetAroundAllianceHeroCount_By_Level(float fDis, DWORD dwLevel);

	///////////Turret///////////////////////////////////////////////////////////////
	// ��ȡ�����������
	Point GetRecentlyTurret(int nIndex, em_Path_Flag emPathFlag);

	// �ƶ������������
	BOOL MoveToTurret(int nIndex, em_Path_Flag emPathFlag);

	// ��ȡ�����ж���Ӫ����������
	BOOL GetAroundEnemyTurretCount(float fDis, Point* pFixPoint);

	// ��ȡҪ��������
	BOOL GetRecentlyEnemyTurret(float fDis, Point* pFixPoint, CMonster& Monster);

	// ��ȡ�Լ�����ѷ�����
	BOOL GetRecentlyAllianceTurret(float fDis, Point* pFixPoint, CMonster& Turret);

	// �Ƿ�����
	BOOL IsAttackTurret();

	// �Ƿ������·ˮ����
	BOOL ExistMiddleCrystalTurret();

	// ƥ���·
	BOOL SetMatchingPath();

	//
	BOOL SetGamePath();

public:

	HANDLE hGuardPersonThread;
public:

};

#endif//__GAMEFUNCTION_H__