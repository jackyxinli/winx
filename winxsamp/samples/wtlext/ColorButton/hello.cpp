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
// $Id: hello.cpp,v 1.1 2006/09/16 18:54:05 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define _WIN32_WINNT	0x0501
#include <winx/Config.h>
#include <winx.h>
#include <winx/Theme.h>
#include <wtlext/ColorButton.h>
#include <wtlext/ColorButton.cpp>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_MAINDLG>
{
	WINX_ICON(IDR_MAINFRAME);
	WINX_REFLECT();

private:
	WTL::CColorButton m_btnMyColor1;
	WTL::CColorButton m_btnMyColor2;
	WTL::CColorButton m_btnMyColor3;
	
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		m_btnMyColor1 .SubclassWindow (GetDlgItem (IDC_COLOR_1));
		m_btnMyColor2 .SubclassWindow (GetDlgItem (IDC_COLOR_2));
		m_btnMyColor3 .SubclassWindow (GetDlgItem (IDC_COLOR_3));
		
		m_btnMyColor2 .SetDefaultText (_T (""));
		m_btnMyColor2 .SetCustomText (_T (""));
		m_btnMyColor3 .SetDefaultText (_T ("My Default Text"));
		m_btnMyColor3 .SetCustomText (_T (""));

		return TRUE;
	}
};

// -------------------------------------------------------------------------

CComModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls();
	CComModuleInit module;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/09/16 18:54:05  xushiwei
// WTL-Extension: BitmapButton, BitmapHyperLink, ColorButton, IconEdit
//
