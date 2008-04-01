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

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#if !defined(NO_SGISTL)

#ifndef __SGI_ROPE_H__
#include "../sgi/rope.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Rope

template <class CharT, class AllocT = ScopeAlloc>
class Rope : public stdext::rope<CharT, StlAlloc<CharT, AllocT> >
{
private:
	typedef StlAlloc<CharT, AllocT> _Alloc;
	typedef stdext::rope<CharT, _Alloc> _Base;

public:
	explicit Rope(AllocT& alloc)
		: _Base(alloc)
	{
	}

	Rope(AllocT& alloc, const CharT* src)
		: _Base(src, alloc)
	{
	}

	Rope(AllocT& alloc, const CharT* src, size_t len)
		: _Base(src, len, alloc)
	{
	}

	Rope(AllocT& alloc, size_t count, CharT ch)
		: _Base(count, ch, alloc)
	{
	}

	template <class Iterator>
	Rope(AllocT& alloc, Iterator first, Iterator last)
		: _Base(first, last, alloc)
	{
	}

	void winx_call copy(const _Base& from) {
		_Base::operator=(from);
	}
};

// -------------------------------------------------------------------------
// class TestRope

template <class LogT>
class TestRope : public TestCase
{
	WINX_TEST_SUITE(TestRope);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void test(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::Rope<char> a(alloc);
		a.append(4, 'a');
		a.append("bcdefg");

		std::Rope<char> b(alloc, "aaaabcdefg");
		AssertExp(a.compare(b) == 0);

		std::Rope<char> c(alloc);
		c.copy(b);
		AssertExp(c == b);
	}
};

// -------------------------------------------------------------------------
// $Log: Rope.h,v $

__NS_STD_END

#endif // !defined(NO_SGISTL)

#endif /* __STDEXT_TEXT_ROPE_H__ */
