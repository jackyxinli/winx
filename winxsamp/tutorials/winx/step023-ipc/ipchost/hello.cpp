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
// Date: 2006-3-8 17:24:30
// 
// $Id: hello.cpp,v 1.2 2006/08/26 03:16:49 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <winx/IPC.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
	WINX_IPCHOST();
private:
	winx::ListBoxHandle lb;

public:
	BOOL OnInitDialog(HWND hWndHost, HWND hWndDefaultFocus)
	{
		lb.DlgItem(hWndHost, IDC_LIST1);

		if (winx::HasIpcHostService(hWndHost))
			lb.AddString(_T("RegisterService succeed!!!\n"));
		else
			lb.AddString(_T("RegisterService failed!!!\n"));
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
	COleAppInit init;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: $
