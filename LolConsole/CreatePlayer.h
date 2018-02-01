#ifndef __LOL_CONSOLE_CREATEPLAYER_H__
#define __LOL_CONSOLE_CREATEPLAYER_H__
#include "SearchPic.h"

class CCreatePlayer
{
public:
	CCreatePlayer();
	~CCreatePlayer();

	static BOOL GetRandName(LPWSTR pwszText);

	BOOL ClickPoint(int x, int y);

	BOOL CreatePlayer();

	// ������ɫ���, д������
	BOOL WriteCreatePlayerConfig(BOOL bCreate = TRUE);

	// �Ƿ���Ҫ������ɫ
	BOOL IsCreate();

	/////ͼɫ�ж�/////////////////////////////////////////////////////////////////////

	// �ж��Ƿ񴴽���ɫ�������
	BOOL IsCreatePlayerSucc();

	// �ж��Ƿ�����¼����ҳ
	BOOL IsLauncherMainPage();

private:
	CBmpFind m_BmpFind;
	RECT m_LauncherRect;
};



#endif// __LOL_CONSOLE_CREATEPLAYER_H__