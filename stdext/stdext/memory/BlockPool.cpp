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
#include <stdext/thread/TLS.h>
#include <stdext/ThreadModel.h>

// -------------------------------------------------------------------------

typedef std::InitializerThreadModel _ThreadModel;

std::TlsKey _g_keyBlockPool;
_ThreadModel::RefCount _g_nRefBlockPool(0);

static void _cleanup_BlockPool(void* p)
{
	delete (std::BlockPool*)p;
}

STDAPI_(std::TlsKey*) _stdext_InitBlockPool()
{
	if (_g_nRefBlockPool.acquire() == 1) {
		_g_keyBlockPool.create(_cleanup_BlockPool);
	}
	return &_g_keyBlockPool;
}

STDAPI_(std::TlsKey*) _stdext_BlockPoolTlsKey()
{
	return &_g_keyBlockPool;
}

STDAPI_(void) _stdext_TermBlockPool()
{
	if (_g_nRefBlockPool.release() == 0) {
		_g_keyBlockPool.clear();
	}
}

STDAPI_(std::BlockPool*) _stdext_CreateBlockPool()
{
	std::BlockPool* p = new std::BlockPool;
	_g_keyBlockPool.put(p);
	return p;
}

STDAPI_(std::BlockPool*) _stdext_GetBlockPool()
{
	void* p = _g_keyBlockPool.get();
	if (p == NULL) {
		_g_keyBlockPool.put(p = new std::BlockPool);
	}
	return (std::BlockPool*)p;
}

// -------------------------------------------------------------------------
// $Log: $
