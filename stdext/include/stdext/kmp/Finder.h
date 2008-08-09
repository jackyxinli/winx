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
// Module: stdext/kmp/Finder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Finder.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_KMP_FINDER_H
#define STDEXT_KMP_FINDER_H

#ifndef _INC_CTYPE
#include <ctype.h>
#endif

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "../archive/Basic.h"
#endif

#ifndef STD_STRING_H
#include "../../std/string.h"
#endif

#define NS_KMP			std::kmp
#define NS_KMP_BEGIN	namespace std { namespace kmp {
#define NS_KMP_END		} }

NS_KMP_BEGIN

// -------------------------------------------------------------------------
// c++ std::istream adapter

// I use 'istream_type' because I don't want to include <istream>.
template <class istream_type>
class _istream_adapter : public istream_type
{
public:
	enum { endch = -1 };
};

// -------------------------------------------------------------------------
// c style string adapter

template <class CharT>
class _cstr_adapter
{
public:
	enum { endch = -1 };

	typedef CharT char_type;
	typedef typename ArchiveCharTraits<char_type>::uchar_type uchar_type;
	typedef typename ArchiveCharTraits<char_type>::int_type int_type;
	
private:
	const CharT* m_p;
	
public:
	_cstr_adapter(const CharT* p) : m_p(p) {}
	
	__forceinline int_type winx_call get()
	{
		return (*m_p ? (uchar_type)*m_p++ : (int_type)endch);
	}

	__forceinline const CharT* winx_call tell() const
	{
		return m_p;
	}
};

// -------------------------------------------------------------------------
// iterator adapter

template <class iterator_type>
class _iterator_adapter
{
private:
	typedef ArchiveIteratorTraits<iterator_type> Tr_;

public:
	enum { endch = -1 };

	typedef iterator_type iterator;
	typedef typename Tr_::value_type char_type;
	typedef typename ArchiveCharTraits<char_type>::uchar_type uchar_type;
	typedef typename ArchiveCharTraits<char_type>::int_type int_type;
	
private:
	iterator m_it;
	
public:
	_iterator_adapter(iterator it) : m_it(it) {}

	int_type winx_call get()
	{
		return (uchar_type)*m_it++;
	}

	iterator winx_call tell() const
	{
		return m_it;
	}
};

// -------------------------------------------------------------------------
// Finder Strategy

/*
@class std::kmp::MatchCase
@brief
	The std::kmp::MatchCase class is provided as a strategy of class \<Finder>. It means the
	searching algorithm will process in case-sensitive mode.
@arg CharT
	The data type of a single character. It must be the same as you pass to \<Finder> class.
@see Finder, MatchNoCase
@*/
template <class CharT>
struct MatchCase
{
	typedef CharT char_type;

	template <class Iterator>
	__forceinline static void winx_call copy(char_type* dest, Iterator src, size_t n)
	{
		while (n--)
			*dest++ = *src++;
	}

	template <class ArchiveT>
	__forceinline static typename ArchiveT::int_type winx_call get(ArchiveT& ar)
	{
		return ar.get();
	}
};

/*
@class std::kmp::MatchNoCase
@brief
	The std::kmp::MatchNoCase class is provided as a strategy of class \<Finder>. It means the
	searching algorithm will process in non case-sensitive mode.
@arg CharT
	The data type of a single character. It must be the same as you pass to \<Finder> class.
@see Finder, MatchCase
@*/
template <class CharT>
struct MatchNoCase
{
	typedef CharT char_type;

	template <class Iterator>
	static void winx_call copy(char_type* dest, Iterator src, size_t n)
	{
		while (n--)
			*dest++ = toupper( *src++ );
	}

	template <class ArchiveT>
	static typename ArchiveT::int_type winx_call get(ArchiveT& ar)
	{
		return toupper( ar.get() );
	}
};

// -------------------------------------------------------------------------
// class Finder

/*
@class std::kmp::Finder
@brief
	The std::kmp::Finder class implements KMP string searching algorithm.
@arg CharT
	The data type of a single character to be found in searching algorithm. It can be char or WCHAR.
@arg Strategy
	The strategy of searching algorithm. Default is \<MatchCase>, and it also can be \<MatchNoCase>.
@*/
template < class CharT, class Strategy = MatchCase<CharT>, class AllocT = DefaultStaticAlloc>
class Finder
{
public:
	typedef CharT char_type;
	typedef typename ArchiveCharTraits<CharT>::uchar_type uchar_type;
	typedef size_t size_type;

	enum { npos = -1 };
	enum { nolimit = -1 };

private:
	uchar_type* m_str_find;
	size_type m_size;
	size_type* m_next;
	
	Finder(const Finder&);
	void operator=(const Finder&);

public:
	/*
	@ctor()
	@brief					Default constructor.
	@see Finder, initPattern, getPattern
	@*/
	Finder()
		: m_str_find(NULL), m_next(NULL), m_size(0)
	{
	}
	
	/*
	@ctor(szPattern,cchLen)
	@brief					Construct the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const char_type* szPattern, size_type cchLen) : m_str_find(NULL)
	{
		initPattern(szPattern, szPattern + cchLen);
	}

	/*
	@ctor(szPattern)
	@brief					Construct the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The null-terminated pattern string.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const char_type* szPattern) : m_str_find(NULL)
	{
		initPattern(szPattern);
	}

	/*
	@ctor(strPattern)
	@brief					Construct the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The pattern string object.
	@see Finder, initPattern, getPattern
	@*/
	template <class Tr, class AllocT2>
	Finder(const std::basic_string<CharT, Tr, AllocT2>& strPattern) : m_str_find(NULL)
	{
		initPattern(strPattern.begin(), strPattern.end());
	}

	template <class Iterator>
	Finder(Iterator patternBegin, Iterator patternEnd) : m_str_find(NULL)
	{
		initPattern(patternBegin, patternEnd);
	}

	~Finder() {
		if (m_str_find)
			AllocT::deallocate(m_str_find);
	}
	
	/*
	@fn initPattern(szPattern,cchLen)
	@brief					Initialize the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see Finder, initPattern, getPattern
	@*/
	template <class Iterator>
	HRESULT winx_call initPattern(Iterator patternBegin, Iterator patternEnd)
	{
		WINX_STATIC_ASSERT(sizeof(char_type) == sizeof(uchar_type));

		if (patternBegin == patternEnd)
			return E_INVALIDARG;

		if (m_str_find)
			AllocT::deallocate(m_str_find);
		
		size_type cchLen = std::distance(patternBegin, patternEnd);
		m_str_find = (uchar_type*)AllocT::allocate( (sizeof(uchar_type) + sizeof(size_type)) * cchLen);
		m_next = (size_type*)(m_str_find + cchLen);
		m_size = cchLen;
		
		Strategy::copy((char_type*)m_str_find, patternBegin, cchLen);
		
		size_type k;
		m_next[0] = npos;
		for (size_type j = 1; j != cchLen; ++j)
		{
			k = m_next[j-1];
			while (k != (size_type)npos && m_str_find[k] != m_str_find[j-1])
				k = m_next[k];
			m_next[j] = k + 1;
		}
		return S_OK;
	}

	/*
	@fn initPattern(szPattern)
	@brief					Initialize the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The null-terminated pattern string.
	@see Finder, initPattern, getPattern
	@*/
	HRESULT winx_call initPattern(const char_type* szPattern)
	{
		if (szPattern == NULL)
			return E_INVALIDARG;
		return initPattern(szPattern, szPattern + std::char_traits<CharT>::length(szPattern));
	}

	/*
	@fn initPattern(strPattern)
	@brief					Initialize the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The pattern string object.
	@see Finder, initPattern, getPattern
	@*/
	template <class Tr, class AllocT2>
	HRESULT winx_call initPattern(const std::basic_string<CharT, Tr, AllocT2>& strPattern)
	{
		return initPattern(strPattern.c_str(), strPattern.size());
	}

	/*
	@fn good
	@brief					Indicates the state of the finder object.
	@return
		@val true(nonzero)
			The finder object is initialized with a pattern string.
		@val false(zero)
			The finder object is uninitialized.
	@*/
	int winx_call good() const
	{
		return m_str_find != NULL;
	}

	/*
	@fn size
	@brief					Get the length of the pattern string.
	@return
		Returns the length of the pattern string if the finder object is initialized.
		Returns zero if the finder object is uninitialized.
	@see good
	@*/
	size_type winx_call size() const
	{
		return m_size;
	}

	/*
	@fn getPattern
	@brief					Get the pattern string.
	@arg [out] strPattern	Returns the pattern string.
	@see Finder, initPattern
	@*/
	template <class StringT>
	void winx_call getPattern(StringT& strPattern) const
	{
		strPattern.assign(m_str_find, m_size);
	}

	/*
	@fn next
	@brief					Searching the pattern string from current position of an archive object.
	@arg [in] ar			The archive object.
	@arg [in] limit
		The limit scope of searching. Default is <em>nolimit</em>.
		The searching operation will be limited within [cp, cp+limit). Here cp means the current position
		of the archive. If limit is specified <em>nolimit</em>, that means limit to the end of the archive.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the archive.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@remark
		If searching succeeds (returns S_OK), the new current position of the archive object is the end of
		pattern string found in the archive. For example, assume the archive contents are "1234abcdefg", 
		and the pattern string is "abc", then after the searching operation, the current position of the
		archive pointer to "defg", not "abcdefg". \p
		If searching fails, the new current position of the archive object is undefined.
	@*/
	template <class ArchiveT>
	HRESULT winx_call next(ArchiveT& ar, size_type limit = (size_type)nolimit) const
	{
		if (m_size == 0)
			return E_ACCESSDENIED;

		size_t j = 0;
		for (; limit != 0; --limit)
		{
			typename ArchiveT::int_type ch_ = Strategy::get(ar);
			if (ch_ == ArchiveT::endch)
				break;

			while (m_str_find[j] != ch_)
			{
				j = m_next[j];
				if (j == (size_t)npos)
					break;
			}
			
			if (++j == m_size)
				return S_OK;
		}
		return S_FALSE;
	}

	/*
	@fn istreamNext
	@brief					Searching the pattern string from current position of an std::istream object.
	@arg [in] is			The istream object.
	@arg [in] limit
		The limit scope of searching. Default is <em>nolimit</em>.
		The searching operation will be limited within [cp, cp+limit). Here cp means the current position
		of the istream. If limit is specified <em>nolimit</em>, that means limit to the end of the istream.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the istream.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@remark
		If searching succeeds (returns S_OK), the new current position of the istream object is the end of
		pattern string found in the istream. For example, assume the istream contents are "1234abcdefg", 
		and the pattern string is "abc", then after the searching operation, the current position of the
		istream pointer to "defg", not "abcdefg". \p
		If searching fails, the new current position of the istream object is undefined.
	@see next
	@*/
	template <class istream_type>
	HRESULT winx_call istreamNext(istream_type& is, size_type limit = (size_type)nolimit) const
	{
		return next( (_istream_adapter<istream_type>&)is, limit );
	}

	/*
	@fn iteratorNext
	@brief					Searching the pattern string in a text specified by an iterator scope.
	@arg [in] it			Start of the text (iterator begin).
	@arg [in] limit
		The limit scope of searching. It can't be <em>nolimit</em>.
		The searching operation will be limited within [it, it+limit).
	@arg [out] pitFind
		If searching succeeds (returns S_OK), Returns the end of pattern string found in the text.
		For example, assume the text are "1234abcdefg", and the pattern string is "abc", then after
		the searching operation, *pitFind will pointer to "defg", not "abcdefg". \p
		If searching fails, *pitFind is undefined.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the text.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@see next
	@*/
	template <class iterator_type>
	HRESULT winx_call iteratorNext(iterator_type it, size_type limit, iterator_type* pitFind) const
	{
		WINX_ASSERT(limit >= 0);
		_iterator_adapter<iterator_type> is( it );
		HRESULT hr = next( is, limit );
		*pitFind = is.tell();
		return hr;
	}

	/*
	@fn cstrNext
	@brief					Searching the pattern string in a text specified by a C-Style string.
	@arg [in] text			The text. It's a null-terminated string.
	@arg [out] ppFind
		If searching succeeds (returns S_OK), Returns the end of pattern string found in the text.
		For example, assume the text are "1234abcdefg", and the pattern string is "abc", then after
		the searching operation, *ppFind will pointer to "defg", not "abcdefg". \p
		If searching fails, *ppFind is undefined.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the text.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@see next
	@*/
	HRESULT winx_call cstrNext(const char_type* text, const char_type** ppFind) const
	{
		_cstr_adapter<char_type> is( text );
		HRESULT hr = next( is );
		*ppFind = is.tell();
		return hr;
	}
};

// -------------------------------------------------------------------------
// class NoCaseFinder

/*
@class std::kmp::NoCaseFinder
@brief
	The std::kmp::NoCaseFinder class is a simple class which sets \<Finder> to non case-sensitive mode.
@arg CharT
	The data type of a single character to be found in searching algorithm. It can be char or WCHAR.
@see Finder
@*/
template <class CharT>
class NoCaseFinder : public Finder< CharT, MatchNoCase<CharT> >
{
private:
	typedef Finder< CharT, MatchNoCase<CharT> > BaseClass;

public:
    typedef typename BaseClass::size_type size_type;
    typedef typename BaseClass::char_type char_type;
    
	/*
	@ctor()
	@brief					Default constructor.
	@see std::kmp::Finder
	@*/
	NoCaseFinder() : BaseClass() {}

	/*
	@ctor(szPattern,cchLen)
	@brief					Construct the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see std::kmp::Finder
	@*/
	NoCaseFinder(const char_type* szPattern, size_type cchLen) : BaseClass(szPattern, cchLen) {}

	/*
	@ctor(szPattern)
	@brief					Construct the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The null-terminated pattern string.
	@see std::kmp::Finder
	@*/
	NoCaseFinder(const char_type* szPattern) : BaseClass(szPattern) {}

	/*
	@ctor(strPattern)
	@brief					Construct the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The pattern string object.
	@see std::kmp::Finder
	@*/
	template <class Tr, class AllocT2>
	NoCaseFinder(const std::basic_string<CharT, Tr, AllocT2>& strPattern) : BaseClass(strPattern) {}
};

// -------------------------------------------------------------------------
// $Log: Finder.h,v $
// Revision 1.4  2006/12/24 10:50:20  xushiwei
// Documentation: MatchCase, MatchNoCase, Finder, NoCaseFinder
//
// Revision 1.1  2006/12/02 08:00:44  xushiwei
// STL-Extension:
//  KMP-String-Find-Algorithm(class std::kmp::Finder/CaseFinder/NoCaseFinder)
//

NS_KMP_END

#endif /* STDEXT_KMP_FINDER_H */
