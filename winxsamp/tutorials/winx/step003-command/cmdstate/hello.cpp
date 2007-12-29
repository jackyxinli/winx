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
// Date: 2006-8-22 14:21:57
// 
// $Id: hello.cpp,v 1.3 2006/09/18 05:26:15 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : 
	public winx::ModelDialog<CHelloDlg, IDD_HELLO>,
	public winx::UpdateUI<CHelloDlg>
{
	WINX_CMDS_BEGIN()
		WINX_CMD(IDC_CMD1, OnCmd1)
		WINX_CMD(IDC_CMD2, OnCmd2)
		WINX_CMD(IDC_ALL, OnCmdEnableAll)
		WINX_CMD(ID_HELP_ABOUT, OnCmdAbout)
	WINX_CMDS_END();

	WINX_UPDATEUI_BEGIN()
		WINX_UPDATEUI(IDC_CMD1, UPDUI_MENUPOPUP)
		WINX_UPDATEUI(IDC_CMD2, UPDUI_MENUPOPUP)
		WINX_UPDATEUI(IDC_ALL, UPDUI_MENUPOPUP)
	WINX_UPDATEUI_END();

public:
	BOOL OnContextMenu(HWND hWnd, winx::CPoint pos)
	{
		winx::PopupMenu(hWnd, pos.x, pos.y, IDR_MAINMENU);
		return TRUE;
	}

	VOID OnCmdAbout(HWND hWnd)
	{
		winx::SimpleDialog dlg;
		dlg.DoModal(hWnd, IDD_ABOUT);
	}

	VOID OnCmd1(HWND hWnd)
	{
		UIEnable(IDC_CMD2, FALSE);
	}
	
	VOID OnCmd2(HWND hWnd)
	{
		UIEnable(IDC_CMD1, FALSE);
	}

	VOID OnCmdEnableAll(HWND hWnd)
	{
		UIEnable(IDC_CMD1, TRUE);
		UIEnable(IDC_CMD2, TRUE);
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
// Revision 1.3  2006/09/18 05:26:15  xushiwei
// UpdateUI - interface changed
//
// Revision 1.2  2006/08/22 08:06:59  xushiwei
// tutorial-003: Focus message - command
//
