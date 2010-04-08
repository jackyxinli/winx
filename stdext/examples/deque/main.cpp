#include <stdext/Deque.h>
#include <iostream>

// -------------------------------------------------------------------------

void testSimple()
{
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPools alloc(recycle);

	typedef NS_STDEXT::Deque<int> IntQ;

	{
		IntQ q(alloc);
		q.push_back(1);
		q.push_back(2);
	
		while (!q.empty())
		{
			std::cout << q.front() << '\n';
			q.pop_front();
		}
	}
	{
		const int aaa[] = { 3, 4, 5 };
		IntQ q(alloc, aaa, aaa + countof(aaa));
	
		while (!q.empty())
		{
			std::cout << q.front() << '\n';
			q.pop_front();
		}
	}
}

// -------------------------------------------------------------------------

void testUserAllocator()
{
	typedef NS_STDEXT::Deque<int, NS_STDEXT::ScopedAlloc> DequeT;

	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedAlloc alloc(recycle);
	DequeT q(alloc);
	for (int i = 0; i < 5; ++i)
		q.push_back(i);
	while (!q.empty())
	{
		std::cout << q.front() << '\n';
		q.pop_front();
	}
}

// -------------------------------------------------------------------------

int main()
{
	testSimple();
	std::cout << "------------------------------------------------\n";
	testUserAllocator();
	return 0;
}

// -------------------------------------------------------------------------

