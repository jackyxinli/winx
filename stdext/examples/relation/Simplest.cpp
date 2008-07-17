#include <iostream>
#include <stdext/Relation.h>

// -------------------------------------------------------------------------

void testBasic()
{
	typedef std::AutoFreeAlloc AllocT;
	typedef std::pair<std::string, int> TupleT;
	typedef std::Relation<TupleT, std::HashMapIndexing, AllocT> RelationT;
	typedef RelationT::Indexing<0> Indexing0;
	typedef RelationT::Indexing<1> Indexing1;

	AllocT alloc;
	RelationT rel(alloc);
	rel.indexing<0>();
	rel.indexing<1>();
	
	rel.insert(TupleT("Mon", 1));
	rel.insert(TupleT("Monday", 1));
	rel.insert(TupleT("Tue", 2));
	rel.insert(TupleT("Wed", 3));
	rel.insert(TupleT("Wednesday", 3));
	std::cout << "total count = " << rel.size() << "\n";
	
	Indexing1::range rg = rel.select<1>(3);
	size_t n = std::distance(rg.first, rg.second);
	std::cout << "select<1>(3) count = " << n << "\n";
	
	for (Indexing1::iterator it = rg.first; it != rg.second; ++it) {
		const TupleT& i = Indexing1::item(it);
		std::cout << "\t" << i.first << "\t" << i.second << "\n";
	}

	Indexing0::range rg2 = rel.select<0>("Mon");
	Indexing0::iterator it2 = rg2.first;
	std::cout << "select<0>(Mon) count = " << std::distance(rg2.first, rg2.second) << "\n";
	const TupleT& i2 = Indexing0::item(it2);
	std::cout << "\t" << i2.first << "\t" << i2.second << "\n";

	std::cout << "count<1>(1) = " << rel.count<1>(1) << "\n";
	std::cout << "erase<1>(1) = " << rel.erase<1>(1) << "\n";
	std::cout << "count<1>(1) = " << rel.count<1>(1) << "\n";
	std::cout << "total count = " << rel.size() << "\n";

	Indexing0::range rg3 = rel.select<0>("Mon");
	std::cout << "select<0>(Mon) count = " << std::distance(rg3.first, rg3.second) << "\n";
	std::cout << "count<0>(Monday) = " << rel.count<0>("Monday") << "\n";
}

int main()
{
	std::cout << "----------------------------------\n";
	testBasic();
	std::cout << "----------------------------------\n";
	return 0;
}

// -------------------------------------------------------------------------

