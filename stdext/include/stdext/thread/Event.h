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
// Module: stdext/thread/Event.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Event.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_EVENT_H
#define STDEXT_THREAD_EVENT_H

#ifndef STDEXT_THREAD_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// =========================================================================
// class WinEvent

#if defined(STDEXT_THREAD_WINDOWS)

class WinEvent
{
private:
	WinEvent(const WinEvent&);
	void operator=(const WinEvent&);

	HANDLE hEvent;

public:
	typedef size_t duration_type;

public:
	WinEvent() {
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	~WinEvent() {
		CloseHandle(hEvent);
	}

	void winx_call wait() {
		WaitForSingleObject(hEvent, INFINITE);
	}

	bool winx_call timed_wait(duration_type ms) {
		return WaitForSingleObject(hEvent, ms) == WAIT_OBJECT_0;
	}

	void winx_call notify() {
		SetEvent(hEvent);
	}
};

#endif // defined(STDEXT_THREAD_WINDOWS)

// =========================================================================
// class Event

#if defined(STDEXT_THREAD_WINDOWS)

typedef WinEvent Event;

#else

typedef PthreadEvent Event;

#endif

// =========================================================================
// $Log: $

NS_STDEXT_END

#endif /* CERL_THREAD_EVENT_H */
