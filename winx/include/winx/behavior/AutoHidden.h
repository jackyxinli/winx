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
// Module: winx/behavior/AutoHidden.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-2 20:42:59
// 
// $Id: AutoHidden.h,v 1.3 2006/11/28 12:45:51 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_BEHAVIOR_AUTOHIDDEN_H__
#define __WINX_BEHAVIOR_AUTOHIDDEN_H__

#ifndef __WINX_BEHAVIOR_WINDOWBEHAVIOR_H__
#include "WindowBehavior.h"
#endif

__WINX_BEGIN

#pragma pack(1)

// =========================================================================
// _MoveWindowScreenPos

inline VOID winx_call _MoveWindowScreenPos(HWND hWnd, LONG x, LONG y)
{
	WINX_ASSERT(!IsChildWindow(hWnd));
	::SetWindowPos(
		hWnd, NULL, x, y, 0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

// -------------------------------------------------------------------------
// class WithinRange - 限制在某个范围内的

class _LimitScreen
{
public:
	static VOID winx_call GetLimitRange(RECT& rcLimit)
	{
		rcLimit.left = rcLimit.top = 0;
		rcLimit.right = GetSystemMetrics(SM_CXSCREEN);
		rcLimit.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
};

class _LimitScreenTop
{
public:
	static VOID winx_call GetLimitRange(RECT& rcLimit)
	{
		rcLimit.left = LONG_MIN;
		rcLimit.top = 0;
		rcLimit.right = LONG_MAX;
		rcLimit.bottom = LONG_MAX;
	}
};

class _LimitScreenLeft
{
public:
	static VOID winx_call GetLimitRange(RECT& rcLimit)
	{
		rcLimit.left = 0;;
		rcLimit.top = LONG_MIN;
		rcLimit.right = LONG_MAX;
		rcLimit.bottom = LONG_MAX;
	}
};

class _LimitScreenRight
{
public:
	static VOID winx_call GetLimitRange(RECT& rcLimit)
	{
		rcLimit.left = LONG_MIN;;
		rcLimit.top = LONG_MIN;
		rcLimit.right = GetSystemMetrics(SM_CXSCREEN);
		rcLimit.bottom = LONG_MAX;
	}
};

template <class RangeClass>
class WithinRange : 
	public WindowBehavior< WithinRange<RangeClass> >,
	public RangeClass
{
private:
	// lpRect -- the current position of the window, in screen coordinates
	VOID winx_msg PreMoving(LPRECT lpRect)
	{
		RECT rcLimit;
		RangeClass::GetLimitRange(rcLimit);

		INT dx, dy;
		if (lpRect->left < rcLimit.left)
			dx = rcLimit.left - lpRect->left;
		else if (lpRect->right > rcLimit.right)
			dx = rcLimit.right - lpRect->right;
		else
			dx = 0;

		if (lpRect->top < rcLimit.top)
			dy = rcLimit.top - lpRect->top;
		else if (lpRect->bottom > rcLimit.bottom)
			dy = rcLimit.bottom - lpRect->bottom;
		else
			dy = 0;

		if (dx || dy)
			OffsetRect(lpRect, dx, dy);
	}

public:
	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch (message)
		{
		case WM_MOVING:
			PreMoving((LPRECT)lParam);
		}
		return FALSE;
	}
};

typedef WithinRange<_LimitScreen> WithinScreen;
typedef WithinRange<_LimitScreenTop> LimitScreenTop;
typedef WithinRange<_LimitScreenLeft> LimitScreenLeft;
typedef WithinRange<_LimitScreenRight> LimitScreenRight;

// =========================================================================
// class AutoHidden

class _HiddenLeft
{
public:
	static VOID winx_msg PreMoving(
		LPRECT lpRect)
	{
		if (lpRect->left < 0)
			::OffsetRect(lpRect, -lpRect->left, 0);
	}

	static BOOL winx_call _Hide(
		LPRECT lpRect, UINT nPixel)
	{
		if (lpRect->left <= 0)
		{
			::OffsetRect(lpRect, nPixel - lpRect->right, 0);
			return TRUE;
		}
		return FALSE;
	}
};

class _HiddenRight
{
public:
	static VOID winx_msg PreMoving(
		LPRECT lpRect)
	{
		LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
		if (lpRect->right > cxScreen)
			::OffsetRect(lpRect, cxScreen - lpRect->right, 0);
	}

	static BOOL winx_call _Hide(
		LPRECT lpRect, UINT nPixel)
	{
		LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
		if (lpRect->right >= cxScreen)
		{
			::OffsetRect(lpRect, cxScreen - lpRect->left - nPixel, 0);
			return TRUE;
		}
		return FALSE;
	}
};

class _HiddenTop
{
public:
	static VOID winx_msg PreMoving(
		LPRECT lpRect)
	{
		if (lpRect->top < 0)
			::OffsetRect(lpRect, 0, -lpRect->top);
	}

	static BOOL winx_call _Hide(
		LPRECT lpRect, UINT nPixel)
	{
		if (lpRect->top <= 0)
		{
			::OffsetRect(lpRect, 0, nPixel - lpRect->bottom);
			return TRUE;
		}
		return FALSE;
	}
};

class _HiddenAnySide
{
public:
	static VOID winx_msg PreMoving(
		LPRECT lpRect)
	{
		INT dx, dy;
		if (lpRect->left < 0)
			dx = -lpRect->left;
		else
		{
			LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
			if (lpRect->right >= cxScreen)
				dx = cxScreen - lpRect->right;
			else
				dx = 0;
		}
		if (lpRect->top < 0)
			dy = -lpRect->top;
		else
			dy = 0;

		if (dx || dy)
			OffsetRect(lpRect, dx, dy);
	}

	static BOOL winx_call _Hide(
		LPRECT lpRect, UINT nPixel)
	{
		return _HiddenTop::_Hide(lpRect, nPixel) ||
			_HiddenLeft::_Hide(lpRect, nPixel) ||
			_HiddenRight::_Hide(lpRect, nPixel);
	}
};

template <class HiddenStyle, class WindowImplClass, UINT nIDTimer = 0x7FED /* 一个比较随机的值 */>
class AutoHidden : public WindowBehavior<AutoHidden<HiddenStyle, WindowImplClass, nIDTimer> >
{
private:
	USHORT m_nPixels;
	// --> AutoHidden窗口在隐藏后，在屏幕中的可见象素点数。
	
	UCHAR m_fEnableAutoHidden;
	// --> 是否允许AutoHidden行为。这个变量没有其实也可以。
	//	   因为我们有更加通用 winx::BehaviorPermit<> 模板做这件事情。
	
	UCHAR m_fTopMost	: 1;	// fTopMost: 内部状态，保留窗口隐藏前的属性。
	UCHAR m_fHidden		: 1;	// fHidden: 当前状态是否隐藏。
	UCHAR m_fInit		: 1;	// fInit: 内部状态，确定是否已经进行初始化。
	UCHAR m_fAwait		: 1;	// fAwait: 一种临时状态，在此状态下，AutoHidden暂时不可用。
	UCHAR m_fActivateApp: 1;	// fActivateApp: 如果为TRUE，在程序活动时不进行自动隐藏。
	UCHAR m_unused		: 3;

public:
	typedef AutoHidden<HiddenStyle, WindowImplClass, nIDTimer> AutoHiddenType;

	struct AutoHiddenParams
	{
		POINT pt;	// current cursor(mouse) position (in screen coordinate)
		RECT rcWin;	// current window rect (in screen coordinate)
	};

	__forceinline
	BOOL winx_msg CannotAutoHidden(HWND hWnd, AutoHiddenParams& para) const
	{
		UINT uStyle = ::GetWindowLong(hWnd, GWL_STYLE);
		if (uStyle & (WS_MAXIMIZE | WS_DISABLED | WS_MINIMIZE))
			return TRUE;
		else
			return ::PtInRect(&para.rcWin, para.pt);
	}

	__forceinline 
	BOOL winx_msg CanAutoShow(HWND hWnd, AutoHiddenParams& para) const
	{
		return ::PtInRect(&para.rcWin, para.pt);
	}
	
private:
	VOID winx_call _ProcessOnTimer(HWND hWnd)
	{
		WINX_ASSERT(!m_fHidden);

		if (m_fAwait)
			return;

		AutoHiddenParams para;
		::GetCursorPos(&para.pt);
		::GetWindowRect(hWnd, &para.rcWin);
		if (_WINX_PWND->CannotAutoHidden(hWnd, para))
			return;

		if (HiddenStyle::_Hide(&para.rcWin, m_nPixels))
		{
			_MoveWindowScreenPos(hWnd, para.rcWin.left, para.rcWin.top);
			m_fHidden = TRUE;
			m_fTopMost = (WS_EX_TOPMOST & ::GetWindowLong(hWnd, GWL_EXSTYLE)) ? TRUE : FALSE;
			::SetWindowPos(
				hWnd, HWND_TOPMOST, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}

	VOID winx_call _ProcessOnHiddenTimer(HWND hWnd)
	{
		WINX_ASSERT(m_fHidden);

		AutoHiddenParams para;
		::GetCursorPos(&para.pt);
		::GetWindowRect(hWnd, &para.rcWin);
		if (m_nPixels == 0)
			::InflateRect(&para.rcWin, 1, 1);

		if (_WINX_PWND->CanAutoShow(hWnd, para))
			_ProcessShow(hWnd);
	}

	VOID winx_call _ProcessShow(HWND hWnd)
	{
		RECT rcWin;
		::GetWindowRect(hWnd, &rcWin);
		
		HiddenStyle::PreMoving(&rcWin);
		_MoveWindowScreenPos(hWnd, rcWin.left, rcWin.top);
		if (!m_fTopMost)
		{
			::SetWindowPos(
				hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		}
		m_fHidden = FALSE;
	}
	
	VOID winx_call _ProcessInit(HWND hWnd)
	{
		m_fInit = TRUE;
		::SetTimer(hWnd, nIDTimer, 600, NULL);
	}

	static VOID winx_call _ProcessDestroy(HWND hWnd)
	{
		::KillTimer(hWnd, nIDTimer);
	}
	
public:
	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (!m_fEnableAutoHidden)
			return FALSE;

		if (m_fHidden)
		{
			switch (message)
			{
#if (0)
			case WM_NCMOUSEMOVE:
			case WM_MOUSEMOVE:
				if (::GetKeyState(VK_LBUTTON) < 0)
					_ProcessShow(hWnd);
				break;
#endif
			case WM_TIMER:
				_ProcessOnHiddenTimer(hWnd);
				break;
			case WM_NCDESTROY:
				_ProcessDestroy(hWnd);
				break;
			}
			return FALSE;
		}
		else
		{
			switch (message)
			{
			case WM_TIMER:
				if (wParam == nIDTimer)
				{
					_ProcessOnTimer(hWnd);
					return TRUE;
				}
				break;
			case WM_MOVING:
				HiddenStyle::PreMoving((LPRECT)lParam);
				break;
			case WM_ACTIVATEAPP:
				if (!wParam)
					m_fAwait = FALSE;
				else if (m_fActivateApp)
					m_fAwait = TRUE;
				break;
			default:
				if (!m_fInit)
					_ProcessInit(hWnd);
				else if (message == WM_NCDESTROY)
					_ProcessDestroy(hWnd);
			}
			return FALSE;
		}
	}

public:
	AutoHidden()
		: m_fInit(FALSE), m_fAwait(FALSE), m_fHidden(FALSE), m_fActivateApp(TRUE),
		  m_nPixels(3), m_fEnableAutoHidden(TRUE)
	{
	}

	VOID winx_call SetPixels(UINT nPixel)
	{
		m_nPixels = nPixel;
	}

	VOID winx_call SetElapse(HWND hWnd, UINT nElapse)
	{
		::SetTimer(hWnd, nIDTimer, nElapse, NULL);
	}

	VOID winx_call SetAutoHidden(HWND hWnd, BOOL fAutoHidden)
	{
		m_fEnableAutoHidden = (UCHAR)fAutoHidden;
		if (!m_fEnableAutoHidden && m_fHidden)
			_ProcessShow(hWnd);
	}

	VOID winx_call SetAwaitFlag(BOOL fAwait)
	{
		m_fAwait = fAwait;
	}

	VOID winx_call SetActivateAppFlag(BOOL fActivateApp)
	{
		m_fActivateApp = fActivateApp;
	}

	VOID winx_call ActivateMe(HWND hWnd, BOOL fAwait = TRUE)
	{
		m_fAwait = fAwait;
		if (m_fHidden)
			_ProcessShow(hWnd);
	}
};

template <class WindowClass>
class AutoHiddenTopT : public AutoHidden<_HiddenTop, WindowClass>
{
public:
	typedef AutoHiddenTopT<WindowClass> AutoHiddenTopType;
};

template <class WindowClass>
class AutoHiddenLeftT : public AutoHidden<_HiddenLeft, WindowClass>
{
public:
	typedef AutoHiddenLeftT<WindowClass> AutoHiddenLeftType;
};

template <class WindowClass>
class AutoHiddenRightT : public AutoHidden<_HiddenRight, WindowClass>
{
public:
	typedef AutoHiddenRightT<WindowClass> AutoHiddenRightType;
};

template <class WindowClass>
class AutoHiddenAnySideT : public AutoHidden<_HiddenAnySide, WindowClass>
{
public:
	typedef AutoHiddenAnySideT<WindowClass> AutoHiddenAnySideType;
};

class AutoHiddenTop : public AutoHiddenTopT<AutoHiddenTop> {};
class AutoHiddenLeft : public AutoHiddenLeftT<AutoHiddenLeft> {};
class AutoHiddenRight : public AutoHiddenRightT<AutoHiddenRight> {};
class AutoHiddenAnySide : public AutoHiddenAnySideT<AutoHiddenAnySide> {};

//
// for backward compatibility
//
typedef AutoHiddenTop AutoHiddenTopI;
typedef AutoHiddenLeft AutoHiddenLeftI;
typedef AutoHiddenRight AutoHiddenRightI;
typedef AutoHiddenAnySide AutoHiddenAnySideI;

// =========================================================================
// $Log: AutoHidden.h,v $
// Revision 1.3  2006/11/28 12:45:51  xushiwei
// simplify WindowBehavior interface
//
// Revision 1.1  2006/09/03 04:32:56  xushiwei
// WINX-Core: Behavior(BehaviorPermit, BehaviorSwitch, AutoHidden, etc)
// WINX-Extension: PropertySheet, PropertyPage
//

#pragma pack()

__WINX_END

#endif /* __WINX_BEHAVIOR_AUTOHIDDEN_H__ */
