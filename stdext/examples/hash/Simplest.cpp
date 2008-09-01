#include <iostream>
#include <stdext/HashMap.h>

// -------------------------------------------------------------------------

void testHashMapCStr()
{
	typedef std::AutoFreeAlloc AllocT;
	typedef std::HashMap<const char*, int, std::HashCompare<const char*>, AllocT> MapT;

	AllocT alloc;
	MapT cont(alloc);

	cont.insert(MapT::value_type("Mon", 1));
	cont.insert(MapT::value_type("Tue", 2));

	const char key[] = "Mon";
	MapT::iterator it = cont.find(key);
	if (it != cont.end())
		std::cout << (*it).second << "\n";
	else
		std::cout << "ERROR: Not found!\n";
}

void testHashMapString()
{
	typedef std::AutoFreeAlloc AllocT;
	typedef std::HashMap<std::String, int, std::HashCompare<std::String>, AllocT> MapT;

	AllocT alloc;
	MapT cont(alloc);

	std::String s1(alloc, "Mon");
	std::String s2(alloc, "Tue");
	
	cont.insert(MapT::value_type(s1, 1));
	cont.insert(MapT::value_type(s2, 2));

	MapT::iterator it = cont.find(s2);
	if (it != cont.end())
		std::cout << (*it).second << "\n";
	else
		std::cout << "ERROR: Not found!\n";
}

int main()
{
	std::cout << "----------------------------------\n";
	testHashMapCStr();
	std::cout << "----------------------------------\n";
	testHashMapString();
	std::cout << "----------------------------------\n";
	return 0;
}

// -------------------------------------------------------------------------

