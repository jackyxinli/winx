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
// Module: stdext/text/sgi/Rope.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Rope.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_SGI_ROPE_H__
#define __STDEXT_TEXT_SGI_ROPE_H__

#ifndef __STDEXT_BASIC_H__
#include "../../Basic.h"
#endif

#if !defined(NO_SGISTL)

#ifndef __SGI_ROPE_H__
#include "../../sgi/rope.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class SequenceBuffer

template <class _Sequence, size_t _Buf_sz = 512>
class SequenceBuffer : public stdext::sequence_buffer<_Sequence, _Buf_sz>
{
private:
	typedef stdext::sequence_buffer<_Sequence, _Buf_sz> _Base;

protected:
	using _Base::_M_buffer;
	using _Base::_M_buf_count;
	using _Base::_M_prefix;

public:
	typedef typename _Base::value_type value_type;

public:
	using _Base::append;

public:
    SequenceBuffer() {}
    SequenceBuffer(_Sequence& __s) : _Base(__s) {}

    void winx_call append(const value_type* __s, size_t __len)
	{
		_Base::append((value_type*)__s, __len);
	}

	void winx_call append(TempString<value_type> s)
	{
		append(s.data(), s.size());
	}

    void winx_call append(size_t __len, const value_type& __val)
    {
        if (__len + _M_buf_count <= _Buf_sz) {
			std::fill_n(_M_buffer + _M_buf_count, __len, __val);
            _M_buf_count += __len;
        } else if (0 == _M_buf_count) {
            _M_prefix->append(__len, __val);
        } else {
			_Base::flush();
            append(__len, __val);
        }
    }
};

// -------------------------------------------------------------------------
// class Rope

template <class CharT, class AllocT = ScopeAlloc>
class Rope : public stdext::rope<CharT, StlAlloc<CharT, AllocT> >
{
private:
	typedef StlAlloc<CharT, AllocT> _Alloc;
	typedef stdext::rope<CharT, _Alloc> _Base;

public:
	using _Base::compare;

public:
	explicit Rope(AllocT& alloc) : _Base(alloc)
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

	void winx_call copy(const _Base& from)
	{
		_Base::operator=(from);
	}

public:
	int winx_call compare(TempString<CharT> s) const
	{
		_Base b(s.begin(), s.end(), _Base::get_allocator());
		return _Base::compare(b);
	}
};

// -------------------------------------------------------------------------
// $Log: Rope.h,v $

__NS_STD_END

#endif // !defined(NO_SGISTL)

#endif /* __STDEXT_TEXT_SGI_ROPE_H__ */
