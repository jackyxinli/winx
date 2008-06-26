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

#ifndef TPL_REGEXP_H
#include "../RegExp.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function get_html_entity

template <class TextT, class Iterator>
inline TextT TPL_CALL get_html_entity(Iterator pos, Iterator pos2)
{
	return TextT();
}

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

typedef XmlSymbolFirstChar HtmlSymbolFirstChar;
typedef XmlSymbolNextChar HtmlSymbolNextChar;

inline Rule<HtmlSymbolFirstChar> TPL_CALL html_symbol_first_char() {
	return Rule<HtmlSymbolFirstChar>();
}

inline Rule<HtmlSymbolNextChar> TPL_CALL html_symbol_next_char() {
	return Rule<HtmlSymbolNextChar>();
}

typedef XmlSymbol HtmlSymbol;
typedef XmlSymbolG HtmlSymbolG;

inline Rule<HtmlSymbolG> TPL_CALL html_symbol() {
	return Rule<HtmlSymbolG>();
}

// =========================================================================
// function html_entity

struct TagAssigHtmlEntity {};
struct AssigHtmlEntity {
	template <class TextT, class Iterator>
	static TextT TPL_CALL get(Iterator pos, Iterator pos2, const void*) {
		++pos; --pos2;
		return get_html_entity<TextT>(pos, pos2);
	}
};

TPL_TEXT_ASSIG_(TagAssigHtmlEntity, AssigHtmlEntity)

// Usage: html_entity()/assign(a_string_var)

typedef Ch<'&'> HtmlEntityStart_;
typedef Ch<';'> HtmlEntityEnd_;

typedef UAnd<HtmlEntityStart_, HtmlSymbol, HtmlEntityEnd_> HtmlEntity;

TPL_REGEX_GUARD(HtmlEntity, HtmlEntityG, TagAssigHtmlEntity)

inline Rule<HtmlEntityG> TPL_CALL html_entity() {
	return Rule<HtmlEntityG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_HTML_LEX_H */

