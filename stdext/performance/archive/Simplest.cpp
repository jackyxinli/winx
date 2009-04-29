#include <stdext/Archive.h>
#include <stdext/Counter.h>

// -------------------------------------------------------------------------

void testReadArchive(const char* path)
{
	enum { N = 8 };
	
	using namespace NS_STDEXT;
	
	typedef NS_STDEXT::MMapReadCache<char> ReadCache;
	typedef NS_STDEXT::PosixReadArchive ArchiveT2;
	
	int nLine;
	OutputLog log;
	Accumulator acc;
	
	log.print("================ MMapReadArchive ===========\n");
	acc.start();
	for (int i = 0; i < N; ++i)
	{
		PerformanceCounter counter;
		
		ReadCache file(path);
		ReadCache::archive_type ar(file);
		nLine = 0;
		std::string str;
		while (ar.getline(str))
			++nLine;
			
		acc.accumulate(counter.trace(log));
	}
	log.print("Total lines: ").print(nLine).newline();
	acc.trace_avg(log);
	
	log.print("================ PosixReadArchive ===========\n");
	acc.start();
	for (int i = 0; i < N; ++i)
	{
		PerformanceCounter counter;

		ArchiveT2 ar(path);
		nLine = 0;
		std::string str;
		while (ar.getline(str))
			++nLine;
			
		acc.accumulate(counter.trace(log));
	}
	log.print("Total lines: ").print(nLine).newline();
	acc.trace_avg(log);
}

// -------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
	testReadArchive(argc == 2 ? argv[1] : __FILE__);
	return 0;
}

// -------------------------------------------------------------------------

