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
// Module: stdext/FileBuf.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-25 15:33:27
// 
// $Id: FileBuf.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_FILEBUF_H
#define STDEXT_FILEBUF_H

#if (1)
#define WINX_FILEBUF_USE_WINFILE
#define WINX_FILEBUF_USE_WINFILEMAPPING
#endif

#if !defined(X_OS_WINDOWS)
#define WINX_FILEBUF_USE_MMAP
#endif

#if defined(WINX_FILEBUF_USE_WINFILE)
#include "filebuf/WinFile.h"
#endif

#if defined(WINX_FILEBUF_USE_WINFILEMAPPING)
#include "filebuf/WinFileMapping.h"
#endif

#if defined(WINX_FILEBUF_USE_MMAP)
#include "filebuf/MMap.h"
#endif

#ifndef STDEXT_FILEBUF_STDIO_H
#include "filebuf/Stdio.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class FileBuf

#if defined(X_OS_WINDOWS) && defined(WINX_FILEBUF_USE_WINFILE)

typedef WinFileBuf FileBuf;

#else

typedef FILEFileBuf FileBuf;

#endif

// -------------------------------------------------------------------------
// class FileMapBuf

#if defined(X_OS_WINDOWS)

typedef WinFileMappingFileBuf FileMapBuf;

#else

typedef MMapFileBuf FileMapBuf;

#endif

// -------------------------------------------------------------------------
// $Log: FileBuf.h,v $

NS_STDEXT_END

#endif /* STDEXT_FILEBUF_H */

