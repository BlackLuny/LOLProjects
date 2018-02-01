#ifndef __LOL_CONSOLE_CONSOLECONFIG_H__
#define __LOL_CONSOLE_CONSOLECONFIG_H__

#include "VarBase.h"

class CConsoleConfig
{
public:
	CConsoleConfig();
	~CConsoleConfig();

	// ս������
	static BOOL WriteFightCount(DWORD dwCount);

	// ս������
	static DWORD GetFightCount();

	// ��Ӯ��־
	static BOOL WriteGameResult();

	// �����ͼ
	static BOOL SaveGameMap(LPCWSTR pwszMapName);

	// ����ս����ͼ
	static BOOL SetGameMap(LPCWSTR wszMapName);

	// �������Ʒ��ص�ͼType
	static BOOL GetGameMap_By_Name(LPCWSTR pwszMapName, MapType& Mt);

	// �޸�Hosts�ļ�
	static BOOL UpdateHosts();

	// ��ȡ��Ϸ·��
	static BOOL GetTGPPath_By_Reg(LPWSTR pwszTGPPath); 

	// ��ȡTGP·��
	static BOOL GetGamePath_By_Process(LPWSTR pwszGamePath);
private:

};



#endif