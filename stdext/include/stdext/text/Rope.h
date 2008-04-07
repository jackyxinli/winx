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
// Module: stdext/text/Rope.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Rope.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_ROPE_H__
#define __STDEXT_TEXT_ROPE_H__

#ifndef __STDEXT_HASH_H__
#include "../Hash.h"
#endif

#ifdef STD_UNITTEST
#include "../HashMap.h"
#endif

#if defined(USE_SGISTL_ROPE) && !defined(NO_SGISTL)

#ifndef __STDEXT_TEXT_SGI_ROPE_H__
#include "sgi/Rope.h"
#endif

#else

#ifndef __STDEXT_TEXT_ROPE_SEQUENCEBUFFER_H__
#include "rope/SequenceBuffer.h"
#endif

#ifndef __STDEXT_TEXT_ROPE_ROPE_H__
#include "rope/Rope.h"
#endif

#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// Rope::operator+ & Rope::operator+=

template <class _E, class _AllocT, class _T2> __forceinline
Rope<_E,_AllocT> operator+(Rope<_E,_AllocT> __left, const _T2& __right)
{
	return __left.append(__right);
}

template <class _E, class _AllocT, class _T2> __forceinline
Rope<_E,_AllocT>& operator+=(Rope<_E,_AllocT>& __left, const _T2& __right)
{
    return __left.append(__right);
}

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator==(const Rope<_E, _AllocT>& a, const _T2& b)
	{return a.compare(b) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const BasicString<_E>& a, const Rope<_E, _AllocT>& b)
	{return b.compare(a) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const BasicString<_E>& a, Rope<_E, _AllocT>& b)
	{return b.compare(a) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const _E* a, const Rope<_E, _AllocT>& b)
	{return b.compare(a) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const _E* a, Rope<_E, _AllocT>& b)
	{return b.compare(a) == 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator!=(const Rope<_E, _AllocT>& a, const _T2& b)
	{return a.compare(b) != 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator<(const Rope<_E, _AllocT>& a, const _T2& b)
	{return a.compare(b) < 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator>(const Rope<_E, _AllocT>& a, const _T2& b)
	{return a.compare(b) > 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator<=(const Rope<_E, _AllocT>& a, const _T2& b)
	{return a.compare(b) <= 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator>=(const Rope<_E, _AllocT>& a, const _T2& b)
	{return a.compare(b) >= 0; }

// -------------------------------------------------------------------------
// Hash of Rope class

template <>
class Hash< Rope<char> >
{
public:
  size_t winx_call operator()(const Rope<char>& __str) const
  {
    size_t __size = __str.size();
    if (0 == __size) return 0;
    return 13*__str[0] + 5*__str[__size - 1] + __size;
  }
};

template <>
class Hash< Rope<WCHAR> >
{
public:
  size_t winx_call operator()(const Rope<WCHAR>& __str) const
  {
    size_t __size = __str.size();
    if (0 == __size) return 0;
    return 13*__str[0] + 5*__str[__size - 1] + __size;
  }
};

// -------------------------------------------------------------------------
// class TestRope

#if defined(STD_UNITTEST)

template <class LogT>
class TestRope : public TestCase
{
	WINX_TEST_SUITE(TestRope);
		WINX_TEST(testBasic);
		WINX_TEST(testSequenceBuffer);
		WINX_TEST(testHash);
		WINX_TEST(testIterator);
	WINX_TEST_SUITE_END();

public:
	void testIterator(LogT& log) // slowly (not recommended)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::Rope<char> a(alloc, "Hello");
		std::Rope<char> b(alloc, "abc");

		std::Rope<char>::iterator it = a.mutable_begin();
		*it = 'e';
		*(it+1) = 'f';
		*(it+2) = 'g';
		AssertExp(a == "efglo");
		std::Rope<char>::const_iterator it2 = it;
		AssertExp(*it == 'e');
		AssertExp(*it2 == 'e');
		AssertExp(*(it2+1) == 'f');
		std::copy(b.begin(), b.end(), it);
		AssertExp(a == "abclo");

		char buffer[256];
		*std::copy(it, a.mutable_end(), buffer) = '\0';
		AssertExp(strcmp(buffer, "abclo") == 0);
	}

	void testHash(LogT& log)
	{
		typedef std::Rope<char> KeyT;

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::HashMap<KeyT, int> cont(alloc);
		
		KeyT a(alloc, "Hello");
		cont[a] = 1;

		KeyT b(alloc, "Hello");
		AssertExp(cont.find(b) != cont.end());
		AssertExp(cont[b] == 1);
	}

	void testSequenceBuffer(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		char buffer[256];

		std::Rope<char> a(alloc);
		{
			std::SequenceBuffer<std::Rope<char>, 8> input(a);
			input.append("Hello");
			input.append(", ");
			input.append("world!");
			input.append(1, '!');
			input.append(4, '?');
		}
		*a.copy(buffer) = '\0';
		AssertExp(a == "Hello, world!!????");
		{
			std::SequenceBuffer<std::Rope<char>, 16> input(a);
			input.append(2, ' ');
			input.append("You");
			input.append("'re");
			input.append(" welcome!");
		}
		*a.copy(buffer) = '\0';
		AssertExp(a == "Hello, world!!????  You're welcome!");
	}

	void testBasic(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::Rope<char> a(alloc);
		a.append(20, 'a');
		a.append("bcdefg");

		std::Rope<char> b(a);
		b.erase(b.size() - 7, 7);
		b.append("abcdefg");
		AssertExp(a.compare(b) == 0);

		std::Rope<char> c(alloc);
		c = a;
		AssertExp(c == b);

		c = "abc";
		c.mutable_reference_at(0) = 'y';
		AssertExp(c == "ybc");
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Rope.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_ROPE_H__ */
