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
// Module: mfcport/afx.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-26 11:51:25
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __MFCPORT_AFX_H__
#define __MFCPORT_AFX_H__

#ifndef __WINX_BASIC_H__
#include "../winx/Basic.h"
#endif

#if defined(WINX_VC_NET)
#pragma warning(disable:4996) // crt declared deprecated
#endif

/////////////////////////////////////////////////////////////////////////////

// Since MFC itself is built with wchar_t as a native type, it will not have
// the correct type info for types built with wchar_t typedef'd to unsigned
// short.  Make sure that the user's app builds this type info in this case.
#ifndef _NATIVE_WCHAR_T_DEFINED
#define _AFX_FULLTYPEINFO
#endif

#ifndef _INC_NEW
#include <new.h>
#endif

#include "afxver_.h"        // Target version control

#ifdef _WIN64
#ifndef _AFX_NO_DAO_SUPPORT
#define _AFX_NO_DAO_SUPPORT
#endif
#endif

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

/////////////////////////////////////////////////////////////////////////////
// Basic types
// abstract iteration position
struct __POSITION {};
typedef __POSITION* POSITION;

// Standard constants
#undef FALSE
#undef TRUE
#undef NULL

#define FALSE   0
#define TRUE    1
#define NULL    0

/////////////////////////////////////////////////////////////////////////////
// UNUSED/UNUSED_ALWAYS

#ifdef _DEBUG
#define UNUSED(x)
#else
#define UNUSED(x) x
#endif

#define UNUSED_ALWAYS(x) x

/////////////////////////////////////////////////////////////////////////////
// _AFX_FUNCNAME definition

#ifdef UNICODE
#define _AFX_FUNCNAME(_Name) _Name##W
#else
#define _AFX_FUNCNAME(_Name) _Name##A
#endif

/////////////////////////////////////////////////////////////////////////////
// Diagnostic support

#ifndef ASSERT
#define ASSERT(e)			WINX_ASSERT(e)
#endif

#ifndef ASSERT_VALID
#define ASSERT_VALID(pOb)	WINX_DEBUG_ONLY((::AfxAssertValidObject(pOb, THIS_FILE, __LINE__)))
#endif

#ifndef VERIFY
#define VERIFY(e)			WINX_VERIFY(e)
#endif

#ifndef TRACE
#define TRACE				WINX_TRACE
#endif

#ifndef TRACE0
#define TRACE0				WINX_TRACE
#endif

#ifndef TRACE1
#define TRACE1				WINX_TRACE
#endif

#ifndef TRACE2
#define TRACE2				WINX_TRACE
#endif

#ifndef TRACE3
#define TRACE3				WINX_TRACE
#endif

#ifndef DEBUG_ONLY
#define DEBUG_ONLY(e)		WINX_DEBUG_ONLY(e)
#endif

#define THIS_FILE          __FILE__

// Debug ASSERTs then throws. Retail throws if condition not met
#define ENSURE_THROW(cond, exception)	\
	do { int __afx_condVal=!!(cond); ASSERT(__afx_condVal); if (!(__afx_condVal)){exception;} } while (false)
#define ENSURE(cond)		ENSURE_THROW(cond, ::AfxThrowInvalidArgException() )
#define ENSURE_ARG(cond)	ENSURE_THROW(cond, ::AfxThrowInvalidArgException() )

// Debug ASSERT_VALIDs then throws. Retail throws if pOb is NULL
#define ENSURE_VALID_THROW(pOb, exception)	\
	do { ASSERT_VALID(pOb); if (!(pOb)){exception;} } while (false)
#define ENSURE_VALID(pOb)	ENSURE_VALID_THROW(pOb, ::AfxThrowInvalidArgException() )

#define ASSERT_POINTER(p, type) \
	ASSERT(((p) != NULL) && AfxIsValidAddress((p), sizeof(type), FALSE))

#define ASSERT_NULL_OR_POINTER(p, type) \
	ASSERT(((p) == NULL) || AfxIsValidAddress((p), sizeof(type), FALSE))

#ifdef _DEBUG
#define REPORT_EXCEPTION(pException, szMsg) \
	do { \
		TCHAR szErrorMessage[512]; \
		if (pException->GetErrorMessage(szErrorMessage, sizeof(szErrorMessage)/sizeof(*szErrorMessage), 0)) \
			TRACE(traceAppMsg, 0, _T("%s (%s:%d)\n%s\n"), szMsg, __FILE__, __LINE__, szErrorMessage); \
		else \
			TRACE(traceAppMsg, 0, _T("%s (%s:%d)\n"), szMsg, __FILE__, __LINE__); \
		ASSERT(FALSE); \
	} while (0)
#else
#define REPORT_EXCEPTION(pException, szMsg) \
	do { \
		CString strMsg; \
		TCHAR  szErrorMessage[512]; \
		if (pException->GetErrorMessage(szErrorMessage, sizeof(szErrorMessage)/sizeof(*szErrorMessage), 0)) \
			strMsg.Format(_T("%s (%s:%d)\n%s"), szMsg, __FILE__, __LINE__, szErrorMessage); \
		else \
			strMsg.Format(_T("%s (%s:%d)"), szMsg, __FILE__, __LINE__); \
		AfxMessageBox(strMsg); \
	} while (0)
#endif

#define EXCEPTION_IN_DTOR(pException) \
	do { \
		REPORT_EXCEPTION((pException), "Exception thrown in destructor"); \
		delete pException; \
	} while (0)
	
#define AFX_BEGIN_DESTRUCTOR try {
#define AFX_END_DESTRUCTOR   } catch (CException *pException) { EXCEPTION_IN_DTOR(pException); }

/////////////////////////////////////////////////////////////////////////////
// Other implementation helpers

#define BEFORE_START_POSITION ((POSITION)-1L)

#undef AFX_DATA
#define AFX_DATA AFX_CORE_DATA

/////////////////////////////////////////////////////////////////////////////
// Standard exception throws

void __declspec(noreturn) AFXAPI AfxThrowMemoryException();
void __declspec(noreturn) AFXAPI AfxThrowNotSupportedException();
void __declspec(noreturn) AFXAPI AfxThrowInvalidArgException();
void __declspec(noreturn) AFXAPI AfxThrowArchiveException(int cause,
	LPCTSTR lpszArchiveName = NULL);
void __declspec(noreturn) AFXAPI AfxThrowFileException(int cause, LONG lOsError = -1,
	LPCTSTR lpszFileName = NULL);
void __declspec(noreturn) AFXAPI AfxThrowOleException(LONG sc);

/////////////////////////////////////////////////////////////////////////////
// CRT functions

#ifndef _INC_ERRNO
#include <errno.h>
#endif

#if (0)
#define STRUNCATE
#endif

typedef int errno_t;

inline errno_t AfxCrtErrorCheck(errno_t error)
{
	switch(error)
	{
	case ENOMEM:
		AfxThrowMemoryException();
		break;
	case EINVAL:
	case ERANGE:
		AfxThrowInvalidArgException();
		break;
#ifdef STRUNCATE
	case STRUNCATE:
#endif
	case 0:
		break;
	default:
		AfxThrowInvalidArgException();
		break;
	}
	return error;
}

#define AFX_CRT_ERRORCHECK(expr) \
	AfxCrtErrorCheck(expr)

/////////////////////////////////////////////////////////////////////////////
// other helpers

// zero fill everything after the vtbl pointer
#define AFX_ZERO_INIT_OBJECT(base_class) \
	memset(((base_class*)this)+1, 0, sizeof(*this) - sizeof(class base_class));

/////////////////////////////////////////////////////////////////////////////
// Diagnostic memory management routines

// Low level sanity checks for memory blocks
BOOL AFXAPI AfxIsValidAddress(const void* lp,
			UINT_PTR nBytes, BOOL bReadWrite = TRUE);
BOOL AFXAPI AfxIsValidString(LPCWSTR lpsz, int nLength = -1);
BOOL AFXAPI AfxIsValidString(LPCSTR lpsz, int nLength = -1);

// Sanity checks for ATOMs
BOOL AfxIsValidAtom(ATOM nAtom);
BOOL AfxIsValidAtom(LPCTSTR psz);

void* AFXAPI AfxAllocMemoryDebug(size_t nSize, BOOL bIsObject,
	LPCSTR lpszFileName, int nLine);
void AFXAPI AfxFreeMemoryDebug(void* pbData, BOOL bIsObject);

// Dump any memory leaks since program started
BOOL AFXAPI AfxDumpMemoryLeaks();

// Return TRUE if valid memory block of nBytes
BOOL AFXAPI AfxIsMemoryBlock(const void* p, UINT nBytes, LONG* plRequestNumber = NULL);

// Return TRUE if memory is sane or print out what is wrong
BOOL AFXAPI AfxCheckMemory();

#define afxMemDF _crtDbgFlag

#if (0)
#define _CRTDBG_CHECK_DEFAULT_DF
#endif

enum AfxMemDF // memory debug/diagnostic flags
{
	allocMemDF          = _CRTDBG_ALLOC_MEM_DF,         // turn on debugging allocator
	delayFreeMemDF      = _CRTDBG_DELAY_FREE_MEM_DF,         // delay freeing memory
	checkAlwaysMemDF    = _CRTDBG_CHECK_ALWAYS_DF,          // AfxCheckMemory on every alloc/free
#ifdef _CRTDBG_CHECK_EVERY_16_DF
	checkEvery16MemDF	= _CRTDBG_CHECK_EVERY_16_DF,
	checkEvery128MemDF	= _CRTDBG_CHECK_EVERY_128_DF,
	checkEvery1024MemDF	= _CRTDBG_CHECK_EVERY_1024_DF,
	checkDefaultMemDF	= _CRTDBG_CHECK_DEFAULT_DF
#endif
};

// Memory state for snapshots/leak detection
struct CMemoryState
{
// Attributes
	enum blockUsage
	{
		freeBlock,    // memory not used
		objectBlock,  // contains a CObject derived class object
		bitBlock,     // contains ::operator new data
		crtBlock,
		ignoredBlock,
		nBlockUseMax  // total number of usages
	};

	_CrtMemState m_memState;
	LONG_PTR m_lCounts[nBlockUseMax];
	LONG_PTR m_lSizes[nBlockUseMax];
	LONG_PTR m_lHighWaterCount;
	LONG_PTR m_lTotalCount;

	CMemoryState();

// Operations
	void Checkpoint();  // fill with current state
	BOOL Difference(const CMemoryState& oldState,
					const CMemoryState& newState);  // fill with difference
	void UpdateData();

	// Output to afxDump
	void DumpStatistics() const;
	void DumpAllObjectsSince() const;
};

class CObject;
class CRuntimeClass;

// Enumerate allocated objects or runtime classes
void AFXAPI AfxDoForAllObjects(void (AFX_CDECL *pfn)(CObject* pObject, void* pContext),
	void* pContext);
void AFXAPI AfxDoForAllClasses(void (AFX_CDECL *pfn)(const CRuntimeClass* pClass,
	void* pContext), void* pContext);

// non-_DEBUG_ALLOC version that assume everything is OK
#ifndef _DEBUG
	#define DEBUG_NEW new
	#define AfxCheckMemory() TRUE
	#define AfxIsMemoryBlock(p, nBytes) TRUE
	#define AfxEnableMemoryTracking(bTrack) FALSE
	#define AfxEnableMemoryLeakOverride(bEnable) TRUE
	#define AfxOutputDebugString(lpsz) ::OutputDebugString(lpsz)
	#define AfxDiagnosticInit() TRUE
#else
	#define AfxOutputDebugString TRACE

	// turn on/off tracking for a short while
	BOOL AFXAPI AfxEnableMemoryTracking(BOOL bTrack);

	// Turn on/off the global flag _afxMemoryLeakOverride. if bEnable is TRUE
	// then further calls to AfxEnableMemoryTracking() wont change the current
	// memory tracking state, until AfxEnableMemoryLeakOverride(BOOL bEnable)
	// is called again with bEnable == FALSE.
	BOOL AFXAPI AfxEnableMemoryLeakOverride(BOOL bEnable);

	// Advanced initialization: for overriding default diagnostics
	BOOL AFXAPI AfxDiagnosticInit(void);

	// A failure hook returns whether to permit allocation
	typedef BOOL (AFXAPI* AFX_ALLOC_HOOK)(size_t nSize, BOOL bObject, LONG lRequestNumber);

	// Set new hook, return old (never NULL)
	AFX_ALLOC_HOOK AFXAPI AfxSetAllocHook(AFX_ALLOC_HOOK pfnAllocHook);
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations

#ifdef _AFX_ENABLE_INLINES
#define _AFX_INLINE AFX_INLINE

#if !defined(_AFX_CORE_IMPL) || !defined(_AFXDLL) || defined(_DEBUG)
#define _AFX_PUBLIC_INLINE AFX_INLINE
#else
#define _AFX_PUBLIC_INLINE
#endif

#endif

/////////////////////////////////////////////////////////////////////////////

#if (0)
#include <afx.inl>
#endif

#undef AFX_DATA
#define AFX_DATA

/////////////////////////////////////////////////////////////////////////////
// $Log: $

#endif /* __MFCPORT_AFX_H__ */
