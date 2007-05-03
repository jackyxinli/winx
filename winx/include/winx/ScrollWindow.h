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
// Module: winx/ScrollWindow.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-26 15:19:14
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __WINX_SCROLLWINDOW_H__
#define __WINX_SCROLLWINDOW_H__

#ifndef __WINX_WINCORE_H__
#include "WinCore.h"
#endif

#ifdef __ATLSCRL_H__
#error "Don't include atlscrl.h first"
#endif

#define DoPaint	__WTL_DoPaint
#include "wtl/WrapperMessage.h"
#include "../../../wtl/include/atlscrl.h"
#include "wtl/UnWrapperMessage.h"
#undef DoPaint

#if (0) && defined(_DEBUG)
#define _DEBUG_SCROLLWINDOW_MSG
#endif

__WINX_BEGIN

// =========================================================================
// class ScrollImpl

template <class WindowClass>
class ScrollImpl : public WTL::CScrollImpl<WindowClass>
{
public:
	typedef WindowClass WindowImplClass;
	typedef ScrollImpl<WindowClass> ScrollImplClass;

	void winx_msg DoPaint(winx::DCHandle dc) {}

	__forceinline void winx_call __WTL_DoPaint(WTL::CDCHandle dc) {
		_WINX_PWND->DoPaint(dc.m_hDC);
	}

#if !((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)) && !defined(_WIN32_WCE)
	ScrollImpl() { m_uMsgMouseWheel = 0xffffffffU; }
#endif
};

// -------------------------------------------------------------------------
// class ScrollWindow

template <class WindowClass, class Base = Window<WindowClass, WindowHandle> >
class ScrollWindow : public Base, public ScrollImpl<WindowClass>
{
public:
	typedef ScrollWindow BaseClass;
	typedef WindowClass WindowImplClass;
	typedef ScrollImpl<WindowClass> ScrollImplClass;

public:
	BOOL winx_call ProcessScrollWindowMessage(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		return ScrollImplClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult);
	}

	BOOL winx_msg OnCommand(
		HWND hWnd, WPARAM wParam, HWND hWndCtlFrom)
	{
		/* handled commands:
		 *
			COMMAND_ID_HANDLER(ID_SCROLL_UP, OnScrollUp)
			COMMAND_ID_HANDLER(ID_SCROLL_DOWN, OnScrollDown)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_UP, OnScrollPageUp)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_DOWN, OnScrollPageDown)
			COMMAND_ID_HANDLER(ID_SCROLL_TOP, OnScrollTop)
			COMMAND_ID_HANDLER(ID_SCROLL_BOTTOM, OnScrollBottom)
			COMMAND_ID_HANDLER(ID_SCROLL_LEFT, OnScrollLeft)
			COMMAND_ID_HANDLER(ID_SCROLL_RIGHT, OnScrollRight)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_LEFT, OnScrollPageLeft)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_RIGHT, OnScrollPageRight)
			COMMAND_ID_HANDLER(ID_SCROLL_ALL_LEFT, OnScrollAllLeft)
			COMMAND_ID_HANDLER(ID_SCROLL_ALL_RIGHT, OnScrollAllRight)
		*/
		LRESULT lResult = 0;
		BOOL f = ScrollImplClass::ProcessWindowMessage(
			hWnd, WM_COMMAND, wParam, (LPARAM)hWndCtlFrom, lResult, 1);
		if (!f)
			f = Base::OnCommand(hWnd, wParam, hWndCtlFrom);
		return f;
	}
};

// =========================================================================
// class FormWindow, AxFormWindow

template <class WindowClass, class Base>
class _ScrollDlgT : public ScrollWindow<WindowClass, Base>
{
public:
	typedef ScrollWindow<WindowClass, Base> BaseClass;

	BOOL winx_call ProcessScrollWindowMessage(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		if (uMsg == WM_INITDIALOG)
		{
			BOOL f;
			ClientRect rect(m_hWnd = hWnd);
			BaseClass::__WTL_OnCreate(WM_CREATE, 0, 0, f);
			BaseClass::SetScrollSize(rect.right, rect.bottom);
			return FALSE;
		}
		return BaseClass::ProcessScrollWindowMessage(hWnd, uMsg, wParam, lParam, lResult);
	}
};

template <class WindowClass, int nDlgId = 0, class HandleClass = WindowHandle>
class FormWindow : public _ScrollDlgT<WindowClass, ModelessDialog<WindowClass, nDlgId, HandleClass> >
{
};

template <class WindowClass, int nDlgId = 0, class HandleClass = WindowHandle>
class AxFormWindow : public _ScrollDlgT<WindowClass, AxModelessDialog<WindowClass, nDlgId, HandleClass> >
{
};


// =========================================================================
// class ZoomScrollImpl

template <class WindowClass>
class ZoomScrollImpl : public WTL::CZoomScrollImpl<WindowClass>
{
public:
	enum {
		ZOOMMODE_OFF = WTL::ZOOMMODE_OFF,
		ZOOMMODE_IN = WTL::ZOOMMODE_IN, // If left mouse button is clicked or dragged, zoom in on point clicked or rectangle dragged.
		ZOOMMODE_OUT = WTL::ZOOMMODE_OUT, // If left mouse button clicked, zoom out on point clicked.
	};

	typedef WindowClass WindowImplClass;
	typedef ZoomScrollImpl<WindowClass> ScrollImplClass;

	void winx_msg DoPaint(winx::DCHandle dc) {}

	__forceinline void winx_call __WTL_DoPaint(WTL::CDCHandle dc) {
		_WINX_PWND->DoPaint(dc.m_hDC);
	}

#if !((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)) && !defined(_WIN32_WCE)
	ZoomScrollImpl() { m_uMsgMouseWheel = 0xffffffffU; }
#endif
};

// -------------------------------------------------------------------------
// class ZoomScrollWindow

template <class WindowClass, class Base = Window<WindowClass, WindowHandle> >
class ZoomScrollWindow : public Base, public ZoomScrollImpl<WindowClass>
{
public:
	typedef ZoomScrollWindow BaseClass;
	typedef WindowClass WindowImplClass;
	typedef ZoomScrollImpl<WindowClass> ScrollImplClass;

public:
	BOOL winx_call ProcessScrollWindowMessage(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		return ScrollImplClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult);
	}

	BOOL winx_msg OnCommand(
		HWND hWnd, WPARAM wParam, HWND hWndCtlFrom)
	{
		/* handled commands:
		 *
			COMMAND_ID_HANDLER(ID_SCROLL_UP, OnScrollUp)
			COMMAND_ID_HANDLER(ID_SCROLL_DOWN, OnScrollDown)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_UP, OnScrollPageUp)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_DOWN, OnScrollPageDown)
			COMMAND_ID_HANDLER(ID_SCROLL_TOP, OnScrollTop)
			COMMAND_ID_HANDLER(ID_SCROLL_BOTTOM, OnScrollBottom)
			COMMAND_ID_HANDLER(ID_SCROLL_LEFT, OnScrollLeft)
			COMMAND_ID_HANDLER(ID_SCROLL_RIGHT, OnScrollRight)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_LEFT, OnScrollPageLeft)
			COMMAND_ID_HANDLER(ID_SCROLL_PAGE_RIGHT, OnScrollPageRight)
			COMMAND_ID_HANDLER(ID_SCROLL_ALL_LEFT, OnScrollAllLeft)
			COMMAND_ID_HANDLER(ID_SCROLL_ALL_RIGHT, OnScrollAllRight)
		*/
		LRESULT lResult = 0;
		BOOL f = ScrollImplClass::ProcessWindowMessage(
			hWnd, WM_COMMAND, wParam, (LPARAM)hWndCtlFrom, lResult, 1);
		if (!f)
			f = Base::OnCommand(hWnd, wParam, hWndCtlFrom);
		return f;
	}
};

// =========================================================================
// $Log: $

__WINX_END

#endif /* __WINX_SCROLLWINDOW_H__ */
