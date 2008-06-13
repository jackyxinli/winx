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

#if !defined(_FUNCTIONAL_) && !defined(_GLIBCXX_FUNCTIONAL) && !defined(_FUNCTIONAL)
#include <functional>
#endif

NS_TPL_EMU_BEGIN

// =========================================================================
// class Stack

template <class ValT>
class Stack : public std::deque<ValT>
{
private:
	typedef std::deque<ValT> Base;
	
public:
	__forceinline const ValT& TPL_CALL top() const {
		return Base::back();
	}
	
	__forceinline void TPL_CALL push(const ValT& val) {
		Base::push_back(val);
	}

	__forceinline void TPL_CALL pop() {
		Base::pop_back();
	}
};

// =========================================================================
// class ExecuteContext

class ExecuteContext
{
private:
	size_t m_ip;
	size_t m_frame;

public:
	ExecuteContext(size_t ip)
		: m_ip(ip), m_frame(0) {
	}

public:
	size_t TPL_CALL frame() const {
		return m_frame;
	}

	void TPL_CALL frame(size_t frame_) {
		m_frame = frame_;
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
private:
	typedef std::deque<Instruction<Stack<ValT>, ExecuteContext> > Base;
	
public:
	typedef Stack<ValT> stack_type;
	typedef ExecuteContext execute_context;
	typedef Instruction<stack_type, execute_context> instruction_type;
	
public:
	Code& TPL_CALL operator<<(const instruction_type& instr_) {
		Base::push_back(instr_);
		return *this;
	}

	Code& TPL_CALL operator,(const instruction_type& instr_) {
		Base::push_back(instr_);
		return *this;
	}
};

// =========================================================================
// function exec

template <class CodeT> inline
void TPL_CALL exec(const CodeT& code, size_t ipFrom, size_t ipTo, typename CodeT::stack_type& stk)
{
	ExecuteContext context(ipFrom);
	while (context.position() != ipTo) {
		const typename CodeT::instruction_type& instr_ = context.next(code);
		instr_(stk, context);
	}
}

// =========================================================================
// class CPU

template <class ValT, class CodeT = Code<ValT> >
class CPU
{
public:
	typedef CodeT code_type;
	typedef typename CodeT::stack_type stack_type;
	typedef typename CodeT::execute_context execute_context;
	typedef typename CodeT::instruction_type instruction_type;

private:
	typedef stack_type StackT;
	typedef execute_context ContextT;
	typedef instruction_type InstructionT;
	
public:
	template <template <class Type> class Op_>
	static InstructionT TPL_CALL op() {
		return OpInstr<Op_, StackT, ContextT>::instr();
	}
	
#define TPL_EMU_OP_(op, op_)		\
	static InstructionT TPL_CALL op() {	\
		return OpInstr<op_, StackT, ContextT>::instr(); \
	}

	TPL_EMU_OP_(add, std::plus)
	TPL_EMU_OP_(sub, std::minus)
	TPL_EMU_OP_(mul, std::multiplies)
	TPL_EMU_OP_(div, std::divides)
	TPL_EMU_OP_(mod, std::modulus)
	
	TPL_EMU_OP_(neg, std::negate)
	
	TPL_EMU_OP_(eq, std::equal_to)
	TPL_EMU_OP_(ne, std::not_equal_to)
	TPL_EMU_OP_(gt, std::greater)
	TPL_EMU_OP_(ge, std::greater_equal)
	TPL_EMU_OP_(lt, std::less)
	TPL_EMU_OP_(le, std::less_equal)
	
#define TPL_EMU_FN_IMPL_(n) 		\
	static InstructionT TPL_CALL op(typename FnInstr<n, StackT, ContextT>::op_type fn) { \
		return FnInstr<n, StackT, ContextT>::instr(fn); \
	}
	
	TPL_EMU_FN_IMPL_(1)
	TPL_EMU_FN_IMPL_(2)
	TPL_EMU_FN_IMPL_(3)
	TPL_EMU_FN_IMPL_(4)
	TPL_EMU_FN_IMPL_(5)
	TPL_EMU_FN_IMPL_(6)

public:
	template <class AllocT>
	static InstructionT TPL_CALL push(AllocT& alloc, const ValT& val) {
		return Push<StackT, ContextT>::instr(alloc, val);
	}
	
	static InstructionT TPL_CALL pop(size_t n = 1) {
		return Pop<StackT, ContextT>::instr(n);
	}

	static InstructionT TPL_CALL jmp(int delta) {
		return Jmp<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL je(int delta) {
		return JmpIfFalse<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL arity(size_t n) {
		return Arity<StackT, ContextT>::instr(n);
	}

	static InstructionT TPL_CALL local(size_t n) {
		return Local<StackT, ContextT>::instr(n);
	}

	static InstructionT TPL_CALL load_arg(int delta) {
		return LoadArg<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL load_vargs() {
		return LoadVArgs<StackT, ContextT>::instr();
	}

	static InstructionT TPL_CALL load_local(size_t delta) {
		return LoadLocal<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL call(int delta) {
		return Call<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL ret(size_t n) {
		return RetN<StackT, ContextT>::instr(n);
	}

	static InstructionT TPL_CALL ret() {
		return Ret<StackT, ContextT>::instr();
	}
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_CODE_H */

