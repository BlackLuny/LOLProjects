#ifndef __GAMEBASE_H__
#define __GAMEBASE_H__
#include "GameStruct.h"
#include "../../MyTools/MyTools/CLVector.h"
/************************************************************************/
/* ȫ�ֱ���                                                                     */
/************************************************************************/
extern HANDLE						g_hFileMap;								//�ڴ湲��ľ��
extern HWND							g_hConsoleHwnd;							//����̨�ľ��
extern PSHARED_INFO					g_pSharedInfo;							//ӳ�䵽�������ڴ�ĵ�ַ
extern PACCOUNT_INFO_GAME			g_pAccountGame;							//������ʺ���Ϣ
extern DWORD						g_dwStartGameCmd;						//�һ���ָ��
extern vector<MYHOOK_CONTENT>		g_HookItemList;							//inline Hook�б�
extern BOOL							g_bStartHook;							// ��ʼ����Hook�����Զ��庯��
extern DWORD						g_dwStudySkillIndex;					// ѧϰ���ܵ�����
extern DWORD						g_dwBuyItemId;							// ������Ʒ��Id
extern BOOL							g_bMove;								// �Ƿ���·
extern Point						g_MovePoint;							// ��·������
extern DWORD						g_dwNormalAttackBase;					// ��ͨ������Obj
extern Point						g_AttackPoint;							// ����������
extern _Hook_UseSkillInfo			g_Hook_UseSkillInfo;					// ʹ�ü��ܲ���
extern BOOL							g_bUseSkill;							// ʹ�ü���
extern DWORD						g_dwHookSimKeyAddr;						// ���Ұ���
extern DWORD						g_dwHookSimKeyCALL;						// ͬ��
extern DWORD						g_dwMainThreadId;						// ���߳�Id
extern BOOL							g_bStopAction;							// �Ƿ�ֹͣ����
extern BOOL							g_bfnMove;								// �Ƿ��������·
extern DWORD						g_dwForgeESP;							// α��Ķ�ջ
extern DWORD						g_dwTimerId;							// ʱ�ӵ�Id
extern DWORD						g_dwUseItemParm;						// ʹ����Ʒ��ECX
extern float						g_fMaxDis_Attack;						// ��󹥻���Χ
extern DWORD						g_dwHookD3dAddr;						// HookD3d
extern DWORD						g_dwHookD3dValue;						// ԭֵ
extern DWORD						g_dwFakeStackFrame[MAX_PATH];			// α���ջ
extern BOOL							g_bGameStart;							// �Ƿ���Ϸ��ʼ��
extern CLVector<GameTalkMsg>		g_vGameTalkMsg;							// ������Ϣ
extern DWORD						g_dwOccupyObjBase;						// ͳ��ռ������Base
extern MapType						g_MapType;								// ��ǰ�ĵ�ͼ����
#endif//__GAMEBASE_H__