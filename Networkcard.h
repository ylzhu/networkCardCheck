
// MFCListMac.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCListMacApp: 
// �йش����ʵ�֣������ MFCListMac.cpp
//

class CMFCListMacApp : public CWinApp
{
public:
	CMFCListMacApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCListMacApp theApp;