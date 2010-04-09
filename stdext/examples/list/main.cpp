#include <stdext/List.h>
#include <iostream>

// -------------------------------------------------------------------------

class Obj
{
private:
	int m_val;
public:
	Obj(int arg = 0) {
		m_val = arg;
	}
	int get() {
		return m_val;
	}
};

void testList()
{
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPools alloc(recycle);
	NS_STDEXT::List<Obj> coll(alloc);
	coll.push_back(1);
	coll.push_back(2);
	coll.push_back(4);
	while (!coll.empty())
	{
		std::cout << coll.back().get() << '\n';
		coll.pop_back();
	}
}

void testSlist()
{
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPools alloc(recycle);
	NS_STDEXT::Slist<int> coll(alloc);
	coll.push_front(1);
	coll.push_front(2);
	coll.push_front(4);
	for (NS_STDEXT::Slist<int>::const_iterator it = coll.begin(); it != coll.end(); ++it)
	{
		std::cout << *it << '\n';
	}
}

// -------------------------------------------------------------------------

int main()
{
	testList();
	std::cout << "------------------------------------------------\n";
	testSlist();
	return 0;
}

// -------------------------------------------------------------------------

