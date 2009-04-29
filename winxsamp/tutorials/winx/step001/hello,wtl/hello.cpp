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
// Date: 2006-8-19 18:24:19
// 
// $Id: hello.cpp,v 1.1 2006/08/19 11:09:45 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <atlbase.h>
#include <atlapp.h>
extern WTL::CAppModule _Module;
#include <atlwin.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public ATL::CDialogImpl<CHelloDlg>
{
public:
	enum { IDD = IDD_HELLO };
	
public:
	BEGIN_MSG_MAP(CHelloDlg)
		COMMAND_RANGE_HANDLER(IDOK, IDNO, OnCloseCmd)
	END_MSG_MAP()
		
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		::EndDialog(m_hWnd, wID);
		return 0;
	}
};

// -------------------------------------------------------------------------

WTL::CAppModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	_Module.Init(NULL, hInstance);
	{
		CHelloDlg dlg;
		dlg.DoModal();
	}
	_Module.Term();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/08/19 11:09:45  xushiwei
// tutorial-001: hello sdk, wtl, winx
//
