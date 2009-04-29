
// Archive:
#include <stdext/Archive.h>

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

// -------------------------------------------------------------------------

template <class WriterT, class ReaderT, class StorageT>
inline void test(LogT& log, WriterT*, ReaderT*, StorageT& stg)
{
	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedAlloc alloc(recycle);

	{
		WriterT ar(stg);
		ar.put("hello\n");
		ar.put('\n');
		AssertExp(stg.size() == 7);
	}
	{
		char szBuf[100];
		ReaderT ar(stg);
		size_t cch = ar.get(szBuf, countof(szBuf));
		szBuf[cch] = '\0';
		AssertExp(strcmp(szBuf, "hello\n\n") == 0);
	}
	stg.erase(stg.begin(), stg.end());
	{
		WriterT ar;
		ar.open(stg);
		ar.put("you're welcome!\n");
	}
	{
		char szBuf[100];
		ReaderT ar(stg.begin(), stg.end());
		size_t cch = ar.get(szBuf, countof(szBuf));
		szBuf[cch] = '\0';
		AssertExp(strcmp(szBuf, "you're welcome!\n") == 0);
	}
	stg.erase(stg.begin(), stg.end());
	{
		char szBuf[100];
		WriterT ar(stg);
		ar.put(_itoa(13242, szBuf, 10));
		ar.put(' ');
		ar.put(_itoa(1111, szBuf, 10));
	}
	{
		ReaderT ar(stg);
		unsigned val;
		NS_STDEXT_IO_TEXT::scan_uint(ar, val);
		AssertExp(val == 13242);
		NS_STDEXT_IO_TEXT::scan_uint(ar, val, 2);
		AssertExp(val == 15);
	}
	stg.erase(stg.begin(), stg.end());
	{
		WriterT ar(stg);
		NS_STDEXT_IO_BINARY::puts(ar, "Hello");
		NS_STDEXT_IO_BINARY::puts(ar, std::string("World!"));
		NS_STDEXT_IO_BINARY::puts(ar, std::vector<char>(256, '!'));
		NS_STDEXT_IO_BINARY::puts(ar, std::vector<char>(65537, '?'));
	}
	{
		ReaderT ar(stg);
		std::string s1;
		AssertExp(NS_STDEXT_IO_BINARY::gets(ar, s1));
		AssertExp(s1 == "Hello");
		std::vector<char> s2;
		AssertExp(NS_STDEXT_IO_BINARY::gets(ar, s2));
		AssertExp(NS_STDEXT::compare(s2.begin(), s2.end(), "World!") == 0);
		NS_STDEXT::String s3;
		AssertExp(NS_STDEXT_IO_BINARY::gets(ar, alloc, s3));
		AssertExp(s3 == std::String(alloc, 256, '!'));
		NS_STDEXT::String s4;
		AssertExp(NS_STDEXT_IO_BINARY::gets(ar, alloc, s4));
		AssertExp(s4 == std::String(alloc, 65537, '?'));
	}
}

// -------------------------------------------------------------------------

template <class LogT>
class TestStdioArchive : public TestCase
{
	WINX_TEST_SUITE(TestStdioArchive);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		const char stg[] = "__teststdio__.txt";

		NS_STDEXT::StdioWriter* w = NULL;
		NS_STDEXT::StdioReader* r = NULL;
		test(log, w, r, stg);
		remove(stg);
	}
};

// -------------------------------------------------------------------------
// class TestPosixArchive

template <class LogT>
class TestPosixArchive : public TestCase
{
	WINX_TEST_SUITE(TestPosixArchive);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		const char stg[] = "__testposix__.txt";

		NS_STDEXT::PosixReader* w = NULL;
		NS_STDEXT::PosixWriter* r = NULL;
		test(log, w, r, stg);
		remove(stg);
	}
};

// -------------------------------------------------------------------------
// class TestMemArchive

template <class LogT>
class TestMemArchive : public TestCase
{
	WINX_TEST_SUITE(TestMemArchive);
		WINX_TEST(testVector);
		WINX_TEST(testStlString);
#ifdef STDEXT_DEQUE_H
		WINX_TEST(testDeque);
#endif
	WINX_TEST_SUITE_END();

#ifdef STDEXT_DEQUE_H
	void testDeque(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
		NS_STDEXT::CharDeque stg(alloc);
		NS_STDEXT::DequeWriter* w = NULL;
		NS_STDEXT::DequeReader* r = NULL;
		test(log, w, r, stg);
	}
#endif

	void testVector(LogT& log)
	{
		NS_STDEXT::CharVector stg;
		NS_STDEXT::VectorWriter* w = NULL;
		NS_STDEXT::VectorReader* r = NULL;
		test(log, w, r, stg);
	}

	void testStlString(LogT& log)
	{
		std::string stg;
		NS_STDEXT::StlStringWriter* w = NULL;
		NS_STDEXT::StlStringReader* r = NULL;
		test(log, w, r, stg);
	}
};

// -------------------------------------------------------------------------
// class TestStreamArchive

template <class LogT>
class TestStreamArchive : public TestCase
{
	WINX_TEST_SUITE(TestStreamArchive);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		const char stg[] = "/__teststream__.txt";

		NS_STDEXT::StreamWriter* w = NULL;
		NS_STDEXT::StreamReader* r = NULL;
		test(log, w, r, stg);
	}
};

// -------------------------------------------------------------------------
// class TestWinReg

#if defined(STD_UNITTEST)

#define _WINX_TEST_WINREG_KEY	WINX_TEXT("Software\\winx\\TestStdExt\\TestWinReg")

template <class LogT>
class TestWinReg : public TestCase
{
	WINX_TEST_SUITE(TestWinReg);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
		{
			NS_STDEXT::WinRegWriter ar(HKEY_CURRENT_USER, _WINX_TEST_WINREG_KEY);
			ar.wputs(L"Hello");
			ar.puts(std::string("World!"));
			ar.puts(std::vector<char>(256, '!'));
			ar.puts(std::vector<char>(65537, '?'));
		}
		{
			NS_STDEXT::WinRegReader ar(HKEY_CURRENT_USER, _WINX_TEST_WINREG_KEY);
			std::string s1;
			AssertExp(ar.gets(s1));
			AssertExp(s1 == "Hello");
			std::vector<WCHAR> s2;
			AssertExp(ar.wgets(s2));
			AssertExp(NS_STDEXT::compare(s2.begin(), s2.end(), L"World!") == 0);
			NS_STDEXT::String s3;
			AssertExp(ar.gets(alloc, s3));
			AssertExp(s3 == NS_STDEXT::String(alloc, 256, '!'));
			NS_STDEXT::String s4;
			AssertExp(ar.gets(alloc, s4));
			AssertExp(s4 == NS_STDEXT::String(alloc, 65537, '?'));
			NS_STDEXT::String s5;
			AssertExp(!ar.gets(alloc, s5));
		}
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// class TestRecord

#if defined(STD_UNITTEST)

template <class LogT>
class TestRecord : public TestCase
{
	WINX_TEST_SUITE(TestRecord);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		typedef NS_STDEXT::RecordWriter<NS_STDEXT::VectorWriter> RecordWriterT;
		typedef NS_STDEXT::RecordReader<NS_STDEXT::VectorReadArchive> RecordReaderT;

		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
	
		NS_STDEXT::CharVector stg;
		{
			RecordWriterT ar(stg);
			
			ar.beginRecord(23);
			NS_STDEXT_IO_BINARY::puts(ar, "Hello, world!");
			ar.put("You're welcome!\n");
			ar.endRecord();

			ar.beginRecord(99);
			NS_STDEXT_IO_BINARY::put16i(ar, 32);
			ar.put("123");
			ar.endRecord();
		}
		{
			RecordReaderT reader(stg);
			RecordReaderT::record_info info;

			AssertExp(reader.next(info));
			AssertExp(info.recId == 23);
			NS_STDEXT::MemReader ar1(info);
			NS_STDEXT::String s;
			AssertExp(NS_STDEXT_IO_BINARY::gets(ar1, alloc, s));
			AssertExp(s == "Hello, world!");
			ar1.getline(alloc, s);
			AssertExp(s == "You're welcome!");
			AssertExp(ar1.get() == std::MemReader::endch);

			AssertExp(reader.next(info));
			AssertExp(info.recId == 99);
			NS_STDEXT::MemReader ar2(info);
			UINT16 w;
			AssertExp(NS_STDEXT_IO_BINARY::get16i(ar2, w));
			AssertExp(w == 32);
			AssertExp(NS_STDEXT_IO_TEXT::get_uint(ar2) == 123);
			AssertExp(ar2.get() == std::MemReader::endch);

			AssertExp(!reader.next(info));
		}
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif
