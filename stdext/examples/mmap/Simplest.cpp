#include <stdext/MMap.h>

using namespace NS_STDEXT;

// -------------------------------------------------------------------------

template <class LogT>
void testAccessBuffer(LogT& log)
{
	char* buf;
	AccessBuffer<MMapRW> ab;

	ab.open(WINX_TEXT("__AccessBuffer__.txt"));

	buf = ab.view(0, 32);
	strcpy(buf, "hello, xushiwei!");

	buf = ab.view(MMapRW::AllocationGranularity-5, 32);
	strcpy(buf, "hello, xushiwei!");

	ViewBuffer<MMapRW::Handle> vb(ab);
	char* buf2 = vb.view(MMapRW::AllocationGranularity-5, 32);

	log.print(buf2).newline();
	AssertExp(strcmp(buf2, "hello, xushiwei!") == 0);
}

template <class LogT>
void testAccessBuffer2(LogT& log)
{
	MMapRW fm(WINX_TEXT("__AccessBuffer2__.txt"));

	AccessBuffer<MMapRW::Handle> ab(fm);
	char* buf = ab.view(0, 32);
	strcpy(buf, "hello, xushiwei!");

	ViewBuffer<MMapRW::Handle> vb(fm);
	char* buf2 = vb.view(0, 32);

	buf = ab.view(MMapRW::AllocationGranularity-5, 32);
	strcpy(buf, "hello, abcdefg!");

	log.print(buf2).newline();
	AssertExp(strcmp(buf2, "hello, xushiwei!") == 0);
}

template <class LogT>
void testSegmentAccessBuffer(LogT& log)
{
	char* buf;
	SegmentAccessBuffer<MMapRW> sab;

	sab.open(WINX_TEXT("__SegmentAccessBuffer1__.txt"));

	buf = sab.view(0);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(32);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(64);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(MMapRW::AllocationGranularity);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testSegmentAccessBuffer2(LogT& log)
{
	char* buf;
	MMapRW fm(WINX_TEXT("__SegmentAccessBuffer2__.txt"));;
	SegmentAccessBuffer<MMapRW::Handle> sab(fm);

	buf = sab.view(0);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(32);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(64);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(MMapRW::AllocationGranularity);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testSegmentAllocBuffer(LogT& log)
{
	char* buf;
	SegmentAllocBuffer<MMapRW> sab;
	SegmentAllocBuffer<MMapRW>::pos_type fc;

	sab.open(WINX_TEXT("__AllocBuffer1__.txt"));

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(MMapRW::AllocationGranularity, fc);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testSegmentAllocBuffer2(LogT& log)
{
	char* buf;
	MMapRW fm(WINX_TEXT("__AllocBuffer2__.txt"));
	SegmentAllocBuffer<MMapRW::Handle> sab(fm);
	MMapRW::pos_type fc;

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(32, fc);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.allocData(MMapRW::AllocationGranularity, fc);
	strcpy(buf, "hello, xushiwei!");
}

template <class LogT>
void testAccessAndAlloc(LogT& log)
{
	::remove("__AccessAndAlloc__.txt");

	char* buf;
	MMapRW fm(WINX_TEXT("__AccessAndAlloc__.txt"));
	SegmentAccessBuffer<MMapRW::Handle> sab(fm);
	SegmentAllocBuffer<MMapRW::Handle> sa(fm);

	buf = sab.view(0);
	log.trace(buf);

	strcpy(buf, "hello, xushiwei!\n");

	buf = sab.view(32);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(64);
	strcpy(buf, "hello, xushiwei!");

	buf = sab.view(MMapRW::AllocationGranularity);
	strcpy(buf, "hello, xushiwei!");

	SegmentAllocInfo info = { 0, MMapRW::AllocationGranularity };
	sa.initBuffer(info);

	MMapRW::pos_type fc;
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

