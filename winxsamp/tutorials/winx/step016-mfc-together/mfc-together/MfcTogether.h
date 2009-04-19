// MfcTogether.h : main header file for the MFCTOGETHER application
//

#if !defined(AFX_MFCTOGETHER_H__6678F41E_6399_4EA4_9ED3_C4E1C2282AD0__INCLUDED_)
#define AFX_MFCTOGETHER_H__6678F41E_6399_4EA4_9ED3_C4E1C2282AD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherApp:
// See MfcTogether.cpp for the implementation of this class
//

class CMfcTogetherApp : public CWinApp
{
public:
	CMfcTogetherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcTogetherApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMfcTogetherApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCTOGETHER_H__6678F41E_6399_4EA4_9ED3_C4E1C2282AD0__INCLUDED_)
