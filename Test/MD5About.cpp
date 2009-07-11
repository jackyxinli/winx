/*****************************************************************************************

***		MD5About.cpp: Implementation of the CMD5About class. 

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#include "stdafx.h"
#include "MD5About.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Copyright text for help-about dialog
const CString CAboutDlg::c_strCopyright("\
This implementation of the RSA MD5 Algorithm by Langfine Ltd\n\
\n\
This software is derived from the RSA Data Security, Inc.\n\
MD5 Message-Digest Algorithm.\n\
\n\
Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All rights reserved.\n\
\n\
RSA Data Security, Inc. makes no representations concerning either \
the merchantability of this software or the suitability of this \
software for any particular purpose. It is provided ""as is"" \
without express or implied warranty of any kind.\n \
\n\
These notices must be retained in any copies of any part of this \
documentation and/or software.\n\
\n\
Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All \
rights reserved.\n\
\n\n\
This implementation of the RSA MD5 Algorithm by Langfine Ltd\n\
\n\
Langfine makes no representations concerning either \
the merchantability of this software or the suitability of this \
software for any particular purpose. It is provided ""as is"" \
without express or implied warranty of any kind.\n\
\n\
In addition to the above, Langfine make no warranty or assurances regarding the \
accuracy of this implementation of the MD5 checksum algorithm nor any assurances regarding \
its suitability for any purposes.\n\
\n\
This implementation may be used freely provided that Langfine is credited \
in a copyright or similar notices (eg, RSA MD5 Algorithm implemented by Langfine \
Ltd.) and provided that the RSA Data Security notices given above and in the source code \
are complied with.");

							 
/*****************************************************************************************
CONSTRUCTOR:	CAboutDlg
DESCRIPTION:	Initialises member data.
ARGUMENTS:		None
NOTES:			None
*****************************************************************************************/
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

/*****************************************************************************************
FUNCTION:		CAboutDlg::DoDataExchange
DETAILS:		See MFC
DESCRIPTION:	See MFC
RETURNS:		See MFC
ARGUMENTS:		See MFC
NOTES:			See MFC
*****************************************************************************************/
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP

	//display copyright text in rich edit control
	DDX_Text(pDX, IDC_EDIT_RSA_COPYRIGHT, (CString)c_strCopyright);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
