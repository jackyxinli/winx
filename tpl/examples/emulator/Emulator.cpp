#define TPL_USE_AUTO_ALLOC
#include <cmath>		// sin, cos, pow
#include <iostream> 	// std::cout
#include <tpl/Emulator.h>

class TestEmulator
{
public:
	typedef tpl::emu::CPU<double> cpu;

	void simplest()
	{
		std::AutoFreeAlloc alloc;
		cpu::code_type code;
		
		// 2.0 * 3.0
		code <<
			cpu::push(alloc, 2.0),
			cpu::push(alloc, 3.0),
			cpu::mul();

		cpu::stack_type stk;
		tpl::emu::exec(code, 0, code.size(), stk);
	
		std::cout << stk.top() << "\n";
	}
	
	void local_var()
	{
		std::AutoFreeAlloc alloc;
		cpu::code_type code;
		
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
		tpl::emu::exec(code, 0, code.size(), stk);
	
		std::cout << stk.top() << "\n";
	}
};

int main()
{
	TestEmulator test;
	test.simplest();
	test.local_var();
}

// -------------------------------------------------------------------------

