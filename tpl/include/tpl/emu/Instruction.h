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
// Module: tpl/emu/Instruction.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Instruction.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_ENU_INSTRUCTION_H
#define TPL_ENU_INSTRUCTION_H

#ifndef TPL_EMU_FUNCTION_H
#include "Function.h"
#endif

NS_TPL_EMU_BEGIN

// =========================================================================
// Instruction

union Operand
{
	int ival;
	size_t val;
	const void* ptr;
};

template <class StackT, class ExecuteContextT>
class Instruction
{
private:
	typedef void (*Op)(Operand para, StackT& stk, ExecuteContextT& context);

public:
	const Op op;
	Operand para;

public:
	Instruction(Op op_) : op(op_) {}
	Instruction(Op op_, int val_) : op(op_) 		{ para.ival = val_; }
	Instruction(Op op_, size_t val_) : op(op_)	 	{ para.val = val_; }
	Instruction(Op op_, const void* ptr_) : op(op_) { para.ptr = ptr_; }

	__forceinline void TPL_CALL operator()(StackT& stk, ExecuteContextT& context) const {
		op(para, stk, context);
	}
};

// =========================================================================
// class OpInstr

// Usage:
//		add		; OpInstr<std::plus, StackT, ContextT>::instr()
//		sub		; OpInstr<std::minus, StackT, ContextT>::instr()
//		mul		; OpInstr<std::multiplies, StackT, ContextT>::instr()
//		div		; OpInstr<std::divides, StackT, ContextT>::instr()

template <template <class Type> class Op_, class StackT, class ExecuteContextT>
class OpInstr
{
private:
	typedef typename StackT::value_type Ty;
	enum { n_arity = ArityTraits<Op_, Ty>::value };

public:
	static void op(Operand, StackT& stk, ExecuteContextT&) {
		Function<Op_<Ty>, n_arity> fn_;
		fn_(stk);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

// =========================================================================
// class FnInstr

// Usage:
//		pow		; FnInstr<2, StackT, ContextT>::instr(pow)
//		sin		; FnInstr<1, StackT, ContextT>::instr(sin)
//		cos		; FnInstr<1, StackT, ContextT>::instr(cos)

template <int nArity, class StackT, class ExecuteContextT>
class FnInstr
{
private:
	typedef typename StackT::value_type Ty; 
	
public:
	typedef typename OpTraits<nArity, Ty>::op_type op_type;

	static void op(StackT& stk, Operand para) {
		Function<op_type, nArity> fn_((op_type)para.ptr);
		fn_(stk);
	}

	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(op_type fn) {
		return Instruction<StackT, ExecuteContextT>(op, fn);
	}
};

// =========================================================================
// class Push/Pop

// Usage: push <val>

template <class StackT, class ExecuteContextT>
class Push
{
private:
	typedef typename StackT::value_type ValT;

public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		stk.push_back(*(const ValT*)para.ptr);
	}
	
	template <class AllocT>
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(AllocT& alloc, const ValT& val) {
		const ValT* p = TPL_NEW(alloc, ValT)(val);
		return Instruction<StackT, ExecuteContextT>(op, p);
	}
};

// Usage: pop <count>

template <class StackT, class ExecuteContextT>
class Pop
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		stk.resize(stk.size() - para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n = 1) {
		return Instruction<StackT, ExecuteContextT>(op, n);
	}
};

// =========================================================================
// class Jmp

// Usage: jmp <delta>

template <class StackT, class ExecuteContextT>
class Jmp
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		context.jump(para.ival);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(int delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class JmpIfFalse

// Usage: je <delta>

template <class StackT, class ExecuteContextT>
class JmpIfFalse
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		const typename StackT::value_type val = stk.back();
		stk.pop_back();
		if (!val)
			context.jump(para.ival);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(int delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class Arity

// Usage: arity <count>

template <class StackT, class ExecuteContextT>
class Arity
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		stk.push_back(para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n) {
		return Instruction<StackT, ExecuteContextT>(op, n);
	}
};

// =========================================================================
// class Local

// Usage: local <count>

template <class StackT, class ExecuteContextT>
class Local
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		stk.resize(stk.size() + para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n) {
		return Instruction<StackT, ExecuteContextT>(op, n);
	}
};

// =========================================================================
// class CallerFrame

// Function Stack Frame
//	 arg1
//	 arg2
//	 ...
//	 argn
//	 [arity]
//	 callerframe
//	 ------------ <frame>
//	 [local_var1]
//	 [local_var2]
//	 ...
//	 [local_varn]
//	 return_val

class CallerFrame
{
public:
	enum { SIZE = 2 };
	
	enum { ARITY = -3 };
	enum { RETURN_IP = -2 };
	enum { BP = -1 };
	
public:
	template <class StackT, class ExecuteContextT>
	static void TPL_CALL call(StackT& stk, ExecuteContextT& context, int delta) {
		stk.push_back(context.position());
		stk.push_back(context.frame());
		context.frame(stk.size());
		context.jump(delta);
	}

	template <class StackT, class ExecuteContextT>
	static void TPL_CALL ret(StackT& stk, ExecuteContextT& context, size_t n) {
		typename StackT::value_type val = stk.top();
		size_t frame_ = context.frame();
		context.frame(stk[frame_ + BP]);
		context.position(stk[frame_ + RETURN_IP]);
		stk.resize(frame_ - n);
		stk.push_back(val);
	}

public:
	template <class StackT, class ExecuteContextT>
	size_t TPL_CALL arity(StackT& stk, ExecuteContextT& context) const {
		const size_t arity_idx_ = context.frame() + ARITY;
		return stk[arity_idx_];
	}

	template <class StackT, class ExecuteContextT>
	typename StackT::value_type TPL_CALL vargs(StackT& stk, ExecuteContextT& context) const {
		const size_t arity_idx_ = context.frame() + ARITY;
		const size_t arity_ = stk[arity_idx_];
		return typename StackT::value_type::array(stk, arity_idx_ - arity_, arity_);
	}
};

// =========================================================================
// class Call

// Usage: call <delta>
//
// Example1:
//	push 3.0
//	push 2.0
//	call offset pow
// Meanings:
//	pow(3.0, 2.0)
//
// Example2:
//	push 3.0
//	push 2.0
//	push 1.0
//	arity 3
//	call offset max
// Meanings:
//	max(3.0, 2.0, 1.0)

template <class StackT, class ExecuteContextT>
class Call
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		CallerFrame::call(stk, context, para.ival);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(int delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class LoadArg/LoadVArgs/LoadLocal

// Usage: load_arg <offset>	; here <offset> can be -n ~ -1
//	 arg1 = load_arg -n
//	 arg2 = load_arg -(n-1)
//	 ...
//	 argn = load_arg -1

template <class StackT, class ExecuteContextT>
class LoadArg
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		stk.push_back(stk[context.frame() + para.ival]);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(int delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta - CallerFrame::SIZE);
	}
};

// Usage: load_vargs

template <class StackT, class ExecuteContextT>
class LoadVArgs
{
public:	
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		stk.push_back(CallerFrame::vargs(stk, context));
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

// Usage: load_local <index> ; here <index> can be 0 ~ n-1
//	 local_var1 = load_local 0
//	 local_var2 = load_local 1
//	 ...
//	 local_varn = load_local n-1

template <class StackT, class ExecuteContextT>
class LoadLocal
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		stk.push_back(stk[context.frame() + para.val]);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class Ret/RetN

// Usage: ret <n>
//
// Example1:
//	 proc my_pow
//		load_arg -2		; push arg1
//		load_arg -1		; push arg2
//		pow				; call pow instruction 
//		ret 2			; return
//	end proc
//
// Example2:
//	 proc my_max
//		load_vargs		; push arg[]
//		max_n			; call max_n instruction
//		ret				; return
//	 end proc

template <class StackT, class ExecuteContextT>
class RetN
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		CallerFrame::ret(stk, context, para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n) {
		return Instruction<StackT, ExecuteContextT>(op, n + CallerFrame::SIZE);
	}
};

template <class StackT, class ExecuteContextT>
class Ret
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		const size_t n = CallerFrame::arity(stk, context) + (CallerFrame::SIZE + 1);
		CallerFrame::ret(stk, context, n);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_ENU_INSTRUCTION_H */

