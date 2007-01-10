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
// Module: winx/win/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 15:22:02
// 
// $Id: Basic.h,v 1.3 2006/12/20 08:56:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_BASIC_H__
#define __WINX_WIN_BASIC_H__

#ifndef __WINX_WIN_BASICTYPES_H__
#include "BasicTypes.h"
#endif

__WINX_BEGIN

// =========================================================================
// MsgBox

inline INT winx_call MsgBox(
	const char* pszMessage,
	const char* pszTitle = NULL, UINT uFlags = MB_OK|MB_TOPMOST)
{
	return ::MessageBoxA(NULL, pszMessage, pszTitle, uFlags);
}

inline INT winx_call MsgBox(
	HWND hWndParent, 
	const char* pszMessage,
	const char* pszTitle = NULL, UINT uFlags = MB_OK)
{
	return ::MessageBoxA(hWndParent, pszMessage, pszTitle, uFlags);
}

inline INT winx_call MsgBox(
	const WCHAR* pszMessage,
	const WCHAR* pszTitle = NULL, UINT uFlags = MB_OK|MB_TOPMOST)
{
	return ::MessageBoxW(NULL, pszMessage, pszTitle, uFlags);
}

inline INT winx_call MsgBox(
	HWND hWndParent, 
	const WCHAR* pszMessage,
	const WCHAR* pszTitle = NULL, UINT uFlags = MB_OK)
{
	return ::MessageBoxW(hWndParent, pszMessage, pszTitle, uFlags);
}

// =========================================================================
// Dir

template <class DoT>
void winx_call Dir(const CString& strDir, DoT& doIt)
{
	CFindFile finder;
	for (BOOL f = finder.FindFile(strDir + _T("\\*.*")); f; f = finder.FindNextFile())
	{
		if (finder.IsDirectory()) {
			if (!finder.IsDots())
				doIt.onDir(finder);
		}
		else {
			doIt.onFile(finder);
		}
	}
}

class FindFileDo
{
public:
	void onDir(const winx::CFindFile& finder) {}
	void onFile(const winx::CFindFile& finder) {}
};

class FindFileRecursive
{
public:
	void onDir(const winx::CFindFile& finder) { Dir(finder.GetFilePath(), *this); }
	void onFile(const winx::CFindFile& finder) {}
};

// =========================================================================
// class BusyFlag/BusyCritical - for general use

class BusyFlag
{
public:
	BOOL m_fBusy;

	BusyFlag() : m_fBusy(FALSE) {}
};

class BusyEnterCritical
{
private:
	BusyFlag& m_flag;

public:
	BusyEnterCritical(BusyFlag& flag) : m_flag(flag)
	{
		WINX_ASSERT(!m_flag.m_fBusy);
		m_flag.m_fBusy = TRUE;
	}
	~BusyEnterCritical()
	{
		m_flag.m_fBusy = FALSE;
	}
};

#define WINX_BUSY_ENTER_CRITICAL(flag)										\
	if (flag.m_fBusy)														\
		return;																\
	BusyEnterCritical __winx_busy_enter_critial(flag)

#define WINX_BUSY_ENTER_CRITICAL_RET(flag, retval)							\
	if (flag.m_fBusy)														\
		return retval;														\
	BusyEnterCritical __winx_busy_enter_critial(flag)

// =========================================================================
// class NullClass

using std::NullClass;

// =========================================================================
// $Log: Basic.h,v $
// Revision 1.3  2006/12/20 08:56:04  xushiwei
// move NullClass to stdext
//
// Revision 1.2  2006/08/20 14:18:22  xushiwei
// FileSystem: Dir
//
// Revision 1.1  2006/08/19 09:42:10  xushiwei
// WINX-Basic:
//   BasicTypes(CString, CFindFile, CRecentDocumentList, CComBSTR, etc)
//   Helper(MsgBox, GetThisModule, InitCommonControls, etc)
//   Init(CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc)
//   Resource(GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc)
//   Gdi(Rgn, Pen, Brush, Font, Bitmap, DC, DoubleBuffer, etc)
//   WindowHandle, WindowMap, MessageMap
//   COM Support(Object, StackObject, FakeObject, COLESTR, etc)
//

__WINX_END

#endif /* __WINX_WIN_BASIC_H__ */
