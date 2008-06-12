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
// Module: tpl/emu/Code.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Code.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU_CODE_H
#define TPL_EMU_CODE_H

#ifndef TPL_EMU_FUNCTION_H
#include "Function.h"
#endif

#ifndef TPL_ENU_INSTRUCTION_H
#include "Instruction.h"
#endif

#if !defined(_DEQUE_) && !defined(_GLIBCXX_DEQUE) && !defined(_DEQUE)
#include <deque>
#endif

NS_TPL_ENU_BEGIN

// =========================================================================
// class Stack

template <class ValT>
class Stack : public std::deque<ValT>
{
};

// =========================================================================
// class ExecuteContext

class ExecuteContext
{
private:
	size_t m_ip;
	size_t m_frame;
	
public:
	size_t TPL_CALL frame() const {
		return m_frame;
	}

	void TPL_CALL frame(size_t frame_) {
		return m_frame = frame_;
	}
	
public:
	size_t TPL_CALL position() const {
		return m_ip;
	}
	
	void TPL_CALL position(size_t ip) {
		m_ip = ip;
	}

	void TPL_CALL jump(int delta) {
		m_ip += delta;
	}
	
	template <class CodeT>
	const typename CodeT::instruction_type& TPL_CALL next(const CodeT& code) {
		return code[m_ip++];
	}
};

// =========================================================================
// class Code

template <class ValT>
class Code : public std::deque<Instruction<Stack<ValT>, ExecuteContext> >
{
public:
	typedef Stack<ValT> stack_type;
	typedef ExecuteContext execute_context;
	typedef Instruction<stack_type, execute_context> instruction_type;
};

// =========================================================================
// $Log: $

NS_TPL_ENU_END

#endif /* TPL_EMU_CODE_H */

