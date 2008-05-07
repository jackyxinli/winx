//
//  boost/detail/winapi/atomic/windows.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/detail/todo.htm for documentation.
//
#ifndef BOOST_DETAIL_WINAPI_ATOMIC_WINDOWS_HPP
#define BOOST_DETAIL_WINAPI_ATOMIC_WINDOWS_HPP

// -------------------------------------------------------------------------

// we use our own implementation of InterlockedExchangePointer because of problems with the one in system headers
__forceinline PVOID WINAPI boost_InterlockedExchangePointer(PVOID* Target, PVOID Value)
{
	return reinterpret_cast<PVOID>(static_cast<LONG_PTR>(
		InterlockedExchange(
			reinterpret_cast<LONG*>(Target),
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(Value))
			)
		));
}

#undef InterlockedExchangePointer
#define InterlockedExchangePointer boost_InterlockedExchangePointer

// -------------------------------------------------------------------------

#if defined(BOOST_DETAIL_WINSDK_VC6)

__forceinline PVOID WINAPI boost_InterlockedCompareExchange(
	LPLONG Destination, LONG Exchange, LONG Comperand)
{
	return InterlockedCompareExchange(
		(PVOID*)Destination, (PVOID)Exchange, (PVOID)Comperand);
}

__forceinline PVOID WINAPI boost_InterlockedCompareExchangePointer(
	PVOID* Destination, PVOID Exchange, PVOID Comperand)
{
	return InterlockedCompareExchange(
		Destination, Exchange, Comperand);
}

#define InterlockedCompareExchange			boost_InterlockedCompareExchange
#define InterlockedCompareExchangePointer	boost_InterlockedCompareExchangePointer

#elif !defined(InterlockedCompareExchangePointer)

__forceinline PVOID WINAPI boost_InterlockedCompareExchangePointer(
	PVOID* Destination, PVOID Exchange, PVOID Comperand)
{
	return reinterpret_cast<PVOID>(static_cast<LONG_PTR>(
		InterlockedCompareExchange(
			reinterpret_cast<LONG*>(Destination),
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(Exchange)),
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(Comperand))
			)
		));
}

#define InterlockedCompareExchangePointer boost_InterlockedCompareExchangePointer

#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* BOOST_DETAIL_WINAPI_ATOMIC_WINDOWS_HPP */
