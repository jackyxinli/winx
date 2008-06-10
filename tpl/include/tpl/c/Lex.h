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

#ifndef TPL_REGEX_UCOMPOSITION_H
#include "../regex/UComposition.h"
#endif

#ifndef TPL_REGEX_FIND_H
#include "../regex/Find.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function c_find_continuable_eol()

class CFindContinuableEol
{
public:
	enum { character = 0 };

	typedef SelfConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		for (;;)
		{
			typename SourceT::int_type c = ar.get();
			switch (c)
			{
			case '\r':
				if (ar.peek() == '\n')
					ar.get();
				return true;
			case '\n':
				return true;
			case '\\':
				if (ar.get() == '\r') {
					if (ar.peek() == '\n')
						ar.get();
				}
				break;
			case SourceT::endch:
				return true;
			}
		}
	}

	TPL_SIMPLEST_GRAMMAR_();
};

typedef CFindContinuableEol CFindContinuableEolG;

inline Rule<CFindContinuableEolG> TPL_CALL c_find_continuable_eol() {
	return Rule<CFindContinuableEolG>();
}

// =========================================================================
// function cpp_single_line_comment()

typedef Ch<'/'> ChDiv_;

typedef UAnd<ChDiv_, CFindContinuableEolG> CppSingleLineCommentEnd_;

typedef UAnd<ChDiv_, CppSingleLineCommentEnd_> CppSingleLineComment;

TPL_REGEX_GUARD(CppSingleLineComment, CppSingleLineCommentG, TagAssigNone);

inline Rule<CppSingleLineCommentG> TPL_CALL cpp_single_line_comment() {
	return Rule<CppSingleLineCommentG>();
}

// =========================================================================
// function c_comment()

typedef Ch<'*'> ChMul_;

struct FindCCommentEnd_ : UFindStr<char> {
	FindCCommentEnd_() : UFindStr<char>("*/") {}
};

typedef UAnd<ChMul_, FindCCommentEnd_> CCommentEnd_;
typedef UAnd<ChDiv_, CCommentEnd_> CComment;

TPL_REGEX_GUARD(CComment, CCommentG, TagAssigNone);

inline Rule<CCommentG> TPL_CALL c_comment() {
	return Rule<CCommentG>();
}

// =========================================================================
// function cpp_comment()

typedef Or<CppSingleLineCommentEnd_, CCommentEnd_> CppCommentEnd_;

typedef UAnd<ChDiv_, CppCommentEnd_> CppComment;

TPL_REGEX_GUARD(CppComment, CppCommentG, TagAssigNone);

inline Rule<CppCommentG> TPL_CALL cpp_comment() {
	return Rule<CppCommentG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_C_LEX_H */

