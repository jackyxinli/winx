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
// Module: stdext/scopedbg/ScopeLog.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-2 10:31:46
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_SCOPEDBG_SCOPELOG_H__
#define __STDEXT_SCOPEDBG_SCOPELOG_H__

#if (0)
	#define WINX_SCOPE_DEQUE_LOG
#endif

#if defined(WINX_SCOPE_DEQUE_LOG)
	#ifndef _DEQUE_
	#include <deque>
	#endif
#else
	#ifndef _VECTOR_
	#include <vector>
	#endif
#endif

#ifndef __STDEXT_STORAGE_H__
	#include "../Storage.h"
#endif

#ifndef __STDEXT_LOG_H__
	#include "../Log.h"
#endif

__NS_STD_BEGIN

// =========================================================================
// printScopeMessage

template <class StorageT, class CharT>
inline void winx_call printScopeMessage(
	StorageT& stg, const CharT* msg, const CharT* msgEnd, char rep_ch, UINT rep_count)
{
	for (;;)
	{
		const CharT* it = std::find(msg, msgEnd, '\n');
		if (it == msgEnd)
		{
			if (msg != msgEnd)
			{
				stg.put(rep_count, rep_ch);
				stg.put(msg, it - msg);
				stg.put('\n');
			}
			return;
		}
		else
		{
			++it;
			stg.put(rep_count, rep_ch);
			stg.put(msg, it - msg);
			msg = it;
		}
	}
}

// =========================================================================
// class ScopeStorage

template <class StorageT>
class ScopeStorage : public StorageT
{
private:
	typedef typename StorageT::char_type CharT;
	typedef std::basic_string<CharT> StringT;
	typedef StringStorage<StringT> StringStorageT;

#if defined(WINX_SCOPE_DEQUE_LOG)
	typedef std::deque<StringT> QueuedMessageT;
#else
	typedef std::vector<StringT> QueuedMessageT;
#endif

	QueuedMessageT m_scopes;
	StringStorageT m_curr; 

public:
	typedef CharT char_type;

	struct _LogTo
	{
		StorageT& m_stg;
		UINT m_level;
		_LogTo(StorageT& stg)
			: m_stg(stg), m_level(0) {
		}
		void winx_call operator()(StringT& str) {
			if (str.size()) {
				const CharT* msg = &*str.begin();
				printScopeMessage(m_stg, msg, msg + str.size(), ' ', m_level<<2);
				str.erase();
			}
			++m_level;
		}
	};

public:
	ScopeStorage() {}

	template <class ArgT>
	ScopeStorage(ArgT arg) : StorageT(arg) {
	}

	~ScopeStorage() {
		if (this->good())
			commit();
	}

public:
	void winx_call enterScope()
	{
		m_scopes.push_back(m_curr);
		m_curr.erase();
	}

	void winx_call leaveScope(bool fCommit = true)
	{
		WINX_ASSERT( !m_scopes.empty() );

		if (fCommit)
			commit();

		if (m_scopes.empty()) {
			m_curr.erase();
		}
		else {
			m_curr.assign(m_scopes.back());
			m_scopes.pop_back();
		}
	}

	void winx_call commit()
	{
		_LogTo log(*this);
		std::for_each(m_scopes.begin(), m_scopes.end(), log)(m_curr);
	}

public:
	void winx_call put(int ch)
	{
		m_curr.put(ch);
	}
	
	void winx_call putw(wint_t wch)
	{
		m_curr.putw(wch);
	}
	
	void winx_call put(size_t count, int ch)
	{
		m_curr.put(count, ch);
	}
	
	void winx_call putw(size_t count, wint_t wch)
	{
		m_curr.put(count, wch);
	}
	
	void winx_call put(const char* s, size_t count)
	{
		m_curr.put(s, count);
	}
	
	void winx_call put(const WCHAR* s, size_t count)
	{
		m_curr.put(s, count);
	}

	void winx_call putv(const char* fmt, va_list args)
	{
		m_curr.putv(fmt, args);
	}
	
	void winx_call putv(const WCHAR* fmt, va_list args)
	{
		m_curr.putv(fmt, args);
	}
};

// =========================================================================
// class ScopeLog

template <class StorageT>
class ScopeLog : public Log< ScopeStorage<StorageT> >
{
protected:
	typedef Log< ScopeStorage<StorageT> > Base;
	using Base::m_stg;

public:
	ScopeLog() {}
	
	template <class ArgT>
	ScopeLog(ArgT arg) : Base(arg) {
	}

	void winx_call enterScope()
	{
		m_stg.enterScope();
	}

	void winx_call leaveScope(bool fCommit = true)
	{
		m_stg.leaveScope(fCommit);
	}

	void winx_call commit()
	{
		m_stg.commit();
	}
};

// =========================================================================
// class OutputScopeLog, ErrorScopeLog, FileScopeLog

class OutputScopeLog : public ScopeLog<FILEStorage>
{
public:
	OutputScopeLog() : ScopeLog<FILEStorage>(stdout) {}
};

class ErrorScopeLog : public ScopeLog<FILEStorage>
{
public:
	ErrorScopeLog() : ScopeLog<FILEStorage>(stderr) {}
};

class FileScopeLog : public ScopeLog<FILEStorage>
{
public:
	FileScopeLog() {}

	template <class ArgT>
	FileScopeLog(ArgT szFile) : ScopeLog<FILEStorage>(szFile) {
	}

	~FileScopeLog() {
		m_stg.close();
	}
};

// =========================================================================
// class TestScopeLog

template <class LogT>
class TestScopeLog
{
	WINX_TEST_SUITE(TestScopeLog);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void test(LogT& log)
	{
		OutputScopeLog slog;
		slog.newline();
		slog.print("message in global scope!!!");
		slog.enterScope();
			slog.print("message in level 1 scope!");
			slog.enterScope();
				slog.print("level 2 message");
				slog.enterScope();
					slog.print("level 3 message");
				slog.leaveScope();
				slog.print("message discard!!!");
			slog.leaveScope(false);
		slog.leaveScope();
		slog.print("done!");
	}
};

// =========================================================================
// $Log: $

__NS_STD_END

#endif /* __STDEXT_SCOPEDBG_SCOPELOG_H__ */
