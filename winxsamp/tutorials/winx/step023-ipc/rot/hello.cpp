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
#include <winx/com/OLESTR.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
private:
	winx::ListBoxHandle m_list;
	winx::ROT m_rot;

public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		USES_CONVERSION;
		m_list.DlgItem(hDlg, IDC_LIST1);

		winx::BringWindowToTopMost(hDlg);
		winx::BindCtxObject bc;
		winx::MonikerPtr m;
		winx::MonikerEnumerator me(m_rot);
		while (me.Next(&m) == S_OK) {
			winx::COLESTR str;
			m->GetDisplayName(bc, NULL, &str);
			if (str)
				m_list.AddString(W2T(str));
			m.Release();
		}
		return TRUE;
	}

	VOID OnIdle(HWND hWnd)
	{
		m_list.ResetContent();
		OnInitDialog(hWnd, m_list);
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	COleAppInit init;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: $
