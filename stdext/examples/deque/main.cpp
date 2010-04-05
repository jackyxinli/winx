#include <stdext/Deque.h>
#include <iostream>

// -------------------------------------------------------------------------

int main()
{
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedPools alloc(recycle);

	typedef NS_STDEXT::PDeque<int> IntQ;

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
	return 0;
}

// -------------------------------------------------------------------------

