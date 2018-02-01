#ifndef __SKILL_H__
#define __SKILL_H__

#include "GameSend.h"
#include "Monster.h"
#include "GameRes.h"

class CSkill
{
public:
	CSkill();
	~CSkill();
	CSkill(DWORD dwIndex, DWORD dwBase);

	VOID Init(DWORD dwBase);
	DWORD GetBase(){ return dwSkillBase; };

	// Level
	DWORD GetSkillLevel();

	// Name
	LPWSTR GetName();

	// Cool?
	BOOL IsCool();

	DWORD GetUseMp();

	BOOL GetUseStatus();

	// ʹ�ü���
	BOOL UseSkill(CMonster& Monster, em_SkillType emSkillType);

	// ��ʼ������
	static BOOL InitNormalSkill();

	// �жϼ����Ƿ���ȴ���
	static BOOL GetSkillCool(em_Skill_Flag emSkillFlag);

	// ʹ�ü���
	static BOOL UseNormalSkill(em_Skill_Flag emSkillFlag, CMonster& Monster);

	// ��ȡ���ܵ�ʩ������
	static float GetSkillDis(em_Skill_Flag emSkillFlag);

	// ѧϰ����
	static BOOL StudySkill(em_Skill_Flag emSkillFlag);

	// ��������
	static UINT GetSkillList(vector<CSkill>& vSkillList);

	// ��������
	static BOOL LevelUpSkill();

	// ʹ����Ʒ����ByName
	static BOOL UseItemSkill(LPCWSTR pwszName);

	/// ʹ����ͨ����ByName
	static BOOL UseNormalSkill_By_Name(LPCWSTR pwszName, CMonster& Monster, em_SkillType emSkillType);
private:
	DWORD dwSkillBase;
	WCHAR wszName[32];
	DWORD dwKeyIndex;
	float fSkillDis;
public:
	em_SkillType SkillType;
	static vector<CSkill> vNormalSkill;
};



#endif