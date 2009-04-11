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
// Module: winx/LinkLib.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 1:38:40
// 
// $Id: LinkLib.h,v 1.1 2006/08/26 03:47:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_LINKLIB_H
#define WINX_LINKLIB_H

// -------------------------------------------------------------------------
// WINX_LINK_SUFFIX

#ifdef _MT
#	ifdef _DLL
#		define WINX_LINK_CRT__	    "-mtdll"
#	else
#		define WINX_LINK_CRT__		"-mt"
#	endif
#else
#	define WINX_LINK_CRT__		    "-st"
#endif

#ifdef _DEBUG
#	define WINX_LINK_DEBUG__		"-dbg"
#else
#	define WINX_LINK_DEBUG__		"-rls"
#endif

#ifdef UNICODE
#	define WINX_LINK_UNICODE__	    "-u"
#else
#	define WINX_LINK_UNICODE__
#endif

#if defined(_MSC_VER) && (_MSC_VER > 1200)
#	if defined(_ATL_VER) && (_ATL_VER <= 0x0710) // VS.NET 2003
#		define WINX_LINK_VER__			"-2003"
#	else
#		define WINX_LINK_VER__			"-2005"
#	endif
#else
#	define WINX_LINK_VER__
#endif

#define WINX_LINK_SUFFIX__				\
	WINX_LINK_CRT__ WINX_LINK_DEBUG__ WINX_LINK_UNICODE__ WINX_LINK_VER__

// -------------------------------------------------------------------------
// link winx

#pragma comment(lib, "winx" WINX_LINK_SUFFIX__)

// -------------------------------------------------------------------------
// $Log: LinkLib.h,v $
// Revision 1.1  2006/08/26 03:47:04  xushiwei
// Winx-Extension:
//    AutoComplete support
//    DropFileHandle(simple wrapper for HDROP)
//

#endif /* WINX_LINKLIB_H */
