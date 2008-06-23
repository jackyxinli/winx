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
// Module: stdext/Hash.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Hash.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_HASH_H
#define STDEXT_HASH_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STD_HASH_H
#include "../std/hash.h"
#endif

#ifndef _WINX_NO_HASH

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class Hash

#if defined(X_STL_NET)

template <class KeyT>
class Hash
{
public:
	size_t winx_call operator()(const KeyT& v) const
	{
		return stdext::hash_value(v);
	}
};

#else

template <class KeyT>
class Hash : public stdext::hash<KeyT>
{
};

#endif

// -------------------------------------------------------------------------
// class HashCompare

template <class KeyT>
class HashCompare
{
public:
	typedef Hash<KeyT> hasher;
	typedef std::equal_to<KeyT> key_equal;
	typedef std::less<KeyT> key_pred;
};

// -------------------------------------------------------------------------
// Configuration

#if defined(X_STL_NET)

template <class KeyT, class HashCompT>
struct _HashComp : public stdext::hash_compare<KeyT, typename HashCompT::key_pred>
{
	typedef stdext::hash_compare<KeyT, typename HashCompT::key_pred> __Base;

	_HashComp() {}
	_HashComp(typename HashCompT::key_pred pr) : __Base(pr) {}

	bool winx_call operator()(const KeyT& key1, const KeyT& key2) const
	{
		return __Base::operator()(key1, key2);
	}

	size_t winx_call operator()(const KeyT& key) const
	{
		typename HashCompT::hasher hash;
		ldiv_t _Qrem = ldiv((long)hash(key), 127773);
		_Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
		if (_Qrem.rem < 0)
			_Qrem.rem += 2147483647;
		return ((size_t)_Qrem.rem);
	}
};

#define _WINX_BASE_HASHMAP(KeyT, DataT, HashCompT, AllocT)					\
	stdext::hash_map<														\
		KeyT, DataT,														\
		std::_HashComp<KeyT, HashCompT>,											\
		std::StlAlloc<DataT, AllocT>												\
	>

#define _WINX_BASE_HASHMULTIMAP(KeyT, DataT, HashCompT, AllocT)				\
	stdext::hash_multimap<													\
		KeyT, DataT,														\
		std::_HashComp<KeyT, HashCompT>,											\
		std::StlAlloc<DataT, AllocT>												\
	>

#define _WINX_BASE_HASHSET(ValT, HashCompT, AllocT)							\
	stdext::hash_set<														\
		ValT,																\
		std::_HashComp<ValT, HashCompT>,											\
		std::StlAlloc<ValT, AllocT>												\
	>

#define _WINX_BASE_HASHMULTISET(ValT, HashCompT, AllocT)					\
	stdext::hash_multiset<													\
		ValT,																\
		std::_HashComp<ValT, HashCompT>,											\
		std::StlAlloc<ValT, AllocT>												\
	>

#else

#define _WINX_BASE_HASHMAP(KeyT, DataT, HashCompT, AllocT)					\
	stdext::hash_map<														\
		KeyT, DataT,														\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		std::StlAlloc<DataT, AllocT>												\
		>

#define _WINX_BASE_HASHMULTIMAP(KeyT, DataT, HashCompT, AllocT)				\
	stdext::hash_multimap<													\
		KeyT, DataT,														\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		std::StlAlloc<DataT, AllocT>												\
	>

#define _WINX_BASE_HASHSET(ValT, HashCompT, AllocT)							\
	stdext::hash_set<														\
		ValT,																\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		std::StlAlloc<ValT, AllocT>												\
		>

#define _WINX_BASE_HASHMULTISET(ValT, HashCompT, AllocT)					\
	stdext::hash_multiset<													\
		ValT,																\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		std::StlAlloc<ValT, AllocT>												\
	>

#endif

// -------------------------------------------------------------------------
// $Log: Hash.h,v $
//

NS_STDEXT_END

#endif // _WINX_NO_HASH

#endif /* STDEXT_HASH_H */
