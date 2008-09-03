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
// Module: stdext/archive/MMapArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: MMapArchive.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_MMAPARCHIVE_H
#define STDEXT_ARCHIVE_MMAPARCHIVE_H

#ifndef STDEXT_ARCHIVE_MEMARCHIVE_H
#include "MemArchive.h"
#endif

#ifndef STDEXT_MMAP_H
#include "../MMap.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class MMapReadCache

template <class CharT>
class MMapReadCache
{
private:
	typedef SegmentAccessBuffer<FileMappingRO, 0, TRUE> SegmentT;
	typedef SegmentT::pos_type PosT;
	
private:
	const void* m_segEnd;
	SegmentT m_seg;
	PosT m_fileSize;
	
public:
	MMapReadCache() {}
	
	MMapReadCache(LPCSTR szFile) {
		m_segEnd = NULL;
		m_seg.open(szFile, &m_fileSize);
	}
	
	HRESULT winx_call open(LPCSTR szFile) {
		m_segEnd = NULL;
		return m_seg.open(szFile, &m_fileSize);
	}
	
	void winx_call close() {
		m_seg.close();
	}
	
	bool winx_call good() const {
		return m_seg.good();
	}
	
private:
	class Iterator : public std::iterator<std::random_access_iterator_tag, CharT>
	{
	private:
		enum { SegmentBits = SegmentT::SegmentBits };
		enum { SegmentSize = SegmentT::SegmentSize };
		enum { SegmentSizeMask = SegmentSize-1 };
	
	private:
		const char* m_pos;
		const char* m_segEnd;
		MMapReadCache* m_cache;
		PosT m_fcSeg;
	
	private:
		const CharT& winx_call _fetch() {
			PosT const pos = m_fcSeg + (SegmentSize - (m_segEnd - m_pos));
			SegmentT& seg = m_cache->m_seg;
			m_pos = seg.view(m_fcSeg);
			m_cache->m_segEnd = m_segEnd = seg.getCurrentView() + SegmentSize;
			m_fcSeg = (PosT)seg.getCurrentSeg() << SegmentBits;
			return *(const CharT*)m_pos;
		}
		
		void winx_call _init(PosT fc) {
			m_fcSeg = fc & ~SegmentSizeMask;
			m_pos = (const char*)(fc & SegmentSizeMask);
			m_segEnd = (const char*)SegmentSize;
		}
		
	public:
		Iterator() {}
		Iterator(PosT fc, const MMapReadCache* cache) : m_cache((MMapReadCache*)cache) {
			_init(fc);
		}
		
		const CharT& winx_call operator*() {
			return m_segEnd == m_cache->m_segEnd ? *(const CharT*)m_pos : _fetch();
		}
		
		Iterator& winx_call operator++() {
			m_pos += sizeof(CharT);
			if (m_pos == m_segEnd) {
				m_fcSeg += SegmentSize;
				m_pos = NULL;
				m_segEnd = (const char*)SegmentSize;
			}
			return *this;
		}
		
		Iterator& winx_call operator--() {
			if (m_pos == m_segEnd - SegmentSize) {
				m_fcSeg -= SegmentSize;
				m_pos = m_segEnd = (const char*)SegmentSize;
			}
			m_pos -= sizeof(CharT);
			return *this;
		}
		
		Iterator& winx_call operator+=(ptrdiff_t delta) {
			_init(m_fcSeg + (SegmentSize - (m_segEnd - m_pos)) + delta);
			return *this;
		}
		
		Iterator& winx_call operator-=(ptrdiff_t delta) {
			_init(m_fcSeg + (SegmentSize - (m_segEnd - m_pos)) - delta);
			return *this;
		}

		Iterator winx_call operator++(int) {
			Iterator tmp = *this;
			++ *this;
			return tmp;
		}

		Iterator winx_call operator--(int) {
			Iterator tmp = *this;
			-- *this;
			return tmp;
		}
		
		friend Iterator winx_call operator+(Iterator a, ptrdiff_t delta) {
			return a += delta;
		}
		
		friend Iterator winx_call operator-(Iterator a, ptrdiff_t delta) {
			return a += -delta;
		}

		friend Iterator winx_call operator+(ptrdiff_t delta, Iterator a) {
			return a += delta;
		}
		
		friend ptrdiff_t winx_call operator-(const Iterator& a, const Iterator& b) {
			return (ptrdiff_t)((a.m_fcSeg - (a.m_segEnd - a.m_pos)) - (b.m_fcSeg - (b.m_segEnd - b.m_pos)));
		}

		bool winx_call operator==(const Iterator& b) const {
			return (m_fcSeg - (m_segEnd - m_pos)) == (b.m_fcSeg - (b.m_segEnd - b.m_pos));
		}
		
		bool winx_call operator!=(const Iterator& b) const {
			return (m_fcSeg - (m_segEnd - m_pos)) != (b.m_fcSeg - (b.m_segEnd - b.m_pos));
		}
	};
	
public:
	typedef Iterator iterator, const_iterator;
	typedef MemReadArchive<Iterator> archive_type;
	
	const_iterator winx_call begin() const {
		return const_iterator(0, this);
	}
	
	const_iterator winx_call end() const {
		return const_iterator(m_fileSize, this);
	}
};

// -------------------------------------------------------------------------
// $Log: MMapReadArchive.h,v $

NS_STDEXT_END

#endif /* STDEXT_ARCHIVE_MMAPARCHIVE_H */

