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
// Module: sgi/hash_set.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: HashMap.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_HASH_SET_H__
#define __SGI_HASH_SET_H__

#pragma warning(disable:4010) // vc6: single-line comment contains line-continuation character

// -------------------------------------------------------------------------
// hash_set/hash_multiset

#ifndef __SGI_CONFIG_H__
#include "config.h"
#endif

#ifndef __SGI_VECTOR_H__
#include "vector.h"
#endif

#ifndef __SGI_STL_INTERNAL_HASHTABLE_H
#include "../../../../stl/stl_hashtable.h"
#endif 

#ifndef __SGI_STL_HASH_SET_H
#include "../../../../stl/hash_set.h"
#endif

// -------------------------------------------------------------------------
// using

namespace std
{
	using stdext::hash;
	using stdext::hashtable;
	using stdext::hash_set;
	using stdext::hash_multiset;
}

// -------------------------------------------------------------------------
// $Log: hash_set.h,v $

#endif /* __SGI_HASH_SET_H__ */
