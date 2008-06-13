#define TPL_USE_AUTO_ALLOC
#include <cmath>		// sin, cos, pow
#include <iostream> 	// std::cout
#include <tpl/Emulator.h>

class TestEmulator
{
public:
	typedef tpl::emu::CPU<double, true> cpu;

	void simplest()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		
		// 2.0 * 3.0
		code <<
			cpu::push(alloc, 2.0),
			cpu::push(alloc, 3.0),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);
	
		std::cout << stk.top() << "\n";
	}
	
	void local_var()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		
		// x = 2.0
		// y = 3.0
		// x * y
		code <<
			cpu::local(2),
			
			cpu::lea_local(0),
			cpu::push(alloc, 2.0),
			cpu::assign(),
			cpu::pop(),
			
			cpu::lea_local(1),
			cpu::push(alloc, 3.0),
			cpu::assign(),
			cpu::pop(),

			cpu::push_local(0),
			cpu::push_local(1),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);
	
		std::cout << stk.top() << "\n";
	}
	
	void local_var_optimization()
	{
		cpu::alloc_type alloc;
		cpu::code_type code(alloc);
		
		// x = 2.0
		// y = 3.0
		// x * y
		code <<
			cpu::local(2),
			cpu::push(alloc, 2.0),
			cpu::assign_local(0),
			cpu::push(alloc, 3.0),
			cpu::assign_local(1),
			cpu::mul();

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);
	
		std::cout << stk.top() << "\n";
	}
	
	void call_proc()
	{
		cpu::alloc_type alloc;
		cpu::proc_type<> my_div;
		cpu::label_type<> my_label;
		
		cpu::code_type code(alloc);
		
		code <<
			cpu::jmp(my_label),

			cpu::proc(my_div),
			cpu::push_arg(-2),
			cpu::push_arg(-1),
			cpu::div(),
			cpu::ret(2),
			
			cpu::label(my_label),
			cpu::push(alloc, 2.0),
			cpu::push(alloc, 3.0),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(0, code.size(), stk);
	
		std::cout << stk.top() << "\n";
	}

	void call_proc2()
	{
		cpu::alloc_type alloc;
		cpu::proc_type<0> my_div;
		cpu::label_type<0> my_label;
		
		cpu::code_type code(alloc);
		
		code <<
			cpu::proc(my_div),
			cpu::push_arg(-2),
			cpu::push_arg(-1),
			cpu::div(),
			cpu::ret(2),
			
			cpu::label(my_label),
			cpu::push(alloc, 2.0),
			cpu::push(alloc, 3.0),
			cpu::call(my_div);

		cpu::stack_type stk;
		code.exec(my_label, code.size(), stk);
	
		std::cout << stk.top() << "\n";
	}
};

int main()
{
	TestEmulator test;
	test.simplest();
	test.local_var();
	test.local_var_optimization();
	test.call_proc();
	test.call_proc2();
}

// -------------------------------------------------------------------------

