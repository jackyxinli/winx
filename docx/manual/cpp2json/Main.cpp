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
		long sourcecp = NS_STDEXT::cp_auto;
		if (!encoding.empty())
		{
			NS_STDEXT::BasicString<char> lowerEncode = NS_STDEXT::lower(alloc, encoding);
			if (lowerEncode == "gbk")
				sourcecp = NS_STDEXT::cp_gbk;
			else if (lowerEncode == "utf-8" || lowerEncode == "utf8")
				sourcecp = NS_STDEXT::cp_utf8;
			else if (lowerEncode == "utf-7" || lowerEncode == "utf7")
				sourcecp = NS_STDEXT::cp_utf7;
			else if (lowerEncode == "big5")
				sourcecp = NS_STDEXT::cp_big5;
		}
		
		json_print(alloc, log, doc, (NS_STDEXT::codepage_t)sourcecp);
		return 0;
	}

	err.print(">>> ERROR: invalid file format!\n");
	return -3;
}

// -------------------------------------------------------------------------
