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
// Module: winx/wtl/Window.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:36:01
// 
// $Id: Window.h,v 1.11 2006/12/11 05:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WTL_WINDOW_H__
#define __WINX_WTL_WINDOW_H__

#if (0)
#define WINX_NO_WTL
#define WINX_NO_WTLPORT
#define WINX_NO_ALTER_WINDOW
#endif

#ifndef __WINX_WTL_BASIC_H__
#include "Basic.h"
#endif

// -------------------------------------------------------------------------

#if !defined(WINX_NO_WTLPORT)
#include "../../../../wtlport/include/wtlport/Basic.h"
typedef ::ATL::CWindow _AlterCWindow;
typedef wtlport::CWindow _WtlCWindow;
#else
typedef ::ATL::CWindow _WtlCWindow;
#define WINX_NO_ALTER_WINDOW
#endif

// -------------------------------------------------------------------------
// $Log: Window.h,v $
//

#endif /* __WINX_WTL_WINDOW_H__ */
