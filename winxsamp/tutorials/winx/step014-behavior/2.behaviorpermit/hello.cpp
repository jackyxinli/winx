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
// Date: 2006-8-25 9:47:19
// 
// $Id: hello.cpp,v 1.3 2006/11/28 12:49:27 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : 
	public winx::ModelDialog<CHelloDlg, IDD_HELLO>,
	public winx::BehaviorPermit<winx::AutoHiddenAnySide>
{
	WINX_BEHAVIOR();
	WINX_CMDS_BEGIN()
		WINX_CMD(IDC_CANAUTOHIDE, OnCmdCanAutoHide)
	WINX_CMDS_END();

public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		::CheckDlgButton(hDlg, IDC_CANAUTOHIDE, TRUE);
		return TRUE;
	}

	VOID OnCmdCanAutoHide(HWND hDlg)
	{
		EnableBehavior(::IsDlgButtonChecked(hDlg, IDC_CANAUTOHIDE));
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.3  2006/11/28 12:49:27  xushiwei
// simplify WindowBehavior interface
//
// Revision 1.2  2006/09/03 04:37:21  xushiwei
// WINX-Core: Behavior(BehaviorPermit, BehaviorSwitch, AutoHidden, etc)
// WINX-Extension: PropertySheet, PropertyPage
//
