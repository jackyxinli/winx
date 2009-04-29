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
// Module: hello.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:26:15
// 
// $Id: hello.cpp,v 1.2 2006/09/17 02:27:06 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <wtlext/DimEdit.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_DIMEDIT_DIALOG>
{
	WINX_ICON(IDR_MAINFRAME);

	WTL::CDimEdit	m_decHint;
	WTL::CDimEdit	m_decPassword;
	WTL::CDimEdit	m_decUsername;
	
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		m_decUsername.SubclassWindow(GetDlgItem(IDC_EDIT1));
		m_decPassword.SubclassWindow(GetDlgItem(IDC_EDIT2));
		m_decHint.SubclassWindow(GetDlgItem(IDC_EDIT3));

		m_decUsername.SetDimText( _T( "Username Required (Max 32 Characters)" ) )
			.SetDimColor( RGB( 0xFF, 0x80, 0x80 ) )
			.LimitText( 32 );

		m_decPassword.SetDimText( _T( "Password Required (Max 32 Characters)" ) )
			.SetDimColor( RGB( 0xFF, 0x80, 0x80 ) )
			.LimitText( 32 );

		m_decHint.SetDimText( _T( "Hint Optional (Max 128 Characters)" ) )
			.LimitText( 128 );

		return TRUE;
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls();
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/09/17 02:27:06  xushiwei
// remove WINX_REFLECT() - WTL::CDimEdit controls don't require Refelection.
//
// Revision 1.1  2006/09/17 02:22:46  xushiwei
// WTL-Extension: DimEdit
//
