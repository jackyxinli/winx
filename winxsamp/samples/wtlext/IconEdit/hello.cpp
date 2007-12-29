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
// $Id: hello.cpp,v 1.2 2006/12/03 08:56:04 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define WINX_REFLECT_CTLCOLOR
#include <winx/Config.h>
#include <winx.h>
#include <wtlext/IconEdit.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_MAINDLG>
{
	WINX_ICON(IDR_MAINFRAME);
	WINX_REFLECT();
	WINX_CMDS_BEGIN()
		WINX_CMD(IDC_BUTTON1, OnBnClicked)
		WINX_CMD_EX(IDC_ED_URL, EN_UPDATE, OnEdURLUpdate)
	WINX_CMDS_END();

private:
	enum { URL_MAXLEN = 2048 };
	TCHAR m_szURL[URL_MAXLEN];
	
	WTL::CIconEdit		c_edURL;
	WTL::CIconEditBlack	c_edBlack;
	WTL::CIconEdit		c_edIconTitleTip;
	
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		c_edIconTitleTip.SubclassWindow(GetDlgItem(IDC_ED_TITLE_TIP_ICON));
		c_edIconTitleTip.SetWindowText(_T("The Quick Brown Fox Jumps Over The Lazy Dog."));
		c_edIconTitleTip.SetIcon(IDR_MAINFRAME);
		
		// Subclass edit controls
		c_edBlack.SubclassWindow(GetDlgItem(IDC_ED_BLACK));
		c_edBlack.SetWindowText(_T("\"The Quick Brown Fox Jumps Over The Lazy Dog.\"\r\n")
			_T("Sentences containing every letter of the alphabet are called \"pangrams\", or \"holalphabetic sentences.\r\n"));
		c_edBlack.SetIcon(IDR_MAINFRAME);
		
		c_edURL.SubclassWindow(GetDlgItem(IDC_ED_URL));
		c_edURL.SetLimitText(URL_MAXLEN);
		c_edURL.SetWindowText(_T("file:///C:/test.zip"));
		::SHAutoComplete(c_edURL, SHACF_DEFAULT);

		return TRUE;
	}

	void OnBnClicked(HWND hWnd)
	{
		WTL::CEdit wndEdit(GetDlgItem(1000 + rand() % 3));
		BOOL bEnable = wndEdit.IsWindowEnabled();
		wndEdit.EnableWindow(bEnable ^= 1);
	}

	void OnEdURLUpdate(HWND hWnd)
	{
		c_edURL.GetWindowText(m_szURL, URL_MAXLEN);
		
		if(0 == ::_tcslen(m_szURL))
		{
			c_edURL.ShowIcon(FALSE);
		}
		else
		{
			static LPCTSTR szHTTP = _T("http:");
			SHFILEINFO sfi;
			if(!::_tcsnicmp(m_szURL, szHTTP, ::_tcslen(szHTTP)))
			{
				// HTTP URL
				::SHGetFileInfo(_T(".htm"),
					FILE_ATTRIBUTE_NORMAL,
					&sfi, 
					sizeof(SHFILEINFO), 
					SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
			}
			else
			{
				TCHAR drive[_MAX_DRIVE];
				TCHAR ext[_MAX_EXT];
				::_tsplitpath(m_szURL, drive, NULL, NULL, ext);
				if(0 < ::_tcslen(drive) && 0 == ::_tcslen(ext))
				{
					// folder
					::SHGetFileInfo(m_szURL,
						FILE_ATTRIBUTE_DIRECTORY,
						&sfi, 
						sizeof(SHFILEINFO), 
						SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
				}
				else
				{
					// file URL
					::SHGetFileInfo(ext,
						FILE_ATTRIBUTE_NORMAL,
						&sfi, 
						sizeof(SHFILEINFO), 
						SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
				}
			}
			
			HICON hIconOld = c_edURL.SetIcon(sfi.hIcon);
			if(hIconOld)
				::DestroyIcon(hIconOld);
			
			c_edURL.ShowIcon(TRUE);
		}
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
	CComAppInit com;
	CComModuleInit module;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/12/03 08:56:04  xushiwei
// Unify style of map macro, such as:
//  WINX_TEST_SUITE/WINX_TEST/WINX_TEST_SUITE_END;
//  WINX_CMDS_BEGIN/WINX_CMD/WINX_CMDS_END;
//  WINX_SYSCMD_BEGIN/WINX_SYSCMD/WINX_SYSCMD_END;
//  WINX_NOTIFY_BEGIN/WINX_NOTIFY/WINX_NOTIFY_END;
//  WINX_REFLECT_NOTIFY_BEGIN/WINX_REFLECT_NOTIFY/WINX_REFLECT_NOTIFY_END;
//
// Revision 1.1  2006/09/16 18:54:05  xushiwei
// WTL-Extension: BitmapButton, BitmapHyperLink, ColorButton, IconEdit
//
