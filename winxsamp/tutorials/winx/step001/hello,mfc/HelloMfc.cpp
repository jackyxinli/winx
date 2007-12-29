// HelloMfc.cpp : implementation file
//

#include "stdafx.h"
#include "HelloMfc.h"
#include "HelloMfcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CHelloMfcApp theApp;

/////////////////////////////////////////////////////////////////////////////
// class CHelloMfcApp

BOOL CHelloMfcApp::InitInstance()
{
	CHelloMfcDlg dlg;
	dlg.DoModal();
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
