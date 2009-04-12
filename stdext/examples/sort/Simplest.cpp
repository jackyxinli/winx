#include <stdext/FileBuf.h>
#include <stdext/Archive.h>
#include <deque>
#include <list>

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

	MapFileBuf input(argv[1]);
	FileWriter output(argv[2]);	
	const String s(input.begin(), input.end());
	
	std::deque<String> lines;
	explode('\n', s, lines);
	std::stable_sort(lines.begin(), lines.end());
	
	output.printLines(lines);
}

inline void test3(const char* argv[])
{
	using namespace NS_STDEXT;

	AutoFreeAlloc alloc;
	FileReader input(argv[1]);
	FileWriter output(argv[2]);

	String line;
	std::deque<String> lines;
	while (input.getline(alloc, line))
		lines.push_back(line);

	std::stable_sort(lines.begin(), lines.end());
	output.printLines(lines);
}

inline void test4(const char* argv[])
{
	using namespace NS_STDEXT;
	
	AutoFreeAlloc alloc;
	FileReader input(argv[1]);
	FileWriter output(argv[2]);
	
	String line;
	std::list<String> lines;
	while (input.getline(alloc, line))
		lines.push_back(line);
	
	lines.sort();
	output.printLines(lines);
}

int main(int argc, const char* argv[])
{
	if (argc < 3)
		return -1;

#define LIST_SORT
#define READ_ARCHIVE
#define FAST_VERSION
#if defined(LIST_SORT)
	test4(argv);
#elif defined(READ_ARCHIVE)
	test3(argv);
#elif defined(FAST_VERSION)
	test2(argv);
#else
	test1(argv);
#endif

	return 0;
}
