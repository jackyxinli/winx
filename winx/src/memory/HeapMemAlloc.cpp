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
// Module: HeapMemAlloc.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 12:25:29
// 
// $Id: HeapMemAlloc.cpp,v 1.2 2006/09/13 13:22:05 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/Memory.h>

// -------------------------------------------------------------------------

__NS_STD_BEGIN

HANDLE HeapMemAlloc::hProcessHeap = ::GetProcessHeap();

__NS_STD_END

// -------------------------------------------------------------------------
// $Log: HeapMemAlloc.cpp,v $
// Revision 1.2  2006/09/13 13:22:05  xushiwei
// Memory: HeapMemAlloc - DefaultDynamicAlloc
//
