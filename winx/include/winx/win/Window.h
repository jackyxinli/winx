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
// Module: winx/win/Window.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:05:53
// 
// $Id: Window.h,v 1.7 2006/11/23 06:12:53 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_WINDOW_H__
#define __WINX_WIN_WINDOW_H__

#ifndef __WINX_WIN_WINDOWHANDLE_H__
#include "WindowHandle.h"
#endif

#ifndef __WINX_WIN_MESSAGE_H__
#include "Message.h"
#endif

__WINX_BEGIN

// =========================================================================
// WINX_CLASS, WINX_CLASS_STYLE

#define WINX_CLASS_EX(ClassName)											\
public:																		\
	static LPCTSTR winx_call GetWndClassName() {							\
		return ClassName;													\
	}																		\
private:

#define WINX_CLASS_STYLE(style)												\
public:																		\
	static UINT winx_call GetClassStyle() { return (style); }				\
private:

#define WINX_DEFAULT_BKGND(brush)											\
public:																		\
	static HBRUSH winx_call GetDefaultBkgnd() { return (brush); }			\
private:

#define WINX_DEFAULT_CURSOR(cursor)											\
public:																		\
	static HCURSOR winx_call GetDefaultCursor() { return (cursor); }		\
private:

// CLASS，可取任意与现有控件不同类名的字符串。
#define WINX_CLASS(ClassName)		WINX_CLASS_EX(WINX_TEXT(ClassName))

// COLOR，可取：COLOR_WINDOW, COLOR_WINDOWFRAME, etc.
#define WINX_DEFAULT_COLOR(color)	WINX_DEFAULT_BKGND(::GetSysColorBrush(color))

// BRUSH，可取：BLACK_BRUSH, WHITE_BRUSH, GRAY_BRUSH, etc.
#define WINX_DEFAULT_BRUSH(brush)	WINX_DEFAULT_BKGND((HBRUSH)::GetStockObject(brush))

// SYSCURSOR，可取：IDC_ARROW, IDC_IBEAM, IDC_WAIT, IDC_CROSS, IDC_UPARROW, etc.
#define WINX_DEFAULT_SYSCURSOR(syscursor)									\
	WINX_DEFAULT_CURSOR(::LoadCursor(NULL, syscursor))

// =========================================================================
// WINX_DEFAULT_STYLE, WINX_DEFAULT_EXSTYLE, WINX_STACK_OBJECT

#define WINX_DEFAULT_STYLE(style)											\
public:																		\
	enum { DefaultWindowStyle = style }

#define WINX_DEFAULT_EXSTYLE(style)											\
public:																		\
	enum { DefaultWindowExStyle = style }

#define WINX_STACK_OBJECT(is)												\
public:																		\
	enum { StackWindowObject = is }

#define WINX_DELETE_THIS_PWND()												\
	if (!WindowImplClass::StackWindowObject)								\
		WINX_DELETE(_WINX_PWND)

// =========================================================================
// class Window

template <class WindowClass, class HandleClass = DefaultWindowHandle>
class Window : 
	public WindowMessage<WindowClass>,
	public HandleClass
{
	WINX_CLASS_STYLE(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS);
	WINX_DEFAULT_COLOR(COLOR_WINDOW);
	WINX_DEFAULT_SYSCURSOR(IDC_ARROW);
	WINX_DEFAULT_STYLE(WS_OVERLAPPEDWINDOW);
	WINX_DEFAULT_EXSTYLE(0);
	WINX_STACK_OBJECT(FALSE);
	WINX_ON_INITIAL();
	WINX_ON_TERMINATE();
	
public:
	using WindowMessage<WindowClass>::Create;

	typedef WindowClass WindowImplClass;
	typedef WindowMessage<WindowClass> BaseClass;
	typedef Window<WindowClass, HandleClass> WindowBase;
	typedef NormalWindowMap WindowMap;

public:
	VOID winx_call ProcessTermMessage(HWND hWnd)
	{
		WindowMap::DestroyWindow(hWnd);
		WINX_DELETE_THIS_PWND();
	}

public:
	static LRESULT CALLBACK WindowProc(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWnd = (WindowClass*)WindowMap::GetWindow(hWnd);
		if (pWnd == NULL)
		{
			if (message != WM_NCCREATE)
				return pWnd->InternalDefault(hWnd, message, wParam, lParam);
			LPCREATESTRUCT lpCS = (LPCREATESTRUCT)lParam;
			if (lpCS->lpCreateParams) {
				pWnd = (WindowClass*)lpCS->lpCreateParams;
				lpCS->lpCreateParams = NULL;
			}
			else {
				if (WindowClass::StackWindowObject) {
					WINX_ASSERT(!"WindowClass::StackWindowObject - unexpected!");
					return FALSE;
				}
				else {
					pWnd = WINX_NEW(WindowClass);
				}
			}
			WindowMap::SetWindow(hWnd, pWnd);
		}
		return pWnd->ProcessMessage(hWnd, message, wParam, lParam);
	}

public:
	static LRESULT winx_msg InternalDefault(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT winx_msg DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_HAS2(OnNcCreate, OnInitial)
		{
			if (message == WM_NCCREATE) // 先构造父类，然后构造子类。
			{
				WINX_MSG_HAS(OnInitial)
					_WINX_PWND->OnInitial(hWnd);
				WINX_MSG_HAS(OnNcCreate)
					return _WINX_PWND->OnNcCreate(hWnd, (LPCREATESTRUCT)lParam);
				goto DoDef;
			}
		}
		WINX_MSG_HAS(OnCreate)
		{
			if (message == WM_CREATE)
				return _WINX_PWND->OnCreate(hWnd, (LPCREATESTRUCT)lParam);
		}
DoDef:	return _WINX_PWND->InternalDefault(hWnd, message, wParam, lParam);
	}
	
public:
	static WindowClass* winx_call GetInstance(HWND hWnd)
	{
		WindowClass* pWnd;
		return winx::GetInstance(&pWnd, hWnd);
	}

	static WindowClass* winx_call GetDlgItemInstance(HWND hDlg, UINT uID)
	{
		WindowClass* pWnd;
		return winx::GetInstance(&pWnd, ::GetDlgItem(hDlg, uID));
	}

	static ATOM winx_call RegisterClass(
		IN HINSTANCE hInst = GetThisModule())
	{
		WNDCLASSEX wcex =
		{
			sizeof(WNDCLASSEX),				// cbSize
			WindowClass::GetClassStyle(),	// style
			WindowClass::WindowProc,		// lpfnWndProc
			0,								// cbClsExtra
			0,								// cbWndExtra
			hInst,							// hInstance
			NULL,							// hIcon
			WindowClass::GetDefaultCursor(),// hCursor
			WindowClass::GetDefaultBkgnd(),	// hbrBackground
			NULL,							// lpszMenuName
			WindowClass::GetWndClassName(),	// lpszClassName
			NULL							// hIconSm
		};
		return ::RegisterClassEx(&wcex);
	}
};

// =========================================================================
// ExGetClassInfo - Helper Function, for Superclass技术

inline BOOL winx_call ExGetClassInfo(
	IN HINSTANCE hInst, IN LPCTSTR szClass, OUT WNDCLASSEX& wcex)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	// try process local class first
	if (!::GetClassInfoEx(hInst, szClass, &wcex))
	{
		// try global class
		return ::GetClassInfoEx(NULL, szClass, &wcex);
	}
	return TRUE;
}

inline BOOL winx_call SafeGetClassInfo(HINSTANCE hInst, WNDCLASSEX& wcexOrg)
{
	ZeroMemory(&wcexOrg, sizeof(wcexOrg));
	wcexOrg.cbSize = sizeof(wcexOrg);
	wcexOrg.lpfnWndProc = ::DefWindowProc;
	wcexOrg.style = CS_HREDRAW|CS_VREDRAW;
	wcexOrg.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	return TRUE;
}

// =========================================================================
// class SuperclassRegister

template <class WindowClass, class HandleClass>
class SuperclassRegister
{
public:
	static LPCTSTR winx_call GetOrgWndClassName()
	{
		return HandleClass::GetWndClassName();
	}
	
	static WNDPROC& winx_call OrgWndProc()
	{
		static WNDPROC lpfnWndProc;
		return lpfnWndProc;
	}
	
	static BOOL winx_call AltGetClassInfo(
		HINSTANCE hInst, WNDCLASSEX& wcexOrg)
	{
		return FALSE;
	}
	
	static ATOM winx_call RegisterClass(
		IN HINSTANCE hInst = GetThisModule())
	{
		WNDCLASSEX wcexOrg;
		if (!ExGetClassInfo(hInst, WindowClass::GetOrgWndClassName(), wcexOrg))
		{
			if (!WindowClass::AltGetClassInfo(hInst, wcexOrg))
				return 0;
		}
		
		OrgWndProc() = wcexOrg.lpfnWndProc;
		
		WNDCLASSEX wcex =
		{
			sizeof(WNDCLASSEX),				// cbSize
			wcexOrg.style,					// style
			WindowClass::SuperclassWndProc,	// lpfnWndProc
			wcexOrg.cbClsExtra,				// cbClsExtra
			wcexOrg.cbWndExtra,				// cbWndExtra
			hInst,							// hInstance
			wcexOrg.hIcon,					// hIcon
			wcexOrg.hCursor,				// hCursor
			wcexOrg.hbrBackground,			// hbrBackground
			wcexOrg.lpszMenuName,			// lpszMenuName
			WindowClass::GetWndClassName(),	// lpszClassName
			wcexOrg.hIconSm					// hIconSm
		};
		return ::RegisterClassEx(&wcex);
	}
};

// =========================================================================
// SubclassWindow

template <
	class WindowClass, class HandleClass = DefaultWindowHandle,
	int nInst = cateSubclassWindow>
class SubclassWindow : 
	public WindowMessage<WindowClass>,
	public HandleClass,
	public SuperclassRegister<WindowClass, HandleClass>
{
	WINX_ON_INITIAL();
	WINX_ON_TERMINATE();
	WINX_STACK_OBJECT(FALSE);
	
public:
	using WindowMessage<WindowClass>::Create;
	
	typedef WindowClass WindowImplClass;
	typedef WindowMessage<WindowClass> BaseClass;
	typedef SubclassWindow<WindowClass, HandleClass, nInst> WindowBase;
	typedef WindowMapEx<nInst> WindowMap;
	
public:
	WNDPROC _m_prevProc;

#if (0)
	LRESULT winx_msg OnSubclass(HWND hWnd) { return 0; }
	VOID winx_msg OnUnsubclass(HWND hWnd) {}
#endif
	
	VOID CALLBACK ProcessTermMessage(HWND hWnd, BOOL fForceTerminate = FALSE)
	{
		WINX_MSG_HAS(OnUnsubclass)
			_WINX_PWND->OnUnsubclass(hWnd);

		WindowMap::RemoveWindow(hWnd);
		
		LONG lCurrProc = ::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)_m_prevProc);
		WINX_ASSERT(
			(WNDPROC)lCurrProc == WindowClass::WindowProc ||
			(WNDPROC)lCurrProc == WindowClass::SuperclassWndProc
			);
		
#if !defined(WINX_NO_RESEND_DESTROY)
		if (!fForceTerminate) {
			// 如果是因为调用Unsubclass进行强制退出的，不能发'WM_NCDESTROY'消息。
			WINX_TRY {
				::CallWindowProc(_m_prevProc, hWnd, WM_NCDESTROY, 0, 0);
			} WINX_TRY_END;
		}
#endif

		WINX_DELETE_THIS_PWND();
	}

	static LRESULT CALLBACK WindowProc(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWnd = (WindowClass*)WindowMap::GetWindow(hWnd);
		WINX_ASSERT(pWnd != NULL);

		return pWnd->ProcessMessage(hWnd, message, wParam, lParam);
	}

	static LRESULT CALLBACK SuperclassWndProc(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWnd;
		pWnd = (WindowClass*)WindowMap::GetWindow(hWnd);
		if (pWnd == NULL)
		{
			WNDPROC fnOrgWndProc;
			LRESULT lResult;
			fnOrgWndProc = OrgWndProc();
			lResult = ::CallWindowProc(fnOrgWndProc, hWnd, message, wParam, lParam);
			if (message == WM_CREATE)
			{
				LPCREATESTRUCT lpCS = (LPCREATESTRUCT)lParam;
				if (lpCS->lpCreateParams) {
					pWnd = (WindowClass*)lpCS->lpCreateParams;
					lpCS->lpCreateParams = NULL;
				}
				else {
					if (WindowClass::StackWindowObject) {
						WINX_ASSERT(!"WindowClass::StackWindowObject - unexpected!");
						return -1;
					}
					else {
						pWnd = WINX_NEW(WindowClass);
					}
				}
				pWnd->_m_prevProc = fnOrgWndProc;
				WindowMap::SetWindow(hWnd, pWnd);

				WINX_MSG_HAS(SuperOnCreate)
					return pWnd->SuperOnCreate(hWnd, (LPCREATESTRUCT)lParam);
				WINX_MSG_ELSE
					pWnd->_FireOnSubclass(hWnd);
			}
			return lResult;
		}
		return pWnd->ProcessMessage(hWnd, message, wParam, lParam);
	}
	
public:
	SubclassWindow() : _m_prevProc(NULL)
	{
	}

	BOOL winx_msg InternalDefault(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(_m_prevProc, hWnd, message, wParam, lParam);
	}

	LRESULT winx_msg DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(_m_prevProc, hWnd, message, wParam, lParam);
	}

public:
	static WindowClass* winx_call UnsafeGetInstance(HWND hWnd)
	{
		return (WindowClass*)WindowMap::GetWindow(hWnd);
	}

	static WindowClass* winx_call GetInstance(HWND hWnd)
	{
		WINX_ASSERT(
			IsWndClassName(hWnd, WindowClass::GetOrgWndClassName()) ||
			IsWndClassName(hWnd, WindowClass::GetWndClassName())
			);
		return (WindowClass*)WindowMap::GetWindow(hWnd);
	}

	static WindowClass* winx_call GetDlgItemInstance(HWND hDlg, UINT uID)
	{
		return GetInstance(::GetDlgItem(hDlg, uID));
	}

	BOOL winx_call _FireOnSubclass(HWND hWnd)
	{
		WINX_MSG_HAS2(OnSubclass, OnInitial)
		{
			WINX_MSG_HAS(OnInitial)
				_WINX_PWND->OnInitial(hWnd);
			WINX_MSG_HAS(OnSubclass)
				_WINX_PWND->OnSubclass(hWnd);
		}
		return TRUE;
	}

	BOOL winx_call Subclass(HWND hWnd)
	{
		WINX_ASSERT(WindowMap::GetWindow(hWnd) == NULL);
		WINX_ASSERT(_m_prevProc == NULL);

		WindowMap::SetWindow(hWnd, _WINX_PWND);
		_m_prevProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)WindowClass::WindowProc);
		WINX_ASSERT(_m_prevProc != NULL);

		return _FireOnSubclass(hWnd);
	}

	VOID winx_call Unsubclass(HWND hWnd)
	{
		WINX_MSG_HAS(OnTerminate)
			_WINX_PWND->OnTerminate(hWnd);
		WINX_MSG_HAS(ProcessTermMessage)
			_WINX_PWND->ProcessTermMessage(hWnd, TRUE);
	}
	
	BOOL winx_call SubclassDlgItem(HWND hDlg, int nIDDlgItem)
	{
		HWND hDlgItem = ::GetDlgItem(hDlg, nIDDlgItem);
		if (hDlgItem)
			return Subclass(hDlgItem);
		else
			return FALSE;
	}

	BOOL winx_call SubclassAxDlgItem(HWND hDlg, int nIDDlgItem)
	{
		HWND hDlgItem = ::GetDlgItem(hDlg, nIDDlgItem);
		if (hDlgItem)
		{
			HWND hAxCtrl = ::GetTopWindow(hDlgItem);
			if (hAxCtrl)
			{
#if defined(_DEBUG)
				TCHAR szClass[64];
				::GetClassName(hAxCtrl, szClass, countof(szClass));
#endif
				return Subclass(hAxCtrl);
			}
		}
		return NULL;
	}

	static WindowClass* winx_call SubclassOnce(HWND hWnd)
	{
		WINX_ASSERT(!WindowClass::StackWindowObject);

		if (WindowClass::StackWindowObject) {
			return NULL;
		}
		else {
			void* lPrevUserData = WindowMap::GetWindow(hWnd);
			if (lPrevUserData != 0) {
				return (WindowClass*)lPrevUserData;
			}
			else {
				WindowClass* pWnd = WINX_NEW(WindowClass);
				WINX_VERIFY(pWnd->Subclass(hWnd));
				return pWnd;
			}
		}
	}
	
	static WindowClass* winx_call SubclassDlgItemOnce(HWND hDlg, int nIDDlgItem)
	{
		HWND hDlgItem = ::GetDlgItem(hDlg, nIDDlgItem);
		if (hDlgItem)
			return SubclassOnce(hDlgItem);
		else
			return NULL;
	}

	static WindowClass* winx_call DoSubclass(HWND hWnd)
	{
		WINX_ASSERT(!WindowClass::StackWindowObject);
		
		if (WindowClass::StackWindowObject) {
			return NULL;
		}
		else {
			WindowClass* pWnd = WINX_NEW(WindowClass);
			WINX_VERIFY(pWnd->Subclass(hWnd));
			return pWnd;
		}
	}

	static WindowClass* winx_call DoSubclassDlgItem(HWND hDlg, int nIDDlgItem)
	{
		HWND hDlgItem = ::GetDlgItem(hDlg, nIDDlgItem);
		if (hDlgItem)
			return DoSubclass(hDlgItem);
		else
			return NULL;
	}
	
	static WindowClass* winx_call DoSubclassAxDlgItem(HWND hDlg, int nIDDlgItem)
	{
		WINX_ASSERT(!WindowClass::StackWindowObject);

		if (WindowClass::StackWindowObject) {
			return NULL;
		}
		else {
			WindowClass* pWnd = WINX_NEW(WindowClass);
			WINX_VERIFY(pWnd->SubclassAxDlgItem(hDlg, nIDDlgItem));
			return pWnd;
		}
	}
};

// =========================================================================
// Subclass, SubclassDlgItem, SubclassOnce, SubclassDlgItemOnce

template <class WindowClass>
__forceinline VOID winx_call Subclass(WindowClass** ppWnd, HWND hWnd)
{
	*ppWnd = WindowClass::DoSubclass(hWnd);
}

template <class WindowClass>
__forceinline VOID winx_call SubclassDlgItem(WindowClass** ppWnd, HWND hDlg, int nIDDlgItem)
{
	*ppWnd = WindowClass::DoSubclassDlgItem(hDlg, nIDDlgItem);
}

template <class WindowClass>
__forceinline VOID winx_call SubclassOnce(WindowClass** ppWnd, HWND hWnd)
{
	*ppWnd = WindowClass::SubclassOnce(hWnd);
}

template <class WindowClass>
__forceinline VOID winx_call SubclassDlgItemOnce(WindowClass** ppWnd, HWND hDlg, int nIDDlgItem)
{
	*ppWnd = WindowClass::SubclassDlgItemOnce(hDlg, nIDDlgItem);
}

// =========================================================================
// class LightSubclassWindow

#ifndef WINX_ASSERT_NULLCLASS
#define WINX_ASSERT_NULLCLASS(WindowClass)	WINX_ASSERT(sizeof(WindowClass) < 4)
#endif

template <
	class WindowClass,
	int nInst = cateLightSubclassWindow,
	class BaseClass = WindowMessage<WindowClass> >
class LightSubclassWindow : public BaseClass
{
public:
	typedef WindowClass WindowImplClass;
	typedef WindowMapEx<nInst> WindowMap;

	static LRESULT CALLBACK DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WNDPROC prevProc = (WNDPROC)WindowMap::GetWindow(hWnd);
		WINX_ASSERT(prevProc != NULL);			
		
		return ::CallWindowProc(prevProc, hWnd, message, wParam, lParam);
	}

	static LRESULT CALLBACK WindowProc(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_HAS(DispatchMessage)
		{
			LRESULT lResult;
			if (_WINX_NULL_PWND->DispatchMessage(hWnd, message, wParam, lParam, lResult))
			{
				return lResult;
			}
		}

		WNDPROC prevProc = (WNDPROC)WindowMap::GetWindow(hWnd);
		WINX_ASSERT(prevProc != NULL);

		if (message == WM_NCDESTROY)
		{
			WindowMap::DestroyWindow(hWnd);
			LONG lCurrProc = ::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)prevProc);
			WINX_ASSERT(
				(WNDPROC)lCurrProc == WindowClass::WindowProc
				);
		}
		return ::CallWindowProc(prevProc, hWnd, message, wParam, lParam);
	}

	static BOOL winx_call Subclass(HWND hWnd)
	{
		WINX_ASSERT_NULLCLASS(WindowClass);
		WINX_ASSERT(WindowMap::GetWindow(hWnd) == NULL);

		WNDPROC prevProc = (WNDPROC)::SetWindowLong(
			hWnd, GWL_WNDPROC, (LONG)WindowClass::WindowProc);

		WINX_ASSERT(prevProc != NULL);
		WindowMap::SetWindow(hWnd, prevProc);
		return TRUE;
	}

	static BOOL winx_call SubclassDlgItem(HWND hDlg, int nIDDlgItem)
	{
		HWND hDlgItem = ::GetDlgItem(hDlg, nIDDlgItem);
		if (hDlgItem)
			return Subclass(hDlgItem);
		return FALSE;
	}

	static BOOL winx_call SubclassAxDlgItem(HWND hDlg, int nIDDlgItem)
	{
		HWND hDlgItem = ::GetDlgItem(hDlg, nIDDlgItem);
		if (hDlgItem)
		{
			HWND hAxCtrl = ::GetTopWindow(hDlgItem);
			if (hAxCtrl)
			{
#if defined(_DEBUG)
				TCHAR szClass[64];
				::GetClassName(hAxCtrl, szClass, countof(szClass));
#endif
				return Subclass(hAxCtrl);
			}
		}
		return FALSE;
	}

	static VOID winx_call SubclassOnce(HWND hWnd)
	{
		if (WindowMap::GetWindow(hWnd) == NULL)
			Subclass(hWnd);
	}
};

// =========================================================================
// class LightSuperclassWindow

template <
	class WindowClass,
	class HandleClass = NullWindowHandle,
	class BaseClass = WindowMessage<WindowClass> >
class LightSuperclassWindow : 
	public BaseClass, public SuperclassRegister<WindowClass, HandleClass>
{
public:
	typedef WindowClass WindowImplClass;

	static LRESULT CALLBACK DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(OrgWndProc(), hWnd, message, wParam, lParam);
	}
	
	static LRESULT CALLBACK SuperclassWndProc(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_HAS(DispatchMessage)
		{
			LRESULT lResult;
			if (_WINX_NULL_PWND->DispatchMessage(hWnd, message, wParam, lParam, lResult))
			{
				return lResult;
			}
		}
#if defined(_DEBUG)
		if (message == WM_NCDESTROY)
		{
			LONG lCurrProc = ::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)OrgWndProc());
			WINX_ASSERT(
				(WNDPROC)lCurrProc == WindowClass::SuperclassWndProc
				);
		}
#endif
		return ::CallWindowProc(OrgWndProc(), hWnd, message, wParam, lParam);
	}
};

// =========================================================================
// WINX_SAFEREG_WNDCLASS

#define WINX_ALTCLASS_NULL()												\
public:																		\
	static BOOL winx_call AltGetClassInfo(									\
		HINSTANCE hInst, WNDCLASSEX& wcexOrg)								\
	{																		\
		return winx::SafeGetClassInfo(hInst, wcexOrg);						\
	}																		\
private:

#define WINX_ALTCLASS_EX(AltClassName)										\
public:																		\
	static BOOL winx_call AltGetClassInfo(									\
		HINSTANCE hInst, WNDCLASSEX& wcexOrg)								\
	{																		\
		return winx::ExGetClassInfo(hInst, AltClassName, wcexOrg);			\
	}																		\
private:

#define WINX_ORGCLASS_EX(ClassName)											\
public:																		\
	static LPCTSTR winx_call GetOrgWndClassName() {							\
		return ClassName;													\
	}																		\
private:

#define WINX_SAFECLASS_EX(ClassName)										\
public:																		\
	static LPCTSTR winx_call GetSafeWndClassName() {						\
		return ClassName;													\
	}																		\
private:

#define WINX_ALTCLASS(AltClassName)											\
	WINX_ALTCLASS_EX(WINX_TEXT(AltClassName))

#define WINX_ORGCLASS(ClassName)											\
	WINX_ORGCLASS_EX(WINX_TEXT(ClassName))

#define WINX_SAFECLASS(ClassName)											\
	WINX_SAFECLASS_EX(WINX_TEXT(ClassName))

// =========================================================================
// SafeCtrl

template <class HandleClass, class AltCtrl>
class SafeCtrl : 
	public LightSuperclassWindow<SafeCtrl<HandleClass, AltCtrl> >
{
	WINX_CLASS_EX(AltCtrl::GetSafeWndClassName());
	WINX_ALTCLASS_EX(AltCtrl::GetWndClassName());
	WINX_ORGCLASS_EX(HandleClass::GetWndClassName());
	WINX_MSG_NULL_HANDLER(DispatchMessage); // --> 这一句仅仅是进行代码优化。

private:
	typedef LightSuperclassWindow<SafeCtrl<HandleClass, AltCtrl> > BaseClass;

public:
	static ATOM winx_call RegisterClass(
		IN HINSTANCE hInst = GetThisModule())
	{
		WINX_VERIFY(AltCtrl::RegisterClass(hInst));
		return BaseClass::RegisterClass(hInst);
	}
};

// =========================================================================
// SimpleSafeCtrl

template <class WindowClass, class HandleClass>
class SimpleSafeCtrl : public LightSuperclassWindow<WindowClass>
{
	WINX_ORGCLASS_EX(HandleClass::GetWndClassName());
	WINX_ALTCLASS_NULL();
	WINX_MSG_NULL_HANDLER(DispatchMessage); // --> 这一句仅仅是进行代码优化。
};

// =========================================================================
// $Log: Window.h,v $
// Revision 1.7  2006/11/23 06:12:53  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModalDialog/AxModalessDialog
//
// Revision 1.5  2006/09/03 06:32:13  xushiwei
// WINX-Extension: PropertySheet, PropertyPage
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
// Revision 1.1  2006/08/19 10:50:56  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//

__WINX_END

#endif /* __WINX_WIN_WINDOW_H__ */
