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

#include <winx/APIHook.h>

// -------------------------------------------------------------------------
// class MsgBoxHook

class MsgBoxHook : public winx::APIHook<MsgBoxHook>
{
	WINX_API_HOOK("user32.dll", MessageBoxA);
public:
	static int WINAPI Process(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
	{
		return WINX_CALL_ORGAPI(hWnd, "MyText Success", "MyCaption", uType);
	}
};

// -------------------------------------------------------------------------
// WinMain

void HookIt(HINSTANCE hinst) // don't inline this function
{
	MsgBoxHook::Hook(hinst);
}

int WINAPI WinMain(HINSTANCE hinst,
                   HINSTANCE hinstPrev,
                   LPSTR pszCmdLine,
                   int nCmdShow)
{
	HookIt(hinst);
	MessageBoxA(NULL, "abc", NULL, 0);
	return 0;
}

// -------------------------------------------------------------------------
// $Log: $
