// Plugin.MFCTest.h : Plugin.MFCTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPluginMFCTestApp
// �йش���ʵ�ֵ���Ϣ������� Plugin.MFCTest.cpp
//

class CPluginMFCTestApp : public CWinApp
{
public:
	CPluginMFCTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
