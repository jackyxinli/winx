/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/filemapping/FileMappingImpl.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_FILEMAPPING_FILEMAPPINGIMPL_H__
#define __STDEXT_FILEMAPPING_FILEMAPPINGIMPL_H__

#ifndef __STDEXT_FILEMAPPING_SIMPLEFILEMAPPING_H__
#include "SimpleFileMapping.h"
#endif

#pragma pack(1)

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class SegmentAllocBufferT - as a simplest allocator
// ***NOTE*** SegmentSize = (1<<SegBits) is maximum bytes that user can allocate.

struct SegmentAllocInfo
{
	DWORD m_iBasePage; // index number of current block for allocating
	DWORD m_cbFree; // free bytes of current block
};

template <class Base, int SegBits = 16>
class SegmentAllocBufferT : public Base, private SegmentAllocInfo
{
private:
	char* m_pView;

	SegmentAllocBufferT(const SegmentAllocBufferT&);
	void operator=(const SegmentAllocBufferT&);

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::pos_argtype pos_argtype;

public:
	enum { AllocationGranularityBits = Base::AllocationGranularityBits };
	enum { SegmentBits = MAX(SegBits, AllocationGranularityBits) };
	enum { SegmentSize = 1 << SegmentBits };
	enum { SegmentSizeMask = SegmentSize-1 };

private:
	enum { _AlignBits = SegmentBits - AllocationGranularityBits };
	enum { _AlignSize = 1 << _AlignBits };

public:
	SegmentAllocBufferT() : m_pView(NULL) { m_cbFree = 0; }
	~SegmentAllocBufferT() {
		if (m_pView)
			UnmapViewOfFile(m_pView);
	}

	void winx_call close()
	{
		if (m_pView)
		{
			UnmapViewOfFile(m_pView);
			m_pView = NULL;
			m_cbFree = 0;
			Base::close();
		}
	}

	void winx_call flush()
	{
		if (m_pView)
			WINX_VERIFY(FlushViewOfFile(m_pView, SegmentSize));
	}

	HRESULT winx_call initBuffer(DWORD iBasePage = 0)
	{
		WINX_ASSERT(m_pView == NULL && m_cbFree == 0);

		m_pView = Base::accessSegment(iBasePage, _AlignSize);
		if (m_pView != NULL)
		{
			m_cbFree = SegmentSize;
			m_iBasePage = iBasePage;
			return S_OK;
		}
		return E_ACCESSDENIED;
	}

	HRESULT winx_call initBuffer(const SegmentAllocInfo& info)
	{
		WINX_ASSERT(m_pView == NULL && m_cbFree == 0);

		m_pView = Base::accessSegment(info.m_iBasePage, _AlignSize);
		if (m_pView != NULL)
		{
			*static_cast<SegmentAllocInfo*>(this) = info;
			return S_OK;
		}
		return E_ACCESSDENIED;
	}

	void winx_call getAllocInfo(SegmentAllocInfo& info) const
	{
		info = *static_cast<const SegmentAllocInfo*>(this);
	}

	char* winx_call allocData(size_type cbSize, pos_type& fcPos)
	{
		WINX_ASSERT(SegmentBits >= AllocationGranularityBits);
		WINX_ASSERT(cbSize <= SegmentSize);

		if (cbSize <= m_cbFree)
		{
Proc:		char* pData = m_pView + (SegmentSize - m_cbFree);
			fcPos = ((pos_type)m_iBasePage << AllocationGranularityBits) + (SegmentSize - m_cbFree);
			m_cbFree -= cbSize;
			return pData;
		}
		if (cbSize <= SegmentSize)
		{
			if (m_pView)
				UnmapViewOfFile(m_pView);

			m_pView = Base::allocSegment(_AlignSize, m_iBasePage);
			WINX_ASSERT(m_pView);
			
			if (m_pView != NULL) {
				m_cbFree = SegmentSize;
				goto Proc;
			}
			m_cbFree = 0;
		}
		return NULL;
	}
};

// -------------------------------------------------------------------------
// class SegmentAccessBufferT - as the simplest way to accessing memory
// ***NOTE*** 
//	SegmentSize = (1<<SegBits) is maximum bytes that user can access.
//	And user can't access memory across	the boundary of Segments.

template <class Base, int SegBits = 16, BOOL ReadOnly = FALSE>
class SegmentAccessBufferT : public Base
{
private:
	char* m_pView;
	DWORD m_iSeg;

	SegmentAccessBufferT(const SegmentAccessBufferT&);
	void operator=(const SegmentAccessBufferT&);

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::pos_argtype pos_argtype;

public:
	enum { AllocationGranularityBits = Base::AllocationGranularityBits };
	enum { SegmentBits = MAX(SegBits, AllocationGranularityBits) };
	enum { SegmentSize = 1 << SegmentBits };
	enum { SegmentSizeMask = SegmentSize-1 };

private:
	enum { _AlignBits = SegmentBits - AllocationGranularityBits };
	enum { _AlignSize = 1 << _AlignBits };

public:
	SegmentAccessBufferT() : m_pView(NULL), m_iSeg((DWORD)-1) {}
	~SegmentAccessBufferT() {
		if (m_pView)
			UnmapViewOfFile(m_pView);
	}

	void winx_call close()
	{
		if (m_pView)
		{
			UnmapViewOfFile(m_pView);
			m_pView = NULL;
			m_iSeg = ((DWORD)-1);
			Base::close();
		}
	}

	void winx_call flush()
	{
		if (m_pView)
			WINX_VERIFY(FlushViewOfFile(m_pView, SegmentSize));
	}

	char* winx_call view(pos_argtype fc)
	{
		DWORD iSeg = (DWORD)(fc >> SegmentBits);
		if (iSeg == m_iSeg)
		{
			return m_pView + (SegmentSizeMask & (size_type)fc);
		}
		else
		{
			if (m_pView)
				UnmapViewOfFile(m_pView);

			if (ReadOnly)
				m_pView = Base::viewSegment(iSeg << _AlignBits, _AlignSize);
			else
				m_pView = Base::accessSegment(iSeg << _AlignBits, _AlignSize);

			WINX_ASSERT(m_pView);

			if (m_pView != NULL) {
				m_iSeg = iSeg;
				return m_pView + (SegmentSizeMask & (size_type)fc);
			}
			else {
				m_iSeg = (DWORD)-1;
				return NULL;
			}
		}
	}
};

// -------------------------------------------------------------------------
// class AccessBufferT - as a general way to accessing memory

template <class Base, BOOL ReadOnly = FALSE>
class AccessBufferT : public Base
{
private:
	char* m_pView;
	DWORD m_nBasePage;
	DWORD m_nNextPage;

	AccessBufferT(const AccessBufferT&);
	void operator=(const AccessBufferT&);

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::pos_argtype pos_argtype;

public:
	enum { AllocationGranularity = Base::AllocationGranularity };
	enum { AllocationGranularityBits = Base::AllocationGranularityBits };
	enum { AllocationGranularityMask = Base::AllocationGranularityMask };

public:
	AccessBufferT() : m_pView(NULL), m_nBasePage(LONG_MAX), m_nNextPage(0) {}
	~AccessBufferT()
	{
		if (m_pView)
			UnmapViewOfFile(m_pView);
	}
	
	void winx_call close()
	{
		if (m_pView)
		{
			WINX_VERIFY(UnmapViewOfFile(m_pView));
			m_pView = NULL;
			m_nBasePage = LONG_MAX;
			m_nNextPage = 0;
			Base::close();
		}
	}

	void winx_call flush()
	{
		if (m_pView)
		{
			WINX_VERIFY(
				FlushViewOfFile(
					m_pView,
					(m_nNextPage - m_nBasePage) << AllocationGranularityBits
					)
				);
		}
	}

	char* winx_call view(pos_argtype offset, size_type count)
	{
		DWORD nBasePage = (DWORD)(
			offset >> AllocationGranularityBits);
		
		DWORD nNextPage = (DWORD)(
			(offset + count + AllocationGranularityMask) >> AllocationGranularityBits);

		WINX_ASSERT(count > 0);
		WINX_ASSERT(nBasePage < nNextPage);

		if (m_nNextPage < nNextPage || m_nBasePage > nBasePage)
		{
			if (m_pView)
				UnmapViewOfFile(m_pView);

			if (ReadOnly)
				m_pView = Base::viewSegment(nBasePage, nNextPage - nBasePage);
			else
				m_pView = Base::accessSegment(nBasePage, nNextPage - nBasePage);

			WINX_ASSERT(m_pView);

			if (m_pView == NULL)
				return NULL;

			m_nBasePage = nBasePage;
			m_nNextPage = nNextPage;
		}

		UINT32 offsetInPage = (UINT32)
			(offset - ((pos_type)m_nBasePage << AllocationGranularityBits));

		return m_pView + offsetInPage;
	}
};

// -------------------------------------------------------------------------
// class FileMappingT

template <class Config, class PosT = UINT32>
class FileMappingT
{
private:
	HANDLE m_hFile;
	HANDLE m_hFileMapping;
	DWORD m_nTotalPage;

	FileMappingT(const FileMappingT&);
	void operator=(const FileMappingT&);

private:
	enum { _ShareMode = Config::FileShareMode };
	enum { _CreationDisposition = Config::FileCreationDisposition };
	enum { _FlagsAndAttributes = Config::FileFlagsAndAttributes };

public:
	enum { _nAGBits = 16 };
	enum { _AllocationGranularityInvBits = sizeof(DWORD)*8 - _nAGBits };

public:
	enum { AllocationGranularityBits = _nAGBits };
	enum { AllocationGranularity = (1 << _nAGBits) };
	enum { AllocationGranularityMask = (AllocationGranularity - 1) };

public:
	typedef UINT size_type;
	typedef PosT pos_type;
	typedef PosT pos_argtype;

public:
	FileMappingT() : m_hFile(INVALID_HANDLE_VALUE) {}
	FileMappingT(
		LPCTSTR szFile,
		DWORD dwShareMode = _ShareMode,
		DWORD dwCreationDisposition = _CreationDisposition,
		DWORD dwFlagsAndAttributes = _FlagsAndAttributes)
	{
		m_hFile = INVALID_HANDLE_VALUE;
		open(szFile, dwShareMode, dwCreationDisposition, dwFlagsAndAttributes);
	}
	~FileMappingT()
	{
		close();
	}

	DWORD winx_call getTotalPages() const
	{
		return m_nTotalPage;
	}

	int winx_call good() const
	{
		return m_hFile != INVALID_HANDLE_VALUE;
	}

	void winx_call close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			if (m_hFileMapping) {
				WINX_VERIFY(CloseHandle(m_hFileMapping));
				m_hFileMapping = NULL;
			}
			WINX_VERIFY(CloseHandle(m_hFile));
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	HRESULT winx_call reopen(DWORD nInitialTotalPage)
	{
		WINX_ASSERT(nInitialTotalPage);

		if (m_hFileMapping)
			WINX_VERIFY(CloseHandle(m_hFileMapping));

		m_nTotalPage = nInitialTotalPage;
		m_hFileMapping = CreateFileMapping(
			m_hFile, NULL, Config::PageProtect,
			m_nTotalPage >> _AllocationGranularityInvBits,
			m_nTotalPage << AllocationGranularityBits,
			NULL);

		return m_hFileMapping ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open(
		LPCTSTR szFile,
		DWORD dwShareMode = _ShareMode,
		DWORD dwCreationDisposition = _CreationDisposition,
		DWORD dwFlagsAndAttributes = _FlagsAndAttributes)
	{
		WINX_ASSERT(!good());
		WINX_ASSERT(dwCreationDisposition >= CREATE_NEW && dwCreationDisposition <= TRUNCATE_EXISTING);
		WINX_ASSERT(
			FILE_SHARE_READ == dwShareMode ||
			(FILE_SHARE_READ|FILE_SHARE_WRITE) == dwShareMode);

		if (good())
			return E_ACCESSDENIED;

		m_hFile = CreateFile(
			szFile, 
			Config::FileDesiredAccess,
			dwShareMode,
			NULL,
			dwCreationDisposition, 
			dwFlagsAndAttributes,
			NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
			return E_ACCESSDENIED;

		m_nTotalPage = 0;
		m_hFileMapping = NULL;
		return S_OK;
	}

	char* winx_call viewSegment(DWORD iBasePage, DWORD nPageCount)
	{
		if (iBasePage + nPageCount > m_nTotalPage)
		{
			if (iBasePage >= m_nTotalPage)
				return NULL;
			else
				nPageCount = m_nTotalPage - iBasePage;
		}
		return (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess,
			iBasePage >> _AllocationGranularityInvBits,
			iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits,
			NULL);
	}

	char* winx_call accessSegment(DWORD iBasePage, DWORD nPageCount)
	{
		if (iBasePage + nPageCount > m_nTotalPage)
		{
			if (m_hFileMapping)
				WINX_VERIFY(CloseHandle(m_hFileMapping));

			m_nTotalPage = iBasePage + nPageCount;

			m_hFileMapping = CreateFileMapping(
				m_hFile, NULL, Config::PageProtect,
				m_nTotalPage >> _AllocationGranularityInvBits,
				m_nTotalPage << AllocationGranularityBits,
				NULL);
		}

		return (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess,
			iBasePage >> _AllocationGranularityInvBits,
			iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits,
			NULL);
	}

	char* winx_call allocSegment(DWORD nPageCount, DWORD& iBasePage)
	{
		WINX_ASSERT(good());

		if (m_hFileMapping)
			WINX_VERIFY(CloseHandle(m_hFileMapping));

		iBasePage = m_nTotalPage;

		m_nTotalPage += nPageCount;

		m_hFileMapping = CreateFileMapping(
			m_hFile, NULL, Config::PageProtect,
			m_nTotalPage >> _AllocationGranularityInvBits,
			m_nTotalPage << AllocationGranularityBits,
			NULL);

		return (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess,
			iBasePage >> _AllocationGranularityInvBits,
			iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits,
			NULL);
	}

public:
	typedef FileMappingT Owner;

	class Handle
	{
	private:
		Owner* m_owner;

	public:
		enum { AllocationGranularityBits = _nAGBits };
		enum { AllocationGranularity = (1 << _nAGBits) };
		enum { AllocationGranularityMask = (AllocationGranularity - 1) };

	public:
		typedef UINT size_type;
		typedef PosT pos_type;
		typedef PosT pos_argtype;

	public:
		Handle() : m_owner(NULL) {}
		Handle(Owner& owner) : m_owner(&owner) {}

		void winx_call init(Owner& owner)
		{
			WINX_ASSERT(!m_owner);
			m_owner = &owner;
		}

		void winx_call close() {
		}

		char* winx_call viewSegment(DWORD iBasePage, DWORD nPageCount)
		{
			return m_owner->viewSegment(iBasePage, nPageCount);
		}

		char* winx_call accessSegment(DWORD iBasePage, DWORD nPageCount)
		{
			return m_owner->accessSegment(iBasePage, nPageCount);
		}

		char* winx_call allocSegment(DWORD nPageCount, DWORD& iBasePage)
		{
			return m_owner->allocSegment(nPageCount, iBasePage);
		}
	};

	class ViewBuffer : public AccessBufferT<Handle, TRUE>
	{
	public:
		ViewBuffer() {}
		ViewBuffer(Owner& owner) { init(owner); }
	};

	class AccessBuffer : public AccessBufferT<Handle>
	{
	public:
		AccessBuffer() {}
		AccessBuffer(Owner& owner) { init(owner); }
	};

	template <int SegBits = 16>
	class SegmentViewBuffer : public SegmentAccessBufferT<Handle, SegBits, TRUE>
	{
	public:
		SegmentViewBuffer() {}
		SegmentViewBuffer(Owner& owner) { init(owner); }
	};

	template <int SegBits = 16, BOOL ReadOnly = FALSE>
	class SegmentAccessBuffer : public SegmentAccessBufferT<Handle, SegBits, ReadOnly>
	{
	public:
		SegmentAccessBuffer() {}
		SegmentAccessBuffer(Owner& owner) { init(owner); }
	};
	
	template <int SegBits = 16>
	class SegmentAllocBuffer : public SegmentAllocBufferT<Handle, SegBits>
	{
	public:
		SegmentAllocBuffer() {}
		SegmentAllocBuffer(Owner& owner) { init(owner); }
	};
};

typedef FileMappingT<FileMappingReadWrite> FileMapping;

// -------------------------------------------------------------------------
// class TestFileMapping

template <class LogT>
class TestFileMapping
{
	WINX_TEST_SUITE(TestFileMapping);
		WINX_TEST(testAccessBuffer);
		WINX_TEST(testAccessBuffer2);
		WINX_TEST(testSegmentAccessBuffer);
		WINX_TEST(testSegmentAccessBuffer2);
		WINX_TEST(testSegmentAllocBuffer);
		WINX_TEST(testSegmentAllocBuffer2);
		WINX_TEST(testAccessAndAlloc);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void testAccessBuffer(LogT& log)
	{
		char* buf;
		AccessBufferT<FileMapping> ab;

		ab.open(WINX_TEXT("/__AccessBuffer__.txt"));

		buf = ab.view(0, 32);
		strcpy(buf, "hello, xushiwei!");

		buf = ab.view(FileMapping::AllocationGranularity-5, 32);
		strcpy(buf, "hello, xushiwei!");

		FileMapping::ViewBuffer vb(ab);
		char* buf2 = vb.view(FileMapping::AllocationGranularity-5, 32);

		log.print(buf2).newline();
		AssertExp(strcmp(buf2, buf) == 0);
	}

	void testAccessBuffer2(LogT& log)
	{
		FileMapping fm(WINX_TEXT("/__AccessBuffer2__.txt"));

		FileMapping::AccessBuffer ab(fm);
		char* buf = ab.view(0, 32);
		strcpy(buf, "hello, xushiwei!");

		FileMapping::ViewBuffer vb(fm);
		char* buf2 = vb.view(0, 32);

		buf = ab.view(FileMapping::AllocationGranularity-5, 32);
		strcpy(buf, "hello, abcdefg!");

		log.print(buf2).newline();
		AssertExp(strcmp(buf2, "hello, xushiwei!") == 0);
	}

	void testSegmentAccessBuffer(LogT& log)
	{
		char* buf;
		SegmentAccessBufferT<FileMapping> sab;

		sab.open(WINX_TEXT("/__SegmentAccessBuffer1__.txt"));

		buf = sab.view(0);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(32);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(64);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(FileMapping::AllocationGranularity);
		strcpy(buf, "hello, xushiwei!");
	}

	void testSegmentAccessBuffer2(LogT& log)
	{
		char* buf;
		FileMapping fm(WINX_TEXT("/__SegmentAccessBuffer2__.txt"));;
		FileMapping::SegmentAccessBuffer<> sab(fm);

		buf = sab.view(0);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(32);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(64);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(FileMapping::AllocationGranularity);
		strcpy(buf, "hello, xushiwei!");
	}

	void testSegmentAllocBuffer(LogT& log)
	{
		char* buf;
		SegmentAllocBufferT<FileMapping> sab;
		SegmentAllocBufferT<FileMapping>::pos_type fc;

		sab.open(WINX_TEXT("/__AllocBuffer1__.txt"));

		buf = sab.allocData(32, fc);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.allocData(32, fc);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.allocData(32, fc);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.allocData(FileMapping::AllocationGranularity, fc);
		strcpy(buf, "hello, xushiwei!");
	}

	void testSegmentAllocBuffer2(LogT& log)
	{
		char* buf;
		FileMapping fm(WINX_TEXT("/__AllocBuffer2__.txt"));
		FileMapping::SegmentAllocBuffer<0> sab(fm);
		FileMapping::pos_type fc;

		buf = sab.allocData(32, fc);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.allocData(32, fc);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.allocData(32, fc);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.allocData(FileMapping::AllocationGranularity, fc);
		strcpy(buf, "hello, xushiwei!");
	}

	void testAccessAndAlloc(LogT& log)
	{
		::remove("/__AccessAndAlloc__.txt");

		char* buf;
		FileMapping fm(WINX_TEXT("/__AccessAndAlloc__.txt"));
		FileMapping::SegmentAccessBuffer<> sab(fm);
		FileMapping::SegmentAllocBuffer<> sa(fm);

		buf = sab.view(0);
		log.trace(buf);

		strcpy(buf, "hello, xushiwei!\n");

		buf = sab.view(32);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(64);
		strcpy(buf, "hello, xushiwei!");

		buf = sab.view(FileMapping::AllocationGranularity);
		strcpy(buf, "hello, xushiwei!");

		SegmentAllocInfo info = { 0, FileMapping::AllocationGranularity };
		sa.initBuffer(info);

		FileMapping::pos_type fc;
		buf = sa.allocData(32, fc);
		strcpy(buf, "1234\n");
		
		buf = sab.view(fc);
		log.trace(buf);
		AssertExp(strcmp(buf, "1234\n") == 0);
	}
};

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#pragma pack()

#endif /* __STDEXT_FILEMAPPING_FILEMAPPINGIMPL_H__ */
