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
// Module: winx/Theme.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 9:47:19
// 
// $Id: Theme.h,v 1.2 2006/08/25 15:13:18 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_THEME_H__
#define __WINX_THEME_H__

#if (0)
#define WINX_NO_DELAYLOAD_THEME
#endif

#ifndef __WINX_BASIC_H__
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// WINX_USE_WINSDK or WINX_USE_DEFSDK

#if defined(WINX_USE_WINSDK)

#ifndef _UXTHEME_H_
#include "../../../winsdk/include/uxtheme.h"
#endif

#ifndef TMSCHEMA_H
#include "../../../winsdk/include/tmschema.h"
#endif

#else // defined(WINX_USE_DEFSDK)

#if defined(WINX_VC6)
#error "Unsupport Theme!!!"
#endif

#ifndef _UXTHEME_H_
#include <uxtheme.h>
#endif

#ifndef TMSCHEMA_H
#include <tmschema.h>
#endif

#endif // defined(WINX_USE_DEFSDK)

// -------------------------------------------------------------------------
// Referent WTL

#if defined(WINX_NO_DELAYLOAD_THEME)
#define _WTL_NO_THEME_DELAYLOAD
#endif

#ifdef __ATLTHEME_H__
#error "Don't include atltheme.h first"
#endif

// -------------------------------------------------------------------------
// class Theme

#define OnCreate			__WTL_OnCreate
#define OnDestroy			__WTL_OnDestroy
#define OnThemeChanged		__WTL_OnThemeChanged
#define OnNcPaint			__WTL_OnNcPaint

#define ATL ::ATL
#include "../../../wtl/include/atltheme.h"
#undef ATL

__WINX_BEGIN

typedef WTL::CTheme ThemeHandle;

template <class T, class TBase = ThemeHandle>
class Theme : public WTL::CThemeImpl<T, TBase>
{
public:
	BOOL winx_msg XPThemeNcPaint(HWND hWnd, HRGN hrgnClip)
	{
		BOOL bHandled;
		OnNcPaint(WM_NCPAINT, (WPARAM)hrgnClip, 0, bHandled);
		return bHandled;
	}
};

__WINX_END

#undef OnCreate
#undef OnDestroy
#undef OnThemeChanged
#undef OnNcPaint

// -------------------------------------------------------------------------
// WINX_THEME

#define WINX_THEME_INIT()													\
public:																		\
	VOID winx_msg OnXPThemeInit(HWND hWnd) {								\
		if(m_lpstrThemeClassList != NULL)									\
			OpenThemeData();												\
	}																		\
	VOID winx_msg OnXPThemeTerm(HWND hWnd) {								\
		CloseThemeData();													\
	}																		\
	VOID winx_msg OnThemeChanged(HWND hWnd) {								\
		CloseThemeData();													\
		if(m_lpstrThemeClassList != NULL)									\
			OpenThemeData();												\
	}

#define WINX_THEME_NCPAINT()												\
public:																		\
	BOOL winx_msg OnXPThemeNcPaint(HWND hWnd, HRGN hrgnClip) {				\
		return XPThemeNcPaint(hWnd, hrgnClip);								\
	}

#define WINX_THEME()														\
	WINX_THEME_INIT()														\
	WINX_THEME_NCPAINT()

// -------------------------------------------------------------------------
// $Log: Theme.h,v $
// Revision 1.2  2006/08/25 15:13:18  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//

#endif /* __WINX_THEME_H__ */
