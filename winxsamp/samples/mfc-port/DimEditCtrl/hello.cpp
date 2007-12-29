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
// $Id: hello.cpp,v 1.1 2006/09/16 18:08:23 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <winx/ext/DimEditCtrl.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_DIMEDIT_DIALOG>
{
	WINX_ICON(IDR_MAINFRAME);
	WINX_REFLECT();

	winx::DimEditCtrl*	m_decHint;
	winx::DimEditCtrl*	m_decPassword;
	winx::DimEditCtrl*	m_decUsername;
	
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		winx::SubclassDlgItem(&m_decUsername, hDlg, IDC_EDIT1);
		winx::SubclassDlgItem(&m_decPassword, hDlg, IDC_EDIT2);
		winx::SubclassDlgItem(&m_decHint, hDlg, IDC_EDIT3);

		//
		//	Set Text Limits...
		//
		m_decUsername->LimitText( 32 );		
		m_decPassword->LimitText( 32 );
		m_decHint->LimitText( 128 );
		
		//
		//	Set Dim Text...
		//
		m_decUsername->SetDimText( _T( "Username Required (Max 32 Characters)" ) );
		m_decPassword->SetDimText( _T( "Password Required (Max 32 Characters)" ) );
		m_decHint->SetDimText( _T( "Hint Optional (Max 128 Characters)" ) );
		
		//
		//	Override Dim Colors For Two Controos...
		//
		m_decUsername->SetDimColor( RGB( 0xFF, 0x80, 0x80 ) );
		m_decPassword->SetDimColor( RGB( 0xFF, 0x80, 0x80 ) );
		
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
// Revision 1.1  2006/09/16 18:08:23  xushiwei
// MFC-Port:
//   Demo - port MFC controls to WINX (DimEditCtrl)
//
