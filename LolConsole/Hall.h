#ifndef __LOL_HALL_H__
#define __LOL_HALL_H__

#include "SearchPic.h"
#include "CrackCaptchaAPI.h"
#include "ConsoleConfig.h"
#include "VerClientSocket.h"

typedef enum _em_GameStatus
{
	em_GameStatus_Load,
	em_GameStatus_Start,
	em_GameStatus_Faild
}em_GameStatus;

typedef enum _GameServerType
{
	GameServerType_Telecom,	// ����
	GameServerType_Netcom,	// ��ͨ
	GameServerType_Other	// ����
}GameServerType;

typedef struct _ServerRect
{
	GameServerType ServerType;
	int x;
	int y;
}ServerRect;

class CHall
{
public:
	CHall(CVerClientSocket* pCVerClientSocket);
	~CHall();

	// ��ʼ��
	BOOL Init();

	//
	BOOL GetHeroBmpName(__in em_Hero_Pro emHero, __out LPWSTR pwszText);

	// ����
	BOOL PraiseTeamMember(RECT& LauncherRect);

	//
	BOOL HandleCloseGameMutex(CBmpFind& dm, RECT& LauncherRect);

	//
	BOOL HandleGameDisappear();

	BOOL HandleBugReport();

	BOOL HandleTV();

	BOOL IsQueuing();

	BOOL HandleQT();

	// �ӳ��Ĵ���
	BOOL CaptainHanle();

	// ��Ա�Ĵ���
	BOOL MemberHandle();

	// �������
	BOOL ClickPoint(int x, int y);

	// 
	BOOL ClearTaskMenu();

	// ����TGP
	BOOL RunTGP();

	BOOL TGPLogin();

	BOOL RunLolClient();

	em_GameStatus WaitToGameStart();

	BOOL IsSealUser();

	BOOL IsLoseCon();

public:
	CVerClientSocket* m_pCVerClientSocket;
private:
	BOOL bGameStart;
	DWORD dwStartGameTick;
	
};



#endif// __LOL_HALL_H__