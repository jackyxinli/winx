// HelloMfc.h : main header file for the HELLOMFC application
//

#if !defined(AFX_HELLOMFC_H__3BAA624D_FF6E_4258_82DB_FF285679F862__INCLUDED_)
#define AFX_HELLOMFC_H__3BAA624D_FF6E_4258_82DB_FF285679F862__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelloMfcApp:
// See HelloMfc.cpp for the implementation of this class
//

class CHelloMfcApp : public CWinApp
{
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelloMfcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOMFC_H__3BAA624D_FF6E_4258_82DB_FF285679F862__INCLUDED_)
