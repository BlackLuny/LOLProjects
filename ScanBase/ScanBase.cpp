// ScanBase.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ScanBase.h"
#include "Game.h"

CGame * pGame = NULL;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CScanBaseApp

BEGIN_MESSAGE_MAP(CScanBaseApp, CWinApp)
END_MESSAGE_MAP()


// CScanBaseApp ����

CScanBaseApp::CScanBaseApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CScanBaseApp ����

CScanBaseApp theApp;


// CScanBaseApp ��ʼ��
DWORD WINAPI ShowThread(LPVOID lpParm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( pGame == NULL )
	{
		pGame = new CGame;
		pGame->DoModal();
		delete pGame;
	}
	return 0;
}

BOOL CScanBaseApp::InitInstance()
{
	CWinApp::InitInstance();
	::CloseHandle(::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ShowThread,NULL,NULL,NULL));
	return TRUE;
}

__declspec(dllexport) int WINAPI BBB(DWORD dwID)
{
	return 0xF;
}