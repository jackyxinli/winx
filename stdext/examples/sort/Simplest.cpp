#include <stdext/FileBuf.h>
#include <stdext/Archive.h>
#include <deque>

inline void test1(const char* argv[])
{
	using namespace NS_STDEXT;

	SystemAlloc alloc;
	FileBuf input(argv[1]);
	FileWriter output(argv[2]);
	
	const String s(input.begin(), input.end());
	output.printLines(explode(alloc, '\n', s).sort());
}

inline void test2(const char* argv[])
{
	using namespace NS_STDEXT;

	FileBuf input(argv[1]);
	FileWriter output(argv[2]);	
	const String s(input.begin(), input.end());
	
	std::deque<String> lines;
	split(lines, '\n', s);
	std::sort(lines.begin(), lines.end());
	
	output.printLines(lines);
}

int main(int argc, const char* argv[])
{
	if (argc < 3)
		return -1;

#define FAST_VERSION
#if defined(FAST_VERSION)
	test2(argv);
#else
	test1(argv);
#endif

	return 0;
}
