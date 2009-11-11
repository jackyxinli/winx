#include <stdext/Basic.h>
#include <stdext/thread/Thread.h>

THREADRET WINAPI writer(void* val)
{
    int count = 0;
    while(1)
    {
        printf("P(%d)\t", count++);
    }
    return 0;
}


THREADRET WINAPI reader(void* val)
{
    int count = 0;
    while(1)
    {
        printf("V(%d)\t", count++);
    }
    return 0;
}

int main(int argc, const char* argv[])
{

	HTHREAD pid1, pid2;
    pid1 = CreateThread(writer, NULL, 0);
    if (pid1 == 0)
        printf("create writer fail\n");
    else
        printf("create writer success\n");

    pid2 = CreateThread(reader, NULL, 0);
    if (pid2 == 0)
        printf("create reader fail\n");
    else
        printf("create reader success\n");

    WaitThread(pid1);
    WaitThread(pid2);
    return 0;
}
