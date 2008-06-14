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
// Module: tpl/emu/CPU.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: CPU.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU_CPU_H
#define TPL_EMU_CPU_H

#ifndef TPL_EMU_CODE_H
#include "Code.h"
#endif

#ifndef TPL_EMU_LABEL_H
#include "Label.h"
#endif

#if !defined(_FUNCTIONAL_) && !defined(_GLIBCXX_FUNCTIONAL) && !defined(_FUNCTIONAL)
#include <functional>
#endif

NS_TPL_EMU_BEGIN

// =========================================================================
// class policy::Default

namespace policy
{
	template <bool bDebug>
	class Default
	{
	public:
		typedef DefaultAllocator alloc_type;
		typedef Context<void, bDebug> execute_context;
	};
	
	template <bool bDebug>
	class ExtDefault : public Default<bDebug>
	{
	public:
		typedef Context<DefaultAllocator, bDebug> execute_context;
	};
}

// =========================================================================
// class Assign/ExtAssign

template <class ValT>
class Assign : public std::binary_function<ValT, ValT, ValT>
{
public:
	ValT TPL_CALL operator()(const ValT& x, const ValT& y) const {
		return variant_to_ref(x) = y;
	}
};

template <class ValT>
class ExtAssign : public std::binary_function<ValT, ValT, ValT>
{
public:
	template <class AllocT>
	ValT TPL_CALL operator()(AllocT& alloc, const ValT& x, const ValT& y) const {
		return ext_assign(alloc, variant_to_ref(x), y);
	}
};

// =========================================================================
// class CPU

template <class ValT, bool bDebug = false, class PolicyT = policy::Default<bDebug> >
class CPU
{
private:
	typedef typename PolicyT::alloc_type AllocT;
	typedef typename PolicyT::execute_context ExecuteContextT;
	typedef Code<ValT, ExecuteContextT, AllocT> CodeT;

public:
	typedef AllocT alloc_type;
	typedef CodeT code_type;
	typedef ExecuteContextT execute_context;
	typedef typename CodeT::stack_type stack_type;
	typedef typename CodeT::instruction_type instruction_type;

private:
	typedef stack_type StackT;
	typedef execute_context ContextT;
	typedef instruction_type InstructionT;
	
public:
	template <size_t n = TPL_EMU_DYNAMIC_LABEL>
	class label_type : public Label<n> {
	};
	
	template <size_t n = TPL_EMU_DYNAMIC_LABEL>
	class proc_type : public Label<n> {
	};
	
	template <size_t n>
	static LabelDefine<Label<n> > TPL_CALL label(Label<n>& label_) {
		return LabelDefine<Label<n> >(label_);
	}

	template <size_t n>
	static LabelDefine<Label<n> > TPL_CALL proc(Label<n>& label_) {
		return LabelDefine<Label<n> >(label_);
	}
	
#define TPL_EMU_LABEL_REF_(op, InstrT)	\
	template <size_t n>					\
	static LabelRefer<Label<n>, InstrT<StackT, ContextT> > TPL_CALL op(Label<n>& label_) {	\
		return LabelRefer<Label<n>, InstrT<StackT, ContextT> >(label_);	\
	}
	
	TPL_EMU_LABEL_REF_(call, Call)
	TPL_EMU_LABEL_REF_(jmp, Jmp)
	TPL_EMU_LABEL_REF_(je, JmpIfFalse)
	
public:
	template <template <class Type> class Op_>
	static InstructionT TPL_CALL op() {
		return OpInstr<Op_, StackT, ContextT>::instr();
	}
	
#define TPL_EMU_OP_(op, op_)			\
	static InstructionT TPL_CALL op() {	\
		return OpInstr<op_, StackT, ContextT>::instr(); \
	}

	TPL_EMU_OP_(add, std::plus)
	TPL_EMU_OP_(sub, std::minus)
	TPL_EMU_OP_(mul, std::multiplies)
	TPL_EMU_OP_(div, std::divides)
	TPL_EMU_OP_(mod, std::modulus)
	TPL_EMU_OP_(assign, Assign)
	
	TPL_EMU_OP_(neg, std::negate)
	
	TPL_EMU_OP_(eq, std::equal_to)
	TPL_EMU_OP_(ne, std::not_equal_to)
	TPL_EMU_OP_(gt, std::greater)
	TPL_EMU_OP_(ge, std::greater_equal)
	TPL_EMU_OP_(lt, std::less)
	TPL_EMU_OP_(le, std::less_equal)
	
#define TPL_EMU_FN_IMPL_(n) 		\
	static InstructionT TPL_CALL func(typename FnInstr<n, StackT, ContextT>::op_type fn) { \
		return FnInstr<n, StackT, ContextT>::instr(fn); \
	}
	
	TPL_EMU_FN_IMPL_(1)
	TPL_EMU_FN_IMPL_(2)
	TPL_EMU_FN_IMPL_(3)
	TPL_EMU_FN_IMPL_(4)
	TPL_EMU_FN_IMPL_(5)
	TPL_EMU_FN_IMPL_(6)
	
#define TPL_EMU_VARGS_IMPL_(IntT)	\
	static InstructionT TPL_CALL func(typename VargsFnInstr<IntT, StackT, ContextT>::op_type fn) { \
		return VargsFnInstr<IntT, StackT, ContextT>::instr(fn);	\
	}
	
	TPL_EMU_VARGS_IMPL_(size_t)
	TPL_EMU_VARGS_IMPL_(int)
	
public:
	static InstructionT TPL_CALL nop() {
		return Nop<StackT, ContextT>::instr();
	}

	template <class ValT2>
	static PushCode<ValT2> TPL_CALL push(const ValT2& val) {
		return PushCode<ValT2>(val);
	}
	
	template <class AllocT2, class ValT2>
	static InstructionT TPL_CALL push(AllocT2& alloc, const ValT2& val) {
		return Push<StackT, ContextT>::instr(alloc, val);
	}

	static InstructionT TPL_CALL pop(size_t n) {
		return Pop<StackT, ContextT>::instr(n);
	}

	static InstructionT TPL_CALL pop() {
		return Pop<StackT, ContextT>::instr();
	}

	static InstructionT TPL_CALL jmp(ptrdiff_t delta) {
		return Jmp<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL je(ptrdiff_t delta) {
		return JmpIfFalse<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL arity(size_t n) {
		return Arity<StackT, ContextT>::instr(n);
	}

	static InstructionT TPL_CALL local(size_t n) {
		return Local<StackT, ContextT>::instr(n);
	}

	static InstructionT TPL_CALL push_arg(ptrdiff_t delta) {
		return PushArg<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL push_vargs() {
		return PushVArgs<StackT, ContextT>::instr();
	}

	static InstructionT TPL_CALL push_local(size_t delta) {
		return PushLocal<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL lea_arg(ptrdiff_t delta) {
		return LeaArg<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL lea_vargs() {
		return PushVArgs<StackT, ContextT>::instr();
	}

	static InstructionT TPL_CALL lea_local(size_t delta) {
		return LeaLocal<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL assign_arg(ptrdiff_t delta) {
		return AssignArg<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL assign_local(size_t delta) {
		return AssignLocal<StackT, ContextT>::instr(delta);
	}

	static InstructionT TPL_CALL call(ptrdiff_t delta) {
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
// ExtCPU: ext-operators

#define TPL_EMU_BINARY_EXTOP_(op) 											\
template <class ValT>														\
class op : public std::binary_function<ValT, ValT, ValT>					\
{																			\
public:																		\
	template <class AllocT>													\
	ValT TPL_CALL operator()(AllocT& alloc, const ValT& x, const ValT& y) const { \
		return ext_##op(alloc, x, y);										\
	}																		\
};

TPL_EMU_BINARY_EXTOP_(plus)
TPL_EMU_BINARY_EXTOP_(minus)
TPL_EMU_BINARY_EXTOP_(multiplies)
TPL_EMU_BINARY_EXTOP_(divides)
TPL_EMU_BINARY_EXTOP_(modulus)

TPL_EMU_BINARY_EXTOP_(equal_to)
TPL_EMU_BINARY_EXTOP_(not_equal_to)
TPL_EMU_BINARY_EXTOP_(greater)
TPL_EMU_BINARY_EXTOP_(greater_equal)
TPL_EMU_BINARY_EXTOP_(less)
TPL_EMU_BINARY_EXTOP_(less_equal)

#define TPL_EMU_UNARY_EXTOP_(op) 											\
template <class ValT>														\
class op : public std::unary_function<ValT, ValT>							\
{																			\
public:																		\
	template <class AllocT>													\
	ValT TPL_CALL operator()(AllocT& alloc, const ValT& x) const {			\
		return ext_##op(alloc, x);											\
	}																		\
};

TPL_EMU_UNARY_EXTOP_(negate)

// -------------------------------------------------------------------------
// class ExtCPU

template <class ValT, bool bDebug = false, class PolicyT = policy::ExtDefault<bDebug> >
class ExtCPU : public CPU<ValT, bDebug, PolicyT>
{
private:
	typedef CPU<ValT, bDebug, PolicyT> Base;
	typedef typename Base::instruction_type InstructionT;
	typedef typename Base::stack_type StackT;
	typedef typename Base::execute_context ContextT;

public:
	using Base::func;
	
	template <template <class Type> class Op_>
	static InstructionT TPL_CALL ext_op() {
		return ExtOpInstr<Op_, StackT, ContextT>::instr();
	}
	
#define TPL_EMU_EXTOP_(op, op_)			\
	static InstructionT TPL_CALL op() {	\
		return ExtOpInstr<op_, StackT, ContextT>::instr(); \
	}

	TPL_EMU_EXTOP_(add, NS_TPL_EMU::plus)
	TPL_EMU_EXTOP_(sub, NS_TPL_EMU::minus)
	TPL_EMU_EXTOP_(mul, NS_TPL_EMU::multiplies)
	TPL_EMU_EXTOP_(div, NS_TPL_EMU::divides)
	TPL_EMU_EXTOP_(mod, NS_TPL_EMU::modulus)
	TPL_EMU_EXTOP_(assign, ExtAssign)
	
	TPL_EMU_EXTOP_(neg, NS_TPL_EMU::negate)
	
	TPL_EMU_EXTOP_(eq, NS_TPL_EMU::equal_to)
	TPL_EMU_EXTOP_(ne, NS_TPL_EMU::not_equal_to)
	TPL_EMU_EXTOP_(gt, NS_TPL_EMU::greater)
	TPL_EMU_EXTOP_(ge, NS_TPL_EMU::greater_equal)
	TPL_EMU_EXTOP_(lt, NS_TPL_EMU::less)
	TPL_EMU_EXTOP_(le, NS_TPL_EMU::less_equal)

#define TPL_EMU_EXTFN_IMPL_(n) 		\
	static InstructionT TPL_CALL func(typename ExtFnInstr<n, StackT, ContextT>::op_type fn) { \
		return ExtFnInstr<n, StackT, ContextT>::instr(fn); \
	}
	
	TPL_EMU_EXTFN_IMPL_(1)
	TPL_EMU_EXTFN_IMPL_(2)
	TPL_EMU_EXTFN_IMPL_(3)
	TPL_EMU_EXTFN_IMPL_(4)
	TPL_EMU_EXTFN_IMPL_(5)
	TPL_EMU_EXTFN_IMPL_(6)
	
#define TPL_EMU_EXTVARGS_IMPL_(IntT)	\
	static InstructionT TPL_CALL func(typename ExtVargsFnInstr<IntT, StackT, ContextT>::op_type fn) { \
		return ExtVargsFnInstr<IntT, StackT, ContextT>::instr(fn);	\
	}
	
	TPL_EMU_EXTVARGS_IMPL_(size_t)
	TPL_EMU_EXTVARGS_IMPL_(int)

public:
	template <class ValT2>
	static ExtPushCode<ValT2> TPL_CALL push(const ValT2& val) {
		return ExtPushCode<ValT2>(val);
	}

	static PushCode<ValT> TPL_CALL push(const ValT& val) {
		return PushCode<ValT>(val);
	}
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_CPU_H */

