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
// Module: winx/wtl/WtlPort.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:36:01
// 
// $Id: WtlPort.h,v 1.11 2006/12/11 05:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_WTL_WTLPORT_H
#define WINX_WTL_WTLPORT_H

//#define WINX_NO_WTL

#if (0)
#define WINX_NO_WTL
#define WINX_NO_ALTER_WINDOW
#endif

#ifndef WINX_WTL_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------

#ifndef WTLPORT_BASIC_H
#include "../../../../wtlport/include/wtlport/Basic.h"
#endif

typedef wtlport::CWindow _WtlCWindow;

// -------------------------------------------------------------------------

#if !defined(WINX_NO_WTL)
typedef ::ATL::CWindow _AlterCWindow;
#else
#define WINX_NO_ALTER_WINDOW
#endif

// -------------------------------------------------------------------------
// $Log: WtlPort.h,v $
//

#endif /* WINX_WTL_WTLPORT_H */
