/*****************************************************************************************

***		MD5ChecksumTest.cpp : Implementation of the CMD5ChecksumTestApp class. 

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#include "stdafx.h"
#include "MD5ChecksumTest.h"
#include "MD5ChecksumTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMD5ChecksumTestApp

BEGIN_MESSAGE_MAP(CMD5ChecksumTestApp, CWinApp)
	//{{AFX_MSG_MAP(CMD5ChecksumTestApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


CMD5ChecksumTestApp theApp; //The one and only CMD5ChecksumTestApp object

/*****************************************************************************************
FUNCTION:		InitInstance
DETAILS:		public, overrides CWinApp::InitInstance
DESCRIPTION:	Displays the applications main dialog box via DoModal
RETURNS:		FALSE - to terminate the application
ARGUMENTS:		None
NOTES:			None
*****************************************************************************************/
BOOL CMD5ChecksumTestApp::InitInstance()
{
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//a rich edit control is used in the help-about box
	AfxInitRichEdit( );

	//display the main dialog box
	CMD5ChecksumTestDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	//The dialog has been closed, so return FALSE so that we exit the
	//application, rather than start the application's message pump.
	return FALSE;
}
