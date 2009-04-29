#include <winx/FilePath.h>
#pragma comment(lib, "shlwapi.lib")

// -------------------------------------------------------------------------

int main()
{
	const char* const szDirs[] =
	{
		"d:\\windows\\system32\\",
		"c:\\windows",
	};

	char szDestFile[_MAX_PATH];
	if (winx::SearchFilePath(szDestFile, "notepad.exe", rangeof(szDirs)))
	{
		puts(szDestFile);
		ShellExecuteA(NULL, "open", szDestFile, NULL, NULL, SW_SHOW);
	}
	else
	{
		puts("notepad.exe not found!\n");
	}
	return 0;
}

// -------------------------------------------------------------------------
