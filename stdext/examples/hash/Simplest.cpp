#include <iostream>
#include <stdext/HashMap.h>

// -------------------------------------------------------------------------

void testHashMapCStr()
{
	typedef NS_STDEXT::AutoAlloc AllocT;
	typedef NS_STDEXT::HashMap<const char*, int, NS_STDEXT::HashCompare<const char*>, AllocT> MapT;

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
	typedef NS_STDEXT::AutoAlloc AllocT;
	typedef NS_STDEXT::HashMap<NS_STDEXT::String, int, NS_STDEXT::HashCompare<NS_STDEXT::String>, AllocT> MapT;

	AllocT alloc;
	MapT cont(alloc);

	NS_STDEXT::String s1(alloc, "Mon");
	NS_STDEXT::String s2(alloc, "Tue");
	
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

