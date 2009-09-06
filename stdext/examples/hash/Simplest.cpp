#include <stlpatch.h>
#include <iostream>
#include <stdext/HashMap.h>
#include <stdext/p/HashMap.h>
#include <stdext/p/HashSet.h>

// -------------------------------------------------------------------------

void testHashMapCStr()
{
	typedef NS_STDEXT::HashMap<const char*, int, NS_STDEXT::HashCompare<const char*> > MapT;

	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedAlloc alloc(recycle);
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
	typedef NS_STDEXT::HashMap<NS_STDEXT::String, int, NS_STDEXT::HashCompare<NS_STDEXT::String> > MapT;

	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedAlloc alloc(recycle);
	MapT cont(alloc);

	NS_STDEXT::String s1 = NS_STDEXT::g_str("Mon");
	NS_STDEXT::String s2 = NS_STDEXT::g_str("Tue");
	
	cont.insert(MapT::value_type(s1, 1));
	cont.insert(MapT::value_type(s2, 2));

	MapT::iterator it = cont.find(s2);
	if (it != cont.end())
		std::cout << (*it).second << "\n";
	else
		std::cout << "ERROR: Not found!\n";
}

// -------------------------------------------------------------------------

void testPHashMapCStr()
{
	typedef NS_STDEXT::PHashMap<const char*, int, NS_STDEXT::HashCompare<const char*> > MapT;
	
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPool alloc(recycle, MapT::node_size());
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

void testPHashMapString()
{
	typedef NS_STDEXT::PHashMap<NS_STDEXT::String, int, NS_STDEXT::HashCompare<NS_STDEXT::String> > MapT;
	
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPool alloc(recycle, MapT::node_size());
	MapT cont(alloc);
	
	NS_STDEXT::String s1 = NS_STDEXT::g_str("Mon");
	NS_STDEXT::String s2 = NS_STDEXT::g_str("Tue");
	
	cont.insert(MapT::value_type(s1, 1));
	cont.insert(MapT::value_type(s2, 2));
	
	MapT::iterator it = cont.find(s2);
	if (it != cont.end())
		std::cout << (*it).second << "\n";
	else
		std::cout << "ERROR: Not found!\n";
}

// -------------------------------------------------------------------------

void testPHashSetCStr()
{
	typedef NS_STDEXT::PHashSet<const char*> MapT;
	
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPool alloc(recycle, MapT::node_size());
	MapT cont(alloc);
	
	cont.insert("Mon");
	cont.insert("Tue");
	
	const char key[] = "Mon";
	MapT::iterator it = cont.find(key);
	if (it != cont.end())
		std::cout << *it << "\n";
	else
		std::cout << "ERROR: Not found!\n";
}

void testPHashSetString()
{
	typedef NS_STDEXT::PHashSet<NS_STDEXT::String> MapT;
	
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPool alloc(recycle, MapT::node_size());
	MapT cont(alloc);
	
	NS_STDEXT::String s1 = NS_STDEXT::g_str("Mon");
	NS_STDEXT::String s2 = NS_STDEXT::g_str("Tue");
	
	cont.insert(s1);
	cont.insert(s2);
	
	MapT::iterator it = cont.find(s2);
	if (it != cont.end())
		std::cout << *it << "\n";
	else
		std::cout << "ERROR: Not found!\n";
}

// -------------------------------------------------------------------------

int main()
{
	std::cout << "----------------------------------\n";
	testHashMapCStr();
	std::cout << "----------------------------------\n";
	testHashMapString();
	std::cout << "----------------------------------\n";
	testPHashMapCStr();
	std::cout << "----------------------------------\n";
	testPHashMapString();
	std::cout << "----------------------------------\n";
	testPHashSetCStr();
	std::cout << "----------------------------------\n";
	testPHashSetString();
	std::cout << "----------------------------------\n";
	return 0;
}

// -------------------------------------------------------------------------

