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
// Module: winx/win/Resource.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 16:34:44
// 
// $Id: Resource.h,v 1.3 2006/12/11 05:19:08 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_RESOURCE_H__
#define __WINX_WIN_RESOURCE_H__

#ifndef __WINX_WIN_BASICTYPES_H__
#include "BasicTypes.h"
#endif

__WINX_BEGIN

// -------------------------------------------------------------------------
// GetInstance/GetAppInstance

typedef WTL::CResource Resource;

inline HINSTANCE winx_call GetInstance(HWND hWnd)
{
	return (HINSTANCE)LongToHandle(::GetWindowLong(hWnd, GWL_HINSTANCE));
}

inline HINSTANCE winx_call GetAppInstance()
{
	return ::GetModuleHandle(NULL);
}

// -------------------------------------------------------------------------
// GetModuleName

inline CString winx_call GetModuleName(IN HINSTANCE hInst = GetThisModule())
{
	TCHAR szModule[_MAX_PATH];
	DWORD cch = ::GetModuleFileName(hInst, szModule, _MAX_PATH);
	return CString(szModule, cch);
}

inline CString winx_call GetModuleName(IN HWND hWnd)
{
	return GetModuleName(GetInstance(hWnd));
}

// -------------------------------------------------------------------------
// GetModulePath/GetAppPath

inline CString winx_call GetModulePath(IN HINSTANCE hInst = GetThisModule())
{
	CString strPath = GetModuleName(hInst);
	int pos = strPath.ReverseFind('\\');
	WINX_ASSERT(pos > 0);
	if (pos > 0)
	{
		strPath.Delete(pos, strPath.GetLength()-pos);
	}
	return strPath;
}

inline CString winx_call GetAppPath()
{
	return GetModulePath(::GetModuleHandle(NULL));
}

// -------------------------------------------------------------------------
// GetModuleIniPath/GetAppIniPath

inline CString winx_call GetModuleIniPath(
	IN LPCTSTR relationName, IN HINSTANCE hInst = GetThisModule())
{
	CString strPath = GetModuleName(hInst);
	int pos = strPath.ReverseFind('\\') + 1;
	WINX_ASSERT(pos > 0);
	if (pos > 0)
	{
		strPath.Delete(pos, strPath.GetLength()-pos);
	}
	strPath += relationName;
	return strPath;
}

inline CString winx_call GetAppIniPath(
	IN LPCTSTR relationName)
{
	return GetModuleIniPath(relationName, ::GetModuleHandle(NULL));
}

// -------------------------------------------------------------------------
// LoadBitmap

inline STDMETHODIMP_(HBITMAP) LoadBitmap(HWND hWnd, int nBitmapID)
{
	return ::LoadBitmap(
		(HINSTANCE)LongToHandle(::GetWindowLong(hWnd, GWL_HINSTANCE)),
		MAKEINTRESOURCE(nBitmapID));
}

// -------------------------------------------------------------------------
// LoadAccelerators

inline HACCEL winx_call LoadAccelerators(HWND hWnd, int nID)
{
	return ::LoadAccelerators(
		(HINSTANCE)LongToHandle(::GetWindowLong(hWnd, GWL_HINSTANCE)),
		MAKEINTRESOURCE(nID));
}

// -------------------------------------------------------------------------
// Icon/Cursor Helper

typedef WTL::CIconHandle IconHandle;
typedef WTL::CIcon Icon;

typedef WTL::CCursorHandle CursorHandle;
typedef WTL::CCursor Cursor;

// -------------------------------------------------------------------------

#if defined(_DEBUG)
#define _WINX_VERIFY_HINSTANCE(hInst)										\
	do {																	\
		static HINSTANCE _winx_nInst = (hInst);								\
		WINX_ASSERT(_winx_nInst == (hInst));								\
	} while (0)
#else
#define _WINX_VERIFY_HINSTANCE(hInst)
#endif

enum CachedResourceCategory
{
	cachedRcAppInstance = -2,
	cachedRcThisModule = -1,
	cachedRcUserModuleMin = 0,
};

// -------------------------------------------------------------------------
// class CachedBitmap

template <int nResId, int nInst = cachedRcThisModule>
class CachedBitmap
{
public:
	static HBITMAP winx_call GetObject(HINSTANCE hInst)
	{
		_WINX_VERIFY_HINSTANCE(hInst);
		static WTL::CBitmap _cache(
			::LoadBitmap(hInst, MAKEINTRESOURCE(nResId))
			);
		return _cache.m_hBitmap;
	}

	static HBITMAP winx_call GetObject(HWND hWnd)
	{
		return GetObject(
			(HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE));
	}
};

#define WINX_CACHED_BITMAP_EX(nResId, hModule, nInst)						\
	(winx::CachedBitmap<nResId, nInst>::GetObject(hModule))

#define WINX_CACHED_BITMAP(nResId, hInst)									\
	WINX_CACHED_BITMAP_EX(nResId, GetThisModule(), -1)

#define WINX_CACHED_APPBITMAP(nResId)										\
	WINX_CACHED_BITMAP_EX(nResId, ::GetModuleHandle(NULL), -2)

// -------------------------------------------------------------------------
// class CachedIcon

template <int nResId, int nInst = cachedRcThisModule>
class CachedIcon
{
public:
	static HICON winx_call GetObject(HINSTANCE hInst)
	{
		_WINX_VERIFY_HINSTANCE(hInst);
		static WTL::CIcon _cache(
			::LoadIcon(hInst, MAKEINTRESOURCE(nResId))
			);
		return _cache.m_hIcon;
	}
	
	static HICON winx_call GetObject(HWND hWnd)
	{
		return GetObject(
			(HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE));
	}
};

#define WINX_CACHED_ICON_EX(nResId, hModule, nInst)							\
	(winx::CachedIcon<nResId, nInst>::GetObject(hModule))

#define WINX_CACHED_ICON(nResId)											\
	WINX_CACHED_ICON_EX(nResId, GetThisModule(), -1)

#define WINX_CACHED_APPICON(nResId)											\
	WINX_CACHED_ICON_EX(nResId, ::GetModuleHandle(NULL), -2)

// -------------------------------------------------------------------------
// class CachedPatternBrush

template <int nBitmapId, int nInst = cachedRcThisModule>
class CachedPatternBrush
{
public:
	static HBRUSH winx_call GetObject(HINSTANCE hInst)
	{
		_WINX_VERIFY_HINSTANCE(hInst);
		static WTL::CBrush _cache(
			::CreatePatternBrush(WINX_CACHED_BITMAP(nBitmapId, hInst))
			);
		return _cache.m_hBrush;
	}
	
	static HBRUSH winx_call GetObject(HWND hWnd)
	{
		return GetObject(
			(HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE));
	}
};

#define WINX_CACHED_PATTERN_EX(nBitmapId, hModule, nInst)					\
	(winx::CachedPatternBrush<nBitmapId, nInst>::GetObject(hModule))

#define WINX_CACHED_PATTERN(nBitmapId)										\
	WINX_CACHED_PATTERN_EX(nBitmapId, GetThisModule(), -1)

#define WINX_CACHED_APPPATTERN(nBitmapId)									\
	WINX_CACHED_PATTERN_EX(nBitmapId, ::GetModuleHandle(NULL), -2)

// -------------------------------------------------------------------------
// Menu Helper

typedef WTL::CMenuItemInfo MenuItemInfo;
typedef WTL::CMenuHandle MenuHandle;
typedef WTL::CMenu Menu;

inline VOID winx_call SetMenuOwnerDraw(HMENU hMenu)
{
	WINX_ASSERT(::IsMenu(hMenu));
	TCHAR szCaption[256];
	MENUITEMINFO mii = { sizeof(mii) };
	UINT nCount = ::GetMenuItemCount(hMenu);

	for (UINT i = 0; i < nCount; ++i)
	{
		mii.fMask = MIIM_TYPE;
	    ::GetMenuItemInfo(hMenu, i, TRUE, &mii);
		if (!(MFT_OWNERDRAW & mii.fType))
		{
			::GetMenuString(hMenu, i, szCaption, countof(szCaption), MF_BYPOSITION);
			mii.fMask = MIIM_TYPE;
			mii.fType |= MFT_OWNERDRAW;
			mii.dwTypeData = szCaption;
		    ::SetMenuItemInfo(hMenu, i, TRUE, &mii);
		}
	}
}

// -------------------------------------------------------------------------
// PopupMenu - µ¯³öÓÒ¼ü²Ëµ¥

inline BOOL winx_call PopupMenu(
	HWND hWndOwner, INT x, INT y,
	HINSTANCE hInst, UINT uMenuId, int idxSubMenu = 0,
	UINT uFlags = TPM_LEFTALIGN | TPM_TOPALIGN,
	LPTPMPARAMS lptpm = NULL
	)
{
	HMENU hMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(uMenuId));
	BOOL fPopup = ::TrackPopupMenuEx(
		GetSubMenu(hMenu, idxSubMenu), uFlags,
		x, y, hWndOwner, lptpm);
	::DestroyMenu(hMenu);
	return fPopup;
}

inline BOOL winx_call PopupMenu(
	HWND hWndOwner, INT x, INT y,
	UINT uMenuId, int idxSubMenu = 0,
	UINT uFlags = TPM_LEFTALIGN | TPM_TOPALIGN,
	LPTPMPARAMS lptpm = NULL
	)
{
	HINSTANCE hInst = (HINSTANCE)LongToHandle(::GetWindowLong(hWndOwner, GWL_HINSTANCE));
	HMENU hMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(uMenuId));
	BOOL fPopup = ::TrackPopupMenuEx(
		GetSubMenu(hMenu, idxSubMenu), uFlags,
		x, y, hWndOwner, lptpm);
	::DestroyMenu(hMenu);
	return fPopup;
}

// -------------------------------------------------------------------------
// LoadAccelerators

typedef WTL::CAcceleratorHandle AcceleratorHandle;
typedef WTL::CAccelerator Accelerator;

// -------------------------------------------------------------------------
// $Log: Resource.h,v $
// Revision 1.3  2006/12/11 05:19:08  xushiwei
// vs2005 __w64 support
//
// Revision 1.2  2006/09/25 02:34:52  xushiwei
// WINX-Basic: GetModulePath/GetAppPath
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

#endif /* __WINX_WIN_RESOURCE_H__ */
