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
// Module: winx/CommonDialogs.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-23 12:07:42
// 
// $Id: CommonDialogs.h,v 1.3 2006/09/03 08:16:03 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_COMMONDIALOGS_H
#define WINX_COMMONDIALOGS_H

#ifndef WINX_WIN_BASIC_H
#include "win/Basic.h"
#endif

// -------------------------------------------------------------------------
// Reference Win32 SDK

#if defined(WINX_USE_WINSDK)

#ifndef _INC_COMMDLG
#include "../../../winsdk/include/commdlg.h"
#endif

#ifndef _SHLOBJ_H_
#include "../../../winsdk/include/shlobj.h"
#endif

#else // defined(WINX_USE_DEFSDK)

#ifndef _INC_COMMDLG
#include <commdlg.h>
#endif

#ifndef _SHLOBJ_H_
#include <shlobj.h>
#endif

#endif

// -------------------------------------------------------------------------
// Reference WTL

#ifndef __ATLDLGS_H__
#include "../../../wtl/include/atldlgs.h"
#endif

NS_WINX_BEGIN

// -------------------------------------------------------------------------
// class FileDialogT

enum FileDialogType
{
	fdtSaveFileDialog = 0,
	fdtOpenFileDialog = 1,
};

template <INT nFileDlgType>
class FileDialogT : public OPENFILENAME
{
private:
	TCHAR m_szFileNameBuf[MAX_PATH];

public:
	//
	//szFilter
	//	- 以\0分隔的Filter列表。
	//nInitFilterIndex
	//	- 当对话框显示在屏幕上时最初显示的过滤器的索引, 第一个是1
	FileDialogT(
		IN LPCTSTR szFilter = NULL,
		IN LPCTSTR szDefExt = NULL,
		IN LPCTSTR szDlgTitle = NULL,
		IN LPCTSTR szInitFileName = NULL,
		IN UINT nInitFilterIndex = 1,
		IN LPCTSTR szInitialDir = NULL)
	{
		ZeroMemory(this, sizeof(OPENFILENAME));
		lStructSize	= sizeof(OPENFILENAME);
		
		lpstrFilter	= szFilter;
		nFilterIndex = nInitFilterIndex;

		lpstrFile = m_szFileNameBuf;
		nMaxFile = MAX_PATH;
		if (szInitFileName)
			_tcscpy(m_szFileNameBuf, szInitFileName);
		else
			m_szFileNameBuf[0] = '\0';

		lpstrInitialDir = szInitialDir;
		lpstrDefExt	= szDefExt;

		lpstrTitle = szDlgTitle;
	}

	int winx_call DoModal(
		HWND hWndParent = NULL, INT dlgType = nFileDlgType, HINSTANCE hInst =GetThisModule())
	{
		hwndOwner = hWndParent;
		hInstance = hInst;
		
		if (fdtOpenFileDialog & dlgType)
			return ::GetOpenFileName(this) ? IDOK : IDCANCEL;
		else
			return ::GetSaveFileName(this) ? IDOK : IDCANCEL;
	}

public:
	short winx_call GetFileTitle(LPTSTR szFileTitle, UINT nBufMax = _MAX_FNAME) const
	{
		return ::GetFileTitle(lpstrFile, szFileTitle, nBufMax);
	}

	CString winx_call GetFileTitle() const
	{
		TCHAR szFileTitle[_MAX_FNAME];
		::GetFileTitle(lpstrFile, szFileTitle, _MAX_FNAME);
		return CString(szFileTitle, _tcslen(szFileTitle));
	}
};

typedef FileDialogT<fdtOpenFileDialog> FileDialog;
typedef FileDialogT<fdtOpenFileDialog> OpenFileDialog;
typedef FileDialogT<fdtSaveFileDialog> SaveFileDialog;

// -------------------------------------------------------------------------
// class MultiSelectFiles

template <class Base>
class MultiSelectFiles : public Base
{
public:
	using Base::lpstrFile;
	
private:
	LPCTSTR m_lpszIter;

#if defined(_DEBUG)
	void winx_call _ValidateBegin()
	{
		LPCTSTR lpszIterBackup = m_lpszIter;
		Reset();
		WINX_ASSERT(lpszIterBackup == m_lpszIter);
		m_lpszIter = lpszIterBackup;
	}

	void winx_call _ValidateBegin() const
	{
		typedef MultiSelectFiles<Base> _Myt;
		((_Myt*)this)->_ValidateBegin();
	}
#endif

public:
	void winx_call Reset()
	{
		m_lpszIter = _tcschr(lpstrFile, '\0') + 1;
		if (*m_lpszIter == '\0') {
			m_lpszIter = lpstrFile;
			WINX_ASSERT(m_lpszIter[1] == ':');
		}
	}

	void winx_call Cancel()
	{
		lpstrFile[0] = '\0';
		m_lpszIter = lpstrFile;
	}

	UINT winx_call Count() const
	{
#if defined(_DEBUG)
		_ValidateBegin();
#endif
		LPCTSTR lpszIter = m_lpszIter;
		UINT count = 0;
		while(*lpszIter)
		{
			lpszIter += lstrlen(lpszIter) + 1;
			++count;
		}
		return count;
	}

	HRESULT winx_call NextFile(OUT LPTSTR szFile)
	{
		if (*m_lpszIter) {
			if (m_lpszIter[1] != ':') {
				szFile = std::strecpy(szFile, lpstrFile);
				if (szFile[-1] != '\\')
					*szFile++ = '\\';
			}
			m_lpszIter += std::strecpy(szFile, m_lpszIter) - szFile + 1;
			return S_OK;
		}
		return S_FALSE;
	}
};

// -------------------------------------------------------------------------
// class MultiFilesDialogT

template <int nFileDlgType>
class MultiFilesDialogT : public MultiSelectFiles<OPENFILENAME>
{
public:
	MultiFilesDialogT(
		IN LPCTSTR szFilter = NULL,
		IN LPCTSTR szTitle = NULL,
		IN UINT nInitFilterIndex = 1,
		IN LPCTSTR szDefExt = NULL,
		IN LPCTSTR szInitialDir = NULL,
		IN int nBufSize = 0x10000,
		IN LPOFNHOOKPROC fnHook = NULL)
	{
		ZeroMemory(this, sizeof(OPENFILENAME));
		lStructSize	= sizeof(OPENFILENAME);
		
		lpstrFilter	= szFilter;
		lpstrTitle = szTitle;
		nFilterIndex = nInitFilterIndex;

		lpstrInitialDir = szInitialDir;
		lpstrDefExt	= szDefExt;
		lpstrFile = (LPTSTR)malloc(sizeof(TCHAR)*nBufSize);
		lpstrFile[0] = '\0';
		nMaxFile = nBufSize;
		lpfnHook = fnHook;

		if (lpfnHook)
			Flags = OFN_EXPLORER|OFN_ALLOWMULTISELECT|OFN_ENABLEHOOK;
		else
			Flags = OFN_EXPLORER|OFN_ALLOWMULTISELECT;
	}

	~MultiFilesDialogT()
	{
		free(lpstrFile);
	}

public:
	int winx_call DoModal(
		HWND hWndParent = NULL, INT dlgType = nFileDlgType, HINSTANCE hInst =GetThisModule())
	{
		hwndOwner = hWndParent;
		hInstance = hInst;
		
		BOOL fRet;
		if (fdtOpenFileDialog & dlgType)
			fRet = ::GetOpenFileName(this);
		else
			fRet = ::GetSaveFileName(this);
		
		if (fRet)
		{
			Reset();
			return IDOK;
		}
		else
		{
			Cancel();
			return IDCANCEL;
		}
	}
};

typedef MultiFilesDialogT<fdtOpenFileDialog> MultiFilesDialog;
typedef MultiFilesDialogT<fdtOpenFileDialog> OpenMultiFilesDialog;
typedef MultiFilesDialogT<fdtSaveFileDialog> SaveMultiFilesDialog;

// -------------------------------------------------------------------------
// $Log: CommonDialogs.h,v $
// Revision 1.3  2006/09/03 08:16:03  xushiwei
// WINX-Extension: Mfc-Together
//
// Revision 1.2  2006/09/03 04:32:56  xushiwei
// WINX-Core: Behavior(BehaviorPermit, BehaviorSwitch, AutoHidden, etc)
// WINX-Extension: PropertySheet, PropertyPage
//
// Revision 1.1  2006/08/23 05:47:06  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
// WINX-CommonDialogs:
//   OpenFileDialog/SaveFileDialog, OpenMultiFilesDialog/SaveMultiFilesDialog
//

NS_WINX_END

#endif /* WINX_COMMONDIALOGS_H */
