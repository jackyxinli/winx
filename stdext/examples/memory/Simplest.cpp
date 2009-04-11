#include <stdext/Basic.h>
#include <stdext/Memory.h>

// -------------------------------------------------------------------------
// memory management

void testAutoFreeAlloc()
{
    NS_STDEXT::AutoFreeAlloc alloc;
    
    int* a;
    a = STD_NEW(alloc, int);
    
    int* b;
    b = STD_NEW_ARRAY(alloc, int, 100);
    
    int* c;
    c = STD_ALLOC(alloc, int);
    
    int* d;
    d = STD_ALLOC_ARRAY(alloc, int, 100);
    
    NS_STDEXT::AutoFreeAlloc* suballoc = STD_NEW(alloc, NS_STDEXT::AutoFreeAlloc);
    
    int* e;
    e = STD_NEW(*suballoc, int);
}

void testScopedAlloc()
{
    NS_STDEXT::BlockPool recycle;
    NS_STDEXT::ScopedAlloc alloc(recycle);
    
    int* a;
    a = STD_NEW(alloc, int);
    // ... --> same as NS_STDEXT::AutoFreeAlloc
}

int main()
{
    testAutoFreeAlloc();
    testScopedAlloc();	
	return 0;
}

// -------------------------------------------------------------------------

