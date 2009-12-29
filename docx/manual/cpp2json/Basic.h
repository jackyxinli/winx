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
// Module: cpp2json/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Basic.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef CPP2JSON_BASIC_H
#define CPP2JSON_BASIC_H

#ifndef TPL_C_LEX_H
#include <tpl/c/Lex.h>
#endif

#ifndef TPL_REGEXP_H
#include <tpl/RegExp.h>
#endif

#ifndef TR
#define TR	TPL_INFO("TRACE")
#endif

typedef NS_STDEXT::String String;

// -------------------------------------------------------------------------

#define eq2(s1, s2)				(eq(s1) || eq(s2))
#define eq3(s1, s2, s3)			(eq(s1) || eq(s2) || eq(s3))
#define eq4(s1, s2, s3, s4)		(eq(s1) || eq(s2) || eq(s3) || eq(s4))

// -------------------------------------------------------------------------

using namespace tpl;

typedef impl::Result dom;

extern dom::Document doc;
extern dom::NodeMark tagSentences;
	extern dom::NodeMark tagTemplate;
		extern dom::Mark tagHeader;
	extern dom::NodeMark tagClass;
		extern dom::Mark tagClassKeyword;
		extern dom::Mark tagName;
			extern dom::NodeMark tagBaseClasses;
				extern dom::Mark tagAccess;
				extern dom::Mark tagName;
		extern dom::NodeMark tagSentences;
			extern dom::NodeMark tagConstructor;
			extern dom::NodeMark tagDestructor;
			extern dom::NodeMark tagMember; // member function or variable 
			extern dom::NodeMark tagTypeCast;
			extern dom::NodeMark tagTypedef;
			extern dom::NodeMark tagEnum;
	extern dom::NodeMark tagGlobal; // global function or variable
		extern dom::Mark tagType;
		extern dom::Mark tagCallType;
		extern dom::NodeMark tagArgs;
			extern dom::Mark tagType;
			extern dom::Mark tagName;
			extern dom::Mark tagDefVal;
		extern dom::Mark tagAttr2; // to distinguish function from variable
	extern dom::NodeMark tagMacro;
		extern dom::Mark tagName;
		extern dom::NodeMark tagMacroArgList;
			extern dom::Mark tagMacroArgs;
		extern dom::Mark tagMacroBody;

// -------------------------------------------------------------------------

extern impl::Allocator alloc;
extern impl::MarkedGrammar rCppSymbol;
extern impl::MarkedGrammar rType;
extern impl::MarkedGrammar rClass;
extern impl::MarkedRule rComment;

extern String className;
extern String encoding;

// -------------------------------------------------------------------------

#endif /* CPP2JSON_BASIC_H */
