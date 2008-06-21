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
// Module: TestMemory.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestMemory.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define STD_UNITTEST
#include <stdext/Memory.h>

// -------------------------------------------------------------------------
// TestCase

void testAutoFreeAlloc()
{
    std::AutoFreeAlloc alloc;
    
    int* a;
    a = STD_NEW(alloc, int);
    
    int* b;
    b = STD_NEW_ARRAY(alloc, int, 100);
    
    int* c;
    c = STD_ALLOC(alloc, int);
    
    int* d;
    d = STD_ALLOC_ARRAY(alloc, int, 100);
    
    std::AutoFreeAlloc* suballoc = STD_NEW(alloc, std::AutoFreeAlloc);
    
    int* e;
    e = STD_NEW(*suballoc, int);
}

void testScopeAlloc()
{
    std::BlockPool recycle;
    std::ScopeAlloc alloc(recycle);
    
    int* a;
    a = STD_NEW(alloc, int);
    // ... --> same as std::AutoFreeAlloc
}

void testMemory()
{
    testAutoFreeAlloc();
    testScopeAlloc();
}

WINX_AUTORUN(testMemory);

// -------------------------------------------------------------------------
// $Log: TestMemory.cpp,v $
//
