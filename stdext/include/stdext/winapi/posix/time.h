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
// Module: stdext/winapi/posix/time.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_WINAPI_POSIX_TIME_H__
#define __STDEXT_WINAPI_POSIX_TIME_H__

#ifndef __STDEXT_PLATFORM_H__
#include "../../Platform.h"
#endif

#ifndef __STDEXT_WINAPI_WTYPES_H__
#include "../wtypes.h"
#endif

#ifndef WINBASEAPI
#define WINBASEAPI inline
#endif

// =========================================================================

#if defined(X_OS_UNIX)
#define STD_USE_MONOTONIC_CLOCK
#endif

#if defined(STD_USE_MONOTONIC_CLOCK)

// int clock_getres(clockid_t clk_id, struct timespec *res);
// int clock_gettime(clockid_t clk_id, struct timespec *tp);
// int clock_settime(clockid_t clk_id, const struct timespec *tp);

#ifndef _TIME_H
#include <time.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

// See http://linux.die.net/man/3/clock_gettime
// On POSIX systems on which these functions are available, the symbol _POSIX_TIMERS
// is defined in <unistd.h> to a value greater than 0.
// The symbols _POSIX_MONOTONIC_CLOCK, _POSIX_CPUTIME, _POSIX_THREAD_CPUTIME indicate 
// that CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID are available.
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0) && defined(_POSIX_MONOTONIC_CLOCK)
#define STD_HAS_MONOTONIC_CLOCK
#endif

#endif // defined(STD_USE_MONOTONIC_CLOCK)

// =========================================================================
// QueryPerformanceFrequency

#if defined(STD_HAS_MONOTONIC_CLOCK)

WINBASEAPI BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lp)
{
	lp->QuadPart = 1000000000;
	return TRUE;
}

WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lp)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    UINT64 ticks = (UINT64)now.tv_sec * 1000000000 + now.tv_nsec;
    lp->QuadPart = ticks;
	return TRUE;
}

#else // !defined(STD_HAS_MONOTONIC_CLOCK)

#if defined(X_OS_WINDOWS)

EXTERN_C BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lp);

#elif defined(X_OS_UNIX)

#ifndef _STDIO_H
#include <stdio.h>
#endif

inline UINT64 WINAPI __get_cpu_freq()
{
    FILE* fd = 0;
    UINT64 freq = 0;
    float freqf = 0;
	char* line = NULL;
    size_t len = 0;

    fd = fopen("/proc/cpuinfo", "r");
    if (!fd)
		return 0;

	while (getline(&line, &len, fd) != EOF) {
		if(sscanf(line, "cpu MHz\t: %f", &freqf) == 1) {
			freqf = freqf * 1000000UL;
            freq = freqf;
            break;
		}
    }
    fclose(fd);
    return freq;
}

WINBASEAPI BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lp)
{
	lp->QuadPart = __get_cpu_freq();
    return TRUE;
}

#else

#error "QueryPerformanceFrequency - todo"

#endif

// -------------------------------------------------------------------------
// QueryPerformanceCounter

#if defined(X_CC_VC)

#pragma warning(disable:4035)
__forceinline __declspec(naked) unsigned __int64 __RDTSC()
{
    _asm _emit  0x0F
    _asm _emit  0x31
	_asm ret
}
#pragma warning(default:4035)

WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lp)
{
	lp->QuadPart = __RDTSC();
    return TRUE;
}

#elif defined(X_CC_GCC)

WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lp)
{
	__asm__ __volatile__("rdtsc" : "=A" (lp->QuadPart));
	return TRUE;
}

#else

#error "QueryPerformanceCounter - todo"

#endif

#endif // defined(STD_HAS_MONOTONIC_CLOCK)

// =========================================================================
// $Log: $

#endif /* __STDEXT_WINAPI_POSIX_TIME_H__ */
