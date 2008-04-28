//
//  boost/memory.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_HPP__
#define __BOOST_MEMORY_HPP__

// -------------------------------------------------------------------------

#ifndef __BOOST_MEMORY_AUTO_ALLOC_HPP__
#include "memory/auto_alloc.hpp"
#endif

#ifndef __BOOST_MEMORY_SCOPED_ALLOC_HPP__
#include "memory/scoped_alloc.hpp"
#endif

// -------------------------------------------------------------------------
// class stl_alloc

__NS_BOOST_BEGIN

template <class _Ty, class _Alloc = scoped_alloc>
class stl_alloc
{
private:
	_Alloc* m_alloc;

public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Ty* pointer;
	typedef const _Ty* const_pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef _Ty value_type;

    template <class U>
    struct rebind { typedef stl_alloc<U, _Alloc> other; };

public:
	pointer address(reference val) const
		{ return &val; }
	const_pointer address(const_reference val) const
		{ return &val; }

	size_type max_size() const
		{ size_type count = (size_type)(-1) / sizeof (_Ty);
		  return (0 < count ? count : 1); }

public:
	stl_alloc(_Alloc& alloc) : m_alloc(&alloc) {}

    template <class U>
	stl_alloc(const stl_alloc<U, _Alloc>& rhs) : m_alloc(rhs._Getalloc()) {}

	pointer allocate(size_type count, const void* = NULL)
		{ return (pointer)m_alloc->allocate(count * sizeof(_Ty)); }
	void deallocate(void* p, size_type cb)
		{ m_alloc->deallocate(p, cb); }
	void construct(pointer p, const _Ty& val)
		{ new(p) _Ty(val); }
	void destroy(pointer p)
		{ p->~_Ty(); }

public:
	char* _Charalloc(size_type cb)
		{ return (char*)m_alloc->allocate(cb); }

	_Alloc* _Getalloc() const { return m_alloc; }
};

template<> class stl_alloc<void, scoped_alloc>
{
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef stl_alloc<U, scoped_alloc> other; };
};

template<> class stl_alloc<void, auto_alloc>
{
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef stl_alloc<U, scoped_alloc> other; };
};

template <class _Ty, class _Alloc>
inline bool operator==(const stl_alloc<_Ty, _Alloc>&,
                       const stl_alloc<_Ty, _Alloc>&) {
    return true;
}

template <class _Ty, class _Alloc>
inline bool operator!=(const stl_alloc<_Ty, _Alloc>&,
                       const stl_alloc<_Ty, _Alloc>&) {
    return false;
}

__NS_BOOST_END

// -------------------------------------------------------------------------
// $Log: memory.hpp,v $

#endif /* __BOOST_MEMORY_HPP__ */
