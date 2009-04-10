#include <stdext/FileBuf.h>
#include <stdext/Archive.h>

int main(int argc, const char* argv[])
{
	using namespace NS_STDEXT;

	if (argc < 3)
		return -1;

	SystemAlloc alloc;
	FileBuf input(argv[1]);
	FileWriter output(argv[2]);
	
	const String s(input.begin(), input.end());
	output.printLines(explode(alloc, '\n', s).sort());
	return 0;
}
