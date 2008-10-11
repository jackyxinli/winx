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

#ifndef STDEXT_STRING_H
#include "String.h" // class String, StringEqualTo, StringLess
#endif

#ifndef STD_HASH_H
#include "../std/hash.h"
#endif

#ifndef WINX_NO_HASH_

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// hashOfString

template <class Iterator>
inline size_t winx_call hashOfString(Iterator it, Iterator itEnd)
{
	unsigned long h = 0;
	for (; it != itEnd; ++it)
		h = 5 * h + *it;
	return (size_t)h;
}

// -------------------------------------------------------------------------
// class Hash

#if defined(X_STL_NET)

template <class KeyT>
class Hash
{
public:
	size_t winx_call operator()(const KeyT& v) const {
		return stdext::hash_value(v);
	}
};

#else

template <class KeyT>
class Hash : public stdext::hash<KeyT>
{
};

#if defined(WINX_NO_PARTIAL_SPECIALIZATION)

template <>
class Hash<std::string>
{
public:
	size_t winx_call operator()(const std::string& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <>
class Hash<std::basic_string<wchar_t> >
{
public:
	size_t winx_call operator()(const std::basic_string<wchar_t>& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <>
class Hash<String>
{
public:
	size_t winx_call operator()(const String& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <>
class Hash<WString>
{
public:
	size_t winx_call operator()(const WString& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

#else

template <class CharT, class Tr, class Ax>
class Hash<std::basic_string<CharT, Tr, Ax> >
{
public:
	size_t winx_call operator()(const std::basic_string<CharT, Tr, Ax>& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <class CharT>
class Hash<BasicString<CharT> >
{
public:
	size_t winx_call operator()(const std::BasicString<CharT>& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

#endif // !defined(WINX_NO_PARTIAL_SPECIALIZATION)

#endif // !defined(X_STL_NET)

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

template <>
class HashCompare<const char*>
{
public:
	typedef Hash<const char*> hasher;
	typedef StringEqualTo<char> key_equal;
	typedef StringLess<char> key_pred;
};

template <>
class HashCompare<const wchar_t*>
{
public:
	typedef Hash<const wchar_t*> hasher;
	typedef StringEqualTo<wchar_t> key_equal;
	typedef StringLess<wchar_t> key_pred;
};

template <>
class HashCompare<char*> : public HashCompare<const char*> {
};

template <>
class HashCompare<wchar_t*> : public HashCompare<const wchar_t*> {
};

// -------------------------------------------------------------------------
// Configuration

#if defined(X_STL_NET)

template <class KeyT, class HashCompT>
struct HashComp_ : public stdext::hash_compare<KeyT, typename HashCompT::key_pred>
{
	typedef stdext::hash_compare<KeyT, typename HashCompT::key_pred> Base;

	HashComp_() {}
	HashComp_(typename HashCompT::key_pred pr) : Base(pr) {}

	bool winx_call operator()(const KeyT& key1, const KeyT& key2) const
	{
		return Base::operator()(key1, key2);
	}

	size_t winx_call operator()(const KeyT& key) const
	{
		typename HashCompT::hasher hash;
		ldiv_t Qrem_ = ldiv((long)hash(key), 127773);
		Qrem_.rem = 16807 * Qrem_.rem - 2836 * Qrem_.quot;
		if (Qrem_.rem < 0)
			Qrem_.rem += 2147483647;
		return ((size_t)Qrem_.rem);
	}
};

#define WINX_BASE_HASHMAP_(KeyT, DataT, HashCompT, AllocT)					\
	stdext::hash_map<														\
		KeyT, DataT,														\
		NS_STDEXT::HashComp_<KeyT, HashCompT>,								\
		NS_STDEXT::StlAlloc<DataT, AllocT>									\
	>

#define WINX_BASE_HASHMULTIMAP_(KeyT, DataT, HashCompT, AllocT)				\
	stdext::hash_multimap<													\
		KeyT, DataT,														\
		NS_STDEXT::HashComp_<KeyT, HashCompT>,								\
		NS_STDEXT::StlAlloc<DataT, AllocT>									\
	>

#define WINX_BASE_HASHSET_(ValT, HashCompT, AllocT)							\
	stdext::hash_set<														\
		ValT,																\
		NS_STDEXT::HashComp_<ValT, HashCompT>,								\
		NS_STDEXT::StlAlloc<ValT, AllocT>									\
	>

#define WINX_BASE_HASHMULTISET_(ValT, HashCompT, AllocT)					\
	stdext::hash_multiset<													\
		ValT,																\
		NS_STDEXT::HashComp_<ValT, HashCompT>,								\
		NS_STDEXT::StlAlloc<ValT, AllocT>									\
	>

#else

#define WINX_BASE_HASHMAP_(KeyT, DataT, HashCompT, AllocT)					\
	stdext::hash_map<														\
		KeyT, DataT,														\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		NS_STDEXT::StlAlloc<DataT, AllocT>									\
		>

#define WINX_BASE_HASHMULTIMAP_(KeyT, DataT, HashCompT, AllocT)				\
	stdext::hash_multimap<													\
		KeyT, DataT,														\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		NS_STDEXT::StlAlloc<DataT, AllocT>									\
	>

#define WINX_BASE_HASHSET_(ValT, HashCompT, AllocT)							\
	stdext::hash_set<														\
		ValT,																\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		NS_STDEXT::StlAlloc<ValT, AllocT>									\
		>

#define WINX_BASE_HASHMULTISET_(ValT, HashCompT, AllocT)					\
	stdext::hash_multiset<													\
		ValT,																\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		NS_STDEXT::StlAlloc<ValT, AllocT>									\
	>

#endif

// -------------------------------------------------------------------------
// $Log: Hash.h,v $
//

NS_STDEXT_END

#endif // WINX_NO_HASH_

#endif /* STDEXT_HASH_H */

