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
// Module: cppunit/linklib.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-14 17:57:09
// 
// $Id: linklib.h,v 1.1 2006/11/30 08:45:42 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __CPPUNIT_LINKLIB_H__
#define __CPPUNIT_LINKLIB_H__

// -------------------------------------------------------------------------

#if !defined(__Linked_cppunit)
#define __Linked_cppunit
#if defined(X_CC_BC)
#	if defined(_WIN32)
#		if defined(USES_KERN)
#			pragma link "cppunitk_bcb.lib"
#		else
#			pragma link "cppunit_bcb.lib"
#		endif
#	else
#		pragma link "libcppunit.so"
#	endif
#elif defined(_MSC_VER)
#	pragma comment(lib, "cppunit")
#endif
#endif

// -------------------------------------------------------------------------
// $Log: linklib.h,v $

#endif /* __CPPUNIT_LINKLIB_H__ */
