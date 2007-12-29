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
// Date: 2007-2-9 15:29:11
// 
// $Id: $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <winx/IPC.h>
#include "idl/MyTestComApp_i.h"
#include "idl/MyTestComApp_i.c"

// -------------------------------------------------------------------------
// WinMain

int WINAPI WinMain(HINSTANCE hinst,
                   HINSTANCE hinstPrev,
                   LPSTR pszCmdLine,
                   int nCmdShow)
{
	CComAppInit com;
	CComPtr<_Application> app;
	::GetActiveObject(CLSID_Application, &app);
	if (app) {
		CComBSTR caption;
		app->get_Caption(&caption);
		app->MsgBox(caption, CComBSTR(L"Title"));
	}
	else {
		winx::MsgBox(L"Can't GetObject!");
	}
	return 0;
}

// -------------------------------------------------------------------------
// $Log: $
