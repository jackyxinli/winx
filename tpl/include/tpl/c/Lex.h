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

#ifndef TPL_REGEXP_H
#include "../RegExp.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function c_find_continuable_eol<bEat>()

template <bool bEat = false>
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
				if (!bEat)
					ar.unget('\r');
				else if (ar.peek() == '\n')
					ar.get();
				return true;
			case '\n':
				if (!bEat)
					ar.unget('\n');
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

template <bool bEat>
inline Rule<CFindContinuableEol<bEat> > TPL_CALL c_find_continuable_eol() {
	return Rule<CFindContinuableEol<bEat> >();
}

inline Rule<CFindContinuableEol<false> > TPL_CALL c_find_continuable_eol() {
	return Rule<CFindContinuableEol<false> >();
}

// -------------------------------------------------------------------------
// function cpp_single_line_comment<bEatEol>()

typedef Ch<'/'> ChDiv_;

typedef UAnd<ChDiv_, CFindContinuableEol<false> > CppSingleLineCommentEnd_;
typedef UAnd<ChDiv_, CFindContinuableEol<true> > CppSingleLineCommentEnd_EatEol_;

typedef UAnd<ChDiv_, CppSingleLineCommentEnd_> CppSingleLineComment;
typedef UAnd<ChDiv_, CppSingleLineCommentEnd_EatEol_> CppSingleLineCommentEatEol;

TPL_REGEX_GUARD(CppSingleLineComment, CppSingleLineCommentG, TagAssigNone);
TPL_REGEX_GUARD(CppSingleLineCommentEatEol, CppSingleLineCommentEatEolG, TagAssigNone);

inline Rule<CppSingleLineCommentG> TPL_CALL cpp_single_line_comment() {
	return Rule<CppSingleLineCommentG>();
}

template <bool bEatEol = false>
struct CppSingleLineCommentTraits {
	typedef CppSingleLineCommentG rule_type;
};

template <>
struct CppSingleLineCommentTraits<true> {
	typedef CppSingleLineCommentEatEolG rule_type;
};

template <bool bEatEol>
inline Rule<typename CppSingleLineCommentTraits<bEatEol>::rule_type> TPL_CALL cpp_single_line_comment() {
	return Rule<typename CppSingleLineCommentTraits<bEatEol>::rule_type>();
}

// -------------------------------------------------------------------------
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

// -------------------------------------------------------------------------
// function cpp_comment<bEatEol>()

typedef Or<CppSingleLineCommentEnd_, CCommentEnd_> CppCommentEnd_;
typedef Or<CppSingleLineCommentEnd_EatEol_, CCommentEnd_> CppCommentEnd_EatEol_;

typedef UAnd<ChDiv_, CppCommentEnd_> CppComment;
typedef UAnd<ChDiv_, CppCommentEnd_EatEol_> CppCommentEatEol;

TPL_REGEX_GUARD(CppComment, CppCommentG, TagAssigNone);
TPL_REGEX_GUARD(CppCommentEatEol, CppCommentEatEolG, TagAssigNone);

inline Rule<CppCommentG> TPL_CALL cpp_comment() {
	return Rule<CppCommentG>();
}

template <bool bEatEol = false>
struct CppCommentTraits {
	typedef CppCommentG rule_type;
};

template <>
struct CppCommentTraits<true> {
	typedef CppCommentEatEolG rule_type;
};

template <bool bEatEol>
inline Rule<typename CppCommentTraits<bEatEol>::rule_type> TPL_CALL cpp_comment() {
	return Rule<typename CppCommentTraits<bEatEol>::rule_type>();
}

// =========================================================================
// function c_skip(), cpp_skip()

typedef Lst<SkipWhiteSpaces, CCommentG> CSkipG; // for C
typedef Lst<SkipNonEolSpaces, CCommentG> CSkipNonEolG; // for C Preprocessor
typedef Lst<SkipWhiteSpaces, CppCommentEatEolG> CppSkipG; // for C++

inline Rule<CSkipG> TPL_CALL c_skip() {
	return Rule<CSkipG>();
}

inline Rule<CSkipNonEolG> TPL_CALL c_skip_non_eol() {
	return Rule<CSkipNonEolG>();
}

inline Rule<CppSkipG> TPL_CALL cpp_skip() {
	return Rule<CppSkipG>();
}

TPL_CONST(Rule<CSkipG>, c_skip_);
TPL_CONST(Rule<CSkipNonEolG>, c_skip_non_eol_);
TPL_CONST(Rule<CppSkipG>, cpp_skip_);

// =========================================================================
// function c_string(), c_char()

struct TagAssigCString {};
struct TagAssigCChar {};

template <int delim = '\"'>
class CStringTraits
{
private:
	typedef Ch<delim> Delim_;
	
	typedef Ch<'\\'> Esc_;
	typedef UAnd<Esc_, ChAny> EscChar_;
	
	typedef NotCh<delim, '\r', '\n'> NotStopChar_;
	typedef Or<EscChar_, NotStopChar_> Char_;
	typedef Repeat0<Char_> Chars_;
	
public:
	typedef UAnd<Delim_, Chars_, Delim_> rule_type;
};

TPL_REGEX_GUARD(CStringTraits<'\"'>::rule_type, CStringG, TagAssigCString);
TPL_REGEX_GUARD(CStringTraits<'\''>::rule_type, CCharG, TagAssigCChar);

typedef Or<CStringG, CCharG> CStringOrCharG;

inline Rule<CStringG> TPL_CALL c_string() {
	return Rule<CStringG>();
}

inline Rule<CCharG> TPL_CALL c_char() {
	return Rule<CCharG>();
}

inline Rule<CStringOrCharG> TPL_CALL c_string_or_char() {
	return Rule<CStringOrCharG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_C_LEX_H */

