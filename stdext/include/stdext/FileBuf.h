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

// -------------------------------------------------------------------------
// class FileBuf

#if defined(WINX_FILEBUF_USE_WINFILE)

#include "filebuf/WinFile.h"

NS_STDEXT_BEGIN

typedef WinFileBuf FileBuf;

NS_STDEXT_END

#else

#include "filebuf/Stdio.h"

NS_STDEXT_BEGIN

typedef FILEFileBuf FileBuf;

NS_STDEXT_END

#endif

// -------------------------------------------------------------------------
// $Log: FileBuf.h,v $

#endif /* STDEXT_FILEBUF_H */

