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
// Module: winx/winsdk/Shell.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 11:07:48
// 
// $Id: Shell.h,v 1.2 2006/08/26 03:47:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WINSDK_SHELL_H__
#define __WINX_WINSDK_SHELL_H__

#ifndef __WINX_WINSDK_BASIC_H__
#include "Basic.h"
#endif

// =========================================================================
// WINX_USE_WINSDK

#if defined(WINX_USE_WINSDK)

#ifndef _INC_SHELLAPI
#include "../../../../winsdk/include/shellapi.h"
#endif

#ifndef _INC_SHLWAPI
#include "../../../../winsdk/include/shlwapi.h"
#endif

#ifndef _SHLOBJ_H_
#include "../../../../winsdk/include/shlobj.h"
#endif

#ifndef __shldisp_h__
#include "../../../../winsdk/include/shldisp.h"
#endif

#ifndef __shtypes_h__
#include "../../../../winsdk/include/shtypes.h"
#endif

#ifndef __shobjidl_h__
#include "../../../../winsdk/include/shobjidl.h"
#endif

#ifndef __propidl_h__
#include "../../../../winsdk/include/propidl.h"
#endif

// =========================================================================
// WINX_USE_DEFSDK

#else // defined(WINX_USE_DEFSDK)

#ifndef _INC_SHELLAPI
#include <shellapi.h>
#endif

#ifndef _INC_SHLWAPI
#include <shlwapi.h>
#endif

#ifndef _SHLOBJ_H_
#include <shlobj.h>
#endif

#if defined(WINX_VC6)

#define WINX_NO_AUTOCOMPLETE

#else

#ifndef __shldisp_h__
#include <shldisp.h>
#endif

#ifndef __shtypes_h__
#include <shtypes.h>
#endif

#ifndef __shobjidl_h__
#include <shobjidl.h>
#endif

#ifndef __propidl_h__
#include <propidl.h>
#endif

#endif

#endif // defined(WINX_USE_DEFSDK)

// =========================================================================
// $Log: Shell.h,v $
// Revision 1.2  2006/08/26 03:47:04  xushiwei
// Winx-Extension:
//    AutoComplete support
//    DropFileHandle(simple wrapper for HDROP)
//

#endif /* __WINX_WINSDK_SHELL_H__ */
