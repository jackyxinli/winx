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
// Module: Performance.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: Performance.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/Relation.h>
#include <stdext/Counter.h>
#include <stdext/Rand.h>
#include <glib.h>

// -------------------------------------------------------------------------
// my_strdup

template <class AllocT, class CharT>
inline CharT* winx_call my_strdup(AllocT& alloc, const CharT* src)
{
	size_t n = std::char_traits<CharT>::length(src) + 1;
	CharT* dst = STD_ALLOC_ARRAY(alloc, CharT, n);
	std::copy(src, src + n, dst);
	return dst;
}

// -------------------------------------------------------------------------

template <class LogT>
class TestRelationPerformance
{
private:
	NS_STDEXT::Accumulator m_acc;
	NS_STDEXT::AutoAlloc m_alloc;
	const char** m_keys;
	int* m_vals;

public:
	enum { Count = 20000 };
	enum { TestN = 16 };
	
	void init()
	{
		NS_STDEXT::Rand rnd;
		char s1[32];
		m_keys = STD_ALLOC_ARRAY(m_alloc, const char*, Count);
		m_vals = STD_ALLOC_ARRAY(m_alloc, int, Count);
		for (int i = 0; i < Count; ++i)
		{
			rnd.srand(s1, 6, countof(s1), 'A', 'z');
			m_keys[i] = my_strdup(m_alloc, s1);
			m_vals[i] = rnd.rand();
		}
	}

	void GRelation_insert(LogT& log)
	{
		log.print("\n===== GRelation (insert) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			GRelation* rel = g_relation_new(2);
			g_relation_index(rel, 0, g_str_hash, g_str_equal);
			g_relation_index(rel, 1, g_direct_hash, g_direct_equal);
		
			NS_STDEXT::PerformanceCounter counter;
			for (int i = 0; i < Count; ++i)
			{
				g_relation_insert(rel, m_keys[i], (gpointer)m_vals[i]);
			}
			m_acc.accumulate(counter.trace(log));

			g_relation_destroy(rel);
		}
		m_acc.trace_avg(log);
	}

	void GRelation_select(LogT& log)
	{
		log.print("\n===== GRelation (select) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			GRelation* rel = g_relation_new(2);
			g_relation_index(rel, 0, g_str_hash, g_str_equal);
			g_relation_index(rel, 1, g_direct_hash, g_direct_equal);
		
			for (int i = 0; i < Count; ++i)
			{
				g_relation_insert(rel, m_keys[i], (gpointer)m_vals[i]);
			}

			NS_STDEXT::PerformanceCounter counter;
			for (int i = 0; i < Count; ++i)
			{
				guint k;
				GTuples* rg = g_relation_select(rel, m_keys[i], 0);
				for (k = 0; k < rg->len; ++k)
				{
					int val = (int)g_tuples_index(rg, k, 1);
					if (val == m_vals[i])
						break;
				}
				WINX_ASSERT(k != rg->len);
				g_tuples_destroy(rg);
			}
			m_acc.accumulate(counter.trace(log));

			g_relation_destroy(rel);
		}
		m_acc.trace_avg(log);
	}

	void Relation_insert(LogT& log)
	{
		log.print("\n===== Relation (insert) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			typedef std::pair<const char*, int> TupleT;
			typedef NS_STDEXT::Relation<TupleT, 3, 0, NS_STDEXT::HashMapIndexing, NS_STDEXT::AutoAlloc> RelationT;
			
			RelationT rel(m_alloc);
		
			NS_STDEXT::PerformanceCounter counter;
			for (int i = 0; i < Count; ++i)
			{
				rel.insert(TupleT(m_keys[i], m_vals[i]));
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void Relation_select(LogT& log)
	{
		log.print("\n===== Relation (select) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			typedef std::pair<const char*, int> TupleT;
			typedef NS_STDEXT::Relation<TupleT, 3, 0, NS_STDEXT::HashMapIndexing, NS_STDEXT::AutoAlloc> RelationT;
			typedef RelationT::Indexing<0> Indexing0;
			
			RelationT rel(m_alloc);
			for (int i = 0; i < Count; ++i)
			{
				rel.insert(TupleT(m_keys[i], m_vals[i]));
			}
		
			NS_STDEXT::PerformanceCounter counter;
			for (int i = 0; i < Count; ++i)
			{
				Indexing0::iterator it;
				Indexing0::range rg = rel.select<0>(m_keys[i]);
				for (it = rg.first; it != rg.second; ++it)
				{
					const TupleT& v = Indexing0::item(it);
					if (v.second == m_vals[i])
						break;
				}
				WINX_ASSERT(it != rg.second);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void testComparison(LogT& log)
	{
		init();

		GRelation_insert(log);
		Relation_insert(log);

		GRelation_select(log);
		Relation_select(log);
	}
};

int main()
{
	NS_STDEXT::ErrorLog log;
	TestRelationPerformance<NS_STDEXT::ErrorLog> test;
	test.testComparison(log);
	return 0;
}

// -------------------------------------------------------------------------
// $Log: Performance.cpp,v $
//
