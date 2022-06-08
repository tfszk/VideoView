
// VideoView.h : main header file for the VideoView application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CVideoViewApp:
// See VideoView.cpp for the implementation of this class
//

class CVideoViewApp : public CWinApp
{
public:
	CVideoViewApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVideoViewApp theApp;
