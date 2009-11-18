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
// Module: cpp2json/Lex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Lex.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef CPP2JSON_LEX_H
#define CPP2JSON_LEX_H

#ifndef CPP2JSON_COMMENT_H
#include "Comment.h"
#endif

// -------------------------------------------------------------------------
// common

class IsCallType
{
public:
	template <class ValueT>
	bool TPL_CALL operator()(const ValueT& v) const
	{
		const String val(v.begin(), v.end());
		if (val.endWithI("callback"))
			return true;
		const String last4 = val.substr(val.size() - 4);
		if (last4.icompare("call") == 0 || last4 == "_msg" || last4 == "cdel")
			return true;
		return val == "WINAPI";
	}
};

#define symbol					(c_symbol()/ne("const"))
#define keyword(key)			gr(c_symbol()/eq(key))
#define zero_keyword			gr(u_integer()/eq(0))
#define typename_keyword		gr(c_symbol()/eq2("class", "typename"))
#define class_keyword			gr(c_symbol()/eq3("class", "interface", "struct")/tagClassKeyword)
#define access					gr(c_symbol()/eq3("private", "public", "protected")/tagAccess)
#define signed_keyword			gr(c_symbol()/eq2("unsigned", "signed"))
#define int_basetypes			gr(c_symbol()/eq4("int", "long", "short", "char"))
#define call_type				gr(c_symbol()/meet(IsCallType())/tagCallType)

// -------------------------------------------------------------------------
// cppsymbol

#define cppsymbol2_templ		( "<" + ref(rType) % ',' + ">" )
#define cppsymbol2				( symbol + !cppsymbol2_templ )

#define operator_sym			( gr("++") | "+=" | "+" | "--" | "-=" | "-" | \
								  "*=" | "*" | "/=" | "/" | "&=" | "&&" | "&" | \
								  "|=" | "||" | "|" | "<=" | "<<" | "<" | ">=" | \
								  ">>" | ">" | "==" | "=" | "!=" | "!" | "%=" | \
								  "%" | "^=" | "^" | "[]" | "()" | "~" )

#define operatorfn				( keyword("operator") + operator_sym )

#define cppsymbol				( operatorfn | cppsymbol2 + !("::" + ref(rCppSymbol)) )

// -------------------------------------------------------------------------
// type

#define int_types				( signed_keyword + int_basetypes )
#define skipconst_				( skipws_[keyword("const")] )
#define basetype				gr( skipws_[int_types | !class_keyword + cppsymbol] )
#define type					gr( skipconst_[basetype + *gr('*') + !gr('&') ]/tagType )

// -------------------------------------------------------------------------
// function

#define pure					( keyword("PURE") | gr('=') + zero_keyword )
#define function_attr2			gr( skipws_[ !keyword("const") + !pure ]/tagAttr2 )

#define defval_expr				( find_set<',', ')'>()/tagDefVal )
#define defval					( '=' + skipws() + defval_expr )
#define function_arg			( type + !gr(symbol/tagName) + !gr(defval) )
#define function_args			( '(' + !(function_arg/tagArgs % ',') + ')' )

#define func_tail				( function_args + function_attr2 )
#define func_or_var				( type + !call_type + symbol/tagName + !func_tail + ';' )

// -------------------------------------------------------------------------
// class body

#define type_cast				( keyword("operator") + type + '(' + ')' + function_attr2 + ';' )
#define enumdef					( keyword("enum") + symbol/tagName + ';' )
#define typedefine				( keyword("typedef") + (symbol/tagName + gr(';') | func_or_var) )

#define constructor				( gr(c_symbol()/eq(className)) + function_args + ';' )

#define class_sentence			( constructor/tagConstructor | type_cast/tagTypeCast | \
								  enumdef/tagEnum | typedefine/tagTypedef | func_or_var/tagMember )

#define class_body				( '{' + *(class_sentence/tagSentences) + '}' )

// -------------------------------------------------------------------------
// class

#define class_header			( class_keyword + symbol/tagName/assign(className) )

#define baseclass				( !access + symbol/tagName )
#define baseclasses				( ':' + baseclass/tagBaseClasses % ',' )

#define classdef				( class_header + !baseclasses + class_body )

// -------------------------------------------------------------------------
// template

#define tempate_arg_type_imp	gr( skipws_[typename_keyword | int_types | symbol]/tagType )
#define template_arg_val		( '=' + gr(skipws_[cppsymbol | c_integer()]/tagDefVal) )
#define template_arg			( tempate_arg_type_imp + symbol/tagName + !template_arg_val )
#define template_header			( keyword("template") + '<' + !(template_arg/tagArgs % ',') + '>' )

#define templatedef				( gr(skipws_[template_header]/tagHeader) + classdef/tagClass )

// -------------------------------------------------------------------------
// global sentences

#define global					( templatedef/tagTemplate | classdef/tagClass )

// -------------------------------------------------------------------------
// document

#define document				( comment[*(global/tagSentences | ';')]/doc )

// -------------------------------------------------------------------------

#endif /* CPP2JSON_LEX_H */
