// HelloMfc2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HelloMfc2.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only CHelloMfc2App object

CHelloMfc2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CHelloMfc2App initialization

BOOL CHelloMfc2App::InitInstance()
{
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	pFrame->Create(NULL, _T("Hello"));
	pFrame->ShowWindow(SW_SHOW);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
