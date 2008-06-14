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

#ifndef TPL_EMU_INSTRUCTION_H
#include "Instruction.h"
#endif

#ifndef TPL_EMU_LABEL_H
#include "Label.h"
#endif

#ifndef STDEXT_DEQUE_H
#include "../../../../stdext/include/stdext/Deque.h"
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

template <bool bDebug = false>
class ExecuteContext
{
private:
	size_t m_ip;
	size_t m_frame;

public:
	enum { debug = bDebug };

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

	void TPL_CALL jump(ptrdiff_t delta) {
		m_ip += delta;
	}
	
	template <class CodeT>
	const typename CodeT::instruction_type& TPL_CALL next(const CodeT& code) {
		return code[m_ip++];
	}
};

// =========================================================================
// class Label(Define/Refer)

template <class LabelT>
class LabelDefine
{
public:
	LabelT& m_label;
	
	LabelDefine(LabelT& label_) : m_label(label_) {
	}
};

template <class LabelT, class InstructionT>
class LabelRefer
{
public:
	typedef InstructionT instruction_type;

	LabelT& m_label;
		
	LabelRefer(LabelT& label_) : m_label(label_) {
	}
};

// =========================================================================
// class PushCode

template <class ValT>
class PushCode
{
public:
	const ValT m_val;
	
	PushCode(const ValT& val) : m_val(val) {
	}
};

// =========================================================================
// class Code

template <class ValT, bool bDebug = false, class AllocT = DefaultAllocator>
class Code : public std::Deque<Instruction<Stack<ValT>, ExecuteContext<bDebug> >, AllocT>
{
private:
	typedef std::Deque<Instruction<Stack<ValT>, ExecuteContext<bDebug> >, AllocT> Base;
	
public:
	typedef AllocT alloc_type;
	typedef Stack<ValT> stack_type;
	typedef ExecuteContext<bDebug> execute_context;
	typedef Instruction<stack_type, execute_context> instruction_type;

	explicit Code(AllocT& alloc)
		: Base(alloc) {
	}

public:
	// Instruction:
	
	Code& TPL_CALL operator,(const instruction_type& instr_) {
		Base::push_back(instr_);
		return *this;
	}

	Code& TPL_CALL operator<<(const instruction_type& instr_) {
		Base::push_back(instr_);
		return *this;
	}

public:
	// Push:
	
	template <class ValT2>
	Code& TPL_CALL operator,(const PushCode<ValT2>& a) {
		Base::push_back(
			Push<stack_type, execute_context>::instr(Base::get_alloc(), a.m_val)
			);
		return *this;
	}
	
	template <class ValT2>
	Code& TPL_CALL operator<<(const PushCode<ValT2>& a) {
		Base::push_back(
			Push<stack_type, execute_context>::instr(Base::get_alloc(), a.m_val)
			);
		return *this;
	}

public:
	// Label:
	
	template <class LabelT>
	Code& TPL_CALL operator,(const LabelDefine<LabelT>& a) {
		a.m_label.define(*this);
		return *this;
	}

	template <class LabelT>
	Code& TPL_CALL operator<<(const LabelDefine<LabelT>& a) {
		a.m_label.define(*this);
		return *this;
	}

	template <class LabelT, class InstructionT>
	Code& TPL_CALL operator,(const LabelRefer<LabelT, InstructionT>& a) {
		Base::push_back(InstructionT::instr(0));
		a.m_label.refer(*this);
		return *this;
	}

	template <class LabelT, class InstructionT>
	Code& TPL_CALL operator<<(const LabelRefer<LabelT, InstructionT>& a) {
		Base::push_back(InstructionT::instr(0));
		a.m_label.refer(*this);
		return *this;
	}

public:
	// Execute:
	
	void TPL_CALL exec(size_t ipFrom, size_t ipTo, stack_type& stk) const
	{
		execute_context context(ipFrom);
		while (context.position() != ipTo) {
			const instruction_type& instr_ = context.next(*this);
			instr_(stk, context);
		}
	}
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_CODE_H */

