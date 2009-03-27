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
// $Id: Lex.h 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_C_LEX_H
#define TPL_C_LEX_H

#ifndef TPL_REGEX_MATCH_H
#include "../regex/Match.h"
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

typedef UAnd<ChDiv_, CppSingleLineCommentEnd_> CppSingleLineCommentU;
typedef UAnd<ChDiv_, CppSingleLineCommentEnd_EatEol_> CppSingleLineCommentEatEolU;

TPL_REGEX_GUARD(CppSingleLineCommentU, CppSingleLineCommentG, TagAssigNone);
TPL_REGEX_GUARD(CppSingleLineCommentEatEolU, CppSingleLineCommentEatEolG, TagAssigNone);

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
typedef UAnd<ChDiv_, CCommentEnd_> CCommentU;

TPL_REGEX_GUARD(CCommentU, CCommentG, TagAssigNone);

inline Rule<CCommentG> TPL_CALL c_comment() {
	return Rule<CCommentG>();
}

// -------------------------------------------------------------------------
// function cpp_comment<bEatEol>()

typedef Or<CppSingleLineCommentEnd_, CCommentEnd_> CppCommentEnd_;
typedef Or<CppSingleLineCommentEnd_EatEol_, CCommentEnd_> CppCommentEnd_EatEol_;

typedef UAnd<ChDiv_, CppCommentEnd_> CppCommentU;
typedef UAnd<ChDiv_, CppCommentEnd_EatEol_> CppCommentEatEolU;

TPL_REGEX_GUARD(CppCommentU, CppCommentG, TagAssigNone);
TPL_REGEX_GUARD(CppCommentEatEolU, CppCommentEatEolG, TagAssigNone);

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
typedef Lst<SkipWhiteSpaces, CppCommentEatEolG> CppSkipG; // for C++

inline Rule<CSkipG> TPL_CALL c_skip() {
	return Rule<CSkipG>();
}

inline Rule<CppSkipG> TPL_CALL cpp_skip() {
	return Rule<CppSkipG>();
}

TPL_CONST(Rule<CSkipG>, c_skip_);
TPL_CONST(Rule<CppSkipG>, cpp_skip_);

// =========================================================================
// function c_skip_non_eol() - deprecated

typedef Lst<SkipNonEolSpaces, CCommentG> CSkipNonEolG; // for C Preprocessor, deprecated

__deprecated("Deprecated, please use function 'tpl::c_pp_skip()'.")
inline Rule<CSkipNonEolG> TPL_CALL c_skip_non_eol() {
	return Rule<CSkipNonEolG>();
}

TPL_CONST(Rule<CSkipNonEolG>, c_skip_non_eol_);

// =========================================================================
// function c_pp_skip()

typedef And<ChDiv_, StrictEolG> CEscapeEol_;
typedef Or<NonEolSpace, CEscapeEol_> CSpacePpG;
typedef Repeat0<CSpacePpG> SkipSpacePpG;

typedef Lst<SkipSpacePpG, CCommentG> CPpSkipG; // for C Preprocessor
typedef Lst<SkipSpacePpG, CppCommentG> CPpSkipPlusG;

inline Rule<CPpSkipG> TPL_CALL c_pp_skip() {
	return Rule<CPpSkipG>();
}

TPL_CONST(Rule<CPpSkipG>, c_pp_skip_);
TPL_CONST(Rule<CPpSkipPlusG>, c_pp_skip_plus_);

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
	
	typedef ChNot<delim, '\r', '\n'> NotStopChar_;
	typedef Or<EscChar_, NotStopChar_> Char_;
	typedef Repeat0<Char_> Chars_;
	
public:
	typedef UAnd<Delim_, Chars_, Delim_> rule_type;
};

typedef CStringTraits<'\"'>::rule_type CStringU;
typedef CStringTraits<'\''>::rule_type CCharU;

TPL_REGEX_GUARD(CStringU, CStringG, TagAssigCString);
TPL_REGEX_GUARD(CCharU, CCharG, TagAssigCChar);

typedef Or<CStringU, CCharU> CStringOrCharU;
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
// function c_hex_integer(), c_oct_integer(), c_integer()

struct TagAssigCHexInteger {};
struct TagAssigCOctInteger {};
struct TagAssigCUInteger {};
struct TagAssigCInteger {};

typedef Ch<'0'> CChar0_;
typedef Ch<'x', 'X'> CCharX_;

typedef UAnd<CChar0_, CCharX_, HexIntegerU> CHexIntegerU;
typedef UAnd<CChar0_, OctIntegerU> COctIntegerU;

typedef UAnd<CChar0_, Or<UAnd<CCharX_, HexIntegerU>, OctIntegerU> > CHexOrOctIntegerU;

typedef Or<CHexOrOctIntegerU, UIntegerU> CUIntegerU;
typedef Or<CHexOrOctIntegerU, IntegerU> CIntegerU;

TPL_REGEX_GUARD(CHexIntegerU, CHexIntegerG, TagAssigCHexInteger);
TPL_REGEX_GUARD(COctIntegerU, COctIntegerG, TagAssigCOctInteger);
TPL_REGEX_GUARD(CUIntegerU, CUIntegerG, TagAssigCUInteger);
TPL_REGEX_GUARD(CIntegerU, CIntegerG, TagAssigCInteger);

inline Rule<CHexIntegerG> TPL_CALL c_hex_integer() {
	return Rule<CHexIntegerG>();
}

inline Rule<COctIntegerG> TPL_CALL c_oct_integer() {
	return Rule<COctIntegerG>();
}

inline Rule<CUIntegerG> TPL_CALL c_u_integer() {
	return Rule<CUIntegerG>();
}

inline Rule<CIntegerG> TPL_CALL c_integer() {
	return Rule<CIntegerG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_C_LEX_H */

