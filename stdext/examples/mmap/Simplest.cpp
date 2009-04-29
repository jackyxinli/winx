#include <stdext/MMap.h>

using namespace NS_STDEXT;

// -------------------------------------------------------------------------

template <class LogT>
void testAccessBuffer(LogT& log)
{
	char* buf;
	AccessBuffer<FileMappingRW> ab;

	ab.open(WINX_TEXT("__AccessBuffer__.txt"));

	buf = ab.view(0, 32);
	strcpy(buf, "hello, xushiwei!");

	buf = ab.view(FileMappingRW::AllocationGranularity-5, 32);
	strcpy(buf, "hello, xushiwei!");

	ViewBuffer<FileMappingRW::Handle> vb(ab);
	char* buf2 = vb.view(FileMappingRW::AllocationGranularity-5, 32);

	log.print(buf2).newline();
	AssertExp(strcmp(buf2, "hello, xushiwei!") == 0);
}

template <class LogT>
void testAccessBuffer2(LogT& log)
{
	FileMappingRW fm(WINX_TEXT("__AccessBuffer2__.txt"));

	AccessBuffer<FileMappingRW::Handle> ab(fm);
	char* buf = ab.view(0, 32);
	strcpy(buf, "hello, xushiwei!");

	ViewBuffer<FileMappingRW::Handle> vb(fm);
	char* buf2 = vb.view(0, 32);

	buf = ab.view(FileMappingRW::AllocationGranularity-5, 32);
	strcpy(buf, "hello, abcdefg!");

	log.print(buf2).newline();
	AssertExp(strcmp(buf2, "hello, xushiwei!") == 0);
}

template <class LogT>
void testSegmentAccessBuffer(LogT& log)
{
	char* buf;
	SegmentAccessBuffer<FileMappingRW> sab;

	sab.open(WINX_TEXT("__SegmentAccessBuffer1__.txt"));

	buf = sab.view(0);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(32);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(64);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(FileMappingRW::AllocationGranularity);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testSegmentAccessBuffer2(LogT& log)
{
	char* buf;
	FileMappingRW fm(WINX_TEXT("__SegmentAccessBuffer2__.txt"));;
	SegmentAccessBuffer<FileMappingRW::Handle> sab(fm);

	buf = sab.view(0);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(32);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(64);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(FileMappingRW::AllocationGranularity);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testSegmentAllocBuffer(LogT& log)
{
	char* buf;
	SegmentAllocBuffer<FileMappingRW> sab;
	SegmentAllocBuffer<FileMappingRW>::pos_type fc;

	sab.open(WINX_TEXT("__AllocBuffer1__.txt"));

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(FileMappingRW::AllocationGranularity, fc);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testSegmentAllocBuffer2(LogT& log)
{
	char* buf;
	FileMappingRW fm(WINX_TEXT("__AllocBuffer2__.txt"));
	SegmentAllocBuffer<FileMappingRW::Handle> sab(fm);
	FileMappingRW::pos_type fc;

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(FileMappingRW::AllocationGranularity, fc);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testAccessAndAlloc(LogT& log)
{
	::remove("__AccessAndAlloc__.txt");

	char* buf;
	FileMappingRW fm(WINX_TEXT("__AccessAndAlloc__.txt"));
	SegmentAccessBuffer<FileMappingRW::Handle> sab(fm);
	SegmentAllocBuffer<FileMappingRW::Handle> sa(fm);

	buf = sab.view(0);
	log.trace(buf);

	strcpy(buf, "hello, xushiwei!\n");

	buf = sab.view(32);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(64);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(FileMappingRW::AllocationGranularity);
	strcpy(buf, "hello, xushiwei!");

	SegmentAllocInfo info = { 0, FileMappingRW::AllocationGranularity };
	sa.initBuffer(info);

	FileMappingRW::pos_type fc;
	buf = sa.allocData(32, fc);
	strcpy(buf, "1234\n");
	
	buf = sab.view(fc);
	log.trace(buf);
	AssertExp(strcmp(buf, "1234\n") == 0);
}

// -------------------------------------------------------------------------

int main()
{
	NS_STDEXT::OutputLog log;
	testAccessBuffer(log);
	testAccessBuffer2(log);
	testSegmentAccessBuffer(log);
	testSegmentAccessBuffer2(log);
	testAccessAndAlloc(log);
	return 0;
}

// -------------------------------------------------------------------------

