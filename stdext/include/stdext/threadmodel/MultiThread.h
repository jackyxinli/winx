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
// Module: stdext/threadmodel/MultiThread.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-3 2:18:33
// 
// $Id: MultiThread.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_THREADMODEL_MULTITHREAD_H__
#define __STDEXT_THREADMODEL_MULTITHREAD_H__

#ifndef __STDEXT_MEMORY_H__
#include "../Memory.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class RefCountMT

typedef NS_BOOST_MEMORY::refcount_mt RefCountMT;

// -------------------------------------------------------------------------
// class CriticalSectionMT

typedef NS_BOOST_MEMORY::critical_section_mt CriticalSectionMT;

// -------------------------------------------------------------------------
//	$Log: $

__NS_STD_END

#endif /* __STDEXT_THREADMODEL_MULTITHREAD_H__ */
