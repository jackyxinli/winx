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
// Module: wrapper/cppunit.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-14 17:57:09
// 
// $Id: cppunit.h,v 1.1 2006/11/30 08:45:42 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WRAPPER_CPPUNIT_H__
#define __WRAPPER_CPPUNIT_H__

#if (0)
#define X_RELEASE_CASE
#define X_RELEASE_TESTCASE
#endif

#if defined(X_RELEASE_TESTCASE)
#define X_RELEASE_CASE
#endif

#ifndef __CPPUNIT_PLATAPI_H__
#include "cppunit/platapi.h"
#endif

#ifndef __CPPUNIT_BASIC_H__
#include "cppunit/basic.h"
#endif

#ifndef __CPPUNIT_LINKLIB_H__
#include "cppunit/linklib.h"
#endif

// -------------------------------------------------------------------------
// $Log: cppunit.h,v $

#endif /* __WRAPPER_CPPUNIT_H__ */
