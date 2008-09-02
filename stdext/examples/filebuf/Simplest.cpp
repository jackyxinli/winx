#include <stdext/FileBuf.h>

// -------------------------------------------------------------------------
// log

int main()
{
	NS_STDEXT::OutputLog log;
	NS_STDEXT::FileBuf file(__FILE__);
	
	log.printString(file.begin(), file.end());
	return 0;
}

// -------------------------------------------------------------------------

