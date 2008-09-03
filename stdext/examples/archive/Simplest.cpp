#include <stdext/Archive.h>
#include <iostream>

// -------------------------------------------------------------------------

void testMMapArchive()
{
	typedef NS_STDEXT::MMapReadCache<char> ReadCache;
	
	ReadCache file(__FILE__);
	ReadCache::archive_type ar(file);

	std::string str;
	while (ar.getline(str))
		std::cout << str << "\n";
}

// -------------------------------------------------------------------------

int main()
{
	testMMapArchive();
	return 0;
}

// -------------------------------------------------------------------------

