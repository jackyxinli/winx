#include <stdext/Archive.h>
#include <iostream>

// -------------------------------------------------------------------------

void testMMapArchive(const char* path)
{
	typedef NS_STDEXT::MMapReadCache<char> ReadCache;
	typedef NS_STDEXT::PosixReadArchive ArchiveT2;
	
	ReadCache file(path);
	ReadCache::archive_type ar(file);
	ArchiveT2 ar2(path);

	int nLine = 0;
	std::string str;
	std::string str2;
	while (ar.getline(str))
	{
		++nLine;
		ar2.getline(str2);
		if (str != str2) {
			std::cout << nLine << ": " << str << "\n";
			break;
		}
	}
	std::cout << "Ok! Total lines: " << nLine << "\n";
}

// -------------------------------------------------------------------------

void testStdin()
{
	std::cout << "Input string (^D to quit): " << std::flush;

	NS_STDEXT::PosixReadArchive ar(STDIN_FILENO);
	std::string str;
	while (ar.getline(str))
		std::cout << str << "\n";
}

// -------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
	testMMapArchive(argc == 2 ? argv[1] : __FILE__);
	testStdin();
	return 0;
}

// -------------------------------------------------------------------------

