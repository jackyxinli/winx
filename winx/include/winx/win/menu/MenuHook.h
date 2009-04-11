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
// Module: winx/win/menu/MenuHook.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Contributor: renfengxing@gmail.com
// Date: 2007-2-9 14:05:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef WINX_WIN_MENU_MENUHOOK_H
#define WINX_WIN_MENU_MENUHOOK_H

#ifndef WINX_APIHOOK_H
#include "../../APIHook.h"
#endif

#ifndef WINX_WIN_WINDOW_H
#include "../Window.h"
#endif

#ifndef WINX_WIN_MENU_MENUITEM_H
#include "MenuItem.h"
#endif

// -------------------------------------------------------------------------

#if (0)
#define WINX_MENUHOOK_TRACE	WINX_TRACE
#else
#define WINX_MENUHOOK_TRACE	__noop
#endif

#ifndef MIIM_STRING
#define MIIM_STRING      0x00000040
#endif

NS_WINX_BEGIN

// -----------------------------------------------------------------------
// struct _ItemInfo

struct _ItemInfo
{
	unsigned char bMenuBar : 1;
	unsigned char nPosition : 7;
	unsigned char height;
	WORD width;

	operator DWORD() const {
		return *(const DWORD*)this;
	}

	_ItemInfo()	
	{
		*(DWORD*)this = 0;
	}
};

// -----------------------------------------------------------------------
// enum WinVer

enum WinVer
{
    wvUndefined,
    wvWin32s,
    wvWin95,
    wvWin98,
    wvWinME,
    wvWinNT3,
    wvWinNT4,
    wvWin2000,
    wvWinXP,
};

// -----------------------------------------------------------------------
// function _GetWinVersion

__forceinline
WinVer WINAPI _GetWinVersion()
{
    static WinVer s_wvVal = wvUndefined;

    if ( s_wvVal != wvUndefined )
    {
        return s_wvVal;
    }
    OSVERSIONINFO osvi;

    ZeroMemory (&osvi, sizeof OSVERSIONINFO);
    osvi.dwOSVersionInfoSize = sizeof OSVERSIONINFO;

    if ( !GetVersionEx (&osvi) )
    {
        return s_wvVal = wvUndefined;
    }
    if ( osvi.dwPlatformId == VER_PLATFORM_WIN32s )
    {
        return s_wvVal = wvWin32s;
    }
    if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )
    {
        if ( osvi.dwMajorVersion == 4L )
        {
            return s_wvVal = wvWinNT4;
        }
        if ( osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L )
        {
            return s_wvVal = wvWin2000;
        }
        if ( osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 1L )
        {
            return s_wvVal = wvWinXP;
        }
        return s_wvVal = wvWinNT3;
    }
    WINX_ASSERT(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);

    if ( osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 10L )
    {
        return s_wvVal = wvWin98;
    }
    if ( osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 90L )
    {
        return s_wvVal = wvWinME;
    }
    return s_wvVal = wvWin95;
}

// -------------------------------------------------------------------------
// class TrackPopupMenuExHook

template <class Unused>
class TrackPopupMenuExHookT : public APIHook< TrackPopupMenuExHookT<Unused> >
{
	WINX_API_HOOK("user32.dll", TrackPopupMenuEx);

public:
	static BOOL s_fEnterMenuLoop;
	static BOOL WINAPI Process(
		HMENU hmenu, UINT fuFlags, int x, int y, HWND hwnd, LPTPMPARAMS lptpm)
	{
		s_fEnterMenuLoop = TRUE;
		BOOL f = WINX_CALL_ORGAPI(hmenu, fuFlags & ~TPM_NONOTIFY, x, y, hwnd, lptpm);
		s_fEnterMenuLoop = FALSE;
		return f;
	}
};

template <class Unused>
BOOL TrackPopupMenuExHookT<Unused>::s_fEnterMenuLoop;

typedef TrackPopupMenuExHookT<void> TrackPopupMenuExHook;

#define _Winx_IsEnterMenuLoop()	winx::TrackPopupMenuExHook::s_fEnterMenuLoop

// -------------------------------------------------------------------------
// class MenuParentWindow

class MenuParentWindow :
	public LightSubclassWindow<MenuParentWindow, cateMenuWindow>
{
	WINX_NO_DEFAULT(); // 不使用Default函数。这将优化部分代码。

private:
	static UINT GetItemPosition(HMENU hMenu, UINT itemID)
	{
		WINX_ASSERT(::IsMenu(hMenu));
		int nItem = ::GetMenuItemCount(hMenu);
		for (int i = 0; i < nItem; ++i)
		{
			UINT _itemID = ::GetMenuItemID(hMenu, i);
			if (_itemID == itemID)
				return i;
		}
		return -1;
	}
public:
	BOOL winx_msg OnDrawItem(HWND hWnd, int nIdCtrl, LPDRAWITEMSTRUCT lpDI)
	{
		if (lpDI->CtlType != ODT_MENU)
			return FALSE;

		_ItemInfo* itemInfo = (_ItemInfo*)(&lpDI->itemData);
		winx::MenuItemImage item((HMENU)lpDI->hwndItem, itemInfo->nPosition);
		return item.Draw(lpDI, itemInfo->bMenuBar);
	}
	
	BOOL winx_msg OnMeasureItem(HWND hWnd, int nIDCtl, LPMEASUREITEMSTRUCT pMS)
	{
		if (pMS->CtlType != ODT_MENU)
			return FALSE;

		_ItemInfo* itemInfo = (_ItemInfo*)(&pMS->itemData);
		pMS->itemHeight = itemInfo->height;

		if (itemInfo->bMenuBar)
		{
			pMS->itemWidth = itemInfo->width + TEXTPADDING_MNUBR;
		}
		else
		{
			pMS->itemWidth = IMGWIDTH + IMGPADDING + itemInfo->width + TEXTPADDING + TEXTPADDING + 4;
 		}

		return TRUE;
	}
};

// -----------------------------------------------------------------------
// class MenuWindow

class MenuWindow : 
	public SubclassWindow<MenuWindow, NullWindowHandle, cateMenuWindow>
{
	WINX_NO_DEFAULT(); // 不使用Default函数。这将优化部分代码。

private:
	typedef WindowDC CWindowDC;
	typedef BrushDC CBrushDC;
	typedef PenDC CPenDC;
	typedef Bitmap CBitmap;
	typedef WindowRect CWindowRect;

	CPoint m_ptMenu;
	CRect m_rcMenu;
	CBitmap m_bmpBkGnd;
	HMENU m_hMenu;

public:
	MenuWindow(HMENU hMenu = NULL)
		: m_hMenu(hMenu)
	{
	}

	BOOL winx_msg OnEraseBkgnd(HWND hWnd, HDC hdc)
	{
		WINX_MENUHOOK_TRACE("\nMenuWindow::OnEraseBkgnd");
		if (!IsWindowVisible (hWnd))
		{
			ClientRect rc(hWnd);
			if (m_bmpBkGnd.m_hBitmap != NULL)
			{
				m_bmpBkGnd.DeleteObject();
			}
			CRect rcBkGnd(
				m_ptMenu.x, m_ptMenu.y, rc.right+m_ptMenu.x+10,
				rc.bottom+m_ptMenu.y+10);
			m_bmpBkGnd.Attach(ScreenCapture(rcBkGnd));
		}
		return FALSE;
	}

	VOID winx_msg OnWindowPosChanging(HWND hWnd, WINDOWPOS& pWP)
	{
		WINX_MENUHOOK_TRACE("\nMenuWindow::OnWindowPosChanging");
		if (_GetWinVersion() < wvWinXP)
		{
			pWP.cx += SM_CXSHADOW;
			pWP.cy += SM_CXSHADOW;
		}
		pWP.y--;
		m_ptMenu.x = pWP.x;
		m_ptMenu.y = pWP.y;
	}
	
	VOID winx_msg OnNcPaint(HWND hWnd, HRGN hrgnClip)
	{
		WINX_MENUHOOK_TRACE("\nMenuWindow::OnNcPaint");

		CWindowDC cDC(hWnd);
		CWindowDC* pDC = &cDC;
		CWindowRect rc(hWnd);

		m_ptMenu.x = rc.left;
		m_ptMenu.y = rc.top;
		rc.OffsetRect (-rc.TopLeft());

		if (rc != m_rcMenu)
		{
			m_rcMenu = rc;

			CPenDC pen (pDC->m_hDC, ::GetSysColor (COLOR_3DDKSHADOW));

			if ( _GetWinVersion() < wvWinXP )
			{
				rc.right -= SM_CXSHADOW;
				rc.bottom -= SM_CXSHADOW;
			}
			pDC->Rectangle(rc);
			pen.Select(HLS_TRANSFORM(::GetSysColor (COLOR_3DFACE), 75, 0));
			rc.DeflateRect(1, 1);
			pDC->Rectangle(rc);
			rc.DeflateRect(1, 1);
			pDC->Rectangle(rc);

			//calc parent menu width
			if (m_hMenu)
			{
				HWND hParent = ::GetParent(hWnd);
				HMENU hParentMenu = ::GetMenu(hParent);			
				if (hParent)
				{
					int n = ::GetMenuItemCount(hParentMenu);
					RECT rcParent = {0, 0, 0, 0};
					for (int i = 0; i < n; ++i)
					{
						HMENU hChildMenu = ::GetSubMenu(hParentMenu, i);
						if (hChildMenu == m_hMenu)
						{
							::GetMenuItemRect(hParent, hParentMenu, i, &rcParent);
							int width = rcParent.right - rcParent.left;
							pen.Select(HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), 20, 0));
							pDC->MoveTo(m_rcMenu.left+1, m_rcMenu.top);
							pDC->LineTo(m_rcMenu.left + width - 5, m_rcMenu.top);
							break;
						}
					}
				}
			}
			
			if (_GetWinVersion() < wvWinXP)
			{
				rc.right += SM_CXSHADOW+2;
				rc.bottom += SM_CXSHADOW+2;
				DrawShadow(pDC->m_hDC, pDC->m_hDC, rc);
			}
		}
	}
};

// -------------------------------------------------------------------------
// class MenuHook

struct _DummyHookExt
{
public:
	static VOID OnInitDlgFrame(HWND hWnd) {}
};

template <class T = _DummyHookExt>
class MenuHook : public MessageHook<MenuHook<T>, WH_CALLWNDPROC>
{
private:
	static T* m_extImpl;

public:
	static VOID OnInitMenuPopup(HWND hWnd, HMENU hMenu)
	{
		SetMenuOwnerDraw(hMenu);
		MenuParentWindow::SubclassOnce(hWnd);
	}

	static VOID OnInitDlgFrame(HWND hWnd)
	{
		HMENU hMenu = ::GetSystemMenu(hWnd, FALSE);
		if (hMenu)
			SetMenuOwnerDraw(hMenu);
	}

public:
	static VOID SetHookExt(T* extImpl)
	{
		m_extImpl = extImpl;
	}

	static LONG winx_call ProcessHookMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		CWPSTRUCT* lpCWP = (CWPSTRUCT*)lParam;
		HWND hWnd = lpCWP->hwnd;
		static HMENU hMenu = NULL;

		switch (lpCWP->message)
		{
		case WM_CREATE:
			LONG style;
			style = ((LPCREATESTRUCT)lpCWP->lParam)->style;
			if (WS_DLGFRAME & style)
			{
				m_extImpl->OnInitDlgFrame(lpCWP->hwnd);

				TCHAR szClass[16];
				if (GetClassName(hWnd, szClass, countof(szClass)) == 6 &&
					_tcscmp(szClass, WINX_TEXT("#32768")) == 0)
				{
					//保存菜单句柄,以便计算菜单之间横线长度
					//如果是右键触发的菜单,hMenu是什么都不重要
					MenuWindow::SubclassOnce(hWnd, hMenu);
				}
			}
			break;

		case WM_INITMENUPOPUP:
			//1:窗口菜单没有这个消息
			//2:菜单右键触发时先发WM_CREATE消息再有WM_INITMENUPOPUP消息
			//3:其他都是先有WM_INITMENUPOPUP再有WM_CREATE
			hMenu = (HMENU)lpCWP->wParam;
			InitMenu(hWnd, hMenu, FALSE);
			MenuParentWindow::SubclassOnce(hWnd);
			break;

		case WM_INITDIALOG:
			MenuParentWindow::SubclassOnce(hWnd);

			HMENU hMenu = ::GetMenu(hWnd);
			if (hMenu)
				InitMenu(hWnd, hMenu, TRUE);

			HMENU hSysMenu = ::GetSystemMenu(hWnd, FALSE);
			if (hSysMenu)
				InitMenu(hWnd, hSysMenu, FALSE);
			break;
		}
		return 0;
	}

private:
	//Calc MenuItem's width, 
	static VOID InitMenu(HWND hWnd, HMENU hMenu, bool bMenuBar = FALSE)
	{
		WINX_ASSERT(::IsMenu(hMenu));
		TCHAR szCaption[256];
		MENUITEMINFO mii = { sizeof(mii) };
		UINT nCount = ::GetMenuItemCount(hMenu);
		HDC hDC = ::GetDC(hWnd);

	    TEXTMETRIC tm;
		::GetTextMetrics(hDC, &tm);
		int nHeight = MAX(tm.tmHeight+4,IMGHEIGHT);
		int nSepHeight = tm.tmHeight/2;
		int nWidth = -1;

		for (int i = nCount; i >= 0; --i)
		{
			mii.fMask = MIIM_FTYPE;
			::GetMenuItemInfo(hMenu, i, TRUE, &mii);

			if (!(MFT_OWNERDRAW & mii.fType))
			{
				mii.fMask = MIIM_FTYPE;

				::GetMenuString(hMenu, i, szCaption, countof(szCaption), MF_BYPOSITION);
				mii.dwTypeData = (LPTSTR)szCaption;

				_ItemInfo itemInfo;
				mii.fMask |= MIIM_DATA;
				mii.fType |= MFT_OWNERDRAW;

				//set bMenu
				itemInfo.bMenuBar = bMenuBar ? 0x00000001 : 0x00000000;

				//set item width
				RECT rc = {0, 0, 0, 0};
				::DrawText(hDC, szCaption, -1, &rc, DT_HIDEPREFIX|DT_SINGLELINE|DT_LEFT|DT_CALCRECT);
				itemInfo.width = nWidth = MAX(nWidth, rc.right-rc.left);

				//set item height
				itemInfo.height = ((mii.fType&MFT_SEPARATOR)==0) ? nHeight : nSepHeight;

				//set item position
				itemInfo.nPosition = i;

				mii.dwItemData = itemInfo;
				::SetMenuItemInfo(hMenu, i, TRUE, &mii);
			}
		}

		::ReleaseDC(hWnd, hDC);
	}
};

template <class T>
T* MenuHook<T>::m_extImpl;

// -------------------------------------------------------------------------
// LookNFeelStyle

enum LookNFeelStyle
{
	lnfAutoStyle = 0,
	lnfXPStyle = 1,
	lnfClassicalStyle = 2,
};

inline LookNFeelStyle winx_call GetLookNFeel(LookNFeelStyle style)
{
	if (style == lnfAutoStyle)
		return lnfXPStyle; //@@todo
	else
		return style;
}

// -------------------------------------------------------------------------
// class MenuLookNFeel

template <class Unused>
class MenuLookNFeelT : public MenuHook<_DummyHookExt>
{
public:
	MenuLookNFeelT(HINSTANCE hInst, LookNFeelStyle style = lnfAutoStyle)
	{
		if (GetLookNFeel(style) == lnfXPStyle)
		{
			TrackPopupMenuExHookT<Unused>::HookGlobal();
			Hook(hInst);
		}
	}
	~MenuLookNFeelT()
	{
		if (IsHooking())
		{
			Unhook();
			TrackPopupMenuExHookT<Unused>::UnhookGlobal();
		}
	}
};

typedef MenuLookNFeelT<void> MenuLookNFeel;

// -------------------------------------------------------------------------
// WINX_APP_MENU

// 注意: 所有WINX_APP_XXX宏，均定义为Application一级属性。
// 而Application一级的属性，实际上通常是一个语句。故此一般出现在WinMain函数中。
#ifndef WINX_APP_MENU
#define WINX_APP_MENU		winx::MenuLookNFeel _winx_appMenu
#endif

// -------------------------------------------------------------------------
// class AppLookNFeel

template <class Unused>
class AppLookNFeelT
{
private:
	AppIconHook m_appNormalStyle;
	MenuHook<AppIconHook> m_appXPStyle;

public:
	AppLookNFeelT(
		HINSTANCE hInst, UINT uIconID, LookNFeelStyle style = lnfAutoStyle)
		: m_appNormalStyle(hInst, uIconID, FALSE)
	{
		if (GetLookNFeel(style) == lnfXPStyle)
		{
			TrackPopupMenuExHookT<Unused>::HookGlobal();
			m_appXPStyle.SetHookExt(&m_appNormalStyle);
			m_appXPStyle.Hook(hInst);
		}
		else
		{
			m_appNormalStyle.Hook(hInst);
		}
	}
	~AppLookNFeelT()
	{
		if (m_appXPStyle.IsHooking())
		{
			m_appXPStyle.Unhook();
			TrackPopupMenuExHookT<Unused>::UnhookGlobal();
		}
		else
		{
			m_appNormalStyle.Unhook();
		}
	}
};

typedef AppLookNFeelT<void> AppLookNFeel;

// -------------------------------------------------------------------------
// WINX_APP_LOOKNFEEL

#ifndef WINX_APP_LOOKNFEEL
#define WINX_APP_LOOKNFEEL	winx::AppLookNFeel _winx_lookNFeel
#endif

// -------------------------------------------------------------------------
// $Log: $

NS_WINX_END

#endif /* WINX_WIN_MENU_MENUHOOK_H */
