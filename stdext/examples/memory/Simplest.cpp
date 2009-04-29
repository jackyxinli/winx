#include <stdext/Basic.h>
#include <stdext/Memory.h>

// -------------------------------------------------------------------------
// memory management

void testAutoAlloc()
{
    NS_STDEXT::AutoAlloc alloc;
    
    int* a;
    a = STD_NEW(alloc, int);
    
    int* b;
    b = STD_NEW_ARRAY(alloc, int, 100);
    
    int* c;
    c = STD_ALLOC(alloc, int);
    
    int* d;
    d = STD_ALLOC_ARRAY(alloc, int, 100);
    
    NS_STDEXT::AutoAlloc* suballoc = STD_NEW(alloc, NS_STDEXT::AutoAlloc);
    
    int* e;
    e = STD_NEW(*suballoc, int);
}

void testScopedAlloc()
{
    NS_STDEXT::BlockPool recycle;
    NS_STDEXT::ScopedAlloc alloc(recycle);
    
    int* a;
    a = STD_NEW(alloc, int);
    // ... --> same as NS_STDEXT::AutoAlloc
}

int main()
{
    testAutoAlloc();
    testScopedAlloc();	
	return 0;
}

// -------------------------------------------------------------------------

