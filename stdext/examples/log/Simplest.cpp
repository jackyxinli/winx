#include <stdext/Basic.h>

// -------------------------------------------------------------------------
// log

int main()
{
	std::OutputLog log;

	log.print('a')
		.print(1)
		.print((short)2)
		.newline()
		.print(1.0)
		.print(1.0, "%4.3lf")
		.newline(); 
	
	return 0;
}

// -------------------------------------------------------------------------

