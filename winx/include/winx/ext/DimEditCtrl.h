/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: winx/ext/DimEditCtrl.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 20:26:15
// 
// $Id: DimEditCtrl.h,v 1.3 2006/12/03 08:40:21 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_EXT_DIMEDITCTRL_H__
#define __WINX_EXT_DIMEDITCTRL_H__

// -------------------------------------------------------------------------

/*|*\
|*|  File:      DimEditCtrl.h
|*|  
|*|  By:        James R. Twine, TransactionWorks, Inc.
|*|             Copyright 2000, TransactionWorks, inc.
|*|  Date:      Thursday, September 21, 2000
|*|             
|*|  Notes:     This Is The Implementation Of A "Dim Edit Control".
|*|             It Provides Visual Instructions Within The Edit
|*|             Control Itself.  It Can Be Used To Indicate Special
|*|             Properties Of A Edit Control Used On A Crowded
|*|             Interface
|*|             
|*|             May Be Freely Incorporated Into Projects Of Any Type
|*|             Subject To The Following Conditions:
|*|             
|*|             o This Header Must Remain In This File, And Any
|*|               Files Derived From It
|*|             o Do Not Misrepresent The Origin Of This Code
|*|               (IOW, Do Not Claim You Wrote It)
|*|             
|*|             A "Mention In The Credits", Or Similar Acknowledgement,
|*|             Is *NOT* Required.  It Would Be Nice, Though! :)
\*|*/
#if !defined(AFX_DIMEDITCTRL_H__CF8D88FB_6945_11D4_8AC4_00C04F6092F9__INCLUDED_)
#define AFX_DIMEDITCTRL_H__CF8D88FB_6945_11D4_8AC4_00C04F6092F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DimEditCtrl.h : header file
//

namespace winx { //--> @@code add

#pragma pack(1)
	
//
//	This Specifies The Length Of The Dim Text Buffer...
//
enum { DIM_TEXT_LEN = 128 };				// Dim Text Buffer Length


/////////////////////////////////////////////////////////////////////////////
// CDimEditCtrl window

class CDimEditCtrl : public Edit<CDimEditCtrl>
{
	WINX_CLASS("WinxDimEdit");
// Construction
public:
	CDimEditCtrl();

// Attributes
public:

// Operations
public:

	void	SetShowDimControl( bool bShow );				// Show Or Hide The Dim Control
	void	SetDimText( LPCTSTR cpText );					// Set The Dim Text
	void	SetDimColor( COLORREF crDColor );				// Set The Dim Color
	void	SetDimOffset( char cRedOS, char cGreenOS, 
					char cBlueOS );							// Set The Dim Color Offset

// Overrides
/*	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDimEditCtrl)
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
*/
//--> @@code modify
	LRESULT OnSubclass(HWND hWnd);

/*	// Generated message map functions
protected:
	//{{AFX_MSG(CDimEditCtrl)
	afx_msg void OnChange();
	afx_msg void OnSetfocus();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
*/
//--> @@code modify
	WINX_REFLECT_CMDS_BEGIN()
		WINX_REFLECT_CMD(EN_CHANGE, OnEnChange)
		WINX_REFLECT_CMD(EN_SETFOCUS, OnEnSetfocus)
		WINX_REFLECT_CMD(EN_KILLFOCUS, OnEnSetfocus)
	WINX_REFLECT_CMDS_END();
public:
	void OnEnChange(HWND hWnd);
	void OnEnSetfocus(HWND hWnd);
	void OnPaint(HWND hWnd);
	BOOL OnEraseBkgnd(HWND hWnd, HDC hDC);
	void OnSettingChange(HWND hWnd, UINT uFlags, LPCTSTR lpszSection);
	
//	DECLARE_MESSAGE_MAP() //--> @@code delete

	void		DrawDimText( void );						// Draw The Dim Text

	COLORREF	m_crDimTextColor;							// "Hard" Dim Text Color
	TCHAR		m_caDimText[DIM_TEXT_LEN];					// Dim Text Buffer
	char		m_bShowDimText	:1;							// Are We Showing The Dim Text?
	char		m_bUseDimOffset	:1;							// Are We Using The Offset Colors (Not Hard Color)?
	char		m_cRedOS;									// Red Color Dim Offset
	char		m_cGreenOS;									// Green Color Dim Offset
	char		m_cBlueOS;									// Blue Color Dim Offset
	int			m_iDimTextLen;								// Length Of Dim Text
};

typedef CDimEditCtrl DimEditCtrl; //--> @@code add

#include "DimEditCtrl.cpp" //--> @@code add

#pragma pack()

}; // namespace winx //--> @@code add

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIMEDITCTRL_H__CF8D88FB_6945_11D4_8AC4_00C04F6092F9__INCLUDED_)

// -------------------------------------------------------------------------
// $Log: DimEditCtrl.h,v $
// Revision 1.3  2006/12/03 08:40:21  xushiwei
// Unify style of map macro, such as:
//  WINX_TEST_SUITE/WINX_TEST/WINX_TEST_SUITE_END;
//  WINX_CMDS_BEGIN/WINX_CMD/WINX_CMDS_END;
//  WINX_SYSCMD_BEGIN/WINX_SYSCMD/WINX_SYSCMD_END;
//  WINX_NOTIFY_BEGIN/WINX_NOTIFY/WINX_NOTIFY_END;
//  WINX_REFLECT_NOTIFY_BEGIN/WINX_REFLECT_NOTIFY/WINX_REFLECT_NOTIFY_END;
//
// Revision 1.2  2006/09/16 18:26:20  xushiwei
// EN_KILLFOCUS
//
// Revision 1.1  2006/09/16 18:07:10  xushiwei
// MFC-Port:
//   Demo - port MFC controls to WINX (DimEditCtrl)
//

#endif /* __WINX_EXT_DIMEDITCTRL_H__ */
