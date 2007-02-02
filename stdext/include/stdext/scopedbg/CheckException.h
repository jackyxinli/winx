/* -------------------------------------------------------------------------
//	文件名		：	stdext/scopedbg/CheckException.h
//	创建者		：	许式伟
//	创建时间	：	2007-1-31 16:30:23
//	功能描述	：	
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_SCOPEDBG_CHECKEXCEPTION_H__
#define __STDEXT_SCOPEDBG_CHECKEXCEPTION_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#if !defined(_MSC_VER)
#error "Not support."
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class CheckException - 该类不允许继承

template <class Base>
class CheckException : public Base
{
private:
	// declared but not implemented, allocated on stack only
	static void* operator new(size_t);
	static void operator delete(void*);

	static BOOL _isNormalCall(PBYTE callAddr)
	{
		return callAddr[0] != 0xC3 && // is not 'ret'
			*((PWORD)callAddr-1) != 0xD0FF; // is not 'call eax'
	}

public:
	// disable global optimization and setup frame pointer to get return address
	#pragma optimize( "g" , off )
	#pragma optimize( "y" , on )

	~CheckException()
	{
		PBYTE callAddr;
		__asm
		{
#if !defined(_DEBUG) && defined(WINX_VC_NET_LE2003)
			mov eax, CheckException<Base>::~CheckException;
			cmp byte ptr [eax], 85; // is 'push ebp' ?
			je has_frame_pointers;
			mov eax, [esp+4];
			jmp set_call_addr;
	has_frame_pointers:
			mov eax, [ebp+4];
	set_call_addr:
			mov callAddr, eax;
#else
			mov eax, [ebp+4];
			mov callAddr, eax;
#endif
		}
		if ( _isNormalCall(callAddr) ) // is normal call?
			Base::onNormalCall();
		else
			Base::onException();
	}

	// use project default optimization settings
	#pragma optimize("", on)

public:
	CheckException() {}

	template <class ArgT1>
	CheckException(ArgT1 arg1) : Base(arg1) {}

	template <class ArgT1, class ArgT2>
	CheckException(ArgT1 arg1, ArgT2 arg2) : Base(arg1, arg2) {}

	template <class ArgT1, class ArgT2, class ArgT3>
	CheckException(ArgT1 arg1, ArgT2 arg2, ArgT3 arg3) : Base(arg1, arg2, arg3) {
	}

	template <class ArgT1, class ArgT2, class ArgT3, class ArgT4>
	CheckException(ArgT1 arg1, ArgT2 arg2, ArgT3 arg3, ArgT4 arg4) : Base(arg1, arg2, arg3, arg4) {
	}
};

// -------------------------------------------------------------------------
// class TestCheckException

template <class LogT>
class TestCheckException
{
	WINX_TEST_SUITE(TestCheckException);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	class SimpleCheck
	{
	private:
		LogT& log;
		bool isNormal;
	public:
		SimpleCheck(LogT* aLog, bool aIsNormal)
			: log(*aLog), isNormal(aIsNormal) {
		}
		void winx_call onNormalCall() {
			log.print("onNormalCall\n");
			AssertExp(isNormal);
		}
		void winx_call onException() {
			log.print("onException\n");
			AssertExp(!isNormal);
		}
	};

	void test(LogT& log)
	{
		try {{
			CheckException<SimpleCheck> chkNormal(&log, true);
		}{
			CheckException<SimpleCheck> chkExcept(&log, false);
			throw 0;
		}}
		catch (...) {
		}
	}
};

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* __STDEXT_SCOPEDBG_CHECKEXCEPTION_H__ */
