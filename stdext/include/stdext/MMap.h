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
// Module: stdext/MMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-31 10:57:34
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_H
#define STDEXT_MMAP_H

// -------------------------------------------------------------------------

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// class MapFile

#if defined(X_OS_WINDOWS)

#ifndef STDEXT_MAPFILE_WINFILEMAPPING_H
#include "mapfile/WinFileMapping.h"
#endif

#else

#ifndef STDEXT_MAPFILE_MMAP_H
#include "mapfile/MMap.h"
#endif

#endif

NS_STDEXT_BEGIN

typedef MapFile<MappingReadWrite> MapFileRW;
typedef MapFile<MappingReadOnly> MapFileRO;

NS_STDEXT_END

// -------------------------------------------------------------------------

/*
#ifndef STDEXT_MMAP_WINFILEMAPPING_H
#include "mmap/WinFileMapping.h"
#endif

#ifndef STDEXT_MMAP_MMAP_H
#include "mmap/MMap.h"
#endif
*/

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_MMAP_H */

