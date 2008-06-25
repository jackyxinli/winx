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
// Module: tpl/RegExp.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEXP_H
#define TPL_REGEXP_H

#ifndef TPL_REGEX_BASIC_H
#include "regex/Basic.h"
#endif

#ifndef TPL_REGEX_MARK_H
#include "regex/Mark.h"
#endif

#ifndef TPL_REGEX_MATCHRESULT_H
#include "regex/MatchResult.h"
#endif

#ifndef TPL_REGEX_CONTEXT_H
#include "regex/Context.h"
#endif

#ifndef TPL_REGEX_CONCRETION_H
#include "regex/Concretion.h"
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "regex/Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "regex/Composition.h"
#endif

#ifndef TPL_REGEX_UCOMPOSITION_H
#include "regex/UComposition.h"
#endif

#ifndef TPL_REGEX_OPERATOR_H
#include "regex/Operator.h"
#endif

#ifndef TPL_REGEX_ACTION_H
#include "regex/Action.h"
#endif

#ifndef TPL_REGEX_ASSIGN_H
#include "regex/Assign.h"
#endif

#ifndef TPL_REGEX_APPEND_H
#include "regex/Append.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "regex/Ref.h"
#endif

#ifndef TPL_REGEX_FIND_H
#include "regex/Find.h"
#endif

#ifndef TPL_REGEX_CONDITION_H
#include "regex/Condition.h"
#endif

#ifndef TPL_REGEX_CASE_H
#include "regex/Case.h"
#endif

#ifndef TPL_REGEX_TRANSFORMATION_H
#include "regex/Transformation.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_OPERATOR_H
#include "regex/grammar/Operator.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_ACTION_H
#include "regex/grammar/Action.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_CONCRETION_H
#include "regex/grammar/Concretion.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_CONDITION_H
#include "regex/grammar/Condition.h"
#endif

#ifndef TPL_REGEX_CUSTOMIZATION_H
#include "regex/Customization.h"
#endif

// -------------------------------------------------------------------------
// operator>>

template <class ContainerT, class RegExT> inline 
bool TPL_CALL operator>>(const ContainerT& src_, const tpl::Rule<RegExT>& rule_)
{
	tpl::simple::Source source(src_);
	tpl::simple::Context context;
	return rule_.match(source, context);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEXP_H */
