/*****************************************************************************************

***		MD5About.h: interface for the CAboutDlg class.		

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#if !defined(AFX_MD5ABOUT_H__2BC7928E_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_)
#define AFX_MD5ABOUT_H__2BC7928E_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "stdafx.h"
#include "resource.h"

/*****************************************************************************************
CLASS:			CAboutDlg
DESCRIPTION:	Implements the Help-About dialog.	
NOTES:			None
*****************************************************************************************/
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static const CString c_strCopyright;	//text for copyright notice
};

#endif	//!defined(AFX_MD5ABOUT_H__2BC7928E_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_)



