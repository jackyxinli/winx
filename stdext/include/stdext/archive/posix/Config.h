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
// Module: stdext/archive/posix/Config.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 21:07:06
// 
// $Id: Config.h,v 1.3 2007/01/10 09:36:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_POSIX_CONFIG_H__
#define __STDEXT_ARCHIVE_POSIX_CONFIG_H__

#ifndef __STDEXT_PLATFORM_H__
#include "../../Platform.h"
#endif

#if defined(X_OS_WINDOWS)
// -------------------------------------------------------------------------
// Windows

#ifndef _INC_IO
#include <io.h>
#endif

#ifndef	_INC_FCNTL
#include <fcntl.h>
#endif

typedef long __off_t;

#elif defined(X_OS_LINUX)
// -------------------------------------------------------------------------
// Linux

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef	_FCNTL_H
#include <fcntl.h>
#endif

#ifndef	_UNISTD_H
#include <unistd.h>
#endif

// -------------------------------------------------------------------------
// $Log: Config.h,v $

#endif

#endif /* __STDEXT_ARCHIVE_POSIX_CONFIG_H__ */
