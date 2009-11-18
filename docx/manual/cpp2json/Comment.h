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
// Module: cpp2json/Comment.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Comment.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef CPP2JSON_COMMENT_H
#define CPP2JSON_COMMENT_H

#ifndef TPL_C_COMMENT_H
#include <tpl/c/Comment.h>
#endif

#ifndef CPP2JSON_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------

extern dom::Mark tagComment;

// -------------------------------------------------------------------------

#define comment_doc		( done()/tagComment )
#define comment			( skipws() % cpp_comment_content(alloc, comment_doc) )

// -------------------------------------------------------------------------

#endif /* CPP2JSON_COMMENT_H */
