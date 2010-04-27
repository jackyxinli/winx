#include <stdext/Registry.h>
#include <stdext/String.h>

// -------------------------------------------------------------------------

#define WINX_TEST_KEY_	WINX_TEXT("Software\\winx\\TestStdExt\\TestWinRegKey")

void testWrite()
{
	NS_STDEXT::WinRegWriteKey(HKEY_CURRENT_USER, WINX_TEST_KEY_)
		.putDWord(WINX_TEXT("dword"), 123);

	NS_STDEXT::WinRegWriteKey(HKEY_CURRENT_USER, WINX_TEST_KEY_)
		.putGuid(WINX_TEXT("guid"), IID_IClassFactory);

	NS_STDEXT::WinRegWriteKey(HKEY_CURRENT_USER, WINX_TEST_KEY_)
		.putString(WINX_TEXT("string"), WINX_TEXT("abc"));

	NS_STDEXT::WinRegWriteKey(HKEY_CURRENT_USER, WINX_TEST_KEY_)
		.putString(WINX_TEXT("string2"), std::vector<TCHAR>(257, '!'));

	std::basic_string<TCHAR> s3(WINX_TEXT("string3"));
	NS_STDEXT::WinRegWriteKey(HKEY_CURRENT_USER, WINX_TEST_KEY_)
		.putString(WINX_TEXT("string3"), s3);
}

void testRead()
{
	NS_STDEXT::WinRegReadKey key(HKEY_CURRENT_USER, WINX_TEST_KEY_);

	DWORD dword = 0;
	key.getDWord(WINX_TEXT("dword"), dword);
	NS_STDEXT::print("dword: %a\n", dword);

	GUID iid;
	key.getGuid(WINX_TEXT("guid"), iid);
	NS_STDEXT::print("guid: {%a}\n", iid);

	std::basic_string<TCHAR> str;
	key.getString(WINX_TEXT("string"), str);
	NS_STDEXT::print("string: %a\n", str);

	NS_STDEXT::BlockPool recycle;
	NS_STDEXT::ScopedAlloc alloc(recycle);

	NS_STDEXT::BasicString<TCHAR> s2;
	key.getString(WINX_TEXT("string2"), alloc, s2);
	NS_STDEXT::print("string2: %a\n", s2);

	std::vector<TCHAR> s3;
	key.getString(WINX_TEXT("string3"), s3);
	NS_STDEXT::print("string3: %a\n", s3);
}

int main()
{
	testWrite();
	testRead();
	return 0;
}

// -------------------------------------------------------------------------
