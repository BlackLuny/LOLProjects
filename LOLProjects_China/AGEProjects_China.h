// AGEProjects_China.h : AGEProjects_China DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAGEProjects_ChinaApp
// �йش���ʵ�ֵ���Ϣ������� AGEProjects_China.cpp
//

class CAGEProjects_ChinaApp : public CWinApp
{
public:
	CAGEProjects_ChinaApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
