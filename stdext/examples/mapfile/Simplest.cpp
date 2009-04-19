#include <stdext/MMap.h>

// -------------------------------------------------------------------------

void testMapFile()
{
	NS_STDEXT::MapFileRW file("__TestMapFiel__.txt");

	file.resize(0x100);
	char* psz = (char*)file.map(0, 32);
	strcpy(psz + 2, "hello, world!");
	file.flush(psz, 32);
	
	NS_STDEXT::MapFileRO file2("__TestMapFiel__.txt");
	psz = (char*)file2.map(0, 32);
	puts(psz + 2);
}

// -------------------------------------------------------------------------

int main()
{
	testMapFile();
	return 0;
}

// -------------------------------------------------------------------------

