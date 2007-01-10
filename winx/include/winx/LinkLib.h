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
#ifndef __WINX_LINKLIB_H__
#define __WINX_LINKLIB_H__

// -------------------------------------------------------------------------
// WINX_LINK_SUFFIX

#ifdef _MT
#	ifdef _DLL
#		define __WINX_LINK_CRT	    "-mtdll"
#	else
#		define __WINX_LINK_CRT		"-mt"
#	endif
#else
#	define __WINX_LINK_CRT		    "-st"
#endif

#ifdef _DEBUG
#	define __WINX_LINK_DEBUG		"-dbg"
#else
#	define __WINX_LINK_DEBUG		"-rls"
#endif

#ifdef UNICODE
#	define __WINX_LINK_UNICODE	    "-u"
#else
#	define __WINX_LINK_UNICODE
#endif

#if defined(_MSC_VER) && (_MSC_VER > 1200)
#	define __WINX_LINK_VS2005		"-2005"
#else
#	define __WINX_LINK_VS2005
#endif

#define __WINX_LINK_SUFFIX			\
	__WINX_LINK_CRT __WINX_LINK_DEBUG __WINX_LINK_UNICODE __WINX_LINK_VS2005

// -------------------------------------------------------------------------
// link winx

#pragma comment(lib, "winx" __WINX_LINK_SUFFIX)

// -------------------------------------------------------------------------
// $Log: LinkLib.h,v $
// Revision 1.1  2006/08/26 03:47:04  xushiwei
// Winx-Extension:
//    AutoComplete support
//    DropFileHandle(simple wrapper for HDROP)
//

#endif /* __WINX_LINKLIB_H__ */
