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
// $Id: hello.cpp,v 1.2 2006/08/26 03:16:49 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx/Config.h>
#include <winx.h>
#include <winx/Shell.h>
#include "resource.h"

#define ACKEY _T("Software\\winx\\tutorials\\autocomplete\\ACHistory")

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
private:
	winx::ACHistory m_acHis;
	HWND m_hWndEdit;
	
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		m_hWndEdit = ::GetDlgItem(hDlg, IDC_EDIT1);
		m_acHis.LoadFromRegistry(HKEY_CURRENT_USER, ACKEY);
		m_acHis.SetLimitCount(5);
		m_acHis.Bind(m_hWndEdit, ACO_AUTOSUGGEST|ACO_USETAB|ACO_UPDOWNKEYDROPSLIST);
		return TRUE;
	}
	
	VOID OnOK(HWND hWnd)
	{
		m_acHis.Commit(m_hWndEdit);
		m_acHis.SaveToRegistry(HKEY_CURRENT_USER, ACKEY);
		::SetWindowText(m_hWndEdit, _T(""));
	}
};

// -------------------------------------------------------------------------
// WinMain

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CComAppInit com;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/08/26 03:16:49  xushiwei
// Demo: AutoComplete
//
