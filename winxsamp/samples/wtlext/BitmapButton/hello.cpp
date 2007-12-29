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
// $Id: hello.cpp,v 1.2 2006/12/03 08:56:03 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <wtlext/BitmapButton.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_MAINDLG>
{
	WINX_ICON(IDR_MAINFRAME);
	WINX_REFLECT();
	WINX_CMDS_BEGIN()
		WINX_CMD(IDAUTOFIRE, OnCmdAutofire)
		WINX_CMD_RANGE(IDC_CHECK1, IDC_CHECK2, OnCmdCheck)
	WINX_CMDS_END();

private:
	WTL::CImageList m_images;

	WTL::CBmpBtn m_ok;
	WTL::CBmpBtn m_cancel;
	WTL::CBmpBtn m_autofire;
	
	WTL::CProgressBarCtrl m_progress;
	
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		m_images.Create(IDB_BUTTONS, 64, 0, RGB(192,192,192));

		// the button extended style
		DWORD dw = BMPBTN_AUTOSIZE|BMPBTN_AUTO3D_SINGLE|BMPBTN_SHAREIMAGELISTS;
		
		// configure OK bitmap button
		m_ok.SetBitmapButtonExtendedStyle(dw);
		m_ok.SetImageList(m_images);
		m_ok.SetImages(0, 1, 2, 3);
		m_ok.SetToolTipText(_T("OK button tip"));
		m_ok.SubclassWindow(GetDlgItem(IDOK));
		
		// configure Cancel bitmap button
		m_cancel.SetBitmapButtonExtendedStyle(dw);
		m_cancel.SetImageList(m_images);
		m_cancel.SetImages(4, 5, 6, 7);
		m_cancel.SetToolTipText(_T("Cancel button tip"));
		m_cancel.SubclassWindow(GetDlgItem(IDCANCEL));
		
		// configure Autofire bitmap button
		dw = BMPBTN_AUTOFIRE|BMPBTN_AUTOSIZE|BMPBTN_AUTO3D_SINGLE|BMPBTN_SHAREIMAGELISTS;
		m_autofire.SetBitmapButtonExtendedStyle(dw);
		m_autofire.SetImageList(m_images);
		m_autofire.SetImages(8);
		m_autofire.SetToolTipText(_T("Autofire button tip"));
		m_autofire.SubclassWindow(GetDlgItem(IDAUTOFIRE));

		// get the handle of the progress control
		m_progress = GetDlgItem(IDC_PROGRESS1);

		return TRUE;
	}

	void OnDestroy(HWND hWnd)
	{
		m_images.Destroy();
	}

	void OnCmdAutofire(HWND hWnd)
	{
		static UINT nPos = 0;
		m_progress.SetPos(++nPos);
	}

	void OnCmdCheck(HWND hWnd, UINT wID)
	{
		UINT nFlag = IsDlgButtonChecked(wID);
		if (wID == IDC_CHECK1)
			m_cancel.EnableWindow(!nFlag); // enable/disable Cancel button
		else if (wID == IDC_CHECK2)
			SetHover(nFlag); // set hover style for buttons
	}

	void SetHover(BOOL bOn = true)
	{
		DWORD dwExtendedStyle = BMPBTN_AUTOSIZE | BMPBTN_AUTO3D_SINGLE;
		
		if (bOn)
			dwExtendedStyle |= BMPBTN_HOVER; // add hover style
		else
			dwExtendedStyle |= ~BMPBTN_HOVER; // remove hover style
		
		// change the OK button style
		m_ok.SetBitmapButtonExtendedStyle(dwExtendedStyle);
		m_ok.Invalidate();
		m_ok.UpdateWindow();
		
		// change the Cancel button style
		m_cancel.SetBitmapButtonExtendedStyle(dwExtendedStyle);
		m_cancel.Invalidate();
		m_cancel.UpdateWindow();
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
// Revision 1.2  2006/12/03 08:56:03  xushiwei
// Unify style of map macro, such as:
//  WINX_TEST_SUITE/WINX_TEST/WINX_TEST_SUITE_END;
//  WINX_CMDS_BEGIN/WINX_CMD/WINX_CMDS_END;
//  WINX_SYSCMD_BEGIN/WINX_SYSCMD/WINX_SYSCMD_END;
//  WINX_NOTIFY_BEGIN/WINX_NOTIFY/WINX_NOTIFY_END;
//  WINX_REFLECT_NOTIFY_BEGIN/WINX_REFLECT_NOTIFY/WINX_REFLECT_NOTIFY_END;
//
// Revision 1.1  2006/09/16 18:54:04  xushiwei
// WTL-Extension: BitmapButton, BitmapHyperLink, ColorButton, IconEdit
//
