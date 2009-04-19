#if defined(_MSC_VER)
#pragma warning(disable: 4819)
#endif

#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <stdext/Relation.h>
#include <stdext/Rand.h>

// -------------------------------------------------------------------------

void testBasic()
{
	typedef NS_STDEXT::AutoAlloc AllocT;
	typedef std::pair<std::string, int> TupleT;
	typedef NS_STDEXT::Relation<TupleT, 3, 0, NS_STDEXT::HashMapIndexing, AllocT> RelationT;
	typedef RelationT::Indexing<0> Indexing0;
	typedef RelationT::Indexing<1> Indexing1;

	AllocT alloc;
	RelationT rel(alloc);
	
	rel.insert(TupleT("Mon", 1));
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

// -------------------------------------------------------------------------

void testPrimaryKey()
{
	typedef NS_STDEXT::AutoAlloc AllocT;
	typedef std::pair<std::string, int> TupleT;
	typedef NS_STDEXT::Relation<TupleT, 3, 3, NS_STDEXT::HashMapIndexing, AllocT> RelationT;
	typedef RelationT::Indexing<0> Indexing0;
	typedef RelationT::Indexing<1> Indexing1;

	AllocT alloc;
	RelationT rel(alloc);
	
	rel.insert(TupleT("Mon", 1));
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

// -------------------------------------------------------------------------
// testRelationDefragment

void testRelationDefragment()
{
	typedef NS_STDEXT::AutoAlloc AllocT;
	typedef std::pair<std::string, int> TupleT;
	typedef NS_STDEXT::Relation<TupleT, 3, 0, NS_STDEXT::HashMapIndexing, AllocT> RelationT;
	typedef NS_STDEXT::Defragment<RelationT> RelationT2;
	typedef RelationT::Indexing<0> Indexing0;
	typedef RelationT::Indexing<1> Indexing1;

	RelationT2 rel;
	
	enum { Count = 20000 };
	enum { TestN = 10000 };

	int i, j;
	char s1[32];
	std::Rand rnd;

	for (i = 0; i < TestN; ++i)
	{
		for (j = 0; j < Count; ++j)
		{
			rnd.srand(s1, 6, countof(s1), 'A', 'z');
			rel.insert(TupleT(s1, j));
		}
		std::cout << i << ": Insert done!" << std::endl;
		for (j = 0; j < Count; ++j)
		{
			rel.erase<1>(j);
		}
		if (rel.size() != 0) {
			std::cerr << "Unexcepted Error: why?\n";
			break;
		}
		std::cout << i << ": Delete done!" << std::endl;
		rel.defrag();
		std::cout << i << ": Defrag done!" << std::endl;
	}
}

// -------------------------------------------------------------------------
// testCustomIndexing

// Field 0: 		rb-tree index
// Field 1..(N-1):  hash-table index

template <int Field, class KeyT, class AllocT>
struct CustomIndexing : public NS_STDEXT::HashMapIndexing<Field, KeyT, AllocT> {
};

template <class KeyT, class AllocT>
struct CustomIndexing<0, KeyT, AllocT> : public NS_STDEXT::MapIndexing<0, KeyT, AllocT> {
};

void testCustomIndexing()
{
	typedef NS_STDEXT::AutoAlloc AllocT;
	typedef std::pair<std::string, int> TupleT;
	typedef NS_STDEXT::Relation<TupleT, 3, 0, CustomIndexing, AllocT> RelationT;
	typedef RelationT::Indexing<0> Indexing0;
	typedef RelationT::Indexing<1> Indexing1;

	AllocT alloc;
	RelationT rel(alloc);
	
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

// -------------------------------------------------------------------------
// testTupleRelation

void testTupleRelation()
{
	typedef NS_STDEXT::AutoAlloc AllocT;
	typedef boost::tuple<std::string, int, char> TupleT;
	typedef NS_STDEXT::Relation<TupleT, 7, 0, CustomIndexing, AllocT> RelationT;
	typedef RelationT::Indexing<0> Indexing0;
	typedef RelationT::Indexing<1> Indexing1;
	typedef RelationT::Indexing<2> Indexing2;

	AllocT alloc;
	RelationT rel(alloc);
	
	rel.insert(TupleT("Mon", 1, 'M'));
	rel.insert(TupleT("Monday", 1, 'M'));
	rel.insert(TupleT("Tue", 2, 'T'));
	rel.insert(TupleT("Wed", 3, 'W'));
	rel.insert(TupleT("Wednesday", 3, 'W'));
	rel.insert(TupleT("Thu", 4, 'T'));
	std::cout << "total count = " << rel.size() << "\n";
	
	Indexing1::range rg = rel.select<1>(3);
	size_t n = std::distance(rg.first, rg.second);
	std::cout << "select<1>(3) count = " << n << "\n";
	
	for (Indexing1::iterator it = rg.first; it != rg.second; ++it) {
		std::cout << "\t" << Indexing1::item(it) << "\n";
	}

	Indexing0::range rg2 = rel.select<0>("Mon");
	Indexing0::iterator it2 = rg2.first;
	std::cout << "select<0>(Mon) count = " << std::distance(rg2.first, rg2.second) << "\n";
	std::cout << "\t" << Indexing0::item(it2) << "\n";

	std::cout << "count<1>(1) = " << rel.count<1>(1) << "\n";
	std::cout << "erase<1>(1) = " << rel.erase<1>(1) << "\n";
	std::cout << "count<1>(1) = " << rel.count<1>(1) << "\n";
	std::cout << "total count = " << rel.size() << "\n";

	Indexing2::range rg3 = rel.select<2>('T');
	std::cout << "select<2>('T') count = " << std::distance(rg3.first, rg3.second) << "\n";

	for (Indexing2::iterator it3 = rg3.first; it3 != rg3.second; ++it3) {
		const TupleT& i = Indexing2::item(it3);
		std::cout << "\t" << boost::tuples::get<0>(i)
			<< "\t" << boost::tuples::get<1>(i)
			<< "\t" << boost::tuples::get<2>(i) << "\n";
	}
}

// -------------------------------------------------------------------------

int main()
{
	std::cout << "----------------------------------\n";
	testBasic();
	std::cout << "----------------------------------\n";
	testPrimaryKey();
	std::cout << "----------------------------------\n";
	testCustomIndexing();
	std::cout << "----------------------------------\n";
	testTupleRelation();
	std::cout << "----------------------------------\n";
	testRelationDefragment();
	std::cout << "----------------------------------\n";
	return 0;
}

// -------------------------------------------------------------------------

