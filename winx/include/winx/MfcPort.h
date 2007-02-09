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
// Module: winx/MfcPort.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-9 19:55:45
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __WINX_MFCPORT_H__
#define __WINX_MFCPORT_H__

#ifndef __WINX_BASIC_H__
#include "winx/Basic.h"
#endif

#if defined(WINX_VC_NET)
#pragma warning(disable:4996) // crt declared deprecated
#endif

// -------------------------------------------------------------------------
// Diagnostic support

#ifndef ASSERT
#define ASSERT(e)			WINX_ASSERT(e)
#endif

#ifndef ASSERT_VALID
#define ASSERT_VALID(pOb)	WINX_ASSERT((pOb) != NULL)
#endif

#ifndef VERIFY
#define VERIFY(e)			WINX_VERIFY(e)
#endif

#ifndef TRACE
#define TRACE				WINX_TRACE
#endif

#ifndef TRACE0
#define TRACE0				WINX_TRACE
#endif

#ifndef TRACE1
#define TRACE1				WINX_TRACE
#endif

#ifndef TRACE2
#define TRACE2				WINX_TRACE
#endif

#ifndef TRACE3
#define TRACE3				WINX_TRACE
#endif

#ifndef DEBUG_ONLY
#define DEBUG_ONLY(e)		WINX_DEBUG_ONLY(e)
#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* __WINX_MFCPORT_H__ */
