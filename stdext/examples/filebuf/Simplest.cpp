#include <stdext/FileBuf.h>

// -------------------------------------------------------------------------
// log

int main()
{
	NS_STDEXT::OutputLog log;

	NS_STDEXT::FileBuf file(__FILE__);	
	log.printString(file.begin(), file.end());
	
	log.newline();
	
	NS_STDEXT::MapFileBuf file2(__FILE__);
	log.printString(file2.begin(), file2.end());
	return 0;
}

// -------------------------------------------------------------------------

