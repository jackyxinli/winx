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
// Module: tpl/html/Lex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Lex.h 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_HTML_LEX_H
#define TPL_HTML_LEX_H

#ifndef TPL_REGEX_MATCH_H
#include "../regex/Match.h"
#endif

#ifndef TPL_C_LEX_H
#include "../c/Lex.h"
#endif

#if !defined(_GLIBCXX_MAP) && !defined(_MAP_) && !defined(_MAP)
#include <map>
#endif

NS_TPL_BEGIN

// =========================================================================

typedef Ch<'>'> HtmlChGt_;
typedef Ch<'/'> HtmlChDiv_;
typedef Ch<'='> HtmlChEq_;
typedef Ch<'<'> HtmlChLt_;
typedef Ch<'!'> HtmlChExcl_; // exclaim
typedef Ch<'-'> HtmlChMus_;

// =========================================================================
// struct AssigHtmlUInt

struct TagAssigHtmlUInt {};
struct AssigHtmlUInt
{
	template <class UIntT, class Iterator>
	static UIntT TPL_CALL get(Iterator pos, Iterator pos2, const void* = NULL)
	{
		TPL_ASSERT(*pos == '#');
		++pos;
		if (isdigit(*pos))
			return AssigUInt::get<UIntT>(pos, pos2);
		else {
			TPL_ASSERT(*pos == 'x' || *pos == 'X');
			return std::CastToUInt<UIntT>::get(++pos, pos2, 16);
		}
	}
};

TPL_ASSIG_(TagAssigHtmlUInt, AssigHtmlUInt)

// =========================================================================
// class HtmlEntityTable

struct HtmlEntityPair
{
	const char* key;
	const wchar_t val;
};

struct LessOfString
{
	bool TPL_CALL operator()(const char* a, const char* b) const {
		return strcmp(a, b) < 0;
	}
	
	bool TPL_CALL operator()(const wchar_t* a, const wchar_t* b) const {
		return wcscmp(a, b) < 0;
	}
};

template <class MapT = std::map<const char*, wchar_t, LessOfString> >
class HtmlEntityTableImp : public MapT
{
public:
	HtmlEntityTableImp() {
		const HtmlEntityPair e[] = {
			#include "entity/Latin-1.h"
			#include "entity/Symbol.h"
		};
		for (size_t i = 0; i < countof(e); ++i) {
			MapT::insert(typename MapT::value_type(e[i].key, e[i].val));
		}
	}
};

typedef HtmlEntityTableImp<> HtmlEntityTable;

// =========================================================================
// function get_html_entity

#ifndef TPL_HTML_ENTITY_LEN_MAX
#define TPL_HTML_ENTITY_LEN_MAX			16
#endif

#ifndef TPL_HTML_ENTITY_DEFAULT_CHAR
#define TPL_HTML_ENTITY_DEFAULT_CHAR	' '
#endif

template <class TableT, class Iterator>
inline wchar_t TPL_CALL get_html_entity(Iterator pos, Iterator pos2)
{
	if (*pos == '#')
		return (wchar_t)AssigHtmlUInt::get<unsigned>(pos, pos2);

	static const TableT entities = TableT();
	
	const size_t len = std::distance(pos, pos2);
	TPL_ASSERT(len < TPL_HTML_ENTITY_LEN_MAX);
	if (len >= TPL_HTML_ENTITY_LEN_MAX)
		return TPL_HTML_ENTITY_DEFAULT_CHAR;
	
	char key[TPL_HTML_ENTITY_LEN_MAX];
	*std::copy(pos, pos2, key) = '\0';
	
	const typename TableT::const_iterator it = entities.find(key);
	TPL_ASSERT(it != entities.end());
	return it != entities.end() ? (*it).second : TPL_HTML_ENTITY_DEFAULT_CHAR;
}

struct TagAssigHtmlEntity {};
struct AssigHtmlEntity {
	template <class TextT, class Iterator>
	static TextT TPL_CALL get(Iterator pos, Iterator pos2, const void* = NULL) {
		++pos; --pos2;
		return get_html_entity<HtmlEntityTable>(pos, pos2);
	}
};

TPL_ASSIG_(TagAssigHtmlEntity, AssigHtmlEntity)

// =========================================================================
// function html_space/html_skipws/html_ws

typedef Space HtmlSpace;

typedef SkipWhiteSpaces HtmlSkipWs;
typedef WhiteSpaces HtmlWs;

inline Rule<HtmlSpace> TPL_CALL html_space() {
	return Rule<HtmlSpace>();
}

inline Rule<HtmlSkipWs> TPL_CALL html_skipws() {
	return Rule<HtmlSkipWs>();
}

inline Rule<HtmlWs> TPL_CALL html_ws() {
	return Rule<HtmlWs>();
}

// =========================================================================
// function html_symbol

#define TPL_HTMLSYMBOL_FIRST_CHAR	STD_XMLSYMBOL_FIRST_CHAR

typedef XmlSymbolFirstChar HtmlSymbolFirstChar;
typedef XmlSymbolNextChar HtmlSymbolNextChar;

inline Rule<HtmlSymbolFirstChar> TPL_CALL html_symbol_first_char() {
	return Rule<HtmlSymbolFirstChar>();
}

inline Rule<HtmlSymbolNextChar> TPL_CALL html_symbol_next_char() {
	return Rule<HtmlSymbolNextChar>();
}

typedef XmlSymbolU HtmlSymbolU;
typedef XmlSymbolG HtmlSymbolG;

inline Rule<HtmlSymbolG> TPL_CALL html_symbol() {
	return Rule<HtmlSymbolG>();
}

// =========================================================================
// function html_value

struct TagAssigHtmlValue {};
struct TagAssigHtmlStrictValue {};
struct TagAssigHtmlSmartValue {};

template <int delim = '\"'>
class HtmlValueTraits
{
private:
	typedef Ch<delim> Delim_;
	typedef FindChSet<true, delim> End_;
	
public:
	typedef UAnd<Delim_, End_> rule_type;
};

//
// ch(delim) + find<true>(delim)
//
typedef HtmlValueTraits<'\"'>::rule_type HtmlDoubleQuoteValueU;
typedef HtmlValueTraits<'\''>::rule_type HtmlSingleQuoteValueU;
typedef Or<HtmlDoubleQuoteValueU, HtmlSingleQuoteValueU> HtmlStrictValueU;

TPL_REGEX_GUARD(HtmlStrictValueU, HtmlStrictValueG, TagAssigHtmlStrictValue);

inline Rule<HtmlStrictValueG> TPL_CALL html_strict_value() {
	return Rule<HtmlStrictValueG>();
}

class HtmlSmartValueG
{
public:
	enum { character = 0 };

	typedef SelfConvertible convertible_type;
	typedef TagAssigHtmlSmartValue assig_tag;

public:
	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typedef ChMask<STD_CTYPE_SPACE|STD_CTYPE_GT|STD_CTYPE_DIV> HtmlSmartValueDelim;
		typedef UAnd<FindIf<HtmlSmartValueDelim>, HtmlSkipWs> HtmlSmartValue1_; // find(space|'/'|'>') + skipws()

		typedef ChMask<TPL_HTMLSYMBOL_FIRST_CHAR|STD_CTYPE_GT|STD_CTYPE_DIV> HtmlSmartValueNext;
		typedef UAnd<HtmlSymbolU, UAnd<HtmlSkipWs, HtmlChEq_> > HtmlSmartValueNextPropU;
		
		const HtmlSmartValue1_ sval = HtmlSmartValue1_();
		const HtmlSmartValueNext next = HtmlSmartValueNext();
		const HtmlSmartValueNextPropU nextProp = HtmlSmartValueNextPropU();
		
Retry:	sval.match(ar, context);

		typename SourceT::int_type c = ar.peek();
		if (next(c)) {
			if (c == '/') {
				ar.get();
				if (ar.peek() != '>')
					goto Retry;
				ar.unget('/');
			}
			else if (c != '>') {
				typename SourceT::iterator pos = ar.position();
				if (!nextProp.match(ar, context))
					goto Retry;
				ar.seek(pos);
			}
			return true;
		}
		ar.get();
		goto Retry;
	}

	TPL_SIMPLEST_GRAMMAR_();
};

typedef HtmlSmartValueG HtmlSmartValueG_;

inline Rule<HtmlSmartValueG> TPL_CALL html_smart_value() {
	return Rule<HtmlSmartValueG>();
}

//
// html_strict_value() | html_smart_value()
//
typedef Or<HtmlStrictValueU, HtmlSmartValueG_> HtmlValueU;
typedef Or<HtmlStrictValueG, HtmlSmartValueG> HtmlValueG_;

TPL_REGEX_GUARD0(HtmlValueG_, HtmlValueG, TagAssigHtmlValue);

inline Rule<HtmlValueG> TPL_CALL html_value() {
	return Rule<HtmlValueG>();
}

// =========================================================================
// function html_property

//
// html_symbol() + html_skipws() + !('=' + html_skipws() + html_value())
//
typedef And<HtmlChEq_, And<HtmlSkipWs, HtmlValueG_> > HtmlPropValG_;
typedef UAnd<HtmlSymbolU, HtmlSkipWs, Repeat01<HtmlPropValG_> > HtmlPropertyG_;

TPL_REGEX_GUARD0(HtmlPropertyG_, HtmlPropertyG, TagAssigNone);

inline Rule<HtmlPropertyG> TPL_CALL html_property() {
	return Rule<HtmlPropertyG>();
}

// =========================================================================
// function html_u_integer

// Usage: html_u_integer()/assign(a_uint_var)

typedef Ch<'#'> HtmlUIPrefix_;
typedef Ch<'x', 'X'> HtmlHexPrefix_;
typedef UAnd<HtmlHexPrefix_, HexIntegerU> HtmlHexIntegerU_;

typedef UAnd<HtmlUIPrefix_, HtmlHexIntegerU_> HtmlHexIntegerU;
typedef UAnd<HtmlUIPrefix_, Or<UIntegerU, HtmlHexIntegerU_> > HtmlUIntegerU;

TPL_REGEX_GUARD(HtmlUIntegerU, HtmlUIntegerG, TagAssigHtmlUInt)

inline Rule<HtmlUIntegerG> TPL_CALL html_u_integer() {
	return Rule<HtmlUIntegerG>();
}

// =========================================================================
// function html_entity

// Usage: html_entity()/assign(a_wchar_var)

typedef Ch<'&'> HtmlEntityStart_;
typedef Repeat01<Ch<';'> > HtmlEntityEnd_;
typedef Or<HtmlSymbolU, HtmlUIntegerU> HtmlEntityVal_;

typedef UAnd<HtmlEntityStart_, HtmlEntityVal_, HtmlEntityEnd_> HtmlEntityU;

TPL_REGEX_GUARD(HtmlEntityU, HtmlEntityG, TagAssigHtmlEntity)

inline Rule<HtmlEntityG> TPL_CALL html_entity() {
	return Rule<HtmlEntityG>();
}

// =========================================================================
// function html_comment

typedef UAnd<HtmlChLt_, HtmlChExcl_, HtmlChMus_, HtmlChMus_> HtmlCommentStartU;
typedef UAnd<HtmlChMus_, HtmlChMus_, HtmlChGt_> HtmlCommentEndU;

struct FindHtmlCommentEnd_ : UFindStr<char> {
	FindHtmlCommentEnd_() : UFindStr<char>("-->") {}
};

typedef UAnd<HtmlCommentStartU, FindHtmlCommentEnd_> HtmlCommentU;

TPL_REGEX_GUARD(HtmlCommentStartU, HtmlCommentStartG, TagAssigNone)
TPL_REGEX_GUARD(HtmlCommentEndU, HtmlCommentEndG, TagAssigNone)
TPL_REGEX_GUARD(HtmlCommentU, HtmlCommentG, TagAssigNone)

inline Rule<HtmlCommentG> TPL_CALL html_comment() {
	return Rule<HtmlCommentG>();
}
 
// =========================================================================
// function html_uncommented_code/html_commented_script_code

/* example:
 *
	<script>
	
	// html_uncommented_script_code:
	
		var hello="Hello, <script>!";
		alert(hello);
		
	<!--
		// html_commented_script_code:
	
			var hello='Hello, </script>-->!';
			alert(hello);
	-->
	</script>
*/

template <int endc1 = '<', class Pred2 = Ch<'/', '!'>, class Pred3 = Ch<'s', 'S', '-'> >
class HtmlScriptCode
{
public:
	enum { character = 0 };

	typedef SelfConvertible convertible_type;
	typedef TagAssigNone assig_tag;

public:
	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typedef FindChSet<false, endc1, '\"', '\''> FindSpec;
		
		const FindSpec find1 = FindSpec();
		const Pred2 pred2 = Pred2();
		const Pred3 pred3 = Pred3();
		const CStringOrCharU stringOrChar = CStringOrCharU();
		
		for (;;)
		{
			if (!find1.match(ar, context))
				return false;
			
			if (ar.peek() == endc1)
			{
				ar.get();
				if ( pred2(ar.peek()) )
				{
					typename SourceT::int_type c2 = ar.get();
					if ( pred3(ar.peek()) ) {
						ar.unget(c2);
						ar.unget(endc1);
						return true;
					}
				}
			}
			else
			{
				TPL_ASSERT(ar.peek() == '\'' || ar.peek() == '\"');
				stringOrChar.match(ar, context);
			}
		}
	}

	TPL_SIMPLEST_GRAMMAR_();
};

typedef UAnd<Not<HtmlChLt_>, HtmlScriptCode<> > HtmlUncommentedScriptCodeU;
typedef HtmlScriptCode<'-', Ch<'-'>, Ch<'>'> > HtmlCommentedScriptCodeU;

TPL_REGEX_GUARD(HtmlUncommentedScriptCodeU, HtmlUncommentedScriptCodeG, TagAssigNone)
TPL_REGEX_GUARD(HtmlCommentedScriptCodeU, HtmlCommentedScriptCodeG, TagAssigNone)

inline Rule<HtmlUncommentedScriptCodeG> TPL_CALL html_uncommented_script_code() {
	return Rule<HtmlUncommentedScriptCodeG>();
}

inline Rule<HtmlCommentedScriptCodeG> TPL_CALL html_commented_script_code() {
	return Rule<HtmlCommentedScriptCodeG>();
}

// =========================================================================
// function html_script_codes

typedef UAnd<HtmlCommentStartU, HtmlCommentedScriptCodeU, HtmlCommentEndU> HtmlCommentedScriptCodeU2_;

TPL_REGEX_GUARD(HtmlCommentedScriptCodeU2_, HtmlCommentedScriptCodeG2_, TagAssigNone)

typedef Or<HtmlUncommentedScriptCodeG, HtmlCommentedScriptCodeG2_> HtmlScriptCodeG;
typedef Repeat0<HtmlScriptCodeG> HtmlScriptCodesG;

inline Rule<HtmlScriptCodeG> TPL_CALL html_script_code() {
	return Rule<HtmlScriptCodeG>();
}

inline Rule<HtmlScriptCodesG> TPL_CALL html_script_codes() {
	return Rule<HtmlScriptCodesG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_HTML_LEX_H */

