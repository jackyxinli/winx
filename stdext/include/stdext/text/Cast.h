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
// Module: stdext/text/Cast.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Cast.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_CAST_H
#define STDEXT_TEXT_CAST_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

// -------------------------------------------------------------------------

NS_STDEXT_BEGIN

inline void winx_call throw_cast_error_() {
	throw std::invalid_argument("lexical_cast failed");
}

template <class UIntT>
struct CastToUInt
{
	template <class Iterator>
	static UIntT winx_call get(Iterator first, Iterator last, unsigned radix = 10)
	{
		unsigned dig;
		UIntT preval = 0;
		while (first != last) {
			dig = DigitTable::toDigit(*first++);
			WINX_ASSERT(dig < radix);
			if (dig < radix)
				preval = preval * radix + dig;
			else
				throw_cast_error_();
		}
		return preval;
	}

	template <class Iterator>
	static UIntT winx_call get(Iterator first, unsigned radix = 10)
	{
		unsigned dig;
		UIntT preval = 0;
		while (*first) {
			dig = DigitTable::toDigit(*first++);
			WINX_ASSERT(dig < radix);
			if (dig < radix)
				preval = preval * radix + dig;
			else
				throw_cast_error_();
		}
		return preval;
	}
};

template <class IntT>
struct CastToInt
{
	template <class Iterator>
	static IntT winx_call get(Iterator first, Iterator last)
	{
		if (*first == '-') {
			return -CastToUInt<IntT>::get(++first, last);
		}
		else if (*first == '+') {
			++first;
		}
		return CastToUInt<IntT>::get(first, last);
	}

	template <class Iterator>
	static IntT winx_call get(Iterator first)
	{
		if (*first == '-') {
			return -CastToUInt<IntT>::get(++first);
		}
		else if (*first == '+') {
			++first;
		}
		return CastToUInt<IntT>::get(first);
	}
};

template <class RealT>
struct CastToReal
{
	static RealT winx_call get(const char* first) {
		RealT result = (RealT)strtod(first, (char**)&first);
		WINX_ASSERT(*first == '\0');
		if (*first)
			throw_cast_error_();
		return result;
	}

	static RealT winx_call get(const wchar_t* first) {
		RealT result = (RealT)wcstod(first, (wchar_t**)&first);
		WINX_ASSERT(*first == '\0');
		if (*first)
			throw_cast_error_();
		return result;
	}
};

template <class Type>
struct Cast {
};

template <>
struct Cast<unsigned short> : CastToUInt<unsigned short> {	
};

template <>
struct Cast<unsigned int> : CastToUInt<unsigned int> {	
};

template <>
struct Cast<unsigned long> : CastToUInt<unsigned long> {	
};

template <>
struct Cast<__uint64> : CastToUInt<__uint64> {	
};

template <>
struct Cast<short> : CastToInt<short> {	
};

template <>
struct Cast<int> : CastToInt<int> {	
};

template <>
struct Cast<long> : CastToInt<long> {	
};

template <>
struct Cast<__int64> : CastToInt<__int64> {	
};

template <>
struct Cast<float> : CastToReal<float> {	
};

template <>
struct Cast<double> : CastToReal<double> {	
};

template <class Type, class Iterator>
__forceinline Type winx_call cast(Iterator first, Iterator last, unsigned radix) {
	return Cast<Type>::get(first, last, radix);
}

template <class Type, class Iterator>
__forceinline Type winx_call cast(Iterator first, Iterator last) {
	return Cast<Type>::get(first, last);
}

template <class Type, class Iterator>
__forceinline Type winx_call cast(Iterator first, unsigned radix) {
	return Cast<Type>::get(first, radix);
}

template <class Type, class Iterator>
__forceinline Type winx_call cast(Iterator first) {
	return Cast<Type>::get(first);
}

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestCast

#if defined(STD_UNITTEST)

template <class LogT>
class TestCast : public TestCase
{
	WINX_TEST_SUITE(TestCast);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		AssertExp(std::cast<int>("12") == 12);
		AssertExp(std::cast<unsigned>("12", 8) == 10);
		AssertExp(std::cast<unsigned>("12", 16) == 18);
		AssertExp(std::cast<__int64>("-12") == -12);
		
		AssertExp(std::cast<double>("1.2e10") == 1.2e10);
		AssertExp(std::cast<float>("1.2e10") == 1.2e10);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Cast.h,v $

#endif /* STDEXT_TEXT_CAST_H */

