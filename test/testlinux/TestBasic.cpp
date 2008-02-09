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
// Module: TestBasic.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestBasic.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/Basic.h>

// -------------------------------------------------------------------------
// TestCase

int testBasic()
{
    std::OutputLog log;
    log.print('a')
       .print(1)
       .print((short)2)
       .newline()
       .print(1.0)
       .print(1.0, "%4.3lf")
       .newline(); 
	return 0;
}

WINX_AUTORUN(testBasic);

// -------------------------------------------------------------------------
// $Log: TestBasic.cpp,v $
//
