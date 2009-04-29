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
// $Id: hello.cpp,v 1.2 2006/09/13 17:07:43 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : 
	public winx::ModelDialog<CHelloDlg, IDD_HELLO>,
	public winx::WinDataExchange<CHelloDlg>
{
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		m_fCheck1 = true;
		m_nSex = 0;
		m_nLevel = 1;
		m_strEdit1 = _T("hello");
		DoDataExchange(false);
		return TRUE;
	}

	void OnOK(HWND hWnd)
	{
		DoDataExchange(true);
		winx::MsgBoxTrace(
			_T("Check=%d, Sex=%d, Level=%d, Edit='%s'\n"),
			m_fCheck1, m_nSex, m_nLevel, (LPCTSTR)m_strEdit1);
		CloseDialog(hWnd);
	}
	
private:
	bool m_fCheck1;
	INT m_nSex;
	INT m_nLevel;
	winx::CString m_strEdit1;

	WINX_DDX_BEGIN()
		DDX_TEXT(IDC_EDIT1, m_strEdit1)
		DDX_CHECK(IDC_CHECK1, m_fCheck1)
		DDX_RADIO(IDC_RADIO1, m_nSex)
		DDX_RADIO(IDC_RADIO3, m_nLevel)
	WINX_DDX_END();
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
// Revision 1.2  2006/09/13 17:07:43  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModalDialog/AxModalessDialog
//
// Revision 1.1  2006/09/13 13:58:40  xushiwei
// WINX-Core:
//  DDX(DataExchange) - class WinDataExchange, WINX_DDX_BEGIN, WINX_DDX_END, etc.
//
