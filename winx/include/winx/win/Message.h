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
// Module: winx/win/Message.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 17:57:23
// 
// $Id: Message.h,v 1.9 2006/12/02 15:28:54 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_MESSAGE_H__
#define __WINX_WIN_MESSAGE_H__

#ifndef __WINX_WIN_MESSAGEPROTOTYPE_H__
#include "MessagePrototype.h"
#endif

#ifndef __WINX_WIN_MESSAGEFILTER_H__
#include "MessageFilter.h"
#endif

__WINX_BEGIN

// =========================================================================
// WINX_ON_PAINT - No Double Buffer Support

#define WINX_ON_PAINT()														\
public:																		\
	VOID winx_msg OnPaint(HWND hWnd)										\
	{																		\
		winx::PaintDC hDC(hWnd);											\
		const RECT& rcPaint = hDC.m_ps.rcPaint;								\
		WINX_MSG_HAS(OnDrawBkgnd)											\
			_WINX_PWND->OnDrawBkgnd(hWnd, hDC, rcPaint);					\
		WINX_MSG_HAS(OnDrawEx)												\
			_WINX_PWND->OnDrawEx(hWnd, hDC, rcPaint);						\
		WINX_MSG_ELSE														\
			_WINX_PWND->OnDraw(hWnd, hDC);									\
	}

// =========================================================================
// WINX_ON_PAINT_DB - Double Buffer Support

#define WINX_ON_PAINT_DB()													\
public:																		\
	VOID winx_msg OnPaint(HWND hWnd)										\
	{																		\
		winx::PaintDC dc(hWnd);												\
		winx::DoubleBuffer doubleBuffer;									\
		const RECT& rcPaint = hDC.m_ps.rcPaint;								\
		doubleBuffer.Begin(dc.m_hDC, rcPaint);								\
																			\
		WINX_MSG_HAS(OnDrawBkgnd)											\
			_WINX_PWND->OnDrawBkgnd(hWnd, dc, rcPaint);						\
		WINX_MSG_HAS(OnDrawEx)												\
			_WINX_PWND->OnDrawEx(hWnd, dc, rcPaint);						\
		WINX_MSG_ELSE														\
			_WINX_PWND->OnDraw(hWnd, dc);									\
																			\
		doubleBuffer.End(dc.m_hDC);											\
	}

// =========================================================================
// WINX_ON_PAINT_DB_COND - Double Buffer Selectable Support

#define WINX_ON_PAINT_DB_COND(fDoubleBuffer)								\
public:																		\
	VOID winx_msg OnPaint(HWND hWnd)										\
	{																		\
		winx::PaintDC dc(hWnd);												\
		winx::DoubleBuffer doubleBuffer;									\
		const RECT& rcPaint = hDC.m_ps.rcPaint;								\
		if (fDoubleBuffer)													\
			doubleBuffer.Begin(dc.m_hDC, rcPaint);							\
																			\
		WINX_MSG_HAS(OnDrawBkgnd)											\
			_WINX_PWND->OnDrawBkgnd(hWnd, dc, rcPaint);						\
		WINX_MSG_HAS(OnDrawEx)												\
			_WINX_PWND->OnDrawEx(hWnd, dc, rcPaint);						\
		WINX_MSG_ELSE														\
			_WINX_PWND->OnDraw(hWnd, dc);									\
																			\
		doubleBuffer.End(dc.m_hDC);											\
	}

// =========================================================================
// WINX_ON_PAINT_DB_DBG
//	Double Buffer For Release Version, And Double Buffer Selectable For Debug

#if defined(_DEBUG)
#define WINX_ON_PAINT_DB_DBG(fDoubleBuffer)									\
		WINX_ON_PAINT_DB_COND(fDoubleBuffer)
#else
#define WINX_ON_PAINT_DB_DBG(fDoubleBuffer)									\
		WINX_ON_PAINT_DB()
#endif

// =========================================================================
// WINX_ON_DRAWBKGRND

#define WINX_ON_DRAWBKGRND(hbrBkgnd)										\
public:																		\
	HBRUSH winx_call GetBkgndBrush() {										\
		return (hbrBkgnd);													\
	}																		\
	BOOL winx_msg OnEraseBkgnd(HWND hWnd, HDC hDC) {						\
		return TRUE;														\
	}																		\
	VOID winx_msg OnDrawBkgnd(HWND hWnd, HDC hDC, const RECT& rcPaint) {	\
		::FillRect(hDC, &rcPaint, (hbrBkgnd));								\
	}

// 取值：Windows定义的COLOR_XXX宏。
// 例如：COLOR_WINDOW, COLOR_WINDOWFRAME, etc.
#define WINX_ON_COLOR(color)	WINX_ON_DRAWBKGRND(::GetSysColorBrush(color))

// 取值：Windows定义的StockBrush，即XXX_BRUSH宏。
// 例如：NULL_BRUSH, WHITE_BRUSH, LTGRAY_BRUSH, GRAY_BRUSH, DKGRAY_BRUSH, etc.
#define WINX_ON_BRUSH(brush)	WINX_ON_DRAWBKGRND((HBRUSH)::GetStockObject(brush))

// =========================================================================
// WINX_REFLECT - Reflect/Notify Support

#define WINX_REFLECT()														\
public:																		\
	enum { EnableReflect = TRUE };


#define WINX_NO_REFLECT()													\
public:																		\
	enum { EnableReflect = FALSE };

// =========================================================================
// WINX_IDLE_TIMER

#define WINX_IDLE_TIMER(nElapse)											\
public:																		\
	enum { IdleTimerElapse = nElapse };

#define WINX_NO_IDLE_TIMER()												\
public:																		\
	enum { IdleTimerElapse = 0 };

#ifndef WINX_IDLE_TIMER_ID
#define WINX_IDLE_TIMER_ID		0x7FEE
#endif

#ifndef _WINX_IDLE_TIMER_INIT
#define _WINX_IDLE_TIMER_INIT()												\
	if (_WINX_PWND->IdleTimerElapse && WINX_MSG_HANDLER_VALID(OnIdle))		\
		::SetTimer(hWnd, WINX_IDLE_TIMER_ID, _WINX_PWND->IdleTimerElapse, 0)
#endif

// =========================================================================
// WINX_DRAGLIST_NOTIFY - OnDragListNotify Support

#define WINX_DRAGLIST_NOTIFY()												\
public:																		\
	enum { EnableDragListNotify = TRUE };

// =========================================================================
// class WindowMessage

template <class WindowClass>
class WindowMessage	// : public MessagePrototype
{
public:
	typedef WindowClass WindowImplClass;
	_WINX_MSG_NULL_HANDLES();

#if (0)
	VOID winx_msg OnClose(HWND hWnd);
	VOID winx_msg OnDestroy(HWND hWnd);
	VOID winx_msg OnNcDestroy(HWND hWnd);
	VOID winx_msg OnFinalMessage(HWND hWnd);
	
	LRESULT winx_msg OnSubclass(HWND hWnd);
	VOID winx_msg OnUnsubclass(HWND hWnd);

	LRESULT winx_msg OnCreate(HWND hWnd, LPCREATESTRUCT lpCS);
	LRESULT winx_msg SuperOnCreate(HWND hWnd, LPCREATESTRUCT lpCS);
	
	BOOL winx_msg OnNcCreate(HWND hWnd, LPCREATESTRUCT lpCS);
	BOOL winx_msg OnInitDialog(HWND hDlg, HWND hWndDefaultFocus);
	VOID winx_msg OnPostInitDialog(HWND hDlg);
	HRESULT winx_msg OnDlgInitCreateAxCtrl(HWND hDlg);

	VOID winx_msg DrawItem(HWND hWnd, PDRAWITEMSTRUCT lpDI);
	VOID winx_msg MeasureItem(HWND hWnd, LPMEASUREITEMSTRUCT lpMI);
	BOOL winx_msg OnReflectCommand(HWND hWnd, WPARAM wParam);
	BOOL winx_msg OnReflectNotify(
		HWND hWnd, LPNMHDR pnmh, LRESULT* pResult);

	HCURSOR winx_msg OnQueryDragIcon(HWND hWnd);

	VOID winx_msg OnOK(HWND hWnd);
	BOOL winx_msg OnCommand(
		HWND hWnd, WPARAM wParam, HWND hWndCtlFrom);

	BOOL winx_msg OnSysCommand(
		HWND hWnd, WPARAM wParam, LPARAM lParam);
	
	BOOL winx_msg OnNotify(
		HWND hWnd, int idCtrl, LPNMHDR pnmh, LRESULT* pResult);

	VOID winx_msg OnParentNotify(
		HWND hWnd, WPARAM wParam, LPARAM lParam);

	BOOL winx_msg OnDrawItem(
		HWND hWnd, int nIdCtrl, LPDRAWITEMSTRUCT lpDI);

	BOOL winx_msg OnMeasureItem(
		HWND hWnd, int nIdCtrl, LPMEASUREITEMSTRUCT lpMI);
	
	VOID winx_msg OnDraw(HWND hWnd, HDC hDC);
	VOID winx_msg OnDrawEx(HWND hWnd, HDC hDC, const RECT& rcPaint);
	VOID winx_msg OnDrawBkgnd(HWND hWnd, HDC hDC, const RECT& rcPaint);
	VOID winx_msg OnPaint(HWND hWnd);
	VOID winx_msg OnNcPaint(HWND hWnd, HRGN hrgnClip);
	BOOL winx_msg OnXPThemeNcPaint(HWND hWnd, HRGN hrgnClip);
	VOID winx_msg OnNcCalcSize(HWND hWnd, BOOL bCalcValidRects, NCCALCSIZE_PARAMS& ncsp);
	
	BOOL winx_msg OnEraseBkgnd(HWND hWnd, HDC hDC);

	VOID winx_msg OnContextMenu(HWND hWnd, winx::CPoint pos);
	VOID winx_msg OnInitMenuPopup(HWND hWnd, HMENU hMenu, LPARAM lParam);

	VOID winx_msg OnWindowPosChanged(HWND hWnd, const WINDOWPOS& wndPos);
	VOID winx_msg OnWindowPosChanging(HWND hWnd, WINDOWPOS& wndPos);

	VOID winx_msg OnSize(HWND hWnd, UINT nType, int cx, int cy);
	VOID winx_msg OnSizing(HWND hWnd, UINT fwSide, LPRECT pRect);
	VOID winx_msg OnMove(HWND hWnd, int x, int y);
	VOID winx_msg OnMoving(HWND hWnd, UINT fwSide, LPRECT pRect);

	VOID winx_msg OnGetMinMaxInfo(HWND hWnd, LPMINMAXINFO lpMM);

	VOID winx_msg OnThemeChanged(HWND hWnd);
	VOID winx_msg OnSettingChange(HWND hWnd, UINT uFlags, LPCTSTR lpszSection);
		
	HBRUSH winx_msg OnCtlColor(
		HWND hWnd, HDC hDC, HWND hWndCtrl, UINT nCtlColor);

	BOOL winx_msg OnSetFocus(HWND hWnd, HWND hWndPrevFocus);
	BOOL winx_msg OnKillFocus(HWND hWnd, HWND hWndNewFocus);
	VOID winx_msg OnEnable(HWND hWnd, BOOL fEnable);

	VOID winx_msg OnMouseMove(HWND hWnd, UINT nFlags, winx::CPoint point);
	VOID winx_msg OnLButtonDown(HWND hWnd, UINT nFlags, winx::CPoint point);
	VOID winx_msg OnLButtonUp(HWND hWnd, UINT nFlags, winx::CPoint point);
	VOID winx_msg OnLButtonDblClk(HWND hWnd, UINT nFlags, winx::CPoint point);
	VOID winx_msg OnRButtonDown(HWND hWnd, UINT nFlags, winx::CPoint point);
	VOID winx_msg OnRButtonUp(HWND hWnd, UINT nFlags, winx::CPoint point);
	VOID winx_msg OnRButtonDblClk(HWND hWnd, UINT nFlags, winx::CPoint point);
	VOID winx_msg OnMouseWheel(
		HWND hWnd, UINT nFlags, INT zDelta, winx::CPoint point);

	BOOL winx_msg OnSetCursor(
		HWND hWnd, HWND hWndContainsCursor, UINT nHitTest, UINT uMouseMsg);

	UINT winx_msg OnNcHitTest(
		HWND hWnd, winx::CPoint point);

	VOID winx_msg OnHScroll(HWND hWnd, UINT nSBCode, INT nPos, HWND hWndScrollBar);
	VOID winx_msg OnVScroll(HWND hWnd, UINT nSBCode, INT nPos, HWND hWndScrollBar);
	
	VOID winx_msg OnChar(HWND hWnd, UINT uChar, UINT uKeyData);
	VOID winx_msg OnKeyDown(HWND hWnd, UINT uVKChar, UINT uKeyData);
	VOID winx_msg OnKeyUp(HWND hWnd, UINT uVKChar, UINT uKeyData);
	VOID winx_msg OnSysKeyDown(HWND hWnd, UINT uVKChar, UINT uKeyData);
	VOID winx_msg OnSysKeyUp(HWND hWnd, UINT uVKChar, UINT uKeyData);

	VOID winx_msg OnTimer(HWND hWnd, UINT nIDEvent);
	VOID winx_msg OnIdle(HWND hWnd);

	BOOL winx_msg TestTranslateAccel(HWND hWnd, LPMSG lpMsg);
	BOOL winx_msg OnTranslateAccel(HWND hWnd, LPMSG lpMsg);

	VOID winx_msg HandleUpdateViews(HWND hWnd);

	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	BOOL winx_msg ProcessHostMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	BOOL winx_call ProcessUpdateUIMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	BOOL winx_call ProcessDialogResizeMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	BOOL winx_call ProcessScrollWindowMessage(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	BOOL winx_call ProcessUserMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
#endif

public:
	VOID winx_msg DoNothing(HWND hWnd)
	{
	}

public:
	HBRUSH winx_call GetBkgndBrush() const { return NULL; }
	HBRUSH winx_call SafeGetBkgndBrush(
		HWND hWnd, HDC hDC, UINT nCtlColor,
		HBRUSH hbrDefault = (HBRUSH)::GetSysColorBrush(COLOR_WINDOW))
	{
		HBRUSH hbrBkgnd = _WINX_PWND->GetBkgndBrush();
		if (hbrBkgnd != NULL)
			return hbrBkgnd;

		QueryCtlColorParam query = { hWnd, NULL, hDC };
		HWND hWndParent = ::GetParent(hWnd);
		SendMessage(hWndParent, WINX_WM_CTLCOLOR, nCtlColor, (LPARAM)&query);
		return (query.hbrCtl ? query.hbrCtl : hbrDefault);
	}
	VOID winx_msg OnQueryCtlColor(
		HWND hWnd, UINT nCtlColor, QueryCtlColorParam* query)
	{
		query->hbrCtl = (HBRUSH)_WINX_PWND->OnCtlColor(
			hWnd, query->hCtlDC, query->hWndCtl, nCtlColor
			);
		if (query->hbrCtl == GetStockObject(NULL_BRUSH))
			query->hbrCtl = _WINX_PWND->GetBkgndBrush();
	}

public:
	// OnDragListNotify

	BOOL winx_msg DragList_OnBeginDrag(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		return TRUE; // allow dragging
	}

	LONG winx_msg DragList_OnDragging(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		int nItem = ::LBItemFromPt(lpDLI->hWnd, lpDLI->ptCursor, TRUE);
		::DrawInsert(hWnd, lpDLI->hWnd, nItem);
		return DL_CURSORSET; // don't change cursor
	}
	
	VOID winx_msg DragList_OnCancelDrag(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		::DrawInsert(hWnd, lpDLI->hWnd, -1);
	}
		
	VOID winx_msg DragList_OnDropped(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		::DrawInsert(hWnd, lpDLI->hWnd, -1);
	}

	LRESULT winx_msg OnDragListNotify(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		UINT message = lpDLI->uNotification;
		
		WINX_MSG_CASE(DL_DRAGGING, DragList_OnDragging)
		{
			return _WINX_PWND->DragList_OnDragging(hWnd, idCtrl, lpDLI);
		}
		WINX_MSG_CASE(DL_BEGINDRAG, DragList_OnBeginDrag)
		{
			return _WINX_PWND->DragList_OnBeginDrag(hWnd, idCtrl, lpDLI);
		}
		WINX_MSG_CASE(DL_DROPPED, DragList_OnDropped)
		{
			_WINX_PWND->DragList_OnDropped(hWnd, idCtrl, lpDLI);
		}
		WINX_MSG_CASE(DL_CANCELDRAG, DragList_OnCancelDrag)
		{
			_WINX_PWND->DragList_OnCancelDrag(hWnd, idCtrl, lpDLI);
		}
		return 0;
	}
	
public:
	enum { EnableReflect = FALSE };
	enum { EnableWindowBehavior = FALSE };
	enum { EnableDragListNotify = FALSE };
	enum { IdleTimerElapse = 100 };

	typedef WindowMessage<WindowClass> BehaviorClass;

public:
	// DispatchMessage

	BOOL winx_call DispatchMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		WINX_MSG_CASE(WM_PAINT, OnPaint)
		{
			_WINX_PWND->OnPaint(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(WM_NCPAINT, OnXPThemeNcPaint)
		{
			if (_WINX_PWND->OnXPThemeNcPaint(hWnd, (HRGN)wParam))
				return TRUE;
			goto NcPaint;
		}
		WINX_MSG_CASE(WM_NCPAINT, OnNcPaint)
		{
NcPaint:	_WINX_PWND->OnNcPaint(hWnd, (HRGN)wParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_NCCALCSIZE, OnNcCalcSize)
		{
			_WINX_PWND->OnNcCalcSize(hWnd, (BOOL)wParam, *(NCCALCSIZE_PARAMS*)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_MOUSEMOVE, OnMouseMove)
		{
			_WINX_PWND->OnMouseMove(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_NCHITTEST, OnNcHitTest)
		{
			lResult = _WINX_PWND->OnNcHitTest(hWnd, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_SETCURSOR, OnSetCursor)
		{
			lResult = _WINX_PWND->OnSetCursor(hWnd, (HWND)wParam, LOWORD(lParam), HIWORD(lParam));
			return TRUE;
		}
		WINX_MSG_CASE(WM_ERASEBKGND, OnEraseBkgnd)
		{
			lResult = _WINX_PWND->OnEraseBkgnd(hWnd, (HDC)wParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_DRAWITEM, OnDrawItem)
		{
			if (_WINX_PWND->OnDrawItem(hWnd, wParam, (LPDRAWITEMSTRUCT)lParam))
				return lResult = TRUE;
			goto ReflectProcess;
		}
		WINX_MSG_CASE(WM_MEASUREITEM, OnMeasureItem)
		{
			if (_WINX_PWND->OnMeasureItem(hWnd, wParam, (LPMEASUREITEMSTRUCT)lParam))
				return lResult = TRUE;
			goto ReflectProcess;
		}
		WINX_MSG_CASE(WM_LBUTTONDOWN, OnLButtonDown)
		{
			_WINX_PWND->OnLButtonDown(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_RBUTTONDOWN, OnRButtonDown)
		{
			_WINX_PWND->OnRButtonDown(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_LBUTTONUP, OnLButtonUp)
		{
			_WINX_PWND->OnLButtonUp(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_RBUTTONUP, OnRButtonUp)
		{
			_WINX_PWND->OnRButtonUp(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		{
			_WINX_PWND->OnLButtonDblClk(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_RBUTTONDBLCLK, OnRButtonDblClk)
		{
			_WINX_PWND->OnRButtonDblClk(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_MOUSEWHEEL, OnMouseWheel)
		{
			_WINX_PWND->OnMouseWheel(hWnd, LOWORD(wParam), (SHORT)HIWORD(wParam), lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_VSCROLL, OnVScroll)
		{
			_WINX_PWND->OnVScroll(
				hWnd, LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_HSCROLL, OnHScroll)
		{
			_WINX_PWND->OnHScroll(
				hWnd, LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_PARENTNOTIFY, OnParentNotify)
		{
			_WINX_PWND->OnParentNotify(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_COMMAND, OnCommand)
		{
			if (_WINX_PWND->OnCommand(hWnd, wParam, (HWND)lParam))
				return lResult = TRUE;
			if (WindowClass::EnableReflect && lParam)
				goto ReflectProcess;
			return FALSE;
		}
		WINX_MSG_CASE(WM_FORWARDMSG, OnTranslateAccel)
		{
			return lResult = _WINX_PWND->OnTranslateAccel(hWnd, (LPMSG)lParam);
		}
		WINX_MSG_CASE(WM_NOTIFY, OnNotify)
		{
			if (_WINX_PWND->OnNotify(hWnd, wParam, (LPNMHDR)lParam, &lResult))
				return TRUE;
			goto ReflectProcess;
		}
		WINX_MSG_IF(WindowClass::EnableDragListNotify)
		{
			WINX_MSG_CASE(GetDragListMessage(), OnDragListNotify)
			{
				lResult = _WINX_PWND->OnDragListNotify(hWnd, wParam, (LPDRAGLISTINFO)lParam);
				return TRUE;
			}
		}
		WINX_MSG_CASE(WM_CHAR, OnChar)
		{
			_WINX_PWND->OnChar(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_KEYDOWN, OnKeyDown)
		{
			_WINX_PWND->OnKeyDown(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_KEYUP, OnKeyUp)
		{
			_WINX_PWND->OnKeyUp(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_SYSKEYDOWN, OnSysKeyDown)
		{
			_WINX_PWND->OnSysKeyDown(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_SYSKEYUP, OnSysKeyUp)
		{
			_WINX_PWND->OnSysKeyUp(hWnd, wParam, lParam);
			return TRUE;
		}
		WINX_MSG_IF((_WINX_PWND->IdleTimerElapse && WINX_MSG_HANDLER_VALID(OnIdle)) || WINX_MSG_HANDLER_VALID(OnTimer))
		{
			if (message == WM_TIMER) 
			{
				if (_WINX_PWND->IdleTimerElapse)
				{
					if (wParam == WINX_IDLE_TIMER_ID)
					{
						_WINX_PWND->OnIdle(hWnd);
						return TRUE;
					}
				}
				_WINX_PWND->OnTimer(hWnd, wParam);
				return TRUE;
			}
		}
		WINX_MSG_CASE(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		{
			_WINX_PWND->OnWindowPosChanging(hWnd, *(LPWINDOWPOS)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		{
			_WINX_PWND->OnWindowPosChanged(hWnd, *(LPWINDOWPOS)lParam);
			return TRUE;
		}
		// void OnMove(HWND hWnd, int x, int y)
		WINX_MSG_CASE(WM_MOVE, OnMove)
		{
			_WINX_PWND->OnMove(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		// void OnMoving(HWND hWnd, UINT fwSide, LPRECT pRect)
		WINX_MSG_CASE(WM_MOVING, OnMoving)
		{
			_WINX_PWND->OnMoving(hWnd, (UINT)wParam, (LPRECT)lParam);
			return TRUE;
		}
		// void OnSize(HWND hWnd, UINT nType, int cx, int cy)
		WINX_MSG_CASE(WM_SIZE, OnSize)
		{
			_WINX_PWND->OnSize(hWnd, (UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		// void OnSizing(HWND hWnd, UINT fwSide, LPRECT pRect)
		WINX_MSG_CASE(WM_SIZING, OnSizing)
		{
			_WINX_PWND->OnSizing(hWnd, (UINT)wParam, (LPRECT)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		{
			_WINX_PWND->OnGetMinMaxInfo(hWnd, (LPMINMAXINFO)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_CONTEXTMENU, OnContextMenu)
		{
			_WINX_PWND->OnContextMenu(hWnd, lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_INITMENUPOPUP, OnInitMenuPopup)
		{
			_WINX_PWND->OnInitMenuPopup(hWnd, (HMENU)wParam, lParam);
			return TRUE;
		}
		//void OnMenuSelect(HWND hWnd , UINT nItemID, UINT nFlags, HMENU hSysMenu)
		WINX_MSG_CASE(WM_MENUSELECT, OnMenuSelect)
		{
			_WINX_PWND->OnMenuSelect(hWnd, (UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)(lParam));
			return 0;
		}
		WINX_MSG_CASE(WM_SYSCOMMAND, OnSysCommand)
		{
			if (_WINX_PWND->OnSysCommand(hWnd, wParam, lParam))
				return lResult = TRUE;
			return FALSE;
		}
		WINX_MSG_CASE(WM_ENABLE, OnEnable)
		{
			_WINX_PWND->OnEnable(hWnd, wParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_SETFOCUS, OnSetFocus)
		{
			lResult = _WINX_PWND->OnSetFocus(hWnd, (HWND)wParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_KILLFOCUS, OnKillFocus)
		{
			lResult = _WINX_PWND->OnKillFocus(hWnd, (HWND)wParam);
			return TRUE;
		}
		WINX_MSG_HAS(OnCtlColor)
		{
			switch (message)
			{
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORSTATIC:
			case WM_CTLCOLOREDIT:
			case WM_CTLCOLORLISTBOX:
			case WM_CTLCOLORDLG:
			case WM_CTLCOLORSCROLLBAR:
			case WM_CTLCOLORMSGBOX:
				lResult = (LRESULT)_WINX_PWND->OnCtlColor(
					hWnd, (HDC)wParam, (HWND)lParam, message);
				return TRUE;
			case WINX_WM_CTLCOLOR:
				_WINX_PWND->OnQueryCtlColor(
					hWnd, wParam, (QueryCtlColorParam*)lParam);
				return TRUE;
			}
		}
		WINX_MSG_IF(WindowClass::EnableReflect)
		{
		ReflectProcess:
			if (ReflectNotifications(hWnd, message, wParam, lParam, lResult))
				return TRUE;
		}
		WINX_MSG_CASE(WM_QUERYDRAGICON, OnQueryDragIcon)
		{
			lResult = (LRESULT)_WINX_PWND->OnQueryDragIcon(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(WM_CLOSE, OnClose)
		{
			_WINX_PWND->OnClose(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(WM_CREATE, SuperOnCreate)
		{
			WINX_MSG_HAS(InternalDefault)
				_WINX_PWND->InternalDefault(hWnd, message, wParam, lParam);
			lResult = _WINX_PWND->SuperOnCreate(hWnd, (LPCREATESTRUCT)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WM_DESTROY, OnDestroy)
		{
			_WINX_PWND->OnDestroy(hWnd);
			return FALSE;
		}
		WINX_MSG_NCDESTROY();
		WINX_MSG_CASE(WM_THEMECHANGED, OnThemeChanged)
		{
			_WINX_PWND->OnThemeChanged(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(WM_SETTINGCHANGE, OnSettingChange)
		{
			_WINX_PWND->OnSettingChange(hWnd, wParam, (LPCTSTR)lParam);
			return 0;
		}
		return FALSE;
	}

public:
	// HandleMessage

	LRESULT winx_msg HandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_HAS(HandleUpdateViews)
		{
			_WINX_PWND->HandleUpdateViews(hWnd);
		}

		LRESULT lResult = 0;
		WINX_MSG_HAS(ProcessHostMessage)
		{
			if (_WINX_PWND->ProcessHostMessage(hWnd, message, wParam, lParam, &lResult))
				return lResult;
		}
		WINX_MSG_IF(WindowClass::EnableWindowBehavior)
		{
			typename WindowClass::BehaviorClass* pBehavior = _WINX_PWND;
			if (pBehavior->PreHandleMessage(hWnd, message, wParam, lParam, &lResult))
				return lResult;
		}
		WINX_MSG_HAS(ProcessUpdateUIMessage)
		{
			// support WTL::CUpdateUI
			if (_WINX_PWND->ProcessUpdateUIMessage(hWnd, message, wParam, lParam, lResult))
				return lResult;
		}
		WINX_MSG_HAS(DispatchMessage)
		{
			// DispatchMessage可能被派生类禁止。
			if (_WINX_PWND->DispatchMessage(hWnd, message, wParam, lParam, lResult))
				return lResult;
		}
		WINX_MSG_HAS(ProcessUserMessage)
		{
			if (_WINX_PWND->ProcessUserMessage(hWnd, message, wParam, lParam, lResult))
				return lResult;
		}
		WINX_MSG_HAS(ProcessDialogResizeMessage)
		{
			// support WTL::CDialogResize
			if (_WINX_PWND->ProcessDialogResizeMessage(hWnd, message, wParam, lParam, lResult))
				return lResult;
		}
		WINX_MSG_HAS(ProcessScrollWindowMessage)
		{
			// support WTL::CScrollImpl
			if (_WINX_PWND->ProcessScrollWindowMessage(hWnd, message, wParam, lParam, lResult))
				return lResult;
		}
		return _WINX_PWND->DefaultHandle(hWnd, message, wParam, lParam);
	}

public:
	// Default

	LRESULT winx_call Default()
	{
		WINX_MSG_HAS(InternalDefault)
		{
			HWND hWnd = _WINX_PWND->m_hWnd;
			const PackedMessage& msg = PackedMessageMap::GetLastMsg(hWnd);
			WINX_ASSERT(msg.thisPtr == _WINX_PWND);
			WINX_ASSERT(msg.message != WM_NCDESTROY);
			return _WINX_PWND->InternalDefault(hWnd, msg.message, msg.wParam, msg.lParam);
		}
		WINX_MSG_ELSE
		{
			return FALSE;
		}
	}

	LRESULT winx_call Default(HWND hWnd)
	{
		WINX_MSG_HAS(InternalDefault)
		{
			const PackedMessage& msg = PackedMessageMap::GetLastMsg(hWnd);
			WINX_ASSERT(msg.thisPtr == _WINX_PWND);
			WINX_ASSERT(msg.message != WM_NCDESTROY);
			return _WINX_PWND->InternalDefault(hWnd, msg.message, msg.wParam, msg.lParam);
		}
		WINX_MSG_ELSE
		{
			return FALSE;
		}
	}

public:
	// ProcessDefault

	static LRESULT winx_call ProcessDefault(HWND hWnd)
	{
		WINX_MSG_HAS(InternalDefault)
		{
			const PackedMessage& msg = PackedMessageMap::GetLastMsg(hWnd);
			WindowImplClass* pWnd = (WindowImplClass*)msg.thisPtr;
			WINX_ASSERT(msg.message != WM_NCDESTROY);
			return pWnd->InternalDefault(hWnd, msg.message, msg.wParam, msg.lParam);
		}
		WINX_MSG_ELSE
		{
			return FALSE;
		}
	}

public:
	// ProcessMessage

	LRESULT winx_call ProcessMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_HAS(InternalDefault)
		{
			PackedMessage msg = { _WINX_PWND, message, wParam, lParam };
			const PackedMessage* msgOld = PackedMessageMap::SetLastMsg(hWnd, &msg);
			LRESULT lResult = _WINX_PWND->HandleMessage(hWnd, message, wParam, lParam);
			PackedMessageMap::SetLastMsg(hWnd, msgOld);
			return lResult;
		}
		WINX_MSG_ELSE
		{
			return _WINX_PWND->HandleMessage(hWnd, message, wParam, lParam);
		}
	}

public:
	// --> CreateWindow

	HWND winx_call Create(
		HWND hWndParent = NULL, LPCTSTR szCaption = NULL,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		int dwStyle = -1, int dwExStyle = -1,
		MENUorID hMenuOrID = 0, HINSTANCE hInst = GetThisModule())
	{
		enum { DefaultWindowStyle = WindowClass::DefaultWindowStyle };
		enum { DefaultWindowExStyle = WindowClass::DefaultWindowExStyle };

		if (dwStyle == -1)
			dwStyle = DefaultWindowStyle;

		if (dwExStyle == -1)
			dwExStyle = DefaultWindowExStyle;

		return ::CreateWindowEx(
			dwExStyle,
			WindowClass::GetWndClassName(),
			szCaption,
			dwStyle,
			x, y, nWidth, nHeight,
			hWndParent,
			(HMENU)hMenuOrID,
			hInst,
			_WINX_PWND
			);
	}

public:
	// --> Enumerations

	enum CtlMaskType // used by WINX_CTLBKGND_XXX, eg. WINX_CTLBKGND_BRUSH
	{
		CtlMsgBox		= 0x0001,
		CtlEdit			= 0x0002,
		CtlListBox		= 0x0004,
		CtlButton		= 0x0008,
		CtlDialog		= 0x0010,
		CtlScrollBar	= 0x0020,
		CtlStatic		= 0x0040,
	};

	enum TranslateAccelState // used by TestTranslateAccel as retval
	{
		CanTranslateAccel = 0,
		CannotTranslateAccel = 1,
	};
};

// =========================================================================
// $Log: Message.h,v $
// Revision 1.9  2006/12/02 15:28:54  xushiwei
// WM_CLOSE, WM_QUERYDRAGICON
//
// Revision 1.8  2006/09/18 03:34:15  xushiwei
// WINX-Core: Message: OnIdle support
//
// Revision 1.7  2006/09/16 18:07:10  xushiwei
// MFC-Port:
//   Demo - port MFC controls to WINX (DimEditCtrl)
//
// Revision 1.6  2006/09/14 14:44:01  xushiwei
// Winx-Extension-ActiveX support bugfix (Visual Studio 2005):
//  Failed to create ActiveX controls in dialog resource template.
//  (Reason: AtlAxDialogBox/AtlAxCreateDialog implementation was changed in high version ATL.)
//
// Revision 1.5  2006/08/25 15:13:19  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//
// Revision 1.4  2006/08/23 05:47:06  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
// WINX-CommonDialogs:
//   OpenFileDialog/SaveFileDialog, OpenMultiFilesDialog/SaveMultiFilesDialog
//
// Revision 1.3  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//
// Revision 1.2  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.1  2006/08/19 10:50:55  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//

__WINX_END

#endif /* __WINX_WIN_MESSAGE_H__ */
