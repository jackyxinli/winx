#include <stdext/Basic.h>
#include <stdext/Memory.h>

// -------------------------------------------------------------------------
// memory management

void testAutoFreeAlloc()
{
    std::AutoFreeAlloc alloc;
    
    int* a;
    a = STD_NEW(alloc, int);
    
    int* b;
    b = STD_NEW_ARRAY(alloc, int, 100);
    
    int* c;
    c = STD_ALLOC(alloc, int);
    
    int* d;
    d = STD_ALLOC_ARRAY(alloc, int, 100);
    
    std::AutoFreeAlloc* suballoc = STD_NEW(alloc, std::AutoFreeAlloc);
    
    int* e;
    e = STD_NEW(*suballoc, int);
}

void testScopedAlloc()
{
    std::BlockPool recycle;
    std::ScopedAlloc alloc(recycle);
    
    int* a;
    a = STD_NEW(alloc, int);
    // ... --> same as std::AutoFreeAlloc
}

int main()
{
    testAutoFreeAlloc();
    testScopedAlloc();	
	return 0;
}

// -------------------------------------------------------------------------

