// ScanBase.h : ScanBase DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScanBaseApp
// �йش���ʵ�ֵ���Ϣ������� ScanBase.cpp
//

class CScanBaseApp : public CWinApp
{
public:
	CScanBaseApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
