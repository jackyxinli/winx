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
// Module: stdext/text/CString.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-9-6 10:08:57
// 
// $Id: CString.h 2009-9-6 10:08:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_CSTRING_H
#define STDEXT_TEXT_CSTRING_H

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class BasicCString

#pragma pack(1)

template <class CharT, class AllocT = DefaultAlloc>
class BasicCString : private BasicString<CharT>
{
private:
	typedef BasicString<CharT> Base;

public:
	typedef AllocT alloc_type;

	typedef typename Base::size_type size_type;
	typedef typename Base::difference_type difference_type;
	typedef typename Base::value_type value_type;

	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reverse_iterator reverse_iterator;
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

	using Base::size;
	using Base::length;
	using Base::empty;
	using Base::begin;
	using Base::end;
	using Base::rbegin;
	using Base::rend;
	using Base::compare;
	using Base::compare_by;
	using Base::icompare;
	
	using Base::data;
	using Base::at;
	using Base::left;
	using Base::right;
	using Base::substr;
	using Base::find;
	using Base::rfind;
	using Base::find_first_of;
	using Base::find_last_of;
	using Base::find_first_not_of;
	using Base::find_last_not_of;
	using Base::contains;
	using Base::startWith;
	using Base::startWithI;
	using Base::endWith;
	using Base::endWithI;
	using Base::stl_str;
	using Base::operator[];

private:
	typedef std::basic_string<CharT> StlString_;
	typedef TempString<CharT> String_;
	typedef BasicCString Myt_;
	
	struct ExtraData
	{
		size_t ref;
		AllocT* alloc;
	};
	
	struct StringData
	{
		ExtraData extra;
		CharT str[1];
	};
	
	static StringData s_null;

	static size_type winx_call alloc_size(size_type cch)
	{
		return sizeof(ExtraData) + (cch + 1) * sizeof(CharT);
	}
	
	ExtraData& winx_call edata() const
	{
		return *((ExtraData*)Base::first - 1);
	}
	
	template <class Iterator>
	void winx_call init(AllocT& alloc, Iterator first, size_type cch)
	{
		ExtraData* const extra = (ExtraData*)alloc.allocate(alloc_size(cch));
		extra->ref = 1;
		extra->alloc = &alloc;
		Base::first = (CharT*)(extra + 1);
		Base::second = std::copy(first, first + cch, (CharT*)Base::first);
		*(CharT*)Base::second = CharT();
	}
	
	template <class Iterator>
	void winx_call init(AllocT& alloc, size_type count, CharT ch)
	{
		ExtraData* const extra = (ExtraData*)alloc.allocate(alloc_size(count));
		extra->ref = 1;
		extra->alloc = &alloc;
		Base::first = (CharT*)(extra + 1);
		Base::second = std::fill_n((CharT*)Base::first, count, ch);
		*(CharT*)Base::second = CharT();
	}
	
	void winx_call acquire() const
	{
		++edata().ref;
	}
	
	void winx_call release()
	{
		ExtraData& extra = edata();
		if (--extra.ref == 0 && extra.alloc != NULL)
		{
			extra.alloc->deallocate(&extra, alloc_size(Base::second - Base::first));
			Base::assign(s_null.str, s_null.str);
		}
	}

public:
	BasicCString()
		: Base(s_null.str, s_null.str) {
	}
	
	BasicCString(const BasicCString& s)
		: Base(s) {
		acquire();
	}
	
	BasicCString(AllocT& alloc, const String_& s) {
		init(alloc, s.begin(), s.size());
	}

	BasicCString(AllocT& alloc, const value_type* pszVal, size_type cch) {
		init(alloc, pszVal, cch);
	}

	BasicCString(AllocT& alloc, size_type count, value_type ch) {
		init(alloc, count, ch);
	}

	template <class Iterator>
	BasicCString(AllocT& alloc, Iterator first, Iterator last) {
		init(alloc, first, last - first);
	}
	
	~BasicCString() {
		release();
	}
	
public:
	Myt_& winx_call operator=(const Myt_& s) {
		s.acquire();
		release();
		Base::assign(s);
		return *this;
	}

	Myt_& winx_call assign(const Myt_& s) {
		return *this = s;
	}

	Myt_& winx_call assign(AllocT& alloc, const String_& s) {
		release();
		init(alloc, s.begin(), s.size());
		return *this;
	}

	Myt_& winx_call assign(AllocT& alloc, const CharT* pszVal, size_type cch) {
		release();
		init(alloc, pszVal, cch);
		return *this;
	}

	Myt_& winx_call assign(AllocT& alloc, size_type count, value_type ch) {
		release();
		init(alloc, count, ch);
		return *this;
	}

	template <class Iterator>
	Myt_& winx_call assign(AllocT& alloc, Iterator first, Iterator last) {
		release();
		init(alloc, first, last - first);
		return *this;
	}

public:
	operator const CharT*() const {
		return Base::first;
	}
	
	operator const TempString<CharT>&() const {
		return *(const TempString<CharT>*)this;
	}

	operator const BasicString<CharT>&() const {
		return *this;
	}

	const BasicString<CharT>& winx_call str() const {
		return *this;
	}

	const CharT* winx_call c_str() const {
		return Base::first;
	}

	void winx_call swap(BasicCString& b) {
		std::swap(Base::first, b.first);
		std::swap(Base::second, b.second);
	}
	
	alloc_type& winx_call getAlloc() const {
		WINX_ASSERT(!empty());
		return *edata().alloc;
	}
};

template <class CharT, class AllocT>
typename BasicCString<CharT, AllocT>::StringData BasicCString<CharT, AllocT>::s_null;

typedef BasicCString<char> CString;
typedef BasicCString<wchar_t> WCString;

#pragma pack()

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_TEXT_CSTRING_H */
