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
// Module: stdext/Relation.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Relation.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_RELATION_H
#define STDEXT_RELATION_H

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

#ifndef STDEXT_TUPLE_H
#include "Tuple.h"
#endif

#ifndef STDEXT_DEQUE_H
#include "Deque.h"
#endif

#ifndef STDEXT_MAP_H
#include "Map.h"
#endif

#ifndef STDEXT_HASHMAP_H
#include "HashMap.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// MapIndexing

template <int Index, class KeyT, class AllocT>
struct MapIndexing
{
	typedef MultiMap<KeyT, void*, std::less<KeyT>, AllocT> type;
};

// -------------------------------------------------------------------------
// HashMapIndexing

template <int Index, class KeyT, class AllocT>
struct HashMapIndexing
{
	typedef HashMultiMap<KeyT, void*, HashCompare<KeyT>, AllocT> type;
};

// -------------------------------------------------------------------------
// IndexingAct

template <
	class TupleT,
	template <int Index, class KeyT, class AllocT> class IndexingT,
	class AllocT,
	int CurrIndex = TupleTraits<TupleT>::Fields - 1>
struct IndexingAct
{
	typedef typename TupleItemTraits<CurrIndex, TupleT>::value_type CurrKeyT;
	typedef typename IndexingT<CurrIndex, CurrKeyT, AllocT>::type CurrIndexingT;
	typedef IndexingAct<TupleT, IndexingT, AllocT, CurrIndex - 1> NextAct;
	
	static void winx_call init(void* dest[])
	{
		dest[CurrIndex] = NULL;
		NextAct::init(dest);
	}
	
	static void winx_call indexing(void* dest[], const TupleT* const& t)
	{
		if (dest[CurrIndex])
		{
			CurrIndexingT& idxDest = *(CurrIndexingT*)dest[CurrIndex];
			const CurrKeyT& key = TupleItemTraits<CurrIndex, TupleT>::get(*t);
			idxDest.insert(
				typename CurrIndexingT::value_type(key, (void*)&t)
				);
		}
		NextAct::indexing(dest, t);
	}

	template <int IndexExcluded>
	static void winx_call eraseIndexing(void* dest[], const TupleT* const& t)
	{
		if (CurrIndex != IndexExcluded && dest[CurrIndex])
		{
			typedef typename CurrIndexingT::iterator It;
			typedef std::pair<It, It> RangeT;

			It it;
			CurrIndexingT& idxDest = *(CurrIndexingT*)dest[CurrIndex];
			const CurrKeyT& key = TupleItemTraits<CurrIndex, TupleT>::get(*t);
			const RangeT rg = idxDest.equal_range(key);
			for (it = rg.first; it != rg.second; ++it) {
				if ((*it).second == (void*)&t) {
					idxDest.erase(it);
					break;
				}
			}
			WINX_ASSERT(it != rg.second);
		}
		NextAct::template eraseIndexing<IndexExcluded>(dest, t);
	}

	template <class DataT>
	static size_t winx_call size(void* const dest[], const DataT& data)
	{
		if (dest[CurrIndex])
		{
			const CurrIndexingT& idxDest = *(const CurrIndexingT*)dest[CurrIndex];
			return idxDest.size();
		}
		return NextAct::size(dest, data);
	}
};

template <
	class TupleT,
	class AllocT,
	template <int Index, class KeyT, class AllocT> class IndexingT>
struct IndexingAct<TupleT, IndexingT, AllocT, -1>
{
	static void winx_call init(void* dest[]) {
		//noting to do
	}
	
	static void winx_call indexing(void* dest[], const TupleT* const& t) {
		//noting to do
	}

	template <int IndexExcluded>
	static void winx_call eraseIndexing(void* dest[], const TupleT* const& t) {
		//noting to do
	}

	template <class DataT>
	static size_t winx_call size(void* const dest[], const DataT& data) {
		return data.size();
	}	
};

// -------------------------------------------------------------------------
// class Relation

template <
	class TupleT,
	template <int Index, class KeyT, class AllocT> class IndexingT = HashMapIndexing,
	class AllocT = ScopedAlloc>
class Relation
{
public:
	enum { Fields = TupleTraits<TupleT>::Fields };

	template <int Index>
	class Indexing
	{
	public:
		typedef typename TupleItemTraits<Index, TupleT>::value_type key_type;
		typedef typename IndexingT<Index, key_type, AllocT>::type type;
		typedef typename type::iterator iterator;
		typedef typename type::const_iterator const_iterator;
		typedef std::pair<iterator, iterator> range;
		typedef std::pair<const_iterator, const_iterator> const_range;

	public:
		enum { HasDestructor = DestructorTraits<key_type>::HasDestructor };

		static const TupleT& winx_call item(const const_iterator& it) {
			return **(const TupleT* const*)(*it).second;
		}
	};

private:
	Relation(const Relation&);
	void operator=(const Relation&);

	typedef Deque<void*, AllocT> DataT;
	typedef IndexingAct<TupleT, IndexingT, AllocT> IndexingActT;

	enum { HasDestructor = TupleTraits<TupleT>::HasDestructor };
	
private:
	DataT m_data;
	void* m_indexs[Fields];

	template <int Index, class MapT>
	static void doIndexing_(MapT* inst, const DataT& data)
	{
		typedef typename Indexing<Index>::key_type KeyT;
		typedef typename DataT::const_iterator It;
		for (It it = data.begin(); it != data.end(); ++it)
		{
			const TupleT* const t = (const TupleT* const)*it;
			if (t) {
				const KeyT& key = TupleItemTraits<Index, TupleT>::get(*t);
				inst->insert(typename MapT::value_type(key, (void*)&t));
			}
		}
	}

public:
	typedef size_t size_type;
	typedef AllocT alloc_type;
	
	explicit Relation(AllocT& alloc) : m_data(alloc) {
		IndexingActT::init(m_indexs);
	}

public:
	alloc_type& winx_call get_alloc() const {
		return m_data.get_alloc();
	}
	
	void winx_call swap(Relation& o) {
		m_data.swap(o.m_data);
		swap(m_indexs, o.m_indexs, Fields);
	}
	
	size_type winx_call size() const {
		return IndexingActT::size(m_indexs, m_data);
	}
	
	void winx_call clear()
	{
		if (m_data.size()) {
			m_data.clear();
			IndexingActT::init(m_indexs);
		}
	}
	
	template <int Index>
	void winx_call indexing()
	{
		typedef typename Indexing<Index>::type MapT;
		if (!m_indexs[Index])
		{
			MapT* inst;
			AllocT& alloc = m_data.get_alloc();
			if (Indexing<Index>::HasDestructor)
				inst = STD_NEW(alloc, MapT)(alloc);
			else
				inst = STD_UNMANAGED_NEW(alloc, MapT)(alloc);
			doIndexing_<Index>(inst, m_data);
			m_indexs[Index] = inst;
		}
	}

	template <int Index>
	bool winx_call indexinged() const {
		return m_indexs[Index] != NULL;
	}
	
	template <int Index>
	void winx_call eraseIndexing() {
		m_indexs[Index] = NULL;
	}

	template <int Index>
	void winx_call erase(
		typename Indexing<Index>::iterator const it)
	{
		WINX_ASSERT(m_indexs[Index] != NULL);
		typedef typename Indexing<Index>::type MapT;

		const TupleT** pt = (const TupleT**)(*it).second;
		IndexingActT::template eraseIndexing<Index>(m_indexs, *pt);
		*pt = NULL;
		((MapT*)m_indexs[Index])->erase(it);
	}

	template <int Index>
	size_type winx_call erase(
		typename Indexing<Index>::iterator const first,
		typename Indexing<Index>::iterator const last)
	{
		WINX_ASSERT(m_indexs[Index] != NULL);
		typedef typename Indexing<Index>::type MapT;

		size_type n = 0;
		for (typename Indexing<Index>::iterator it = first; it != last; ++it) {
			const TupleT** pt = (const TupleT**)(*it).second;
			IndexingActT::template eraseIndexing<Index>(m_indexs, *pt);
			*pt = NULL;
			++n;
		}
		((MapT*)m_indexs[Index])->erase(first, last);
		return n;
	}

	template <int Index>
	size_type winx_call erase(const typename Indexing<Index>::key_type& key)
	{
		WINX_ASSERT(m_indexs[Index] != NULL);
		typedef typename Indexing<Index>::type MapT;
		typedef typename Indexing<Index>::range RangeT;

		const RangeT rg = ((MapT*)m_indexs[Index])->equal_range(key);
		return erase<Index>(rg.first, rg.second);
	}

	template <int Index>
	size_type winx_call count(const typename Indexing<Index>::key_type& key) const
	{
		WINX_ASSERT(m_indexs[Index] != NULL);
		typedef typename Indexing<Index>::type MapT;

		return ((const MapT*)m_indexs[Index])->count(key);
	}
	
	template <int Index>
	typename Indexing<Index>::const_range winx_call
		select(const typename Indexing<Index>::key_type& key) const
	{
		WINX_ASSERT(m_indexs[Index] != NULL);
		typedef typename Indexing<Index>::type MapT;

		return ((const MapT*)m_indexs[Index])->equal_range(key);
	}

	template <int Index>
	typename Indexing<Index>::range winx_call
		select(const typename Indexing<Index>::key_type& key)
	{
		WINX_ASSERT(m_indexs[Index] != NULL);
		typedef typename Indexing<Index>::type MapT;

		return ((MapT*)m_indexs[Index])->equal_range(key);
	}

	void winx_call insert(const TupleT& val) {
		AllocT& alloc = m_data.get_alloc();
		TupleT* t = HasDestructor ? STD_NEW(alloc, TupleT)(val) : STD_UNMANAGED_NEW(alloc, TupleT)(val);
		m_data.push_back(t);
		IndexingActT::indexing(m_indexs, *(const TupleT* const*)&m_data.back()); 
	}
	
	void winx_call copy(const Relation& from) {
		clear();
		typedef typename Relation::DataT::const_iterator It;
		for (It it = from.m_data.begin(); it != from.m_data.end(); ++it) {
			const TupleT* const t = (const TupleT* const)*it;
			if (t)
				insert(*t);
		}
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestRelation

#if defined(STD_UNITTEST)

template <class LogT>
class TestRelation : public TestCase
{
	WINX_TEST_SUITE(TestRelation);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		typedef std::AutoFreeAlloc AllocT;
		typedef std::pair<std::string, int> TupleT;
		typedef std::Relation<TupleT, std::HashMapIndexing, AllocT> RelationT;
		typedef RelationT::Indexing<0> Indexing0;
		typedef RelationT::Indexing<1> Indexing1;
		
		AllocT alloc;
		RelationT rel(alloc);
		rel.indexing<0>();
		rel.indexing<1>();
		
		rel.insert(TupleT("Mon", 1));
		rel.insert(TupleT("Monday", 1));
		rel.insert(TupleT("Tue", 2));
		rel.insert(TupleT("Wed", 3));
		rel.insert(TupleT("Wednesday", 3));
		AssertExp(rel.size() == 5);
		
		Indexing1::range rg = rel.select<1>(3);
		AssertExp(std::distance(rg.first, rg.second) == 2);
		for (Indexing1::iterator it = rg.first; it != rg.second; ++it) {
			const TupleT& i = Indexing1::item(it);
			log.printString(i.first).newline();
			AssertExp(i.first == "Wed" || i.first == "Wednesday");
			AssertExp(i.second == 3);
		}

		Indexing0::range rg2 = rel.select<0>("Mon");
		AssertExp(std::distance(rg2.first, rg2.second) == 1);
		Indexing0::iterator it2 = rg2.first;
		const TupleT& i2 = Indexing0::item(it2);
		AssertExp(i2.first == "Mon" && i2.second == 1);

		AssertExp(rel.count<1>(1) == 2);
		AssertExp(rel.erase<1>(1) == 2);
		AssertExp(rel.count<1>(1) == 0);
		AssertExp(rel.size() == 3);
		
		Indexing0::range rg3 = rel.select<0>("Mon");
		AssertExp(std::distance(rg3.first, rg3.second) == 0);
		AssertExp(rel.count<0>("Monday") == 0);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Relation.h,v $

#endif /* STDEXT_RELATION_H */

