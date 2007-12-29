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
// Date: 2006-8-22 16:45:25
// 
// $Id: hello.cpp,v 1.1 2006/08/23 05:49:54 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// class MyView

class MyDialog : public winx::ModelessDialog<MyDialog, IDD_MYVIEW>
{
	WINX_CLASS("MyDialog");
	WINX_CMDS_BEGIN()
		WINX_CMD(ID_HELP_ABOUT, OnCmdAbout)
	WINX_CMDS_END();
public:
	VOID OnCmdAbout(HWND hWnd)
	{
		winx::MsgBox(hWnd, _T("Insert dialogs into another dialog as child controls!"), _T("SuperDialog"));
	}
};

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
};

// -------------------------------------------------------------------------
// WinMain

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	WINX_APP_ICON(hInstance, IDI_SEARCH);
	winx::MsgBox(_T("Hello, winx::MsgBox!!!"), _T("HaHa"));
	
	MyDialog::RegisterClass();
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/08/23 05:49:54  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
//
// Revision 1.1  2006/08/22 09:02:17  xushiwei
// tutorial-004: Demonstrate how to implement user-defined controls
//
