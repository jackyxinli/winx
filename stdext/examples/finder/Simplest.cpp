#include <stdext/Search.h>
#include <stdext/Archive.h>
#include <iostream>
#include <fstream>
#include <deque>

// -------------------------------------------------------------------------

template <class FinderT>
struct Examples
{
	static void searchInArchive()
	{
		std::string line;
		NS_STDEXT::StdioReader ar(__FILE__);

		FinderT finder("StdioReader");
		HRESULT hr = finder.next(ar);

		if (hr == S_OK) {
			ar.getline(line);
			printf("line = '%s'\n", line.c_str());
		}
	}

	static void searchInFStream()
	{
		std::string line;
		std::ifstream is(__FILE__);
		
		FinderT finder("std::ifstream");
		HRESULT hr = finder.istreamNext(is);

		if (hr == S_OK) {
			std::getline(is, line);
			printf("line = '%s'\n", line.c_str());
		}
	}

	static void searchInCStr()
	{
		const char* p;
		const char dest[] = "1234ababcde";
		
		FinderT finder("abc");
		HRESULT hr = finder.cstrNext(dest, &p);

		if (hr == S_OK) {
			printf("line = '%s'\n", p);
		}
	}

	static void searchInDeque()
	{
		typedef std::deque<char> Container;

		const char destBuf[] = "1234ababcde";
		Container::iterator itFind;
		Container dest(sizeof(destBuf));
		std::copy(destBuf, destBuf+sizeof(destBuf), dest.begin());

		FinderT finder("abc");
		HRESULT hr = finder.iteratorNext(dest.begin(), dest.end(), &itFind);
		if (hr == S_OK) {
			char buf[16];
			*std::copy(itFind, dest.end(), buf) = '\0';
			printf("line = '%s'\n", buf);
		}
	}
	
	static void all()
	{
		searchInArchive();
		searchInFStream();
		searchInCStr();
		searchInDeque();
	}
};

// -------------------------------------------------------------------------

int main()
{
	std::cout << "----------------------------------\n";
	typedef NS_KMP::Finder<char> KmpFinder;
	Examples<KmpFinder>::all();
	std::cout << "----------------------------------\n";
	typedef NS_SHIFTOR::Finder<char, size_t> FastShiftOrFinder;
	Examples<FastShiftOrFinder>::all();
	std::cout << "----------------------------------\n";
	typedef NS_SHIFTOR::Finder<char> ShiftOrFinder;
	Examples<ShiftOrFinder>::all();
	std::cout << "----------------------------------\n";
	return 0;
}

// -------------------------------------------------------------------------
