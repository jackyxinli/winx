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
// Module: ShellGuid.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 10:37:01
// 
// $Id: ShellGuid.cpp,v 1.1 2006/08/25 10:09:46 xushiwei Exp $
// -----------------------------------------------------------------------*/

#ifndef __WINX_CONFIG_H__
#include <winx/Config.h>
#endif

// -------------------------------------------------------------------------

#if defined(WINX_USE_WINSDK)

#include "../../../winsdk/include/initguid.h"

#ifndef _SHLGUID_H_
#include "../../../winsdk/include/shlguid.h"
#endif

#else // defined(WINX_USE_DEFSDK)

#include <initguid.h>

#ifndef _SHLGUID_H_
#include <shlguid.h>
#endif

#endif

// -------------------------------------------------------------------------
// $Log: ShellGuid.cpp,v $
// Revision 1.1  2006/08/25 10:09:46  xushiwei
// ShellGuid
//
