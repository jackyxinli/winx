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
// Module: tpl/regex/RegEx.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_REGEX_H
#define TPL_REGEX_REGEX_H

#ifndef STDEXT_MEMORY_H
#include "../../../stdext/include/stdext/Memory.h"
#endif

#ifndef STDEXT_ARCHIVE_MEM_H
#include "../../../stdext/include/stdext/archive/Mem.h"
#endif

#ifndef TPL_REGEX_BASICREGEX_H
#include "BasicRegEx.h"
#endif

#ifndef TPL_REGEX_CONTEXT_H
#include "Context.h"
#endif

// -------------------------------------------------------------------------
// class Source

typedef std::PointerReadArchive Source;

// -------------------------------------------------------------------------
// class SubMatch, MatchResult

typedef BasicSubMatch<Source::iterator> SubMatch;
typedef BasicMatchResult<Source::iterator> MatchResult;

// -------------------------------------------------------------------------
// class Allocator

typedef std::BlockPool BlockPool;
typedef std::ScopeAlloc Allocator;

// -------------------------------------------------------------------------
// class Context

typedef BasicContext<Source::iterator, Allocator> Context;

// -------------------------------------------------------------------------
// class RegEx

typedef BasicRegEx<Source, Context> RegEx;
typedef BasicRegEx<Source, Context, UnmanagedFactory> SimpleRegEx;

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_REGEX_H */
