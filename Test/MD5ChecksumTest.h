/*****************************************************************************************

***		MD5ChecksumTest.h : Interface for the CMD5ChecksumTestApp class.

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#if !defined(AFX_MD5CHECKSUMTEST_H__2BC79284_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_)
#define AFX_MD5CHECKSUMTEST_H__2BC79284_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/*****************************************************************************************
CLASS:			CMD5ChecksumTestApp
DESCRIPTION:	Main application class.
NOTES:			The application consists of a single modal dialog which is created via
				"DoModal" in the InitInstance function. 
*****************************************************************************************/
class CMD5ChecksumTestApp : public CWinApp
{
public:
	//{{AFX_VIRTUAL(CMD5ChecksumTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMD5ChecksumTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MD5CHECKSUMTEST_H__2BC79284_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_)
