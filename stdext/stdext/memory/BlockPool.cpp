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
// Module: stdext/memory/BlockPool.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-1-20 13:46:39
// 
// $Id: BlockPool.cpp,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include <stdext/memory/ScopeAlloc.h>

// -------------------------------------------------------------------------

std::_TlsBlockPool _tls_blockPool;

STDAPI_(std::_TlsBlockPool*) _stdext_TlsBlockPool()
{
	return &_tls_blockPool;
}

// -------------------------------------------------------------------------
// $Log: $
