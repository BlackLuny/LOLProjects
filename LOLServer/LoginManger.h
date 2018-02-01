#ifndef __LOL_LOGINMANGER_H__
#define __LOL_LOGINMANGER_H__

#include "DataBaseModel.h"

typedef struct _LoginGameUser
{
	WCHAR wszIp[16];			// ���ڵ�IP
	WCHAR wszLoginName[32];		// �ʺ�

	DWORD dwUseCount;			// ʹ������,ÿ20���ж�һ�� NowTick - dwTick ����< 1����
	ULONGLONG dwTickCount;		// ��ʹ��������0ʱ,���Ҳ����������0

	DWORD dwVerCode;			// ����Key
	ULONGLONG dwVerTick;		// У��ʱ��,�������޸�ʱ,������0

	ULONGLONG dwSurplusHour;	// ����ʣ��ʱ��Hour
	ULONGLONG dwSurplusTick;	// ����ʣ��ʱ��Tick

	DWORD dwLoginTick;			// У��ĵ�¼ʱ��

	CardStruct Card;			// ����
}LoginGameUser;

class CLoginManger
{
public:
	CLoginManger();
	~CLoginManger();

	// �жϿ����Ƿ��Ѿ���ռ����,�жϿ����������Ƿ�һ��
	static LoginGameUser* _ExistCard(CardStruct & Card);

	// �ж��Ƿ���ʣ��Ŀ���ʱ��
	static BOOL _EnoughTime(LoginGameUser* LgGameUser);

	// �ж��Ƿ��ڵ�¼ʱ�䷶Χ��
	static BOOL _EnoughLoginTime(LoginGameUser* LgGameUser);

	// �ж�У��ʱ���Ƿ������
	static BOOL _OverdueVer(LoginGameUser* LgGameUser);

	// �ж�IP�Ƿ�һ��
	static BOOL _SameIp(LoginGameUser* LgGameUser, LPCWSTR pwszIp);

	// �жϴ����Ƿ�ʱ
	static BOOL _TimeOut_ByCount(LoginGameUser* LgGameUser);

	// У�鿨�����
	static BOOL VerCard(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg);

	// �������ɽṹ
	static LoginGameUser* InitLoginUser(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg);

	// �������ŵ��ڴ�
	static BOOL AddNewCard(CardStruct& Card, int nTime, DWORD dwToken, LPCWSTR pwszIp);

	static DWORD _CheckCard(LPCWSTR pwszCard);

	static BOOL _InitCard(vector<CardInfo>& vlst);

	static DWORD _GetCardTime(LPCWSTR pwszCardNo, DWORD dwToken, LPCWSTR pwszIp);
public:
	static vector<LoginGameUser> LgUserLst;
};

#endif// __LOL_LOGINMANGER_H__