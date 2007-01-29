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
// Module: winx/Trace.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-29 23:30:17
// 
// $Id: Trace.h,v 1.3 2006/12/21 04:01:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_TRACE_H__
#define __WINX_TRACE_H__

#ifndef __WINX_BASIC_H__
#include "Basic.h"
#endif

__WINX_BEGIN

// -------------------------------------------------------------------------
// _vsnprintf_dbg, _vsnwprintf_dbg

#if !defined(_vsnprintf_dbg) // _vsnwprintf_dbg
	#if defined(_TRUNCATE)
		#define _vsnprintf_dbg(buff, cchBuff, fmt, arglist)					\
			_vsnprintf_s(buff, cchBuff, _TRUNCATE, fmt, arglist)
		#define _vsnwprintf_dbg(buff, cchBuff, fmt, arglist)				\
			_vsnwprintf_s(buff, cchBuff, _TRUNCATE, fmt, arglist)
	#else
		#define _vsnprintf_dbg(buff, cchBuff, fmt, arglist)					\
			_vsnprintf(buff, cchBuff, fmt, arglist)
		#define _vsnwprintf_dbg(buff, cchBuff, fmt, arglist)				\
			_vsnwprintf(buff, cchBuff, fmt, arglist)
	#endif
#endif

// -------------------------------------------------------------------------
// _snprintf_dbg

#if !defined(_snprintf_dbg)
	#if defined(_TRUNCATE)
		#define _snprintf_dbg									_snprintf_s
		#define _TRUNCATE_PARAM									_TRUNCATE,
	#else
		#define _snprintf_dbg									_snprintf
		#define _TRUNCATE_PARAM
	#endif
#endif

// -------------------------------------------------------------------------
// MsgBoxTrace

inline void winx_call ExMsgBoxTraceV(
	LPCWSTR fmt, va_list arglist, LPCWSTR caption = L"Information", HWND hWnd = NULL)
{
	WCHAR buff[2048];	
	_vsnwprintf_dbg(buff, 2048, fmt, arglist);
	::MessageBoxW(hWnd, buff, caption, MB_TOPMOST|MB_OK);
}

inline void winx_call ExMsgBoxTraceV(
	LPCSTR fmt, va_list arglist, LPCSTR caption = "Information", HWND hWnd = NULL)
{
	char buff[2048];
	_vsnprintf_dbg(buff, 2048, fmt, arglist);
	::MessageBoxA(hWnd, buff, caption, MB_TOPMOST|MB_OK);
}

template <class E>
inline void __cdecl MsgBoxTrace(const E* fmt, ...)
{
	va_list arglist;
	va_start(arglist, fmt);
	ExMsgBoxTraceV(fmt, arglist);
	va_end(arglist);
}

template <class E>
inline void __cdecl ExMsgBoxTrace(const E* caption, const E* fmt, ...)
{
	va_list arglist;
	va_start(arglist, fmt);
	ExMsgBoxTraceV(fmt, arglist, caption);
	va_end(arglist);
}

template <class E>
inline void __cdecl ExMsgBoxTrace(HWND hWnd, const E* caption, const E* fmt, ...)
{
	va_list arglist;
	va_start(arglist, fmt);
	ExMsgBoxTraceV(fmt, arglist, caption, hWnd);
	va_end(arglist);
}

// -------------------------------------------------------------------------
// SimpleTrace

inline void winx_call SimpleTraceV(LPCWSTR fmt, va_list arglist, int nErrLevel = 0)
{
	WCHAR buff[2048];	
	_vsnwprintf_dbg(buff, 2048, fmt, arglist);
	::OutputDebugStringW(buff);
}

inline void winx_call SimpleTraceV(LPCSTR fmt, va_list arglist, int nErrLevel = 0)
{
	char buff[2048];
	_vsnprintf_dbg(buff, 2048, fmt, arglist);
	::OutputDebugStringA(buff);
}

template <class E>
inline void __cdecl SimpleTrace(const E* fmt, ...)
{
	va_list arglist;
	va_start(arglist, fmt);
	SimpleTraceV(fmt, arglist);
	va_end(arglist);
}

template <class E>
inline void __cdecl SimpleTrace(int nErrLevel, const E* fmt, ...)
{
	va_list arglist;
	va_start(arglist, fmt);
	SimpleTraceV(fmt, arglist, nErrLevel);
	va_end(arglist);
}

// -------------------------------------------------------------------------
// SimpleTraceFileLine

inline void winx_call _SimpleTraceFileLine(LPCSTR szFile, int nLine)
{
	SimpleTrace("%s(%d):\n >>> ", szFile, nLine);
}

#ifndef SimpleTraceFileLine
#define SimpleTraceFileLine()	_SimpleTraceFileLine(__FILE__, __LINE__)
#endif

// -------------------------------------------------------------------------
// class TraceStorage

template <class Base = std::ILogStorage>
class TraceStorageT : public Base
{
public:
	void winx_call put(int ch)
	{
		char msg[] = { ch, '\0' };
		OutputDebugStringA(msg);
	}
	
	void winx_call putw(wint_t wch)
	{
		WCHAR msg[] = { wch, '\0' };
		OutputDebugStringW(msg);
	}
	
	void winx_call put(size_t count, int ch)
	{
		char* msg = (char*)_alloca(count + 1);
		memset(msg, ch, count);
		msg[count] = '\0';
		OutputDebugStringA(msg);
	}
	
	void winx_call putw(size_t count, wint_t wch)
	{
		WCHAR* msg = (WCHAR*)_alloca(sizeof(WCHAR)*(count + 1));
		wmemset(msg, wch, count);
		msg[count] = '\0';
		OutputDebugStringW(msg);
	}
	
	void winx_call put(const char* s, size_t count)
	{
		char* msg = (char*)_alloca(count + 1);
		memcpy(msg, s, count);
		msg[count] = '\0';
		OutputDebugStringA(msg);
	}
	
	void winx_call put(const WCHAR* s, size_t count)
	{
		WCHAR* msg = (WCHAR*)_alloca(sizeof(WCHAR)*(count + 1));
		wmemcpy(msg, s, count);
		msg[count] = '\0';
		OutputDebugStringW(msg);
	}
	
	void winx_call putv(const char* fmt, va_list args)
	{
		SimpleTraceV(fmt, args);
	}

	void winx_call putv(const WCHAR* fmt, va_list args)
	{
		SimpleTraceV(fmt, args);
	}
};

typedef TraceStorageT<std::StorageBase> TraceStorage;

// -------------------------------------------------------------------------
// class TraceWindow

typedef std::Log<TraceStorage> TraceWindow;

// -------------------------------------------------------------------------
// $Log: Trace.h,v $
// Revision 1.3  2006/12/21 04:01:10  xushiwei
// STL-Extension: MultiStorage(select, enable, enableAll)
//
// Revision 1.1  2006/09/02 01:49:08  xushiwei
// WINX-Basic:
//   Log: TraceWindow, Storage: TraceStorage
// WINX-Extension-Gdiplus:
//   EncoderParameters: ColorDepth, Transformation, SaveFlag, Compression, Quality
//

__WINX_END

#endif /* __WINX_TRACE_H__ */
