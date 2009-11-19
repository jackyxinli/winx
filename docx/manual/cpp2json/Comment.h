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

extern dom::NodeMark tagCommentDoc;
	extern dom::Mark tagCategory;
	extern dom::Mark tagText;
	extern dom::NodeMark tagTopic;
		extern dom::Mark tagTopicType;
		extern dom::Mark tagPath;
		extern dom::Mark tagTopicArgs;
		extern dom::Mark tagTopicBrief;
	extern dom::NodeMark tagBrief; // extend text
		extern dom::NodeMark tagTable;
			extern dom::NodeMark tagVals;
				extern dom::Mark tagName;
				extern dom::Mark tagText;
		extern dom::Mark tagText;
	extern dom::NodeMark tagDescs;
		extern dom::Mark tagCaption;
		extern dom::NodeMark tagBody; // extend text
	extern dom::NodeMark tagArgs;
		extern dom::Mark tagAttr;
		extern dom::Mark tagName;
		extern dom::NodeMark tagBody; // extend text
	extern dom::NodeMark tagReturn; // extend text
	extern dom::NodeMark tagRemark; // extend text
	extern dom::NodeMark tagSees;
		extern dom::Mark tagSeeTopics;

class TUnused;

// -------------------------------------------------------------------------

#define attribute(tag)	!( '[' + find(']')/tag + ']' )
#define symbol_arg		!( '(' + find<true>(')') )
#define topicsymbol		( c_symbol() + symbol_arg )
#define comma			( skipws() + ',' + skipws() )

#define text			( token<'@'>() )

#define val_enum		( "@val" + ws() + find_eol()/tagName + skipws() + text/tagText )
#define table			( +(val_enum/tagVals) + !(skipws() + "@end") )

#define extend_text		+( (table/tagTable | text/tagText) )

#define topic_args		( topicsymbol/tagTopicArgs % comma )
#define topic_gen		( ws() + attribute(tagPath) + skipws() + topic_args + \
						  skip_non_eol_ws() + !(paragraph()/tagTopicBrief) )

#define ctordoc			( '@' + "ctor"/tagTopicType + symbol_arg + \
						  skip_non_eol_ws() + !(paragraph()/tagTopicBrief) )

#define topicdoc1		( '@' + (str("fn") | "class" | "macro")/tagTopicType + topic_gen )
#define topicdoc		( (topicdoc1 | ctordoc)/tagTopic )

// -------------------------------------------------------------------------

#define categorydoc		( "@category" + ws() + find_eol()/tagCategory )

#define retdoc			( "@return" + ws() + extend_text/tagReturn )

#define arg_name_list	( c_symbol() % comma | "..." )
#define argdoc_one		( "@arg" + ws() + attribute(tagAttr) + skipws() + arg_name_list/tagName + skipws() + extend_text/tagBody )
#define argdoc			( +(skipws() + argdoc_one/tagArgs) )

#define descdoc_one		( "@desc" + ws() + find_eol()/tagCaption + skipws() + extend_text/tagBody )
#define descdoc			( +(skipws() + descdoc_one/tagDescs) )

#define seedoc_one		( "@see" + ws() + topicsymbol/tagSeeTopics % comma )
#define seedoc			( +(skipws() + seedoc_one/tagSees) )

#define briefdoc		( "@brief" + ws() + extend_text/tagBrief )
#define remarkdoc		( "@remark" + ws() + extend_text/tagRemark )
#define eofdoc			( '@' + eos() )

// -------------------------------------------------------------------------

#define comment_doc_one	( argdoc | seedoc | topicdoc | retdoc | remarkdoc | briefdoc | \
						  descdoc | eofdoc | categorydoc | done()/tagText )
#define comment_doc		( *(not_eos() + skipws() + comment_doc_one) )
#define comment			( cpp_comment_content(alloc, comment_doc/tagCommentDoc) )

// -------------------------------------------------------------------------

#endif /* CPP2JSON_COMMENT_H */
