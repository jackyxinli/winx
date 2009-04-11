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
// Module: winx/Config.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 11:10:31
// 
// $Id: Config.h,v 1.2 2006/08/25 15:13:18 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_CONFIG_H
#define WINX_CONFIG_H

// -------------------------------------------------------------------------
// WINX_USE_WINSDK or WINX_USE_DEFSDK

#if !defined(WINX_USE_WINSDK) && !defined(WINX_USE_DEFSDK)

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	#define WINX_USE_WINSDK // if vc++ 6.0
#else
	#define WINX_USE_DEFSDK
#endif

#endif

// -------------------------------------------------------------------------
// $Log: Config.h,v $
// Revision 1.2  2006/08/25 15:13:18  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//

#endif /* WINX_CONFIG_H */
