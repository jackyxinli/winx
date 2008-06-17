#define TPL_USE_AUTO_ALLOC
#include <cmath>		// sin, cos, pow
#include <iostream> 	// std::cout
#include <tpl/Emulator.h>
#include <stdext/Variant.h>

// class test::Variant

enum TypeList {
	vtNone,
	vtInt,
	vtDouble,
};

WINX_VARIANT_TYPES_BEGIN(test, TypeList, vtNone)
	WINX_VARIANT_TYPE(int, vtInt)
	WINX_VARIANT_TYPE(double, vtDouble)
WINX_VARIANT_TYPES_END()

typedef test::Variant Variant;

inline double TPL_CALL to_double(const Variant& a)
{
	switch (a.type())
	{
	case vtDouble:	return a.get<double>();
	case vtInt:		return a.get<int>();
	default:		throw std::bad_cast();
	}
	return 0;
}

namespace std {

template <class OStreamT>
inline OStreamT& TPL_CALL operator<<(OStreamT& os, const test::Variant& a)
{
	switch (a.type())
	{
	case vtDouble:	return os << a.get<double>();
	case vtInt:		return os << a.get<int>();
	default:		return os << "(unassigned)";
	}
}

inline bool operator<(const test::Variant& a, const test::Variant& b)
{
	return to_double(a) < to_double(b);
}

}

template <class AllocT>
inline Variant TPL_CALL ext_multiplies(
	AllocT& alloc, const Variant& a, const Variant& b)
{
	if (a.type() == vtInt && b.type() == vtInt) {
		return Variant(alloc, a.get<int>() * b.get<int>());
	}
	else
		return Variant(alloc, to_double(a) * to_double(b));
}

template <class AllocT>
inline Variant TPL_CALL ext_divides(
	AllocT& alloc, const Variant& a, const Variant& b)
{
	if (a.type() == vtInt && b.type() == vtInt)
		return Variant(alloc, a.get<int>() / b.get<int>());
	else
		return Variant(alloc, to_double(a) / to_double(b));
}

template <class AllocT>
inline Variant TPL_CALL ext_assign(
	AllocT& alloc, Variant& a, const Variant& b)
{
	if (a.type() != vtNone)
		throw std::logic_error("variable is already assigned!");
	return a = b;
}

// class TestEmulator

class TestEmulator
{
public:
	typedef tpl::emu::ExtCPU<Variant, true> cpu;

	void simplest()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		
		// 3 / 2
		code <<
			cpu::push(3),
			cpu::push(2),
			cpu::div();

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	void local_var()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		cpu::var_type x, y;
		
		// x = 2
		// y = 3.0
		// x * y
		code <<
			cpu::var(x, y),
			
			cpu::lea(x),
			cpu::push(2),
			cpu::assign(),
			cpu::pop(),
			
			cpu::lea(y),
			cpu::push(3.0),
			cpu::assign(),
			cpu::pop(),

			cpu::push(x),
			cpu::push(y),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);

		std::cout << stk.top() << "\n\n";
	}
	
	void local_var_optimization()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		cpu::var_type x, y;
		
		// x = 2.0
		// y = 3.0
		// x * y
		code <<
			cpu::push(2.0),
			cpu::var_init(x),
			cpu::push(3.0),
			cpu::var_init(y),
			
			cpu::push(x),
			cpu::push(y),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
	
	static Variant my_sin(cpu::alloc_type& alloc, const Variant& x)
	{
		return Variant(alloc, sin(to_double(x)));
	}

	static Variant max_value(const Variant x[], int count)
	{
		return *std::max_element(x, x+count);
	}

	void vargs()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);

		// max(2.0, 3.0, sin(4.0))
		code <<
			cpu::push(2.0),
			cpu::push(3.0),
			cpu::push(4.0),
			cpu::func(my_sin),
			cpu::arity(3),
			cpu::func(max_value);
		
		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);

		std::cout << stk.top() << "\n\n";
	}

	void call_proc()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);

		cpu::proc_type<> my_div;
		cpu::label_type<> my_label;
		cpu::var_type x, y;

		code <<
			cpu::jmp(my_label),

			cpu::proc(my_div),
			cpu::arg(x),
			cpu::arg(y),
			cpu::end_arglist(),
			cpu::push(x),
			cpu::push(y),
			cpu::div(),
			cpu::ret(2),

			cpu::label(my_label),
			cpu::push(3),
			cpu::push(2),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(alloc, 0, code.size(), stk);

		std::cout << stk.top() << "\n\n";
	}

	void call_proc2()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);

		cpu::proc_type<0> my_div;
		cpu::label_type<0> my_label;
		cpu::var_type x, y;

		code <<
			cpu::proc(my_div, x, y),
			cpu::push(x),
			cpu::push(y),
			cpu::div(),
			cpu::ret(2),

			cpu::label(my_label),
			cpu::push(3),
			cpu::push(2.0),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(alloc, my_label, code.size(), stk);
	
		std::cout << stk.top() << "\n\n";
	}
};

int main()
{
	TestEmulator test;
	test.simplest();
	test.local_var();
	test.local_var_optimization();
	test.vargs();
	test.call_proc();
	test.call_proc2();
}

// -------------------------------------------------------------------------

