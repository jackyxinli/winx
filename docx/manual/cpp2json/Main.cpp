#define TPL_USE_AUTO_ALLOC
#include "Lex.h"
#include "Lex.inl"
#include <stdext/FileBuf.h>

// -------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
	NS_STDEXT::ErrorLog err;
	if (argc < 2) {
		err.print("Usage: cpp2json <cpp_file>\n");
		return -1;
	}

	NS_STDEXT::FileBuf file(argv[1]);
	if (!file.good()) {
		err.print(">>> ERROR: open input file failed!\n");
		return -2;
	}
	
	if (file == document)
	{
		NS_STDEXT::OutputLog log;
		json_print(alloc, log, doc);
		return 0;
	}

	err.print(">>> ERROR: invalid file format!\n");
	return -3;
}

// -------------------------------------------------------------------------
