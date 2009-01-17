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
// Module: winx/win/WindowHandle.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 16:19:47
// 
// $Id: WindowHandle.h,v 1.3 2006/09/13 17:05:11 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_WINDOWHANDLE_H__
#define __WINX_WIN_WINDOWHANDLE_H__

#if (0)
#define WINX_NULL_WINDOW_HANDLE
#define WINX_NO_WTLPORT
#endif

#ifndef __WINX_WIN_BASICTYPES_H__
#include "BasicTypes.h"
#endif

__WINX_BEGIN

// =========================================================================
// IsChildWindow/MoveToScreenRightMost/MoveWindowScreenPos/SetWindowScreenPos

inline BOOL winx_call IsChildWindow(HWND hWnd)
{
	return (WS_CHILD & ::GetWindowLong(hWnd, GWL_STYLE));
}

inline void winx_call MoveToScreenRightMost(HWND hWnd)
{
	WINX_ASSERT(!IsChildWindow(hWnd));

	RECT rcMe;
	GetWindowRect(hWnd, &rcMe);
	
	INT cxWidth = GetSystemMetrics(SM_CXMAXIMIZED) - 2*GetSystemMetrics(SM_CXFRAME);
	
	SetWindowPos(
		hWnd, NULL,
		cxWidth - (rcMe.right - rcMe.left), rcMe.top,
		0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

inline VOID winx_call MoveWindowScreenPos(HWND hWnd, LONG x, LONG y) // “∆∂Ø
{
	UINT uStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	if (WS_CHILD & uStyle)
	{
		POINT pt = { x, y };
		HWND hWndParent = ::GetParent(hWnd);
		::ScreenToClient(hWndParent, &pt);
		::SetWindowPos(
			hWnd, NULL, pt.x, pt.y, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		::SetWindowPos(
			hWnd, NULL, x, y, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

inline VOID winx_call SetWindowScreenPos(HWND hWnd, const RECT& rcWin) // …Ë÷√Œª÷√
{
	UINT uStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	if (WS_CHILD & uStyle)
	{
		POINT pt = { rcWin.left, rcWin.top };
		::ScreenToClient(::GetParent(hWnd), &pt);
		::SetWindowPos(
			hWnd, NULL, 
			pt.x, pt.y, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		::SetWindowPos(
			hWnd, NULL, 
			rcWin.left, rcWin.top, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

// =========================================================================
// BringWindowToTopMost

inline BOOL winx_call BringWindowToTopMost(HWND hWnd)
{
	return ::SetWindowPos(
		hWnd, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
}

// =========================================================================
// SetWindowIcon - Change a window's icon

inline HICON winx_call SetWindowIcon(
	HWND hWnd, HICON hIconNew, WPARAM iconType = ICON_BIG)
{
	return (HICON)::SendMessage(hWnd, WM_SETICON, iconType, (LPARAM)hIconNew);
}

inline HICON winx_call GetWindowIcon(
	HWND hWnd, WPARAM iconType = ICON_BIG)
{
	return (HICON)::SendMessage(hWnd, WM_GETICON, iconType, 0);
}

inline HICON winx_call UpdateWindowIcon(
	HWND hWndDest, HWND hWndSrc, WPARAM iconType)
{
	return SetWindowIcon(hWndDest, GetWindowIcon(hWndSrc, iconType), iconType);
}

inline VOID winx_call UpdateWindowIcon(
	HWND hWndDest, HWND hWndSrc)
{
	UpdateWindowIcon(hWndDest, hWndSrc, ICON_BIG);
	UpdateWindowIcon(hWndDest, hWndSrc, ICON_SMALL);
}

// =========================================================================
// Change a window's style

inline BOOL winx_call ModifyWindowStyle(
	HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_STYLE)
{
	WINX_ASSERT(hWnd != NULL);
	DWORD dwStyle = ::GetWindowLong(hWnd, nStyleOffset);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle == dwNewStyle)
		return FALSE;
	
	::SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
	::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
	return TRUE;
}

inline BOOL winx_call ModifyWindowStyleEx(
	HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
{
	return ModifyWindowStyle(hWnd, dwRemove, dwAdd, nFlags, GWL_EXSTYLE);
}

// =========================================================================
// InheritFont

inline VOID winx_call InheritFont(HWND hWnd, BOOL bRedraw = FALSE)
{
	HWND hWndParent = ::GetParent(hWnd);
	HFONT hFontParent = (HFONT)::SendMessage(hWndParent, WM_GETFONT, 0, 0);
	::SendMessage(hWnd, WM_SETFONT, (WPARAM)hFontParent, MAKELPARAM(bRedraw, 0));
}

inline VOID winx_call InheritFont(HWND hDlg, int nID, BOOL bRedraw)
{
	HWND hWndItem = ::GetDlgItem(hDlg, nID);
	if (hWndItem)
	{
		HFONT hFont = (HFONT)::SendMessage(hDlg, WM_GETFONT, 0, 0);
		::SendMessage(hWndItem, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
	}
}

// =========================================================================
// GetWindowText

inline CString winx_call GetWindowText(HWND hWnd)
{
	CString str;
	int nLen = ::GetWindowTextLength(hWnd);
	if (nLen)
	{
		::GetWindowText(hWnd, str.GetBufferSetLength(nLen), nLen+1);
		str.ReleaseBuffer();
	}
	return str;
}

template <class AllocT>
inline std::TString winx_call GetWindowText(AllocT& alloc, HWND hWnd)
{
	int nLen = ::GetWindowTextLength(hWnd);
	if (nLen)
	{
		TCHAR* psz = STD_NEW_ARRAY(alloc, TCHAR, nLen+1);
		::GetWindowText(hWnd, psz, nLen+1);
		return std::TString(psz, nLen);
	}
	return std::TString();
}

template <class StringT>
inline int winx_call GetWindowTextAnsi(HWND hWnd, StringT& rString)
{
	WINX_ASSERT(::IsWindow(hWnd));
	
	int nLen = ::GetWindowTextLengthA(hWnd);
	if (nLen)
	{
		int ret = ::GetWindowTextA(hWnd, std::resize(rString, nLen+1), nLen+1);
		rString.erase(rString.end() - 1);
		return ret;
	}
	rString = StringT();
	return 0;
}

template <class StringT>
inline int winx_call GetWindowTextUni(HWND hWnd, StringT& rString)
{
	WINX_ASSERT(::IsWindow(hWnd));
	
	int nLen = ::GetWindowTextLengthW(hWnd);
	if (nLen)
	{
		int ret = ::GetWindowTextW(hWnd, std::resize(rString, nLen+1), nLen+1);
		rString.erase(rString.end() - 1);
		return ret;
	}
	rString = StringT();
	return 0;
}

__forceinline int winx_call GetWindowText(HWND hWnd, AnsiString& rString)
{
	return GetWindowTextAnsi(hWnd, rString);
}

__forceinline int winx_call GetWindowText(HWND hWnd, UniString& rString)
{
	return GetWindowTextUni(hWnd, rString);
}

inline CString winx_call GetDlgItemText(HWND hDlg, int nID)
{
	HWND hWndItem = ::GetDlgItem(hDlg, nID);
	return GetWindowText(hWndItem);
}

template <class StringT>
inline int winx_call GetDlgItemText(HWND hDlg, int nID, StringT& rString)
{
	HWND hWndItem = ::GetDlgItem(hDlg, nID);
	return GetWindowText(hWndItem, rString);
}

// =========================================================================
// WINX_HANDLE_CLASS

#if defined(_MSC_VER)
#define WINX_HANDLE_CLASS(HandleT)											\
	public:																	\
		HandleT() {}														\
		HandleT(HWND hWnd) { m_hWnd = hWnd; }								\
		void operator=(HWND hWnd) { m_hWnd = hWnd; }						\
	private:
#else
#define WINX_HANDLE_CLASS(HandleT)											\
	public:																	\
		using HandleT::m_hWnd;												\
		HandleT() {}														\
		HandleT(HWND hWnd) { m_hWnd = hWnd; }								\
		void operator=(HWND hWnd) { m_hWnd = hWnd; }						\
	private:
#endif

// -------------------------------------------------------------------------
// HandleClass Extension

template <class HandleClass>
class HandleT : public HandleClass
{
	WINX_HANDLE_CLASS(HandleT);

public:
	BOOL winx_call GetWindowText(BSTR* pbstrText) const {
		return ((HandleClass*)this)->GetWindowText(*pbstrText);
	}
	BOOL winx_call GetWindowText(BSTR& bstrText) const {
		return ((HandleClass*)this)->GetWindowText(bstrText);
	}
	int winx_call GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const {
		return ((HandleClass*)this)->GetWindowText(lpszStringBuf, nMaxCount);
	}
	int winx_call GetWindowText(AnsiString& rString) const {
		return winx::GetWindowText(m_hWnd, rString);
	}
	int winx_call GetWindowText(UniString& rString) const {
		return winx::GetWindowText(m_hWnd, rString);
	}
	CString winx_call GetWindowText() const {
		return winx::GetWindowText(m_hWnd);
	}
	VOID winx_call GetWindowText(CString& strText) const {
		strText = winx::GetWindowText(m_hWnd);
	}

	CString winx_call GetDlgItemText(int nID) const {
		return winx::GetDlgItemText(m_hWnd, nID);
	}
	UINT winx_call GetDlgItemText(int nID, LPTSTR lpStr, int nMaxCount) const {
		return HandleClass::GetDlgItemText(nID, lpStr, nMaxCount);
	}
	BOOL winx_call GetDlgItemText(int nID, BSTR& bstrText) const {
		return HandleClass::GetDlgItemText(nID, bstrText);
	}
	template <class StringT> inline
	int winx_call GetDlgItemText(int nID, StringT& rString) const {
		HWND hWndItem = ::GetDlgItem(m_hWnd, nID);
		return winx::GetWindowText(hWndItem, rString);
	}

	UINT winx_call SetTimer(UINT nIDEvent, UINT nElapse) {
		WINX_ASSERT(::IsWindow(m_hWnd));
		return ::SetTimer(m_hWnd, nIDEvent, nElapse, NULL);
	}
	UINT winx_call SetTimer(UINT nIDEvent, UINT nElapse, TIMERPROC lpTimerFunc) {
		WINX_ASSERT(::IsWindow(m_hWnd));
		return ::SetTimer(m_hWnd, nIDEvent, nElapse, lpTimerFunc);
	}
	
	HandleT<HandleClass>& winx_call DlgItem(HWND hDlg, UINT nIDDlgItem) {
		m_hWnd = ::GetDlgItem(hDlg, nIDDlgItem);
		return *this;
	}

	BOOL winx_call ForwardCommand(WPARAM wParam) {
		return ::SendMessage(m_hWnd, WM_COMMAND, wParam, 0);
	}
};

typedef _WtlCWindow BaseWindowHandle;
typedef HandleT<BaseWindowHandle> WindowHandle;

// =========================================================================
// class NullWindowHandle/WindowHandle

class NullWindowHandle
{
public:
	void winx_call operator=(HWND hWnd) {}
};

// =========================================================================
// DefaultWindowHandle

#if defined(WINX_NULL_WINDOW_HANDLE)
typedef NullWindowHandle DefaultWindowHandle;
#else
typedef WindowHandle DefaultWindowHandle;
#endif

// =========================================================================
// $Log: WindowHandle.h,v $
// Revision 1.3  2006/09/13 17:05:11  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModelDialog/AxModelessDialog
//
// Revision 1.2  2006/08/20 04:49:57  xushiwei
// MFC-Compatibility:
//   GdiObject(CDC, CClientDC, CPaintDC, CBitmap, CPalette, etc),  Diagnost(ASSERT, VERIFY, etc)
//   CreditStatic - Demonstrate how to port MFC code to WINX --- see @@code in source code
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

#endif /* __WINX_WIN_WINDOWHANDLE_H__ */
