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
// Module: tpl/c/Lex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: RegExp.h 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_C_LEX_H
#define TPL_C_LEX_H

#ifndef TPL_REGEX_BASIC_H
#include "../regex/Basic.h"
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "../regex/Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "../regex/Composition.h"
#endif

#ifndef TPL_REGEX_UCOMPOSITION_H
#include "../regex/UComposition.h"
#endif

#ifndef TPL_REGEX_FIND_H
#include "../regex/Find.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function c_comment()

typedef Ch<'/'> ChDiv_;
typedef Ch<'*'> ChMul_;
typedef UAnd<ChDiv_, ChMul_> CCommentStart_;

struct FindCCommentEnd_ : UFindStr<char> {
	FindCCommentEnd_() : UFindStr<char>("*/") {}
};

typedef UAnd<CCommentStart_, FindCCommentEnd_> CComment;

TPL_REGEX_GUARD(CComment, CCommentG, TagAssigNone);

inline Rule<CCommentG> TPL_CALL c_comment() {
	return Rule<CCommentG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_C_LEX_H */

