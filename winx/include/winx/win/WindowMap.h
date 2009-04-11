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
// Module: winx/win/WindowMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 16:25:06
// 
// $Id: WindowMap.h,v 1.3 2006/12/11 05:19:08 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_WIN_WINDOWMAP_H
#define WINX_WIN_WINDOWMAP_H

#if (0)
#define WINX_WINDOWMAP_SIMPLE
#define WINX_WINDOWMAP_STDMAP
#define WINX_MESSAGEMAP_USERDATA
#define WINX_MT_MESSAGEMAP
#endif

#if defined(WINX_WINDOWMAP_STDMAP) && !defined(_MAP_)
#include <map>
#endif

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#ifndef WINX_WIN_BASICTYPES_H
#include "BasicTypes.h"
#endif

NS_WINX_BEGIN

// =========================================================================
// class WindowMapEx

#if defined(WINX_WINDOWMAP_STDMAP)

template <int nInst>
class WindowMapEx
{
public:
	typedef void* HWndPtr;
	typedef void* PWndPtr;
	typedef std::map<HWndPtr, PWndPtr> _WndMapType;

private:
	static _WndMapType s_wndMap;

public:
	static void* winx_call GetWindow(HWND hWnd)
	{
		_WndMapType::const_iterator it = s_wndMap.find(hWnd);
		if (it != s_wndMap.end())
			return (*it).second;
		else
			return NULL;
	}
	
	static void winx_call SetWindow(HWND hWnd, void* pWnd)
	{
		WINX_ASSERT(GetWindow(hWnd) == NULL);

		s_wndMap[hWnd] = pWnd;
	}
	
	static void winx_call RemoveWindow(HWND hWnd)
	{
		s_wndMap.erase(hWnd);
	}

	static void winx_call DestroyWindow(HWND hWnd)
	{
		s_wndMap.erase(hWnd);
	}
};

template <int nInst>
typename WindowMapEx<nInst>::_WndMapType WindowMapEx<nInst>::s_wndMap;

#else
// -------------------------------------------------------------------------

template <int nInst>
class WindowMapEx
{
private:
	static TCHAR s_szProp[];

public:
	static void* winx_call GetWindow(HWND hWnd)
	{
		return (void*)::GetProp(hWnd, s_szProp);
	}
	
	static void winx_call SetWindow(HWND hWnd, void* pWnd)
	{
		WINX_ASSERT(GetWindow(hWnd) == NULL);

		::SetProp(hWnd, s_szProp, (HANDLE)pWnd);
	}
	
	static void winx_call RemoveWindow(HWND hWnd)
	{
		::RemoveProp(hWnd, s_szProp);
	}

	static void winx_call DestroyWindow(HWND hWnd)
	{
	}
};

#ifndef WINX_APP
#define	WINX_APP		'w', 'i', 'n', 'x'
#endif

template <int nInst>
TCHAR WindowMapEx<nInst>::s_szProp[] = {
	WINX_APP, ':',
	'0' + (nInst >> 4),
	'0' + (nInst & 0x0f),
	'\0'
};

#endif

// -------------------------------------------------------------------------
// WindowCategory

enum WindowCategory
{
	cateNormalWindow = 0x00,
	cateSubclassWindow = cateNormalWindow,
	cateLightSubclassWindow = cateNormalWindow,
	cateIAutoCompletePtr = 0x96,
	cateAnchorWindow = 0x97,
	cateMenuWindow = 0x98,
	cateHostWindow = 0x99,
};

// -------------------------------------------------------------------------
// class NormalWindowMap

#if defined(WINX_WINDOWMAP_SIMPLE)

class NormalWindowMap
{
public:
	static void* winx_call GetWindow(HWND hWnd)
	{
		return (void*)GetWindowLong(hWnd, GWL_USERDATA);
	}

	static void winx_call SetWindow(HWND hWnd, void* pWnd)
	{
		WINX_ASSERT(GetWindow(hWnd) == NULL);

		SetWindowLong(hWnd, GWL_USERDATA, (LONG)pWnd);
	}
	
	static void winx_call RemoveWindow(HWND hWnd)
	{
		SetWindowLong(hWnd, GWL_USERDATA, 0);
	}

	static void winx_call DestroyWindow(HWND hWnd)
	{
	}
};

#else

typedef WindowMapEx<cateNormalWindow> NormalWindowMap;

#endif

// =========================================================================
// IsWndClassName

inline BOOL winx_call IsWndClassName(HWND hWnd, LPCTSTR szClassReq)
{
	const INT nMaxCount = (INT)_tcslen(szClassReq) + 2;
	const INT cbBytes = nMaxCount * sizeof(TCHAR);
	TCHAR* szClass = (TCHAR*)_alloca(cbBytes);
	const INT nCount = ::GetClassName(hWnd, szClass, nMaxCount + 2);
	return (nCount == nMaxCount - 2) &&
		memcmp(szClass, szClassReq, nMaxCount-sizeof(TCHAR)) == 0;
}

// -------------------------------------------------------------------------
// SafeGetInstance/SafeGetDlgItemInstance
// GetInstance/GetDlgItemInstance

template <class WindowClass>
inline WindowClass* winx_call SafeGetInstance(
	WindowClass** ppWnd, HWND hWnd)
{
	typedef typename WindowClass::WindowMap WindowMap;

	LPCTSTR szClassReq = WindowClass::GetWndClassName();
	WINX_ASSERT(
		IsWndClassName(hWnd, szClassReq)
		);

	if (IsWndClassName(hWnd, szClassReq))
		return *ppWnd = (WindowClass*)WindowMap::GetWindow(hWnd);
	else
		return *ppWnd = NULL;
}

template <class WindowClass>
inline WindowClass* winx_call SafeGetDlgItemInstance(
	WindowClass** ppWnd, HWND hDlg, UINT uID)
{
	return SafeGetInstance(ppWnd, ::GetDlgItem(hDlg, uID));
}

template <class WindowClass>
inline WindowClass* winx_call GetInstance(
	WindowClass** ppWnd, HWND hWnd)
{
	typedef typename WindowClass::WindowMap WindowMap;
	WINX_ASSERT(
		IsWndClassName(hWnd, WindowClass::GetWndClassName())
		);

	return *ppWnd = (WindowClass*)WindowMap::GetWindow(hWnd);
}

template <class WindowClass>
inline WindowClass* winx_call GetDlgItemInstance(
	WindowClass** ppWnd, HWND hDlg, UINT uID)
{
	return GetInstance(ppWnd, ::GetDlgItem(hDlg, uID));
}

// =========================================================================
// PackedMessageMap

struct PackedMessage
{
	LPCVOID thisPtr; // verify
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};

#if defined(WINX_MESSAGEMAP_USERDATA)

#if defined(WINX_WINDOWMAP_SIMPLE)
#error "WindowMap conflict with MessageMap!!!"
#endif

class PackedMessageMap
{
public:
	static const PackedMessage& winx_call GetLastMsg(HWND hWnd)
	{
		const PackedMessage* msg = (const PackedMessage*)::GetWindowLong(hWnd, GWL_USERDATA);
		if (msg == NULL)
		{
			WINX_REPORT("GetLastMsg error - unexpected");
			static PackedMessage _null_msg;
			return _null_msg;
		}
		return *msg;
	}
	
	static const PackedMessage* winx_call SetLastMsg(HWND hWnd, const PackedMessage* msg)
	{
		return (const PackedMessage*)::SetWindowLong(hWnd, GWL_USERDATA, (LONG)msg);
	}
};

#else

#if defined(WINX_MT_MESSAGEMAP)
#define _WINX_MESSAGE_MT_THREAD	__declspec(thread)
#else
#define _WINX_MESSAGE_MT_THREAD
#endif

template <int nInst>
class PackedMessageMapEx
{
private:
	static _WINX_MESSAGE_MT_THREAD const PackedMessage* m_lastMsg;

public:
	static const PackedMessage& winx_call GetLastMsg(HWND hWnd)
	{
		WINX_ASSERT(m_lastMsg != NULL);
		return *m_lastMsg;
	}

	static const PackedMessage* winx_call SetLastMsg(HWND hWnd, const PackedMessage* msg)
	{
		const PackedMessage* lastMsg  = m_lastMsg;
		m_lastMsg = msg;
		return lastMsg;
	}
};

template <int nInst>
_WINX_MESSAGE_MT_THREAD const PackedMessage* PackedMessageMapEx<nInst>::m_lastMsg;

typedef PackedMessageMapEx<0> PackedMessageMap;

#endif

// =========================================================================
// $Log: WindowMap.h,v $
// Revision 1.3  2006/12/11 05:19:08  xushiwei
// vs2005 __w64 support
//
// Revision 1.2  2006/08/23 05:47:06  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
// WINX-CommonDialogs:
//   OpenFileDialog/SaveFileDialog, OpenMultiFilesDialog/SaveMultiFilesDialog
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

NS_WINX_END

#endif /* WINX_WIN_WINDOWMAP_H */
