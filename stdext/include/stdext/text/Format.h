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
// Module: stdext/text/Format.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Format.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_H
#define STDEXT_TEXT_FORMAT_H

#ifndef STDEXT_TCHAR_H
#include "../tchar.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// macro WINX_STRBUF_SIZE

template <class Target>
struct StringBufTraits_ {};

#define WINX_STRBUF_TRAITS(Type, nBufSize) \
template <> \
struct StringBufTraits_<> { \
	enum { value = nBufSize }; \
};

#define WINX_STRBUF_SIZE(Target) \
	std::StringBufTraits_<Target>::value

// -------------------------------------------------------------------------
// function to_str (integer/real)

WINX_STRBUF_TRAITS(int, 32)
WINX_STRBUF_TRAITS(unsigned int, 32)
WINX_STRBUF_TRAITS(long, 64)
WINX_STRBUF_TRAITS(unsigned long, 64)

template <class E>
__forceinline E* winx_call to_str(E buf[], int i, int radix = 10) {
	return tchar::itoa(i, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], unsigned int i, int radix = 10) {
	return tchar::ultoa(i, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], long i, int radix = 10) {
	return tchar::ltoa(i, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], unsigned long i, int radix = 10) {
	return tchar::ultoa(i, buf, radix);
}

#ifndef _NO_INT64

WINX_STRBUF_TRAITS(__int64, 64)
WINX_STRBUF_TRAITS(__uint64, 64)

template <class E>
__forceinline E* winx_call to_str(E buf[], const __int64& i64, int radix = 10) {
	tchar::i64toa(i64, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], const __uint64& i64, int radix = 10) {
	tchar::ui64toa(i64, buf, radix);
}

#endif

WINX_STRBUF_TRAITS(float, 64)
WINX_STRBUF_TRAITS(double, 64)

__forceinline char* winx_call to_str(char buf[], double val, int ndigit = 12) {
	return _gcvt(val, ndigit, buf);
}

// -------------------------------------------------------------------------
// function str

template <class E, class AllocT, class Target>
__forceinline BasicString<E> winx_call str(AllocT& alloc, const Target& val) {
	enum { bufsize = WINX_STRBUF_SIZE(Target) };
	E buf[bufsize];	
	return BasicString<E>(alloc, to_str(buf, val));
}

template <class E, class AllocT, class Target>
__forceinline BasicString<E> winx_call str(AllocT& alloc, const Target& val, int radix) {
	enum { bufsize = WINX_STRBUF_SIZE(Target) };
	E buf[bufsize];
	return BasicString<E>(alloc, to_str(buf, val, radix));
}

template <class E, class AllocT, class Tr, class AllocT2>
__forceinline BasicString<E> winx_call str(AllocT& alloc, const std::basic_string<E, Tr, AllocT2>& val) {
	return BasicString<E>(alloc, val.begin(), val.end());
}

template <class E, class AllocT, class AllocT2>
__forceinline BasicString<E> winx_call str(AllocT& alloc, const std::vector<E, AllocT2>& val) {
	return BasicString<E>(alloc, val.begin(), val.end());
}

template <class E, class AllocT>
__forceinline BasicString<E> winx_call str(AllocT& alloc, const E* val) {
	return BasicString<E>(alloc, val);
}

template <class E, class AllocT>
__forceinline BasicString<E> winx_call str(AllocT& alloc, const E val) {
	return BasicString<E>(alloc, &val, 1);
}

// -------------------------------------------------------------------------
// $Log: Format.h,v $

NS_STDEXT_END

#endif /* STDEXT_TEXT_FORMAT_H */
