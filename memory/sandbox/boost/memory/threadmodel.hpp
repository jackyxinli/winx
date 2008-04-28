//
//  boost/memory/threadmodel.hpp (*)
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_THREADMODEL_HPP__
#define __BOOST_MEMORY_THREADMODEL_HPP__

// -------------------------------------------------------------------------

#ifndef __BOOST_MEMORY_THREADMODEL_SINGLE_THREAD_H__
#include "threadmodel/single_thread.hpp"
#endif

#ifndef __BOOST_MEMORY_THREADMODEL_MULTI_THREAD_H__
#include "threadmodel/multi_thread.hpp"
#endif

__NS_BOOST_BEGIN

// -------------------------------------------------------------------------
// class auto_lock

template <class LockT>
class auto_lock
{
private:
	LockT& m_lock;

private:
	auto_lock(const auto_lock&);
	void operator=(const auto_lock&);

public:
	auto_lock(LockT& lock) : m_lock(lock)
	{
		m_lock.acquire();
	}
	~auto_lock()
	{
		m_lock.release();
	}
};

// -------------------------------------------------------------------------
// class multi_thread

class multi_thread
{
public:
	typedef refcount_mt refcount;
	typedef critical_section_mt critical_section;

public:
	typedef critical_section cs;
	typedef auto_lock<cs> cslock;
};

// -------------------------------------------------------------------------
// class single_thread

class single_thread
{
public:
	typedef refcount_st refcount;
	typedef critical_section_st critical_section;

public:
	typedef critical_section cs;
	typedef auto_lock<cs> cslock;
};

// -------------------------------------------------------------------------
// class default_threadmodel

#if defined(_MT)
typedef multi_thread default_threadmodel;
#else
typedef single_thread default_threadmodel;
#endif

typedef single_thread initializer_threadmodel;

// -------------------------------------------------------------------------
// $Log: $

__NS_BOOST_END

#endif /* __BOOST_MEMORY_THREADMODEL_HPP__ */
