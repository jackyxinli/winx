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
// class TestRope

template <class LogT>
class TestRope : public TestCase
{
	WINX_TEST_SUITE(TestRope);
		WINX_TEST(testBasic);
		WINX_TEST(testSequenceBuffer);
	WINX_TEST_SUITE_END();

public:
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

// -------------------------------------------------------------------------
// $Log: Rope.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_ROPE_H__ */
