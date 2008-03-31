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
// Module: stdext/archive/MemArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: MemArchive.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_MEMARCHIVE_H__
#define __STDEXT_ARCHIVE_MEMARCHIVE_H__

#ifndef __STDEXT_ARCHIVE_BASIC_H__
#include "Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class MemWriteArchive

// BaseStg = std::vector, std::deque or std::basic_string
template <class BaseStg>
class MemWriteArchive
{
private:
	MemWriteArchive(const MemWriteArchive&);
	void operator=(const MemWriteArchive&);

public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename BaseStg::value_type char_type;
	typedef typename ArchiveCharTraits<char_type>::int_type int_type;
	typedef typename size_type pos_type;
	typedef typename difference_type off_type;
	
protected:
	typedef typename BaseStg::iterator InnerPosT;

	BaseStg* m_stg;
	InnerPosT m_pos;

public:
	template <class AllocT>
	explicit MemWriteArchive(AllocT& /* alloc */)
		: m_stg(NULL)
	{
	}

	template <class AllocT>
	MemWriteArchive(AllocT& alloc, BaseStg* stg)
		: m_stg(stg)
	{
		m_pos = stg->begin();
	}

	void winx_call clear_cache()
	{
	}

	void winx_call flush()
	{
	}

	pos_type winx_call tell() const
	{
		m_pos - m_stg->begin();
	}

	void winx_call seek(pos_type offset)
	{
		if (offset > size())
			m_stg->resize(offset);
		m_pos = m_stg->begin() + offset;
	}

	void winx_call seek_end()
	{
		m_pos = m_stg->end();
	}
	
public:
	void winx_call reput(size_type offset, const char_type* lpBuf, size_type cch)
	{
		WINX_ASSERT((int)offset > 0 && offset >= cch);
		
		std::copy(lpBuf, lpBuf + cch, m_pos - offset);
	}

	size_type winx_call put(const char_type* lpBuf, const size_type cch)
	{
		const InnerPosT posEnd = m_stg->end();
		const size_type rest = posEnd - m_pos;
		if (rest == 0) {
			m_stg->insert(posEnd, lpBuf, lpBuf + cch);
		}
		else if (rest < cch) {
			std::copy(lpBuf, lpBuf + rest, m_pos);
			m_stg->insert(posEnd, lpBuf + rest, lpBuf + cch);
		}
		else {
			std::copy(lpBuf, lpBuf + cch, m_pos);
			m_pos += cch;
			return cch;
		}
		m_pos = m_stg->end();
		return cch;
	}

	size_type winx_call put(char_type ch)
	{
		const InnerPosT posEnd = m_stg->end();
		if (m_pos != posEnd) {
			*m_pos++ = ch;
		}
		else {
			m_stg->insert(posEnd, ch);
			m_pos = m_stg->end();
		}
		return 1;
	}

public:
	HRESULT winx_call open(BaseStg* stg)
	{
		if (good())
			return E_ACCESSDENIED;
		m_stg = stg;
		m_pos = stg->begin();
		return S_OK;
	}

	void winx_call close()
	{
		m_stg = NULL;
	}

public:
	pos_type winx_call size() const
	{
		WINX_ASSERT(m_stg);
		return m_stg->size();
	}

	int winx_call operator!() const
	{
		return m_stg == NULL;
	}

	int winx_call good() const
	{
		return m_stg != NULL;
	}

	int winx_call bad() const
	{
		return m_stg == NULL;
	}
};

// -------------------------------------------------------------------------
// class MemReadArchive

template <class Iterator>
class MemReadArchive
{
public:
	typedef typename ArchiveIteratorTraits<Iterator>::char_type char_type;

private:
	typedef ArchiveCharTraits<char_type> _Tr;

public:
	enum { endch = _Tr::endch };

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename _Tr::int_type int_type;
	typedef typename _Tr::uchar_type uchar_type;

	typedef size_type pos_type;
	typedef difference_type off_type;
	
protected:
	Iterator m_pos;
	Iterator m_first;
	Iterator m_last;

private:
	MemReadArchive(const MemReadArchive&);
	void operator=(const MemReadArchive&);
	
public:
	template <class AllocT>
	MemReadArchive(AllocT& /* alloc */, Iterator first, Iterator last)
		: m_pos(first), m_first(m_pos), m_last(last)
	{
	}

	template <class AllocT, class ContainerT>
	MemReadArchive(AllocT& /* alloc */, const ContainerT* cont)
		: m_pos(cont->begin()), m_first(m_pos), m_last(cont->end())
	{
	}

public:
	void winx_call clear_cache()
	{
	}

	pos_type winx_call tell() const
	{
		return m_pos - m_first;
	}

	Iterator winx_call position() const
	{
		return m_pos;
	}

	void winx_call seek(pos_type offset)
	{
		if (offset > size())
			throw std::out_of_range("Seek file position - out of range!");
		m_pos = m_first + offset;
	}

	void winx_call seek_end()
	{
		m_pos = m_last;
	}

public:
	size_type winx_call skip(size_type nMax)
	{
		const size_type nRest = m_last - m_pos;
		if (nRest < nMax) {
			m_pos = m_last;
			return nRest;
		}
		else {
			m_pos += nMax;
			return nMax;
		}
	}

	size_type winx_call get(char_type* lpBuf, size_type nMax)
	{
		const size_type nRest = m_last - m_pos;
		if (nRest < nMax) {
			std::copy(m_pos, m_last, lpBuf);
			m_pos = m_last;
			return nRest;
		}
		else {
			std::copy(m_pos, m_pos + nMax, lpBuf);
			m_pos += nMax;
			return nMax;
		}
	}

	int_type winx_call get()
	{
		if (m_pos != m_last) {
			return (uchar_type)*m_pos++;
		}
		else {
			return endch;
		}
	}

	void winx_call reget(size_type offset, char_type* lpBuf, size_type cch)
	{
		WINX_ASSERT((int)offset > 0 && offset >= cch);

		std::copy(m_pos - offset, m_pos - offset + cch, lpBuf);
	}

	int winx_call unget()
	{
		if (m_pos != m_first) {
			--m_pos;
			return 1;
		}
		return 0;
	}

	void winx_call unget(int_type ch)
	{
		if (ch != endch)
			unget();
	}

	int_type winx_call peek()
	{
		if (m_pos != m_last) {
			return (uchar_type)*m_pos;
		}
		else {
			return endch;
		}
	}

public:
	pos_type winx_call size() const
	{
		return m_last - m_first;
	}

	int winx_call operator!() const
	{
		return FALSE;
	}

	int winx_call good() const
	{
		return TRUE;
	}

	int winx_call bad() const
	{
		return FALSE;
	}
};

// -------------------------------------------------------------------------
// $Log: MemArchive.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_MEMARCHIVE_H__ */
